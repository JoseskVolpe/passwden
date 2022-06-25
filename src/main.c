/*
 * main.c
 *
 * Copyright 2022 Josesk Volpe <joseskvolpe@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

#include <termios.h>
#include "message.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json-c/json.h>
#include "configuration.h"
#include "passfile.h"

#define BOOL char
#define TRUE 1
#define FALSE 0

INIconf iconf;

const int invalidArguments(int argc, char *argv[], int index);
void showHelp();
void showKeyDefineHelp();
const BOOL beSure();
const int checkArguments(int argc, char* argv[]);
const char * askNewPassword();

int main(int argc, char* argv[]){

    if(argc==1 || strcmp(argv[1], "--help")==0 || strcmp(argv[1], "-h")==0){
        showHelp();
        return 0;
    }

    if(checkConfigDir()==-1)
        return -1;

    loadConfigFile(&iconf);

    return checkArguments(argc, argv);
}

const int displayPassword(int argc, char* argv[]){

    char * website = argv[1];
    const char * loginName = NULL, * sec;

    if(argc>3){
        return invalidArguments(argc, argv, 3);
    }else if(argc==3)
        loginName = argv[2];

    struct json_object *jobj = get_passwords(iconf.fingerprint);
    struct json_object *wjobj;
    if(!json_object_object_get_ex(jobj, website, &wjobj)){
        fprintf(stderr, "Website not registered\n");
        return -1;
    }


    struct json_object *ljobj, *passobj;

    //TODO: Display passwords in 'less', for security reasons. Terminal must NOT keep passwords in history
    if(loginName==NULL){
        struct json_object_iterator jit, jitEnd;
        jit = json_object_iter_begin(wjobj);
        jitEnd = json_object_iter_end(wjobj);

        while (!json_object_iter_equal(&jit, &jitEnd)) {

            loginName = json_object_iter_peek_name(&jit);
            ljobj = json_object_iter_peek_value(&jit);
            json_object_object_get_ex(ljobj, "password", &passobj);
            sec = json_object_get_string(passobj);

            printf("%s: %s\n",loginName, sec);
            json_object_iter_next(&jit);
        }
    }else{
        if(!json_object_object_get_ex(wjobj, loginName, &ljobj)){
            fprintf(stderr, "%s was not registered on %s\n", loginName, website);
            return -1;
        }

        json_object_object_get_ex(ljobj, "password", &passobj);
        sec = json_object_get_string(passobj);
        printf("%s: %s\n",loginName, sec);
    }

    return 0;
}

typedef struct user_info{
    const char  *website,
                *login,
                *sec;
}user_info;
void getUserInfo(int argc, char* argv[], struct user_info * ui){
    #define BUFFER 120 /* TODO: User realloc() */

    switch(argc){
        case 3:
            ui->website = argv[2];
            printf("Website: %s\n", ui->website);
            ui->login = malloc(BUFFER*sizeof(char));
            printf("Login: ");
            scanf("%s", ui->login);
            break;
        case 4:
            ui->website = argv[2];
            fprintf(stdout, "Website: %s\n", ui->website);
            ui->login = argv[3];
            fprintf(stdout, "Login: %s\n", ui->login);
            break;
        default:
            printf("Website name: ");
            ui->website = malloc(BUFFER*sizeof(char));
            scanf("%s", ui->website);
            printf("Login: ");
            ui->login = malloc(BUFFER*sizeof(char));
            scanf("%s", ui->login);
            break;
    }
}

