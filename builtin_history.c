#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "minish.h"
#include "wrappers.h"

//imprime desde el indice buffer_print_start una cantidad de buffer_print_qty elementos
//del buffer al stream indicado
void
print_buffer(int buffer_print_start, int buffer_print_qty, FILE *stream){
    int lcount = 0;         // Contador de cuantas lineas voy imprimiendo
    for (int i = buffer_print_start; lcount < buffer_print_qty;i = (i + 1) % MAXHIST, lcount++){
            fprintf (stream, "%s", buffer[i]);
    }
}

void
save_history (){ //HAY QUE COMENTAR POR ACA
    // si en el paso siguiente no sobreescribo ninguna palabra todavía no llene el buffer
    int buffer_start;
    int buffer_used_qty;
    if(history == NULL){
        fprintf(stderr, "Error. No se pudo guardar el historial  de comandos.\n");
        return;
    }
    if (buffer[buffer_idx][0] == '\0') { // buffer no lleno
            buffer_start = 0;
            buffer_used_qty = buffer_idx;
    }
    else { // buffer lleno
            buffer_start = buffer_idx;
            buffer_used_qty = MAXHIST;
    }
    print_buffer(buffer_start,buffer_used_qty,history);
    if (fclose(history) != 0) {
        fprintf (stderr, "Error: Error al guardar el historial de comandos\n");
    }
}

void
load_history (char *home_path) {
    char history_path[MAXCWD];
    strcpy (history_path, home_path);
    strcat (history_path, HISTORY_FILE);
    umask(TAKEN_PERMITS);
    history = fopen_or_warn(history_path, "a+");    // Modo append y read
    if(history == NULL){
        return;
    }
    int fd = fileno (history);                      // File descriptor del history file
    struct stat s;      // Para conseguir el tamano del archivo
    fstat (fd, &s);     // Carga atributos del archivo en s
    history_size = s.st_size;
    history_map = (char *)mmap (0, history_size, PROT_READ, MAP_PRIVATE, fd, 0); // Me carga el archivo como si fuera un string
}

int
builtin_history (int argc, char **argv) {
    int N;
    if (argc > 2){
        fprintf (stderr, "Error: Sintaxis incorrecta del comando \"history\"\n");
        char *help_argv[] = { "help", "history" };
        builtin_help (2, help_argv);
        return EXIT_FAILURE;
    }

    if (argc == 1) {
            N = 10;
    }
    else {
        for (int i = 0; argv[1][i] != '\0'; i++) {
            if (!isdigit (argv[1][i])) {
                fprintf (stderr, "Error: Sintaxis incorrecta del comando \"history\"; N DEBE ser un entero positivo\n");
                char *help_argv[] = { "help", "history" };
                builtin_help (2, help_argv);
                return EXIT_FAILURE;
            }
        }
        N = atoi (argv[1]);
    }

    int buffer_print_start;
    int buffer_print_qty;
    if (buffer[buffer_idx][0] == '\0'){ // buffer no lleno
        buffer_print_qty = (N < buffer_idx) ? N : buffer_idx; // min(N,buffer_idx)
        buffer_print_start = buffer_idx - buffer_print_qty;

    }
    else {                              // buffer lleno
        buffer_print_qty = (N < MAXHIST) ? N : MAXHIST; // min(N,MAXHIST);
        buffer_print_start = (buffer_idx - buffer_print_qty >= 0) ? buffer_idx - buffer_print_qty : MAXHIST + buffer_idx - buffer_print_qty; //(por ser un buffer circular)
    }

    N-=buffer_print_qty;
    if (N > 0) {  // lee el archivo
        int i;
        for (i = history_size - 1; i >= 0; i--) {
            if (history_map[i] == '\n') {
                    N--;
            }
            if (N < 0)
                break;
        }
        fprintf (stdout, "%.*s", history_size - (i + 1), &history_map[i + 1]);
    }
    // lee el buffer
    print_buffer(buffer_print_start,buffer_print_qty,stdout);

    return EXIT_SUCCESS;
}