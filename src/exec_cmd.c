#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // pour fork, execvp
#include <string.h>    // pour strlen, strcpy
#include <sys/types.h> // pour pid_t
#include <sys/wait.h>  // pour waitpid
#include "exec_cmd.h"
#include "parser_cmd.h"

void exec_cmd(command *cmd)
{
    if(strcmp(cmd->name, "cd") == 0)
    {
        chdir(cmd->arg[1]);
    } else if (strcmp(cmd->name, "pwd") == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    } else
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
    
}
