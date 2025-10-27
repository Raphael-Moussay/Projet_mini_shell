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
    // Initialiser la structure à un état sûr
    cmd_parsee->name = NULL;
    cmd_parsee->arg = NULL;
    cmd_parsee->nb_arg = 0;

    if (input_cmd == NULL || input_cmd[0] == '\0') {
        return; // Ne rien faire si l'entrée est vide
    }

    char input_cmd_2[strlen(input_cmd)+2];
    // si il y a un & à la fin du dernier argument, on met un espace avant
    if (input_cmd[strlen(input_cmd)-1]=='&')
    {
        for (int i=0; i<((int) strlen(input_cmd))-1; i++)
        {
            input_cmd_2[i]=input_cmd[i];
        }
        input_cmd_2[strlen(input_cmd)-1]=' ';
        input_cmd_2[strlen(input_cmd)]='&';
        input_cmd_2[strlen(input_cmd)+1]='\0';
    } else
    {
        strcpy(input_cmd_2,input_cmd);
    }
    char *input_copy = strdup(input_cmd_2);
    char *token;
    token = strtok(input_copy, " ");
    while (token != NULL) 
    {
        cmd_parsee->nb_arg++;
        token = strtok(NULL, " ");
    }
    // Allouer suffisamment d'espace pour le tableau d'arguments
    cmd_parsee->arg = malloc((cmd_parsee->nb_arg+1) * sizeof(char *)); 

    token = strtok(input_cmd_2, " ");
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