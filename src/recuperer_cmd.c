#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "recuperer_cmd.h"
#include "nouvelle_ligne.h"


char *recuperer_cmd()
{
    char *cmd = malloc(200 * sizeof(char));
    nouvelle_ligne();
    if (fgets(cmd, 200, stdin) != NULL) 
    {
        cmd[strcspn(cmd, "\n")] = '\0';  // enlève le saut de ligne
        char *cmd_good_size = malloc((strlen(cmd) + 1) * sizeof(char));
        strcpy(cmd_good_size, cmd);
        free(cmd);
        return cmd_good_size;
    }

    free(cmd);
    return NULL;
}