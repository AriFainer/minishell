#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "minish.h"

int 
builtin_uid(int argc, char ** argv) {
    if (argc > 1) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"uid\"\n");
        char *help_argv[] = {"help", "uid"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }
    uid_t u_id = getuid();
    struct passwd *password = getpwuid(u_id);
    if (password != NULL) {
        char *name = password->pw_name;
        fprintf(stdout,"userid: %d \nusername: %s\n", u_id, name);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
