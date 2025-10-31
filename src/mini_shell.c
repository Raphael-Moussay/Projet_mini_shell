#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include "recuperer_cmd.h"
#include "exec_cmd.h"
#include "parser_cmd.h"
#include "test_fin_pid_background.h"

/**
 * @file mini_shell.c
 * @brief Boucle principale du mini-shell interactif.
 *
 * Ce fichier contient la fonction main() qui initialise la liste des jobs
 * en arrière-plan, récupère les commandes utilisateur, les parse et les
 * exécute. La commande spéciale "exit" termine proprement le shell.
 */


/**
 * @brief Point d'entrée du mini-shell.
 *
 * Lit les commandes de l'utilisateur en boucle, met à jour la liste des jobs
 * en arrière-plan via test_fin_pid_background(), et exécute les commandes
 * parsées. Retourne 0 en cas de sortie normale.
 *
 * @return int Code de sortie (0 = OK).
 */

int main(void)
{
    char *cmd;

    job_t *job_list_head = NULL;

    while (1)
    {
        if (job_list_head != NULL)
        {
            test_fin_pid_background(&job_list_head);
        }

        cmd = recuperer_cmd();

        // Ignorer la commande si elle est vide
        if (cmd == NULL || cmd[0] == '\0') 
        {
            free(cmd);
            continue;
        }

        if (strcmp(cmd, "exit") == 0)  // quitter le mini-shell
        {
            // Libérer toute la liste de jobs avant de quitter
            free_job_list(&job_list_head);
            free(cmd);
            return 0;
        }

        command *command_list_head = parser_cmd_multiple(cmd);
        
        // Vérifier si le parsing a produit une commande valide
        if (command_list_head != NULL) 
        {
            exec_cmd(command_list_head, &job_list_head);
        }

        free(cmd);
        free_cmd(command_list_head);
    }
}