#ifndef EXEC_CMD_H
#define EXEC_CMD_H
#include "parser_cmd.h"
#include "test_fin_pid_background.h"

/**
 * @file exec_cmd.h
 * @brief Fonctions d'exécution des commandes du mini-shell.
 *
 * Ce fichier expose les fonctions permettant d'exécuter des commandes
 * simples, en arrière-plan, avec redirections et avec pipes.
 */

/**
 * @enum exec_mode
 * @brief Mode d'exécution pour les pipelines.
 *
 * FOREGROUND: attendre la fin des processus
 * BACKGROUND: lancer en arrière-plan
 */
typedef enum exec_mode{
    FOREGROUND,
    BACKGROUND
} exec_mode;

/**
 * @brief Exécute une liste de commandes (chaînées) en tenant compte des séparateurs.
 *
 * Parcourt la liste de commandes fournie par le parser et appelle
 * la fonction d'exécution appropriée selon le séparateur (&, |, >, ...).
 *
 * @param cmd Liste chaînée de commandes à exécuter (peut contenir plusieurs commandes liées par des séparateurs).
 * @param job_list_head Pointeur vers la tête de la liste des jobs en arrière-plan.
 */
void exec_cmd(command *cmd, job_t **job_list_head);

/**
 * @brief Exécute une commande au premier plan.
 *
 * Traite les commandes internes (cd, pwd) et les commandes externes via fork/exec.
 * @param cmd Pointeur vers la commande à exécuter.
 */
void exec_cmd_foreground(command *cmd);

/**
 * @brief Exécute une commande en arrière-plan.
 *
 * Lance la commande dans un fils sans attendre sa terminaison, puis
 * ajoute le job à la liste des jobs en arrière-plan.
 * @param cmd Pointeur vers la commande à exécuter.
 * @param job_list_head Pointeur vers la tête de la liste des jobs en arrière-plan.
 */
void exec_cmd_background(command *cmd, job_t **job_list_head);

/**
 * @brief Exécute une série de commandes reliées par des pipes.
 *
 * Crée les pipes nécessaires, fork/exec pour chaque maillon et
 * gère le comportement foreground/background selon @p mode.
 *
 * @param cmd Pointeur vers la première commande de la chaîne de pipes.
 * @param nombre_pipe Nombre de pipes (nombre de séparations '|').
 * @param job_list_head Pointeur vers la tête de la liste des jobs en arrière-plan.
 * @param mode Mode d'exécution (FOREGROUND ou BACKGROUND).
 */
void exec_cmd_pipe(command *cmd, int nombre_pipe, job_t **job_list_head, exec_mode mode);

/**
 * @brief Exécute une redirection simple de type 'commande > fichier'.
 *
 * @param cmd La commande dont le champ next contient le fichier de sortie.
 */
void exec_cmd_arrow(command *cmd);

#endif /* EXEC_CMD_H */
