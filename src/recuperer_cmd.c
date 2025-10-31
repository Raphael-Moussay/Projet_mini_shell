#include <stdio.h> // pour printf, fgets
#include <string.h> // pour strlen, strcpy, strcspn
#include <stdlib.h> // pour malloc, free, exit
#include "recuperer_cmd.h" // pour la déclaration de la fonction
#include "nouvelle_ligne.h" // pour nouvelle_ligne()

/**
 * @file recuperer_cmd.c
 * @brief User input collection utilities for the shell prompt.
 */

char *recuperer_cmd()
{
    char *cmd = malloc(200 * sizeof(char));

    if (cmd == NULL) // gestion d'erreur malloc
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    nouvelle_ligne();

    if (fgets(cmd, 200, stdin) != NULL) // lit la commande entrée par l'utilisateur
    {
        cmd[strcspn(cmd, "\n")] = '\0';  // enlève le saut de ligne
        char *cmd_good_size = malloc((strlen(cmd) + 1) * sizeof(char)); // allocation de la taille exacte

        if (cmd_good_size == NULL) // gestion d'erreur malloc
        {
            perror("malloc");
            free(cmd);
            exit(EXIT_FAILURE);
        }

        strcpy(cmd_good_size, cmd); // copie la commande dans la taille exacte
        free(cmd);
        return cmd_good_size;
    }

    free(cmd);
    return NULL;
}

void afficher_cmd(char *cmd)
{
    printf("Commande récupérée : %s\n", cmd);
}