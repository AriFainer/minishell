#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "minish.h"

int 
builtin_exit(int argc, char ** argv) { //arreglar
    if (argc > 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"exit\"\n");
        char *help_argv[] = {"help", "exit"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE;
    }
    char *retorno = argv[1];
    fputc('\n', stderr);
    bool numeric_arg = true;
    if (argc == 2) {
        if (isdigit(argv[1][0]) || argv[1][0] == '-') {
            for (int i = 1; argv[1][i] != '\0'; i++) {
                if (!isdigit(argv[1][i])) {
                    numeric_arg = false;
                }
            }
        }
        else {
            numeric_arg = false;
        }
        if (!numeric_arg) {
            fprintf(stderr, "Error: Argumento no numerico ingresado en el comando \"exit\"\n");
            globalstatret = EXIT_FAILURE;
        }
        else {
            globalstatret = atoi(retorno);
        }
        
    }
    fprintf(stderr, "Exiting %s ...\n", progname);
    clean_argv(argv);
    save_history();
    exit(globalstatret);
    return EXIT_FAILURE; //No deberia llegar a este punto
}