const int setAccount(int argc, char* argv[]){

    struct user_info ui;

    struct json_object *jobj = get_passwords(iconf.fingerprint);
    getUserInfo(argc, argv, &ui);
    ui.sec = askNewPassword();

    struct json_object *wjobj, *ljobj, *passobj, *secj;
    if(!json_object_object_get_ex(jobj, ui.website, &wjobj)){
        wjobj = json_object_new_object();
        json_object_object_add(jobj, ui.website, wjobj);
    }

    secj = json_object_new_string(ui.sec);
    passobj = json_object_new_object();
    json_object_object_add(passobj, "password", secj);
    json_object_object_add(wjobj, ui.login, passobj);

    return update_passwords(jobj, iconf.fingerprint);
}
const int removePassword(int argc, char* argv[]){

    struct json_object *jobj, *jweb, *jlogin;
    char *website = argv[2], *login;

    switch(argc){
        case 2:
            fprintf(stderr,
                "At least a website must be specified\n"
                "Usage:\n"
                "%s --remove <WEBSITE> [LOGIN]\n",
                app_name
            );
            return -1;

        case 3:

            goto WEBSITE_SEARCH;
            AFT_WEB_SEARCH3:
            printf("Delete ALL passwords for website \"%s\". ", website);
            if(beSure()==FALSE){
                printf("Operation canceled\n");
                goto FINAL;
            }

            json_object_object_del(jobj, website);
            if(update_passwords(jobj, iconf.fingerprint))
                return -1;
            printf("Website cleaned from database!\n");

            return 0;

        case 4:

            goto WEBSITE_SEARCH;
            AFT_WEB_SEARCH4:
            login = argv[3];
            if(!json_object_object_get_ex(jweb, login, &jlogin)){
                fprintf(stderr, "Login not registered\n");
                return -1;
            }
            printf("Delete \"%s\" from \"%s\". ", login, website);
            if(beSure()==FALSE){
                printf("Operation canceled\n");
                goto FINAL;
            }

            json_object_object_del(jweb, login);
            if(update_passwords(jobj, iconf.fingerprint))
                return -1;
            printf("Login removed from database!\n");

            return 0;

        default:
            invalidArguments(argc, argv, 4);
    }

    WEBSITE_SEARCH:
    jobj = get_passwords(iconf.fingerprint);
    if(!json_object_object_get_ex(jobj, website, &jweb)){
        fprintf(stderr, "Website not registered\n");
        return -1;
    }
    switch(argc){
        case 3:
            goto AFT_WEB_SEARCH3;
        case 4:
            goto AFT_WEB_SEARCH4;
    }

    FINAL:
    return 0;
}

const int list(int argc, char* argv[]){

    const char *website = NULL;
    if(argc==3)
        website = argv[2];
    else if(argc>3)
        return invalidArguments(argc, argv, 3);

    struct json_object *jweb, *jlogin;
    struct json_object *jobj = get_passwords(iconf.fingerprint);
    struct json_object_iterator jidw, jidwEnd, jidl, jidlEnd;

    if(website!=NULL){
        if(!json_object_object_get_ex(jobj, website, &jweb)){
            fprintf(stderr, "Website not registered\n");
            return -1;
        }
        jlogin=jweb;
        goto CHECK_WEBSITE;
    }

    jidw = json_object_iter_begin(jobj);
    jidwEnd = json_object_iter_end(jobj);
    while(!json_object_iter_equal(&jidw, &jidwEnd)){
        website = json_object_iter_peek_name(&jidw);
        jlogin = json_object_iter_peek_value(&jidw);
        goto CHECK_WEBSITE;
        WEBSITE_ITERATION:
        json_object_iter_next(&jidw);
    } goto FINAL;

    CHECK_WEBSITE:
    printf("*%s:\n", website);
    jidl = json_object_iter_begin(jlogin);
    jidlEnd = json_object_iter_end(jlogin);
    while(!json_object_iter_equal(&jidl, &jidlEnd)){
        printf("    %s\n", json_object_iter_peek_name(&jidl));
        json_object_iter_next(&jidl);
    }
    printf("\n");
    if(argc==2)
        goto WEBSITE_ITERATION;

    FINAL:
    return 0;
}

