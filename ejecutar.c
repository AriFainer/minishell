#include <stdlib.h>
#include "minish.h"

int 
ejecutar (int argc, char **argv) {
    // El valor de retorno es el status de retorno del comando.
    char *cmd = argv[0];
    struct builtin_struct *builtin = builtin_lookup(cmd);
    if (builtin != NULL) { // Ejecuto el comando interno
        return builtin->func(argc, argv);
    } 
    return externo(argc, argv); // Invoco a externo()
}