#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>


#define BLOCK "\"\'"
#define MAXLINE 1024
#define MAXWORDS 256
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"


int linea2argv (char *linea, int argc, char **argv){
    int j = 0; //palabra actual
    int i = 0; //tamaño de word actual
    char word[MAXLINE]={'\0'};
    char *lectura = linea;
    bool entre_comillas = false;
    int tipo_comilla = 0;
    char block[]=BLOCK;
    while (strchr("\n\0",*lectura)==NULL){
        if (entre_comillas) {
            if ( block[tipo_comilla] == *lectura) {
                entre_comillas = 0;
                lectura++;
                continue;
            }
            word[i]=*lectura;
            lectura++;
            i++;
            continue;
        }
        if ( strchr (block, *lectura) != NULL) {
            entre_comillas = 1;
            tipo_comilla = (int)(strchr (block, *lectura)-&block[0]);
            lectura++;
            continue;
        }
        if (isspace(*lectura)) {
            if (i>0) {word[i]='\0';argv[j++]=strdup(word);}
            lectura++;
            i=0;
            continue;
        }
	word[i]=*lectura;
        lectura++;
        i++;
    }
    if (i>0) argv[j++]=strdup(word);
    return j;
}

void
prompt(char *ps) {
    // ps is the prompt string
    char direccion[MAXLINE]={0};
    getcwd(direccion,MAXLINE);
    fprintf(stderr, "(%s)" YELLOW "  %s "  RESET ">", ps, direccion);
}


void
sigint_handler(int signum) {                    // the handler for SIGINT
    fprintf(stderr, "Interrupt! (signal number %d)\n", signum);
}

int
main(int argc, char *argv[]) {
    char line[MAXLINE];
    char *progname = argv[0];
    struct sigaction oldact, newact;
    
    sigaction(SIGINT, NULL, &newact);           // the  previous action for SIGINT is saved in oldact
    newact.sa_handler = sigint_handler;
    sigaction(SIGINT, &newact, NULL);           // set SIGINT handler for loop


    for (;;) {
        prompt(progname);
        if (fgets(line, MAXLINE, stdin) == NULL) {  // EOF

            if (feof(stdin)) {
                break;      // normal EOF, break loop
            } else {
                continue;   // not EOF, read system call was interrupted, continue loop
            }
        }

        fprintf(stderr, "Will execute command %s", line);
	char **arr_arg=malloc(sizeof(char*)*MAXWORDS);
	int cant_palabras;
        if (cant_palabras = linea2argv(line,MAXWORDS,arr_arg)>0) {
	    if (strcmp(arr_arg[0],"cd")==0){
		chdir(arr_arg[1]);

	    }
            else{
            pid_t pid;                          // process ID: an unsigned integer type
            int wait_status;                    // wait status will be filled by waitpid syscall

            fprintf(stderr, "Will fork %s command\n",arr_arg[0]);

            sigaction(SIGINT, NULL, &oldact);   // the  previous action for SIGINT is saved in oldact
            newact = oldact;

            if ((pid = fork()) < 0) {           // fork error, i.e. too many processes
                error(0, errno, "fork error\n"); // will fprintf the error and go on
            }

            else if (pid == 0) {                 // child process


                newact.sa_handler = SIG_DFL;
                sigaction(SIGINT, &newact, NULL);   // reset SIGINT default for child
                execvp(arr_arg[0],arr_arg);
                error(EXIT_FAILURE, errno, "execvp error\n"); // if exec not successful, just exit child
            }

            else {                               // pid > 0: parent (shell) process


                newact.sa_handler = SIG_IGN;
                sigaction(SIGINT, &newact, NULL);   // ignore SIGINT while waiting

                waitpid(pid, &wait_status, 0);


                sigaction(SIGINT, &oldact, NULL);   // restore SIGINT when child finishes

                fprintf(stderr, "Ended child process\n");

                // do something with wait_status
            }
        }}
    }

    fputc('\n', stderr);
    fprintf(stderr, "Exiting %s ...\n", progname);
    exit(EXIT_SUCCESS);
}
