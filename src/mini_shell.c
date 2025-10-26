#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "recuperer_cmd.h"
#include "exec_cmd.h"
#include "parser_cmd.h"


int main(void)
{
    char *cmd;
    //int nombre_de_cmd;
    while (1)
    {
        cmd = recuperer_cmd();
        if (strcmp(cmd, "exit") == 0)
        {
            free(cmd);
            return 0;
        }
        command command_parsed;
        parser_cmd(cmd, &command_parsed);
        exec_cmd(&command_parsed);
        free(cmd);
        free_cmd(&command_parsed);
    }
}