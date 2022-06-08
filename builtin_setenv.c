#include <stdlib.h>
#include <stdio.h>

#include "minish.h"

int builtin_setenv(int argc, char **argv)
{
    if (argc!=3){
        fprintf(stderr,"Error de sintaxis\n");
        char *help_argv[] = {"help", "setenv"};
        builtin_help(2, help_argv);
        return -1;
    }
    if (setenv(argv[1], argv[2], 1) == 0)
        fprintf(stderr, "Variable de ambiente %s seteada en %s\n", argv[1], argv[2]);

    else
        fprintf(stderr, "ERROR hubo un problema al setear la variable de ambiente %s\n", argv[1]);

    return 0;
}