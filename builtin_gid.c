#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <grp.h>
#include "minish.h"

int 
builtin_gid(int argc, char ** argv) {
    gid_t secondary_groups_list[NGROUPS_MAX]; // Es el maximo que puede tener un usuario en el SO
    gid_t g_id = getgid(); // Me da el id del grupo principal
    struct group *main_group = getgrgid(g_id);
    if(main_group == NULL){
        fprintf(stderr, "Error en el acceso al nombre de el grupo principal.");
        return -1;
    }
    struct group *secondary_group;

    int cantidad_de_grupos = getgroups(NGROUPS_MAX, secondary_groups_list);
    if (cantidad_de_grupos < 0) {
        fprintf(stderr, "Error en el acceso a los grupos secundarios del usuario.");
        return -1;
    }
    else if (cantidad_de_grupos == 0) {
        fprintf(stdout,"Grupo principal: %d \nNombre grupo principal: %s\nEl usuario no tiene grupos secundarios.", g_id, main_group->gr_name);
    }
    else {
        fprintf(stdout,"Grupo principal: %d \nNombre grupo principal: %s\nGrupos secundarios: \n", g_id, main_group->gr_name);
        for(int i=0 ; i<cantidad_de_grupos; i++){
            gid_t secondary_group_id = secondary_groups_list[i];
            secondary_group = getgrgid(secondary_group_id);
            if (secondary_group == NULL) {
                fprintf(stderr, "Error en el acceso al nombre de los grupos secundarios.");
                return -1;
            }
            fprintf(stdout,"\tId grupo: %d\n\tNombre grupo: %s\n", secondary_group_id, secondary_group->gr_name);
        }
    }
    return 0;
}