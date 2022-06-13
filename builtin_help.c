#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "minish.h"

int 
builtin_help(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"help\"\n");
        char *help_argv[] = {"help", "help"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE;
    }
    if (argc == 1) {
        printf("Los siguientes comandos del minishell son definidos internamente, para informacion mas detallada ingresar\"help [command name]\":\n");
        for (int i = 0; syntax_array[i][0] != '\0'; i++) {
            printf("\t%s\n", syntax_array[i]);
        } 
        return EXIT_SUCCESS;
    }
    struct builtin_struct *command_struct = builtin_lookup(argv[1]);
    if (command_struct->cmd != NULL) {
        printf("%s\n", command_struct->help_txt);
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "ERROR: No existe el comando interno %s\n", argv[1]); //esta bien o que hay que hacer aca?
    return EXIT_FAILURE;
}