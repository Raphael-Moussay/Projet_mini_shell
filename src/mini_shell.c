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
        char *args[] = {cmd, NULL};
        exec_cmd(cmd, args);
        free(cmd);
    }
    // command ** ens_command=decouper_cmd(cmd,&nombre_de_cmd);

    // for (int i=0;i<nombre_de_cmd;i++)
    // {
    //     printf("Commande %d récupérée : %s\n", i, ens_command[i]->name);
    //     for (int j=0;j<ens_command[i]->nb_arg;j++)
    //     {
    //         printf("%s", ens_command[i]->arg[j]);
    //     }
    //     printf("\n\n");
    //     free(*ens_command[i]->arg);
    // }
    // free(*ens_command);
    return 0;
}