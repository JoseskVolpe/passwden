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

#include "message.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <json-c/json.h>
#include "configuration.h"
#include "passfile.h"

INIconf iconf;

const int invalidArguments(int argc, char *argv[], int index);
void showHelp();
void showKeyDefineHelp();
const int checkArguments(int argc, char* argv[]);

int main(int argc, char* argv[]){

    if(argc==1 || strcmp(argv[1], "--help")==0){
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

    if(argc==3)
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

const int keyArgument(int argc, char* argv[]);
const int checkArguments(int argc, char* argv[]){
    if(strcmp(argv[1], "--key")==0){
        return keyArgument(argc, argv);
    }

    return displayPassword(argc, argv);
}

const int keyArgument(int argc, char* argv[]){
    switch(argc){
            case 2: //Show key

                if(strlen(iconf.fingerprint)==0){
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
    printf("No key fingerprint is defined, to set your key fingerprint, use:\n%s --key <fingerprint>\n", app_name);
}

const int invalidArguments(int argc, char* argv[], int index){
    printf("Invalid arguments: ");

    int i;
    for(i=index; i<argc; i++)
        printf("%s ", argv[i]);

    printf("\n");
    return -1;
}
