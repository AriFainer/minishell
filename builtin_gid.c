#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>

#include "minish.h"

int builtin_gid(int argc, char ** argv)
{
    gid_t secondary_groups_list[NGROUPS_MAX]; // Es el maximo que puede tener un usuario en el SO
    gid_t *g_id = getgid(); // Me da el id del grupo principal
    
    int cantidad_de_grupos = getgroups(NGROUPS_MAX, secondary_groups_list);
    if(cantidad_de_grupos < 0){
        fprintf(stderr, "Error en el acceso a los grupos secundarios del usuario.");
        return -1;
    }else if (cantidad_de_grupos == 0){
        fprintf(stdout,"Grupo principal: %d \nEl usuario no tiene grupos secundarios.", g_id);
    }else{

        fprintf(stdout,"Grupo principal: %d \nGrupos secundarios: \n", g_id);
        for(int i=0 ; i<cantidad_de_grupos; i++){
            fprintf(stdout,"\t%s\n", secondary_groups_list[i]);
        }
    }
    return 0;

}