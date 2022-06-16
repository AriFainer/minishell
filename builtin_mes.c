#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "minish.h"


void
set_to_end_of_month(struct tm *fecha){
    for (int mes_actual=fecha->tm_mon;fecha->tm_mon==mes_actual;fecha->tm_mday++,mktime(fecha));
    fecha->tm_mday--;
    mktime(fecha);
}

int
get_wday_of_the_first(struct tm *fecha){
    set_to_end_of_month(fecha);
    return 7 + (fecha->tm_wday - fecha->tm_mday) % 7;
}

void
print_month(struct tm *end_of_month, int wday){ 
    printf("%s %d\n",meses[end_of_month->tm_mon],1900 + end_of_month->tm_year);
    printf("Su\tMo\tTu\tWe\tTh\tFr\tSa\n");
    //La primera vez me acomodo de acuerdo a donde empieza el mes
    wday++;
    for(int j = 0; j<wday; j++){
        printf("\t");
    }
    for(int i=1;i<=end_of_month->tm_mday;i++, wday = (wday+1)%7){
        if(wday == 0 && i != 1){ // Me fijo si tengo que cambiar de renglon
            printf("\n%i\t", i);
        }else{
            printf("%i\t", i); 
        }
    }
    fputc('\n',stdout);
}


int 
builtin_mes(int argc, char **argv) {


    time_t tiempo;
    time(&tiempo);
    struct tm *fecha;
    if (argc > 3) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"mes\"\n");
        char *help_argv[] = {"help", "mes"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }
    if (argc == 1) { // No me pasan argumentos
        if((fecha = localtime(&tiempo))==NULL){
            fprintf(stderr, "Error: Hubo un error en la busqueda de la fecha actual\n");
            return EXIT_FAILURE;
        }
    }
    /*if (argc == 2) { // Me pasan solo un mes
        if((fecha = localtime(&tiempo, argv[1]))!=0){
            fprintf(stderr, "Error: Hubo un error en la busqueda de la fecha actual\n");
            return EXIT_FAILURE;
        }
    }
    if (argc == 3) { // Me pasan un mes y un año
         if((fecha = localtime(&tiempo, argv[1], argv[2]))!=0){
            fprintf(stderr, "Error: Hubo un error en la busqueda de la fecha actual\n");
            return EXIT_FAILURE;
        }
    }*/

    int wday_1 = get_wday_of_the_first(fecha);
    print_month(fecha, wday_1);




    return EXIT_SUCCESS;
}