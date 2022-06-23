 /*
 * configuration.c
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

#include "configuration.h"
#include <stdlib.h>
#include <dirent.h>
#include <ini.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

char *configDirPath(){

    char *cfg = getenv("XDG_CONFIG_HOME");
    char *con_loc;

    if(cfg){
        con_loc = malloc(strlen(cfg)+strlen(app_name)+2);
        strcpy(con_loc, cfg);
        strcat(con_loc, "/");
    }else{
        const char *home = getenv("HOME");
        con_loc = malloc(strlen(home)+strlen("/.config/")+strlen(app_name)+1);
        strcpy(con_loc, home);
        strcat(con_loc, "/.config/");
    }

    strcat(con_loc, app_name);
    strcat(con_loc, "/");
    return con_loc;
}

int checkConfigDir(){

    char *con_loc = configDirPath();
    DIR *con_dir = opendir(con_loc); //Open directory
    if(ENOENT == errno){
        //Create configuration directory if it doesn't exist
        if(mkdir(con_loc, S_IRWXU)!=0){
            fprintf(stderr, "Error creating configuration directory %s", con_loc);
            free(con_loc);
            return -1;
        }
        free(con_dir);
        free(con_loc);
        return 0;
    }else if(!con_loc){
        fprintf(stderr, "Error accessing configuration directory %s", con_loc);
        free(con_loc);
        return -1;
    }

    free(con_dir);
    free(con_loc);
    return 0;
}

void updateConfigFile(){



}
