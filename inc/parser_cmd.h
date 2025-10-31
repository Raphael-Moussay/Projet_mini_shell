#ifndef PARSER_CMD_H
#define PARSER_CMD_H

/**
 * @file parser_cmd.h
 * @brief Parser minimal pour découper une ligne de commande en structures.
 *
 * Le parser produit une liste chaînée de @c command, chaque élément représentant
 * une portion de la ligne séparée par des opérateurs (&, |, >, ...).
 */

/**
 * @struct command
 * @brief Représente une commande et ses arguments.
 *
 * - @c name : nom de la commande (ex: "ls").
 * - @c arg  : tableau d'arguments, terminé par NULL.
 * - @c nb_arg : nombre d'arguments (sans compter le NULL final).
 * - @c separator : caractère séparateur qui a suivi la commande ('&', '|', '>', ';' ou '\0').
 * - @c next : pointeur vers la commande suivante dans la chaîne.
 */
typedef struct command {
    char * name;
    char ** arg;
    int nb_arg;
    char separator;
    struct command * next;
} command;

/**
 * @brief Libère une liste chaînée de commandes allouées par le parser.
 * @param cmd_head Pointeur vers la tête de la liste à libérer.
 */
void free_cmd(command * cmd);

/**
 * @brief Parse une ligne et retourne une liste de commandes.
 *
 * La fonction duplique et découpe la chaîne d'entrée. Chaque élément de la
 * liste contient les arguments séparés, le nom de la commande et le séparateur
 * qui suit.
 *
 * @param input_cmd Chaîne de caractères contenant la ligne de commande.
 * @return Pointeur vers la tête de la liste de commandes (NULL si vide ou erreur).
 */
command * parser_cmd_multiple(char *input_cmd);

/**
 * @brief Affiche (pour le débogage) la liste de commandes parsée.
 * @param cmd_head Pointeur vers la tête de la liste à afficher.
 */
void afficher_cmd_list(command *cmd_head);

#endif // PARSER_CMD_H