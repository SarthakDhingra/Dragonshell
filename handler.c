#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "handler.h"

char cwd[256];

void pwd(void){
    getcwd(cwd, sizeof(cwd));
    printf("%s", cwd);
    printf("\n");
}

void change_directory(const char *path){
    chdir(path);
}

int exit_program(void){
    printf("Exiting\n");
    return 0;
}
