#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#include "minish.h"

int builtin_gid(int argc, char ** argv)
{
    gid_t *g_id = getgid();
    struct passwd *password = getpwuid(g_id);
    if(password != NULL){
        char *name = password->pw_name;
        fprintf(stdout,"Main group: %d \nSecondary groups: %s", g_id, name);
        return 0;
    }
    return -1;

}