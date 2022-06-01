#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "minish.h"

extern int externo(__attribute__((unused)) int argc, char **argv){
    pid_t pid;       // process ID: an unsigned integer type
    int wait_status; // wait status will be filled by waitpid syscall
    int return_value = 0; //status

    fprintf(stderr, "Will fork %s command\n", argv[0]);

    sigaction(SIGINT, NULL, &oldact); // the  previous action for SIGINT is saved in oldact
    newact = oldact;

    if ((pid = fork()) < 0)
    {                                    // fork error, i.e. too many processes
        error(0, errno, "fork error\n"); // will fprintf the error and go on
    }

    else if (pid == 0)
    { // child process

        newact.sa_handler = SIG_DFL;
        sigaction(SIGINT, &newact, NULL); // reset SIGINT default for child
        return_value = execvp(argv[0], argv);
        error(EXIT_FAILURE, errno, "execvp error\n"); // if exec not successful, just exit child
    }

    else
    { // pid > 0: parent (shell) process

        newact.sa_handler = SIG_IGN;
        sigaction(SIGINT, &newact, NULL); // ignore SIGINT while waiting

        waitpid(pid, &wait_status, 0);

        sigaction(SIGINT, &oldact, NULL); // restore SIGINT when child finishes

        fprintf(stderr, "Ended child process\n");

        // do something with wait_status

        return return_value;
    }
    return return_value;
}