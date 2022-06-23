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
#include "message.h"
#include <stdlib.h>
#include <dirent.h>
#include <ini.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


#define STRINGLEN(s) strlen(s)*sizeof(char)

const char *config_file = "config.ini";

char *configDirPath(){

    char *cfg = getenv("XDG_CONFIG_HOME");
    char *con_loc;

    if(cfg){
        con_loc = malloc(STRINGLEN(cfg)+STRINGLEN(app_name)+2);
        strcpy(con_loc, cfg);
        strcat(con_loc, "/");
    }else{
        const char *home = getenv("HOME");
        con_loc = malloc(STRINGLEN(home)+STRINGLEN("/.config/")+STRINGLEN(app_name)+1);
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

static int inihandler(void *user, const char* section, const char* name, const char* value){
    struct INIconf* iconf = (struct INIconf*) user;

    #define MATCH(s,n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if(MATCH("key", "fingerprint")){
        if(strlen(value)!=16 && strlen(value)!=0){
            fprintf(stderr, INVALID_FINGERPRINT_LENGTH_MESSAGE);
            exit(-1);
        }
        iconf -> fingerprint = strdup(value);
    }else{
        return 0;
    }

    return 1;

}

const char *configFilePath(){
    char *con_loc = configDirPath();
    char *con_file = malloc(STRINGLEN(con_loc)+STRINGLEN(config_file));
    strcpy(con_file, con_loc);
    strcat(con_file, config_file);

    free(con_loc);
    return con_file;
}

void loadConfigFile(struct INIconf * iconf){

    const char *con_file = configFilePath();

    ini_parse(con_file, inihandler, iconf);

}

const int updateConfigFile(struct INIconf * iconf){

    FILE *fd = fopen(configFilePath(), "w+");

    fprintf(fd,

        "[key]\n"
        "fingerprint=%s\n"
        ,
        iconf->fingerprint
    );

    fclose(fd);

    return 0;

}
