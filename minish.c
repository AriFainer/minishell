#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pwd.h>

#include "minish.h"
#include "wrappers.h"

#define GREEN "\033[1;32m"
#define RESET "\033[0m"

// habria que poner el "-" en los argumentos? y tambien agregar lo de home
#define HELP_CD      "cd [..|dir]\n\tCambia el directorio corriente de trabajo del minishell.\n"\
                     "\n\tEl directorio se cambia por el argumento dir. Si se ingresa como argumento \"-\" se vuelve al directorio"\
                     " de trabajo antrior. El directorio por defecto (cd sin argumento) es el directorio HOME.\n"\
                     "\n\tExit status: \n\tDevuelve 0 si se logra cambiar el directorio y -1 en caso contrario (error de sintaxis o no existe el directorio dir)."
#define HELP_DIR     "dir [texto/directorio]\n\tLista los archivos de un directorio. En caso de no recibir argumentos muestra los archivos del directorio actual..\n"\
                     "\nSi recibe un solo argumento y este coincide con \n"\
                     "\n\tArgumentos:\n\tDIRECTORIO  Directorio especificando de que directorio listar los archivos.\n\n\tExit Status:\n"\
                     "\tDevuelve un status exitoso, a menos que ocurra un error o se ingrese una opcion invalida."
#define HELP_EXIT    "exit [N]\n\tFinaliza el minish con N como status de retorno.\n\n\tSi N es omitido, el status de retorno es el del ultimo comando ejecutado."
#define HELP_HELP    "help [cd|dir|exit|help|history|getenv|pid|setenv|status|uid]\n\tMuestra breves resumenes sobre los comandos internos.\n\n"\
                     "\tDe recibir un parametro, provee ayuda detallada sobre el comando especificado.\n\n"\
                     "\tArgumentos:\n\tCOMANDO  Comando especificando el topico de ayuda\n\n\tExit Status:\n"\
                     "\tDevuelve un status exitoso, a menos que no se encuentre el comando o se ingrese una opcion invalida."
#define HELP_HISTORY "history [N] - muestra los ??ltimos N (10) comandos escritos."
#define HELP_GETENV  "getenv [var ...] - muestra valor de variable(s) de ambiente."
#define HELP_PID     "pid - muestra Process Id del minish."
#define HELP_SETENV  "setenv var val - agrega o cambia valor de variable de ambiente."
#define HELP_STATUS  "status - muestra status de retorno de ultimo comando ejecutado."
#define HELP_UID     "uid - muestra nombre y n??mero de usuario due??o del minish."
#define HELP_GID     "gid - muestra el grupo principal del usuario y sus grupos secundarios en caso de tenerlos."
#define HELP_UNSETENV "unsetenv [var ...] - elimina las variables de ambiente que se le indican."
#define HELP_MES      "mes [mm [YYYY]] - muestra el calendario correspondiente al mes y a??o indicados.\nDe no recibir argumentos, muestra el calendario correspondiente a la fecha actual."

struct builtin_struct builtin_arr[] = {
        { "cd", builtin_cd, HELP_CD },
        { "dir", builtin_dir, HELP_DIR},
        { "exit", builtin_exit, HELP_EXIT },
        { "help", builtin_help, HELP_HELP },
        { "history", builtin_history, HELP_HISTORY },
        { "getenv", builtin_getenv, HELP_GETENV },
        { "pid", builtin_pid, HELP_PID },
        { "setenv",builtin_setenv,HELP_SETENV},
        { "unsetenv",builtin_unsetenv,HELP_UNSETENV},
        { "status", builtin_status, HELP_STATUS },
        { "uid", builtin_uid, HELP_UID },
        { "gid", builtin_gid, HELP_GID },
        {"mes", builtin_mes, HELP_MES}, 
        { NULL, NULL, NULL }
};

char syntax_array[][MAXLINE] = {
        "cd [..|dir]",
        "dir [texto/directorio]",
        "exit[status]",
        "getenv var [var ...]",
        "gid",
        "help [comando]",
        "history [N]",
        "pid",
        "setenv var val",
        "status",
        "uid",
        "unsetenv [var ...]",
        "mes [mes [a??o]]",
        "\0" 
};

int globalstatret = 0;
char *progname;
struct sigaction oldact, newact;
char directory[MAXCWD];
char prevdirectory[MAXCWD];
char buffer[MAXHIST][MAXLINE] = {'\0'};
int buffer_idx;
char meses[][10] = {
                "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", 
                "Agosto", "Setiembre", "Octubre", "Noviembre", "Diciembre"
};
FILE *history = NULL;
char *history_map;
int history_size = 0;

void
prompt(char *ps) { // ps is the prompt string
    struct passwd *user = getpwuid(getuid());
    if (user != NULL)
        fprintf(stderr, "(%s)" GREEN " %s:%s "  RESET "> ", ps, user->pw_name , directory);
    else
        fprintf(stderr, "(%s)" GREEN " %d:%s "  RESET "> ", ps, getuid() , directory);
}

void
sigint_handler(int signum) {                    
    fprintf(stderr, "Interrupt! (signal number %d)\n", signum); // the handler for SIGINT
}

int 
main(int argc, char* argv[]) { 
    char line[MAXLINE];
    progname = argv[0];
    
    sigaction(SIGINT, NULL, &newact);           // the  previous action for SIGINT is saved in oldact
    newact.sa_handler = sigint_handler;
    sigaction(SIGINT, &newact, NULL);           // set SIGINT handler for loop
    getcwd(directory, MAXCWD);
    strcpy(prevdirectory, directory);

    char *arr_arg[MAXWORDS] = {NULL};
    buffer_idx=0;
    char *home_path;
    if ((home_path = getenv("HOME"))!=NULL){
        load_history(home_path);
    }

    for (;;) {
        prompt(progname);
        if (fgets(line, MAXLINE, stdin) == NULL) {  // EOF
            if (feof(stdin)) {
                break;      // normal EOF, break loop
            } else {
                continue;   // not EOF, read system call was interrupted, continue loop
            }
        }
        
        int cant_palabras;
        if ((cant_palabras = linea2argv(line, MAXWORDS, arr_arg)) > 0) {
            strcpy(buffer[buffer_idx],line);
            buffer_idx = (buffer_idx + 1) % MAXHIST;
            globalstatret = ejecutar(cant_palabras, arr_arg);
        }
    }

    builtin_exit(1, arr_arg);
}