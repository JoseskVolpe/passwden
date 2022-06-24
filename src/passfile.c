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
#include <gcrypt.h>
#include "configuration.h"

#define STRINGLEN(s) strlen(s)*sizeof(char)
#define PASSFILENAME "passwords.gpg"
#define BUFFERSIZE 9000

const char * getPassFilePath();
const char * decrypt(const char * src, const char * fingerprint);
const char * encrypt(const char * src, const char * fingerprint);

const struct json_object * get_passwords(const char * fingerprint){

    const char *passfilepath = getPassFilePath();

    //Read file
    char *buffer;
    FILE *fd = fopen(passfilepath, "r");

    buffer=malloc(BUFFERSIZE*sizeof(char));
    if(fd!=NULL) {
        fread(buffer, BUFFERSIZE, 1, fd);
        fclose(fd);
    }

    const char * bufferd = decrypt(buffer, fingerprint);
    const struct json_object *jobj = json_tokener_parse(bufferd);

    free(buffer);

    return jobj;
}


const char * getPassFilePath(){
    const char *dir_path = configDirPath();
    char *passfilepath = malloc(STRINGLEN(dir_path)+sizeof(char)+STRINGLEN(PASSFILENAME));
    strcpy(passfilepath, dir_path);
    strcat(passfilepath, "/");
    strcat(passfilepath, PASSFILENAME);

    return passfilepath;
}

const char * decrypt(const char * src, const char * fingerprint){
    printf("WARNING: decrypt() not yet implemented!\n");
    return src; //TODO: Encryption and decryption
}

const char * encrypt(const char * src, const char * fingerprint){
    printf("WARNING: encrypt() not yet implemented!\n");
    return src; //TODO: Encryption and decryption
}
