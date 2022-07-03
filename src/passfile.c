/*
 * passfile.c
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

#include <stdio.h>
#include <json-c/json.h>
#include <sys/stat.h>
#include "configuration.h"
#include <string.h>
#include "message.h"
#include "crypt.h"
#include "passfile.h"

#define STRINGLEN(s) strlen(s)*sizeof(char)
#define PASSFILENAME "passwords.gpg"


const char * getPassFilePath();

struct json_object * get_passwords(const char * fingerprint){

    const char *passfilepath = getPassFilePath();

    //Read file
    char *buffer;
    FILE *fd = fopen(passfilepath, "r");

    struct json_object *jobj;

    buffer=malloc(BUFFERSIZE*sizeof(char));
    if(fd!=NULL) {
        fread(buffer, BUFFERSIZE, 1, fd);
        fclose(fd);

        const char * bufferd = decrypt(buffer);
        jobj = json_tokener_parse(bufferd);
    }else{
        jobj = json_object_new_object();
    }

    free(buffer);

    return jobj;
}

const int update_passwords(struct json_object *jobj, const char * fingerprint){
    const char *passfilepath = getPassFilePath();

    FILE *f, *bk;
    char *bk_path = malloc(STRINGLEN(passfilepath)+STRINGLEN(".bk")+sizeof(char));
    strcpy(bk_path, passfilepath);
    strcat(bk_path, ".bk");

    const char *enc = encrypt(json_object_get_string(jobj), fingerprint);

    if((f = fopen(passfilepath, "rw")) != NULL ){ //Backup
        bk = fopen(bk_path, "w");

        char c;
        while((c = fgetc(f)) != EOF )
            fputc(c, bk);

        fclose(bk);
        fclose(f);
        chmod(bk_path, strtol("0600", 0, 8));
        f = fopen(passfilepath, "w");
    }else{
        f = fopen(passfilepath, "w");
    }

    fprintf(f, enc);
    fclose(f);
    chmod(passfilepath, strtol("0600", 0, 8));

    return 0;

}

const char * getPassFilePath(){
    const char *dir_path = configDirPath();
    char *passfilepath = malloc(STRINGLEN(dir_path)+sizeof(char)*2+STRINGLEN(PASSFILENAME));
    strcpy(passfilepath, dir_path);
    strcat(passfilepath, "/");
    strcat(passfilepath, PASSFILENAME);

    return passfilepath;
}

