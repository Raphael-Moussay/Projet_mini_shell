#include <stdio.h>  // pour printf
#include <stdlib.h> // pour malloc, free
#include <string.h> // pour strlen, strcpy, strtok
#include "parser_cmd.h"

void free_cmd(command * cmd)
{
    free(cmd->name);
    for (int i=0; i < cmd->nb_arg; i++)
    {
        free(cmd->arg[i]);
    }
    free(cmd->arg);
}

void parser_cmd(char *input_cmd, command *cmd_parsee)
{
    cmd_parsee->nb_arg=0;
    char *input_copy = strdup(input_cmd);
    char *token;
    token = strtok(input_copy, " ");
    while (token != NULL) 
    {
        cmd_parsee->nb_arg++;
        token = strtok(NULL, " ");
    }
    // Allouer suffisamment d'espace pour le tableau d'arguments
    cmd_parsee->arg = malloc((cmd_parsee->nb_arg+1) * sizeof(char *)); 

    token = strtok(input_cmd, " ");
    cmd_parsee->name = strdup(token); // le nom de la commande est le premier token
    int i = 0;
    while (token != NULL) 
    {
        cmd_parsee->arg[i] = strdup(token);
        i++;
        token = strtok(NULL, " ");
    }
    cmd_parsee->arg[cmd_parsee->nb_arg] = NULL; // execvp a besoin d'un tableau terminé par NULL
    free (input_copy);  
}