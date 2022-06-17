#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "minish.h"

int 
builtin_dir(int argc, char **argv) {
    if (argc > 3) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"dir\"\n");
        char *help_argv[] = {"help", "dir"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }
    if (argc == 1) {
        DIR *directory = opendir(".");
        struct dirent *dir_entry;
        if (directory != NULL) {
            while ((dir_entry = readdir(directory)) != NULL) {
                
                printf("%s\n", dir_entry->d_name);
            }
            return EXIT_SUCCESS;
        }
        fprintf(stderr, "Error: Error al abrir el directorio corriente\n");
        return EXIT_FAILURE;        
    }
    if (strcmp(argv[1], "-") == 0) {
        if(chdir(prevdirectory)!=0){
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
/*
stat 
dirent
opendir
readdir
S_ISREG  
DIR
*/


