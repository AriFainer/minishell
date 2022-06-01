#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "minish.h"

int builtin_cd(__attribute__((unused)) int argc, char **argv)
{
    if (argv[1] == NULL)
    {
        chdir(getenv("HOME"));
        return 0;
    }
    if (chdir(argv[1]) == 0)
    {
        return 0;
    }
    fprintf(stderr, "No existe el directorio %s", argv[1]);
    return -1;
}