#include <stdlib.h>
#include <string.h>

#include "minish.h"

struct builtin_struct 
*builtin_lookup(char *cmd) {
    struct builtin_struct *function;
    int i;
    for (i = 0; builtin_arr[i].cmd != NULL && strcmp(cmd, builtin_arr[i].cmd) != 0; i++);
    if (builtin_arr[i].cmd == NULL) {
        function = NULL;
    }
    else {
        function = &builtin_arr[i];
    }
    return function;
}