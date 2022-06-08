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

#define GREEN "\033[1;32m"
#define RESET "\033[0m"

#define HELP_CD      "cd [..|dir] - cambia de directorio corriente"
#define HELP_DIR     "dir [texto/directorio]- muestra archivos en directorio corriente, que tengan 'str'"
#define HELP_EXIT    "exit [N] - finaliza el minish con N como status de retorno.\n\nSi N es omitido, el status de retorno es el del ultimo comando ejecutado."
#define HELP_HELP    "help [cd|dir|exit|help|history|getenv|pid|setenv|status|uid]\n\nMuestra breves resumenes sobre los comandos internos.\n\n"\
                    " De recibir un parametro, provee ayuda detallada sobre el comando especificado.\n\n"\
                     "Argumentos:\n\tCOMANDO  Comando especificando el topico de ayuda\n Exit Status: \n"\
                     "\tDevuelve un status exitoso, a menos que no se encuentre el comando o se ingrese una opcion invalida."
#define HELP_HISTORY "history [N] - muestra los últimos N (10) comandos escritos"
#define HELP_GETENV  "getenv var [var] - muestra valor de variable(s) de ambiente"
#define HELP_PID     "pid - muestra Process Id del minish"
#define HELP_SETENV  "setenv var valor - agrega o cambia valor de variable de ambiente"
#define HELP_STATUS  "status - muestra status de retorno de ultimo comando ejecutado"
#define HELP_UID     "uid - muestra nombre y número de usuario dueño del minish"
#define HELP_GID     "ARRGELAR"
#define HELP_UNSETENV "ARRGELAR"

struct builtin_struct builtin_arr[] = {
        { "cd", builtin_cd, HELP_CD },
        //{ "dir", builtin_dir, HELP_DIR},
        { "exit", builtin_exit, HELP_EXIT },
        { "help", builtin_help, HELP_HELP },
        /*{ "history", builtin_history, HELP_HISTORY },*/
        { "getenv", builtin_getenv, HELP_GETENV },
        { "pid", builtin_pid, HELP_PID },
        { "setenv",builtin_setenv,HELP_SETENV},
        { "unsetenv",builtin_unsetenv,HELP_UNSETENV},
        { "status", builtin_status, HELP_STATUS },
        { "uid", builtin_uid, HELP_UID },
        { "gid", builtin_gid, HELP_GID },
        { NULL, NULL, NULL }
};

char syntax_array[][MAXLINE] = {
        "cd [..|dir]",
        "dir [texto/directorio]",
        "exit[status]",
        "getenv variable [variable ...]",
        "gid",
        "help [comando]",
        "history [N]",
        "pid",
        "setenv variable valor",
        "status",
        "uid",
        "unsetenv [var ...]",
        "\0" 
};

int globalstatret = 0;
char *progname;
struct sigaction oldact, newact;
char directory[MAXCWD];
char prevdirectory[MAXCWD];

void
prompt(char *ps) {
    // ps is the prompt string
    char *name = getpwuid(getuid())->pw_name;
    fprintf(stderr, "(%s)" GREEN " %s:%s "  RESET ">", ps, name , directory);
}

void
sigint_handler(int signum) {                    
    fprintf(stderr, "Interrupt! (signal number %d)\n", signum); // the handler for SIGINT
}

int 
main(__attribute__((unused)) int argc, char* argv[]) { // al profe dijo que no le gustaba el unused, que usaramos el argc para algo 
    
    char line[MAXLINE];
    progname = argv[0];
    
    sigaction(SIGINT, NULL, &newact);           // the  previous action for SIGINT is saved in oldact
    newact.sa_handler = sigint_handler;
    sigaction(SIGINT, &newact, NULL);           // set SIGINT handler for loop
    getcwd(directory, MAXCWD);
    strcpy(prevdirectory, directory);

    for (;;) {
        prompt(progname);
        if (fgets(line, MAXLINE, stdin) == NULL) {  // EOF
            if (feof(stdin)) {
                break;      // normal EOF, break loop
            } else {
                continue;   // not EOF, read system call was interrupted, continue loop
            }
        }
        
        
        char **arr_arg = malloc(sizeof(char*)*MAXWORDS);
        int cant_palabras;
        if ((cant_palabras = linea2argv(line, MAXWORDS, arr_arg)) > 0) {
            fprintf(stderr, "Will execute command %s\n", arr_arg[0]);
            globalstatret = ejecutar(cant_palabras, arr_arg);
        }
    }

    char *exit_argv[] = {"exit",NULL};
    builtin_exit(2, exit_argv);
}