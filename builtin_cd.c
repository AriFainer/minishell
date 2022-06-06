#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "minish.h"

int 
builtin_cd(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"cd\"\n");
        char help_argv[] = {"help", "cd"};
        builtin_help(2, help_argv);
        return -1; 
    }
    if (argc == 1) {
        chdir(getenv("HOME"));
        strcpy(prevdirectory, directory);
        getcwd(directory, MAXCWD);
        return 0;
    }
    if (strcmp(argv[1], "-") == 0) {
        chdir(prevdirectory);
        strcpy(prevdirectory, directory);
        getcwd(directory, MAXCWD);
        return 0;
    }
    if (chdir(argv[1]) == 0)
    {
        strcpy(prevdirectory, directory);
        getcwd(directory, MAXCWD);
        return 0;
    }
    fprintf(stderr, "Error: No existe el directorio %s\n", argv[1]);
    return -1;
}