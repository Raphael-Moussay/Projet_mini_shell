#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // pour fork, execvp
#include <string.h>    // pour strlen, strcpy
#include <sys/types.h> // pour pid_t
#include <sys/wait.h>  // pour waitpid
#include "exec_cmd.h"
#include "parser_cmd.h"
#include "test_fin_pid_background.h" // pour job_t

/*******************************************************************************
 * @file exec_cmd.c
 * @brief Exécution des commandes (foreground, background, pipes, redirections).
 *
 * Contient les implémentations pour lancer des commandes simples,
 * gérer les processus en arrière-plan, construire des pipelines et
 * réaliser des redirections basiques.
 */

void exec_cmd(command *cmd, job_t **job_list_head)
{
    command *current = cmd;
    while (current != NULL)
    {
        if (current->name == NULL)
        {
            fprintf(stderr, "Syntax error: unexpected token\n");
            current = current->next;
            continue;
        }

        if (current->separator == '&')
        {
            exec_cmd_background(current, job_list_head);
            current = current->next;
            continue;
        }

        if (current->separator == '|')
        {
            int nombre_pipe = 0;
            command *pipeline_start = current;
            command *pipeline_end = current;
            int invalid_pipeline = 0; // flag pour détecter une syntaxe invalide

            if (current->next == NULL) // si il n'y a pas de commande après le pipe alors syntaxe invalide
            {
                invalid_pipeline = 1;
            }
            else
            {
                command *walker = current->next; // pour parcourir la pipeline
                while (1)
                {
                    nombre_pipe++;
                    pipeline_end = walker; // met à jour la fin de la pipeline

                    if (walker->name == NULL) // si une commande est manquante dans la pipeline
                    {
                        invalid_pipeline = 1;
                    }

                    if (walker->separator != '|' || walker->next == NULL) // fin de la pipeline
                    {
                        if (walker->separator == '|' && walker->next == NULL) // si le dernier séparateur est un pipe sans commande après c'est invalide
                        {
                            invalid_pipeline = 1;
                        }
                        break;
                    }

                    walker = walker->next; // passe à la commande suivante dans la pipeline
                }
            }

            if (invalid_pipeline) // si la syntaxe est invalide, on affiche une erreur
            {
                fprintf(stderr, "Syntax error: missing command in pipeline\n");
                current = (pipeline_end != NULL) ? pipeline_end->next : NULL;
                continue;
            }

            exec_mode mode = (pipeline_end->separator == '&') ? BACKGROUND : FOREGROUND;
            exec_cmd_pipe(pipeline_start, nombre_pipe, job_list_head, mode);
            current = pipeline_end->next;
            continue;
        }

        if (current->separator == '>')
        {
            command *target = current->next;
            if (target == NULL || target->name == NULL)
            {
                fprintf(stderr, "Syntax error: expected filename after '>'\n");
                current = (target != NULL) ? target->next : NULL;
                continue;
            }

            exec_cmd_arrow(current);
            current = target->next;
            continue;
        }

        exec_cmd_foreground(current);
        current = current->next;
    }
}

void exec_cmd_foreground(command *cmd)
{
    if (cmd->name == NULL)
    {
        fprintf(stderr, "Syntax error: missing command\n");
        return;
    }

    // Si la commande est cd ou pwd, on l'exécute directement
    if(strcmp(cmd->name, "cd") == 0)
    {
        // si la commande est cd ou cd ~, on va à la racine
        if (cmd->arg[1] == NULL || strcmp(cmd->arg[1], "~") == 0)
        {
            chdir("/");
        } else // sinon on va au répertoire spécifié
        {
            chdir(cmd->arg[1]);
        }
    } else if (strcmp(cmd->name, "pwd") == 0) // si la commande est pwd, on affiche le répertoire courant
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
            // le processus fils exécute la commande
            execvp(cmd->name, cmd->arg);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            // le processus parent attend la fin du processus fils
            waitpid(pid, NULL, 0);
        }
    }
}

void exec_cmd_background(command *cmd, job_t **job_list_head)
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
        if (strcmp(cmd->name, "cd") == 0)
        {
            printf("[%d] %d\n", job_list_head == NULL ? 1 : (*job_list_head)->id + 1, getpid());
            printf("[%d]+   Fini\t\t\t", job_list_head == NULL ? 1 : (*job_list_head)->id + 1);
            for (int i = 0; i < cmd->nb_arg; i++)
            {
                printf("%s ", cmd->arg[i]);
            }
            printf("\n");
        } else if (strcmp(cmd->name, "pwd") == 0)
        {
            printf("[%d] %d\n", job_list_head == NULL ? 1 : (*job_list_head)->id + 1, getpid());
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
            printf("[%d]+   Fini\t\t\t", job_list_head == NULL ? 1 : (*job_list_head)->id + 1);
            for (int i = 0; i < cmd->nb_arg; i++)
            {
                printf("%s ", cmd->arg[i]);
            }
            printf("\n");
        } else 
        {
            // le processus fils exécute la commande
            execvp(cmd->name, cmd->arg);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // processus parent ne fait rien (exécution en arrière-plan)
        int job_id = ajouter_job_background(pid, cmd->arg, cmd->nb_arg, job_list_head);
        printf("[%d] %d\n", job_id, pid);
    }
}

