#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "minish.h"

int 
builtin_help(int argc, char **argv) {
    if (argc > 2) {
        // correr el built in help de help y indicar error de sintaxis
        return -1;
    }
    if (argc == 1) {
        // texto de ayuda indicando que comandos externos existen 
        return 0;
    }
    // tengo que recorrer el coso de help 
    /* 
    if (strcmp(argv[1], "-") == 0) {
        chdir(prevdirectory);
        strcpy(prevdirectory, directory);
        getcwd(directory, MAXCWD);
        return 0;
    }
    */
    fprintf(stderr, "ERROR: No existe el comando interno %s\n", argv[1]); //esta bien o que hay que hacer aca?
    return -1;
}