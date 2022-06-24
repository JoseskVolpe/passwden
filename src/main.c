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

    struct json_object *jobj = get_passwords(iconf.fingerprint);
    printf("jobj from str:\n---\n%s\n---\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY));

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

                iconf.fingerprint = strdup(argv[2]);
                updateConfigFile(&iconf);
                return 0;

            default:

                printf("Invalid arguments: ");

                int i;
                for(i=3; i<argc; i++)
                    printf("%s ", argv[i]);

                printf("\n");
                return -1;

    }
}

void showHelp(){
    printf("¡Write a help page here! >w< \n"); //TODO: Help page
}

void showKeyDefineHelp(){
    printf("No key fingerprint is defined, to set your key fingerprint, use:\n%s --key <fingerprint>\n", app_name);
}
