#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "minish.h"

#define COMILLAS "\"\'"
#define MAXLINE 1024
#define MAXWORDS 256

int 
linea2argv (char *linea, int argc, char **argv) {
    int j = 0; //palabra actual
    int i = 0; //tamaño de word actual
    char word[MAXLINE]={'\0'};
    char *lectura = linea;
    bool entre_comillas = false;
    int tipo_comilla = 0;
    char comillas[]=COMILLAS;
    while (strchr("\n\0",*lectura)==NULL){
        if (entre_comillas) {
            if ( comillas[tipo_comilla] == *lectura) {
                entre_comillas = 0;
                lectura++;
                continue;
            }
            word[i]=*lectura;
            lectura++;
            i++;
            continue;
        }
        if ( strchr (comillas, *lectura) != NULL) {
            entre_comillas = true;
            tipo_comilla = (*lectura==comillas[0]) ? 0 : 1;
            lectura++;
            continue;
        }
        if (isspace(*lectura)) {
            if (i>0) {word[i]='\0';argv[j++]=strdup(word);}
            if (j == argc) return j;
            lectura++;
            i=0;
            continue;
        }
	word[i]=*lectura;
        lectura++;
        i++;
    }
    if (i>0) {word[i]='\0';argv[j++]=strdup(word);}
    return j;
}