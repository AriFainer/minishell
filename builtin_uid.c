#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include "minish.h"

int builtin_uid()
{
    uid_t *u_id = getuid();
    char *name = getpwuid(u_id)->pw_name;
    fprintf(stdout,"userid: %d \nusername: %s", u_id, name);
    return 0;

    /* ARREGLAR ESTO. CONTROLAR EL ERROR
    if(){
        fprintf(stdout,"userid: %d \nusername: %s", u_id, name);

            return 0; // Nunca tira error
    }else{
        fprintf(stderr, "ERROR: No existe el usuario. ");
        return -1;
    }*/

}