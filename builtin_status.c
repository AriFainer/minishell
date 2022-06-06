#include <stdio.h>
#include <stdlib.h>
#include "minish.h"

int builtin_exit(int argc, char ** argv)
{
    fprintf(stdout, "El status de retorno del ultimo comando ejecutado es %d", globalstatret);
    return globalstatret;
}