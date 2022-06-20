#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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
    return (7 + (fecha->tm_wday - fecha->tm_mday + 1) % 7)%7;
}

void
print_month(struct tm *end_of_month, int wday){ 
    printf("\t\t    %s %d\n",meses[end_of_month->tm_mon],1900 + end_of_month->tm_year);
    printf("Su\tMo\tTu\tWe\tTh\tFr\tSa\n");
    //La primera vez me acomodo de acuerdo a donde empieza el mes
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
mes_syntax_error(char* msg) {
    fprintf(stderr, "Error: Sintaxis incorrecta del comando \"mes\"\n%s", msg);
    char *help_argv[] = {"help", "mes"};
    builtin_help(2, help_argv);
    return EXIT_FAILURE;
}

int 
builtin_mes(int argc, char **argv) {

    time_t tiempo;
    time(&tiempo);
    struct tm *fecha;
    if (argc > 3) {
        return mes_syntax_error(""); 
    }
    
    if((fecha = localtime(&tiempo))==NULL){
        fprintf(stderr, "Error: Hubo un error en la busqueda de la fecha actual\n");
        return EXIT_FAILURE;
    }

    if (argc >= 2) { // Me pasan mes
        for (int i = 0; argv[1][i] != '\0'; i++) {
            if (!isdigit (argv[1][i])) {
                return mes_syntax_error("mm debe ser un entero positivo entre 1 y 12\n"); 
            }
        }

        int mm = atoi (argv[1]);
        if (mm < 1 || mm > 12) {
            return mes_syntax_error("mm debe ser un número entre 1 y 12\n"); 
        }
        fecha ->tm_mon = mm-1;
        
        if(argc == 3) { // Me pasan mes y año
           for (int i = 0; argv[2][i] != '\0'; i++) {
                if (!isdigit (argv[2][i])) {
                    return mes_syntax_error("YYYY DEBE ser un entero positivo\n");
                }
            }
            int YYYY = atoi (argv[2]) - 1900;
            fecha ->tm_year = YYYY;
        }
        mktime(fecha);
    }

    int wday_1 = get_wday_of_the_first(fecha);
    print_month(fecha, wday_1);
    return EXIT_SUCCESS;
}