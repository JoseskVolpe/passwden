#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

const char* app_name = "passwden";

char return_val = 0;
char *home, *con_loc;
DIR *con_dir;

int main(int argc, char* argv[]){

    home = getenv("HOME");
    con_loc = malloc(strlen(home)+strlen("/.config/")+strlen(app_name)+1);
    strcpy(con_loc, home);
    strcat(con_loc, "/.config/");
    strcat(con_loc, app_name);
    strcat(con_loc, "/");

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
    free(con_loc);
    return return_val;
}
