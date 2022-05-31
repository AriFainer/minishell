#include <stdlib.h>
#include <stdio.h>
#include <error.h>

#include "minish.h"

int builtin_getenv (int argc, char ** argv){
    char* env;
    for(int i = 1; i < argc;i++){
        if ((env=getenv(argv[i]))!=NULL) fprintf(stdout, "%s: %s\n",argv[i],env) ;
        else fprintf(stderror,"No existe la variable de ambiente %s",argv[i])
    }
    return 0;
}