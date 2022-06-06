#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#include "minish.h"

int builtin_gid()
{
    gid_t *g_id = getgid();
    char *name = getpwuid(g_id)->pw_name;
    fprintf(stdout,"main group: %d \nsecondary groups: %s", g_id, name);
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