#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // pour fork, execvp
#include <string.h>    // pour strlen, strcpy
#include <sys/types.h> // pour pid_t
#include <sys/wait.h>  // pour waitpid
#include "exec_cmd.h"
#include "parser_cmd.h"
#include "test_fin_pid_background.h" // pour job_t

void exec_cmd(command *cmd, job_t **job_list_head)
{
    int etat=FOREGROUND;

    // S'il y a un & à la fin des arguments, c'est une commande en arrière-plan
    if (strcmp(cmd->arg[cmd->nb_arg-1], "&") == 0)
    {
        etat=BACKGROUND;
        cmd->arg[cmd->nb_arg-1]=NULL; // enlever le & des arguments
        cmd->nb_arg--;
    }

    // Si la commande est cd ou pwd, on l'exécute directement
    if(strcmp(cmd->name, "cd") == 0)
    {
        if (cmd->arg[1] == NULL || strcmp(cmd->arg[1], "~") == 0)
        {
            chdir("/");
        } else 
        {
            chdir(cmd->arg[1]);
        }
    } else if (strcmp(cmd->name, "pwd") == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }

    // Si la commande n'est ni cd ni pwd, on la lance dans un nouveau processus
    else
    {
        pid_t pid = fork();

        // gestion des erreurs de fork
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // processus fils
            if (etat == BACKGROUND)
            {
                // détacher le processus fils du terminal
                setsid();
            }
            execvp(cmd->name, cmd->arg);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (etat == FOREGROUND)
            {
                // processus parent attend la fin du processus fils
                waitpid(pid, NULL, 0);
            } else if (etat == BACKGROUND)
            {
                // processus parent ne fait rien (exécution en arrière-plan)
                int job_id = ajouter_job_background(pid, cmd->arg, cmd->nb_arg, job_list_head);
                printf("[%d] %d\n", job_id, pid);
            }
        }
    }
    
}
