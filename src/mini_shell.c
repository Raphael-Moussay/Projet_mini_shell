#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include "recuperer_cmd.h"
#include "exec_cmd.h"
#include "parser_cmd.h"
#include "test_fin_pid_background.h"



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
        if (cmd == NULL || cmd[0] == '\0') {
            free(cmd);
            continue;
        }

        if (strcmp(cmd, "exit") == 0)
        {
            // Libérer toute la liste de jobs avant de quitter
            free_job_list(&job_list_head);
            free(cmd);
            return 0;
        }
        command command_parsed;
        parser_cmd(cmd, &command_parsed);

        // Vérifier si le parsing a produit une commande valide
        if (command_parsed.name != NULL) {
            exec_cmd(&command_parsed, &job_list_head);
        }

        free(cmd);
        free_cmd(&command_parsed);
    }
}