/* Slighty copied from GNU. I've took so much time figuring out this so yiff off*/
ssize_t getpass (char* message, char **lineptr)
{
    struct termios old, new;
    int nread;

    size_t n = 0;

    printf(message);

    /* Turn echoing off and fail if we can’t. */
    if (tcgetattr (fileno (stdin), &old) != 0)
    return -1;
    new = old;
    new.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stdin), TCSAFLUSH, &new) != 0)
        return -1;

    /* Read the passphrase */
    nread = getline (lineptr, &n, stdin);

    /* Restore terminal. */
    (void) tcsetattr (fileno (stdin), TCSAFLUSH, &old);
    lineptr[0][strcspn(lineptr[0], "\n" )] = '\0';

    printf("\n");
    return nread;
}


const char * askNewPassword(){

    char *sec, *sec2;

    INSERT_PASSWORD:
    if(getpass("Password: ", &sec)<0)
        exit(-1);
    if(strlen(sec)<6){
        fprintf(stderr, "Password or PIN must have at-least 6 characters\n");
        goto INSERT_PASSWORD;
    }

    if(getpass("Confirm password: ", &sec2)<0)
        exit(-1);
    if(strcmp(sec, sec2)!=0){
        fprintf(stderr, "Passwords mismatch, try again\n");
        goto INSERT_PASSWORD;
    }
    free(sec2);
    return sec;
}

const int keyArgument(int argc, char* argv[]);
const int checkArguments(int argc, char* argv[]){
    if(strcmp(argv[1], "--key")==0 || strcmp(argv[1], "-k")==0){
        return keyArgument(argc, argv);
    }

    if(iconf.fingerprint==NULL || strlen(iconf.fingerprint)==0){
        showKeyDefineHelp();
        return -1;
    }

    if(strcmp(argv[1], "--set")==0 || strcmp(argv[1], "-s")==0){
        return setAccount(argc, argv);
    }
    if(strcmp(argv[1], "--list")==0 || strcmp(argv[1], "-l")==0){
        return list(argc, argv);
    }
    if(strcmp(argv[1], "--remove")==0 || strcmp(argv[1], "-r")==0){
        return removePassword(argc, argv);
    }

    return displayPassword(argc, argv);
}

const int keyArgument(int argc, char* argv[]){
    switch(argc){
            case 2: //Show key

                if(iconf.fingerprint==NULL || strlen(iconf.fingerprint)==0){
                    showKeyDefineHelp();
                    return 0;
                }
                printf("Key fingerprint: %s\n", iconf.fingerprint);
                return 0;

            case 3:

                if(strlen(argv[2])!=16){
                    fprintf(stderr, INVALID_FINGERPRINT_LENGTH_MESSAGE);
                    return -1;
                }

                //FIXME: Check if fingerprint has a private key
                printf("FIXME: Program must check if fingerprint is a private key\n");

                iconf.fingerprint = strdup(argv[2]); //TODO: Reencrypt passwords JSON
                updateConfigFile(&iconf);
                return 0;

            default:
                return invalidArguments(argc, argv, 3);

    }
}

void showHelp(){
    printf("¡Write a help page here! >w< \n"); //TODO: Help page
}

void showKeyDefineHelp(){
    fprintf(stderr, FINGERPRINT_NOT_DEFINED, app_name);
}

const int invalidArguments(int argc, char* argv[], int index){
    printf("Invalid arguments: ");

    int i;
    for(i=index; i<argc; i++)
        printf("%s ", argv[i]);

    printf("\n");
    return -1;
}

BOOL beSure(){

    struct termios new, old;

    if (tcgetattr (fileno (stdin), &old) != 0)
    return -1;
    new = old;
    new.c_lflag &= ~ICANON;
    tcsetattr (fileno (stdin), TCSAFLUSH, &new);

    printf("Are you sure? (y/N) ");
    int c = getchar();
    printf("\n");

    tcsetattr (fileno (stdin), TCSAFLUSH, &old);

    return c=='y' || c=='Y';
}
