#include <stdio.h>
#include <stdlib.h>
#include "minish.h"

int builtin_exit(int argc, char ** argv)
{
    char *retorno = argv[1];
    if(retorno == NULL){
        fprintf(stderr, "Exiting %s ...\n", progname);
        exit(globalstatret);
    }else{
        fprintf(stderr, "Exiting %s ...\n", progname);
        exit(retorno);
    }
    
    return -1; //Nunca se deberia llegar a este punto
}