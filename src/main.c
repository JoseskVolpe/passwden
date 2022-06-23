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

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include "configuration.h"

const char* app_name = "passwden";

char return_val = 0;
char *home, *con_loc;
DIR *con_dir;

void showHelp();

int main(int argc, char* argv[]){

    if(argc==1 || strcmp(argv[1], "--help")==0){
        showHelp();
        return 0;
    }

    home = getenv("HOME");
    con_loc = malloc(strlen(home)+strlen("/.config/")+strlen(app_name)+1);
    strcpy(con_loc, home);
    strcat(con_loc, "/.config/");
    strcat(con_loc, app_name);
    strcat(con_loc, "/");

    //Create and get directory
    con_dir = opendir(con_loc);
    if(ENOENT == errno){
        //Create configuration directory
        if(mkdir(con_loc, S_IRWXU)!=0){
            fprintf(stderr, "Error creating configuration directory %s", con_loc);
            return_val = -1;
            goto FINAL;
        }
        con_dir = opendir(con_loc);
    }else if(!con_loc){
        fprintf(stderr, "Error accessing configuration directory %s", con_loc);
        return_val = -1;
        goto FINAL;
    }





    FINAL:
    if(con_dir!=NULL) free(con_dir);
    free(con_loc);
    return return_val;
}

void showHelp(){
    printf("¡Write a help page here! >w< \n"); //TODO: Help page
}
