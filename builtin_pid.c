#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "minish.h"

int 
builtin_pid(int argc, char ** argv) {
    if (argc > 1) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"pid\"\n");
        char *help_argv[] = {"help", "pid"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }
    pid_t p_id = getpid();
    fprintf(stdout, "pid: %d \n", p_id);
    return EXIT_SUCCESS; // Nunca tira error.
}