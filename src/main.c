#include <stdlib.h>
#include <string.h>

const char* app_name = "passwden";

char *home, *config_dir;

int main(int argc, char* argv[]){

    home = getenv("HOME");
    config_dir = malloc(strlen(home)+strlen("/.config/")+strlen(app_name)+1);
    strcpy(config_dir, home);
    strcat(config_dir, "/.config/");
    strcat(config_dir, app_name);
    strcat(config_dir, "/");



    free(config_dir);

    return 0;
}
