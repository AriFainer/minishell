#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "minish.h"

int 
builtin_cd(int argc, char **argv) {
    if (argc > 2) {
        // correr el built in help de cd , error de sintaxis
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
        return 0;
    }
    fprintf(stderr, "ERROR: No existe el directorio %s\n", argv[1]);
    return -1;
}