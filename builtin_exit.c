#include <stdio.h>
#include <stdlib.h>

#include "minish.h"

int 
builtin_exit(int argc, char ** argv) {
    if (argc > 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"exit\"\n");
        char *help_argv[] = {"help", "exit"};
        builtin_help(2, help_argv);
        return -1;
    }
    char *retorno = argv[1];
    fputc('\n', stderr);
    if (retorno != NULL) {
        globalstatret=atoi(retorno);
    }
    fprintf(stderr, "Exiting %s ...\n", progname);
    exit(globalstatret);
    return -1; //Nunca se deberia llegar a este punto
}