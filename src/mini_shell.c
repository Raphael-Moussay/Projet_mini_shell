#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "recuperer_cmd.h"
#include "decouper_cmd.h"
#include "exec_cmd.h"


int main(void)
{
    char *cmd;
    //int nombre_de_cmd;
    while (1)
    {
        cmd = recuperer_cmd();
        //printf("Commande récupérée : %s\n", cmd);
        if (strcmp(cmd, "exit") == 0)
        {
            free(cmd);
            break;
        }
        command command_parsed;
        parser_cmd(cmd, &command_parsed);
        exec_cmd(&command_parsed);
        free(cmd);
        free_cmd(&command_parsed);
    }
    return 0;
}