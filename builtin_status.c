#include <stdio.h>
#include <stdlib.h>
#include "minish.h"

int 
builtin_status(int argc, char ** argv) {
    if (argc > 1) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"status\"\n");
        char help_argv[] = {"help", "status"};
        builtin_help(2, help_argv);
        return -1;
    }
    fprintf(stdout, "El status de retorno del ultimo comando ejecutado es %d", globalstatret);
    return globalstatret;
}