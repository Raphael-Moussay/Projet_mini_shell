#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser_cmd.h"

void free_cmd(command *cmd_head) 
{
    command *current = cmd_head;
    command *next;
    while (current != NULL) 
    {
        next = current->next;
        if (current->name) 
        {
            free(current->name);
        }
        if (current->arg) {
            for (int i = 0; i < current->nb_arg; i++) 
            {
                if (current->arg[i]) 
                {
                    free(current->arg[i]);
                }
            }
            free(current->arg);
        }
        free(current);
        current = next;
    }
}

command *parser_cmd_multiple(char *input_cmd) 
{
    if (input_cmd == NULL || *input_cmd == '\0') 
    {
        return NULL;
    }

    char *cmd_copy = strdup(input_cmd);
    char *saveptr_cmd; // Pointeur de sauvegarde pour le strtok_r externe (découpe par commandes)

    command *head = NULL;
    command *tail = NULL;

    // Première passe : découpe la chaîne par les séparateurs de commandes
    char *token_cmd = strtok_r(cmd_copy, "&|;>", &saveptr_cmd);

    while (token_cmd != NULL) {
        command *new_cmd = malloc(sizeof(command));
        if (!new_cmd) {
            perror("malloc");
            free(cmd_copy);
            free_cmd(head);
            return NULL;
        }
        new_cmd->next = NULL;

        // Détermine le séparateur qui suivait ce token
        char separator = '\0';

        // Calcule l'index du caractère juste après le token actuel
        size_t end_of_token_offset = (token_cmd - cmd_copy) + strlen(token_cmd);
        
        // Si cet index est dans les limites de la chaîne originale, on lit le séparateur
        if (end_of_token_offset < strlen(input_cmd)) {
            separator = input_cmd[end_of_token_offset];
        }
        new_cmd->separator = separator;

        // --- Début du parsing des arguments pour le token actuel ---
        char *arg_copy = strdup(token_cmd);
        char *saveptr_arg; // Pointeur de sauvegarde pour le strtok_r interne (découpe par arguments)
        int nb_arg = 0;

        // Copie pour compter les arguments
        char *count_copy = strdup(token_cmd);
        char *p = count_copy;
        while (strtok_r(p, " \t\n", &p)) {
            nb_arg++;
        }
        free(count_copy);

        new_cmd->nb_arg = nb_arg;
        if (nb_arg > 0) {
            new_cmd->arg = malloc((nb_arg + 1) * sizeof(char *));
            
            int i = 0;
            char *token_arg = strtok_r(arg_copy, " \t\n", &saveptr_arg);
            while (token_arg != NULL) {
                new_cmd->arg[i++] = strdup(token_arg);
                token_arg = strtok_r(NULL, " \t\n", &saveptr_arg);
            }
            new_cmd->arg[i] = NULL;
            new_cmd->name = strdup(new_cmd->arg[0]);
        } else {
            new_cmd->arg = NULL;
            new_cmd->name = NULL;
        }
        free(arg_copy);
        // --- Fin du parsing des arguments ---

        // Ajoute la nouvelle commande à la liste chaînée
        if (head == NULL) {
            head = new_cmd;
            tail = new_cmd;
        } else {
            tail->next = new_cmd;
            tail = new_cmd;
        }

        // Passe au token de commande suivant
        token_cmd = strtok_r(NULL, "&|;>", &saveptr_cmd);
    }

    free(cmd_copy);
    return head;
}


void afficher_cmd_list(command *cmd_head) 
{
    command *current = cmd_head;
    while (current != NULL) 
    {
        printf("Commande: (%s)\n", current->name);
        printf("Arguments (%d): ", current->nb_arg);
        for (int i = 0; i < current->nb_arg; i++) 
        {
            printf("(%s) ", current->arg[i]);
        }
        printf("\nSéparateur: (%c)\n", current->separator ? current->separator : ' ');
        printf("-----\n");
        current = current->next;
    }
}