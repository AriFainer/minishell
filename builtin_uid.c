#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#include "minish.h"

int builtin_uid(int argc, char ** argv)
{
    uid_t *u_id = getuid();
    struct passwd *password = getpwuid(u_id);
    if(password != NULL){
        char *name = password->pw_name;
        fprintf(stdout,"userid: %d \nusername: %s", u_id, name);
        return 0;
    }
    return -1;
}