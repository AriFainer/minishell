#include <stdlib.h>
#include <stdio.h>

#include "minish.h"

int builtin_unsetenv(int argc, char **argv){
    if (argc < 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"unsetenv\"\n");
        char *help_argv[] = {"help", "unsetenv"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE;
    }
    for(int i = 1; i < argc;i++){
        if(unsetenv(argv[i])==0)
            fprintf(stderr, "Variable %s eliminada\n",argv[i]) ;
        else
            fprintf(stderr,"No existe la variable de ambiente %s\n",argv[i]);
    }
    return 0;
}