void exec_cmd_pipe(command *cmd, int nombre_pipe, job_t **job_list_head, exec_mode mode)
{
    int pipes[nombre_pipe][2]; // tableau de pipes pour pouvoir enchaîner les pipes
    
    for (int i = 0; i < nombre_pipe; i++)
    {
        if (pipe(pipes[i]) == -1) // gestion d'erreur pipe
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pids[nombre_pipe + 1]; // tableau pour stocker les PIDs des processus fils
    
    for (int i = 0; i < nombre_pipe + 1; i++)
    {
        pid_t pid = fork(); // créer un nouveau processus par commande
        
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // processus fils
            if (i > 0)
            {
                // rediriger l'entrée standard vers le pipe précédent
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < nombre_pipe)
            {
                // rediriger la sortie standard vers le pipe actuel
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // fermer tous les pipes dans le fils
            for (int j = 0; j < nombre_pipe; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // exécuter la commande
            command *current_cmd = cmd;
            
            for (int k = 0; k < i; k++)
            {
                current_cmd = current_cmd->next;
            }
            if (current_cmd == NULL || current_cmd->name == NULL)
            {
                fprintf(stderr, "Syntax error: missing command in pipeline\n");
                exit(EXIT_FAILURE);
            }
            execvp(current_cmd->name, current_cmd->arg);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else
        {
            // processus parent conserve le PID du processus qui vient d'être créé
            pids[i] = pid;
        }
    }

    // on ferme tous les pipes dans le parent
    for (int j = 0; j < nombre_pipe; j++)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    if (mode == FOREGROUND) // si il n'y a pas de & a la fin, on attend la fin de tous les processus fils
    {
        for (int i = 0; i < nombre_pipe + 1; i++)
        {
            waitpid(pids[i], NULL, 0);
        }
    }
    else if (mode == BACKGROUND) // si il y a un & a la fin, on ajoute le dernier processus à la liste des jobs en arrière-plan
    {
        // on prépare les arguments pour ajouter le job en arrière-plan

        int nb_cmd = nombre_pipe + 1;
        int nb_tokens = 0;

        command *iter = cmd;

        for (int i = 0; i < nb_cmd; i++)
        {
            nb_tokens += iter->nb_arg;
            if (i < nb_cmd - 1)
            {
                nb_tokens++; // on inclut le séparateur "|"
            }
            iter = iter->next;
        }

        char **all_args = malloc(nb_tokens * sizeof(char *));

        if (all_args == NULL) // gestion d'erreur malloc
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        iter = cmd;

        int index = 0;

        for (int i = 0; i < nb_cmd; i++)
        {
            for (int j = 0; j < iter->nb_arg; j++)
            {
                all_args[index++] = strdup(iter->arg[j]);
            }
            if (i < nb_cmd - 1)
            {
                all_args[index++] = strdup("|"); // ajouter le séparateur "|" si ce n'est pas la dernière commande
            }
            iter = iter->next;
        }

        int job_id = ajouter_job_background(pids[nombre_pipe], all_args, nb_tokens, job_list_head); // ajouter le job à la liste des jobs en arrière-plan
        
        printf("[%d] %d\n", job_id, pids[nombre_pipe]); // afficher l'ID du job et le PID

        for (int i = 0; i < nb_tokens; i++) // libérer la mémoire allouée pour les arguments
        {
            free(all_args[i]);
        }
        free(all_args);
    }
}

void exec_cmd_arrow(command *cmd)
{
    pid_t pid = fork();

    // gestion des erreurs de fork
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0)
    {
        // le processus fils utilise freopen pour rediriger la sortie standard vers le fichier spécifié
        FILE *fp = freopen(cmd->next->arg[0], "w", stdout);
        if (!fp) // gestion d'erreur freopen
        {  
            perror("freopen");
            exit(EXIT_FAILURE);
        }
        execvp(cmd->name, cmd->arg);
        perror("execvp"); // gestion d'erreur execvp
        exit(EXIT_FAILURE);
    } else
    {
        // le processus parent attend la fin du processus fils
        waitpid(pid, NULL, 0);
    }
}