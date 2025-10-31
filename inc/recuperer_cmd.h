#ifndef RECUPERER_CMD_H
#define RECUPERER_CMD_H

/**
 * @file recuperer_cmd.h
 * @brief Fonctions de lecture de la ligne de commande utilisateur.
 */

/**
 * @brief Lit une ligne depuis l'entrée standard et la retourne allouée dynamiquement.
 *
 * La chaîne retournée doit être libérée par l'appelant via free().
 * En cas d'erreur ou d'EOF, la fonction peut retourner NULL.
 *
 * @return Pointeur vers une chaîne allouée contenant la commande sans le '\n', ou NULL.
 */
char *recuperer_cmd();

/**
 * @brief Affiche une commande (pour le débogage).
 * @param cmd Chaîne contenant la commande à afficher.
 */
void afficher_cmd(char *cmd);

#endif /* RECUPERER_CMD_H */

