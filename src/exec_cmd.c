#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // pour fork, execvp
#include <string.h>    // pour strlen, strcpy
#include <sys/types.h> // pour pid_t
#include <sys/wait.h>  // pour waitpid
#include "parser_cmd.h"
#include "exec_cmd.h"

void exec_cmd(command *cmd)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // processus fils
        execvp(cmd->name, cmd->arg);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        // processus parent
        while (wait(NULL) > 0); // attente de tous les enfants
    }
}
