#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "minish.h"

int 
builtin_pid(int argc, char ** argv) {
    pid_t p_id = getpid();
    fprintf(stdout, "pid: %d \n", p_id);
    return 0; // Nunca tira error.
}