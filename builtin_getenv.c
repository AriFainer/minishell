#include <stdlib.h>
#include <stdio.h>

#include "minish.h"

int
builtin_getenv(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"getenv\"\n");
        char *help_argv[] = {"help", "getenv"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE;
    }
    char *env;
    for (int i = 1; i < argc; i++){
        if ((env = getenv(argv[i])) != NULL)
            fprintf(stdout, "%s: %s\n", argv[i], env);
        else
            fprintf(stderr, "No existe la variable de ambiente %s\n", argv[i]);
    }
    return EXIT_SUCCESS;
}