#include <stdio.h>
#include <stdlib.h>

#include "minish.h"

int builtin_pid()
{
    pid_t *p_id = getpid();

    fprintf(stdout,"pid: %d ", p_id);

    return 0; // Nunca tira error.

}