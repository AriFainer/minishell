#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "minish.h"

void
save_history(){
    int buffer_start;
    //si en el paso siguiente no piso ninguna palabra todavía no llené el buffer
    buffer_start = (buffer[buffer_idx][0] == '\0') ? 0 : buffer_idx;
    for (int i=buffer_start; i!=buffer_start-1 && buffer[i][0]!='\0'; i = (i+1) % MAXHIST){
        fprintf(history,"%s",buffer[i]);
    }
}


void
print_reversed_string(char* string, int size){
    for(int i=size-1;i>=0;i--){
        fputc(string[i],stdout);
    }
    fputc('\n',stdout);
}

int
builtin_history (int argc, char **argv)
{
    int N;
    if (argc > 2) {
        fprintf(stderr, "Error: Sintaxis incorrecta del comando \"history\"\n");
        char *help_argv[] = {"help", "history"};
        builtin_help(2, help_argv);
        return EXIT_FAILURE; 
    }
    if (argc == 1){
        N = 10;
    }
    else{
        for (int i=0;argv[1][i]!='\0';i++){
            if (!isdigit(argv[1][i])){
                    fprintf(stderr, "Error: Sintaxis incorrecta del comando \"history\"; N DEBE ser un entero positivo\n");
                    char *help_argv[] = {"help", "history"};
                    builtin_help(2, help_argv);
                    return EXIT_FAILURE; 
            }
        }
        N=atoi(argv[1]);
    }
    //lee el buffer
    for (int i = (buffer_idx - 1 >= 0) ? buffer_idx - 1 : MAXHIST - 1;
         N > 0 && i != buffer_idx && buffer[i][0] != '\0';N--,
         // si i es negativo voy hasta el último lugar (por ser un buffer circular)
         i = (i - 1 >= 0) ? i - 1 : MAXHIST + i - 1){ 
            fprintf (stdout, "%s", buffer[i]);
    }
    //lee el archivo
    fseek(history, 0, SEEK_END); //muevo el indicador de posición del archivo a la última línea
    int size = ftell(history); //la cantidad de líneas
    char linea[MAXLINE]="";
    char lectura='\0';
    int j=0;
    for (int i=1; N>0 && i<size; i++,j++){
        fseek(history, size-1-i, SEEK_SET);
        if ((lectura=fgetc(history))=='\n'){
            print_reversed_string(linea, j);
            j=-1;
            N--;  
        }else{
            linea[j]=lectura;
        }
        
        
    }
    return EXIT_SUCCESS;
}