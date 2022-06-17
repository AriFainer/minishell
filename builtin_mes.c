#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#include "minish.h"

int 
builtin_mes(int argc, char **argv) {
    if (argc > 3) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"mes\"\n");
        char *help_argv[] = {"help", "mes"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }
    if (argc == 1) { //sin argumento
        mktime(localtime(time));
    }
    struct tm *time = malloc(sizeof(struct tm));
    if (argc == 2) { //solo año
        time->tm_year = atoi(argv[1]);
        mktime(time);
    }
    if (argc == 3) { // año y mes
        time->tm_mon = atoi(argv[1]);
        time->tm_year = atoi(argv[2]);
        mktime(time);
    }
    fprintf(stderr, "Error: Error en el comando %s\n", argv[1]); //no deberia llegar a aca
    return EXIT_FAILURE;
}