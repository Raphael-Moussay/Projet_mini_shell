#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "nouvelle_ligne.h"

/*******************************************************************************
 * @file nouvelle_ligne.c
 * @brief Affichage du prompt du mini-shell avec couleurs ANSI.
 *
 * Fournit la fonction nouvelle_ligne() qui affiche l'invite contenant
 * l'utilisateur, le nom d'hôte et le répertoire courant.
 */

// Définitions des codes ANSI pour la lisibilité
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_BOLD     "\x1b[1m"
#define ANSI_COLOR_RESET    "\x1b[0m" // Réinitialisation


void nouvelle_ligne()
{
    char hostname[1024];
    if (gethostname(hostname, 1024) != 0) 
    {
        // En cas d'erreur
        snprintf(hostname, 1024, "localhost");
    }
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s%s%s@%s%s", ANSI_COLOR_GREEN, ANSI_COLOR_BOLD, getenv("USER"), hostname, ANSI_COLOR_RESET);
    printf(":");
    printf("%s%s%s%s", ANSI_COLOR_RED, ANSI_COLOR_BOLD, cwd, ANSI_COLOR_RESET);
    printf("$ ");
}