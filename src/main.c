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
#include <stdio.h>
#include "configuration.h"


void showHelp();

int main(int argc, char* argv[]){

    if(argc==1 || strcmp(argv[1], "--help")==0){
        showHelp();
        return 0;
    }



    if(checkConfigDir()==-1)
        return -1;

    updateConfigFile();


    return 0;
}

void showHelp(){
    printf("¡Write a help page here! >w< \n"); //TODO: Help page
}
