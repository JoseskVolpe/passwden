  /*
 * crypt.c
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
#include <stdio.h>
#include <gpgme.h>
#include <string.h>
#include "passfile.h"
#include "message.h"

void getCtx(gpgme_ctx_t *ctx){

    const gpgme_error_t err=gpgme_new(ctx);

    if(!err) return;
    fprintf(stderr, GPGME_ERROR, gpgme_strerror(err));
    exit(-1);

}

const char * decrypt(const int * src){
    char *dest;

    gpgme_ctx_t ctx;
    gpgme_error_t err;
    getCtx(&ctx);

    gpgme_data_t plain, cipher;
    err = gpgme_data_new_from_mem(&cipher, src, strlen(src), 0);
    if(err && err!=GPG_ERR_NO_ERROR)
        goto ERROR;
    err = gpgme_data_new(&plain);
    if(err && err!=GPG_ERR_NO_ERROR)
        goto ERROR;

    err = gpgme_op_decrypt(ctx, cipher, plain);
    if(err && err!=GPG_ERR_NO_ERROR)
        goto ERROR;

    size_t destal = BUFFERSIZE*sizeof(char)+sizeof(char);
    unsigned long offs=0;
    dest = malloc(destal);
    gpgme_data_seek(plain, 0, SEEK_SET);

    READ:{
        size_t bufsiz = gpgme_data_read(plain, &dest[offs], destal-offs);
        if(bufsiz<0){
            fprintf(stderr, GPGME_CIPHERTEXT_ERROR, destal, bufsiz);
            exit(-1);
        }
        if(bufsiz+offs>=destal-sizeof(char)){
            offs=bufsiz-1;
            destal+=BUFFERSIZE*sizeof(char);
            dest = (char *)realloc(dest, destal);
            gpgme_data_seek(plain, bufsiz-sizeof(char), SEEK_SET);
            goto READ;
        }
        goto FINAL;
    }

    ERROR:{
        fprintf(stderr, GPGME_ERROR, gpgme_strerror(err));
        exit(-1);
    }

    FINAL:{
        dest[destal-sizeof(char)]='\0';
        gpgme_data_release(cipher);
        gpgme_data_release(plain);
        return dest;
    }
}

const int * encrypt(const char * src, const char * fingerprint){

    int *dest;

    gpgme_ctx_t ctx;
    gpgme_key_t key;
    gpgme_error_t  err;
    getCtx(&ctx);
    if((err = gpgme_get_key(ctx, fingerprint, &key, 1)) && err!=GPG_ERR_NO_ERROR)
        goto ERROR;
    gpgme_set_armor(ctx, 1);

    gpgme_data_t plain, cipher;
    err = gpgme_data_new_from_mem(&plain, src, strlen(src), 0);
    if(err && err!=GPG_ERR_NO_ERROR)
        goto ERROR;
    err = gpgme_data_new(&cipher);
    if(err && err!=GPG_ERR_NO_ERROR)
        goto ERROR;

    gpgme_key_t keys[] = {key, NULL};
    err = gpgme_op_encrypt(ctx, keys, GPGME_ENCRYPT_ALWAYS_TRUST, plain, cipher);
    if(err && err!=GPG_ERR_NO_ERROR)
        goto ERROR;

    gpgme_key_release(key);
    gpgme_release(ctx);

    gpgme_data_seek(cipher, 0, SEEK_SET);
    size_t destal = BUFFERSIZE*sizeof(int)+sizeof(int);
    unsigned long offs=0;
    dest = malloc(destal);
    WRITE:{
        size_t bufsiz = gpgme_data_read(cipher, &dest[offs], destal-offs);
        if(bufsiz<0){
            fprintf(stderr, GPGME_CIPHERTEXT_ERROR, destal, bufsiz);
            exit(-1);
        }
        if(bufsiz+offs>=destal-sizeof(int)){
            offs=bufsiz-1;
            destal+=BUFFERSIZE*sizeof(int);
            dest = (int *)realloc(dest, destal);
            gpgme_data_seek(cipher, bufsiz-sizeof(int), SEEK_SET);
            goto WRITE;
        }
    }

    goto FINAL;

    ERROR:{
        fprintf(stderr, GPGME_ERROR, gpgme_strerror(err));
        exit(-1);
    }

    FINAL:{
        gpgme_data_release(cipher);
        gpgme_data_release(plain);
        return dest;
    }
}
