#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "minish.h"

int 
builtin_cd(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"cd\"\n");
        char *help_argv[] = {"help", "cd"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }
    if (argc == 1) {
        if(chdir(getenv("HOME")) != 0){
            fprintf(stderr, "Error: La variable de entorno HOME no contiene un directorio existente\n");
            return EXIT_FAILURE;
        }
        strcpy(prevdirectory, directory);
        getcwd(directory, MAXCWD);
        return EXIT_SUCCESS;
    }
    if (strcmp(argv[1], "-") == 0) {
        if(chdir(prevdirectory) != 0){
            fprintf(stderr, "Error: Ya no existe el directorio anterior\n");
            return EXIT_FAILURE;
        }
        strcpy(prevdirectory, directory);
        getcwd(directory, MAXCWD);
        return EXIT_SUCCESS;
    }
    if (chdir(argv[1]) == 0) {
        strcpy(prevdirectory, directory);
        getcwd(directory, MAXCWD);
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "Error: No existe el directorio %s\n", argv[1]);
    return EXIT_FAILURE;
}