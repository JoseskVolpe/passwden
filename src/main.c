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
#include "configuration.h"

const INIconf *iconf;

void showHelp();
void showKeyDefineHelp();

int main(int argc, char* argv[]){

    if(argc==1 || strcmp(argv[1], "--help")==0){
        showHelp();
        return 0;
    }



    if(checkConfigDir()==-1)
        return -1;

    struct INIconf iconf;
    loadConfigFile(&iconf);

    if(strcmp(argv[1], "--key")==0){
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

    return 0;
}

void showHelp(){
    printf("¡Write a help page here! >w< \n"); //TODO: Help page
}

void showKeyDefineHelp(){
    printf("No key fingerprint is defined, to set your key fingerprint, use:\n%s --key <fingerprint>\n", app_name);
}
