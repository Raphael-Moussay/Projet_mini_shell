#ifndef TEST_FIN_PID_BACKGROUND_H
#define TEST_FIN_PID_BACKGROUND_H

#include <sys/types.h>

/**
 * @file test_fin_pid_background.h
 * @brief Gestion simple des jobs lancés en arrière-plan.
 */

/**
 * @enum job_status_t
 * @brief État possible d'un job en arrière-plan.
 */
typedef enum job_status_t {
    JOB_RUNNING,
    JOB_DONE,
    JOB_STOPPED
} job_status_t;

/**
 * @struct job_t
 * @brief Représente un job en arrière-plan.
 *
 * - @c id : identifiant du job (affiché entre crochets).
 * - @c pid : PID du processus fils associé.
 * - @c arg : copie des arguments de la commande lancée.
 * - @c nb_arg : nombre d'arguments (sans compter le NULL final).
 * - @c status : état actuel du job.
 * - @c next : pointeur vers le job suivant dans la liste chaînée.
 */
typedef struct job_t {
    int id;
    pid_t pid;
    char ** arg;
    int nb_arg;
    job_status_t status;
    struct job_t *next;
} job_t;

/**
 * @brief Libère toute la liste des jobs et leur mémoire.
 * @param job_list_head Pointeur vers la tête de la liste de jobs.
 */
void free_job_list(job_t **job_list_head);

/**
 * @brief Teste si des processus en arrière-plan sont terminés.
 *
 * Parcourt la liste des jobs et met à jour leur état si leur PID a terminé.
 * Affiche un message pour chaque job terminé.
 * @param job_list_head Pointeur vers la tête de la liste de jobs.
 */
void test_fin_pid_background(job_t **job_list_head);

/**
 * @brief Ajoute un nouveau job en arrière-plan à la liste.
 *
 * Duplique les arguments pour que la liste de jobs garde sa propre copie.
 * @param pid PID du processus lancé en arrière-plan.
 * @param arg Tableau d'arguments utilisé pour afficher le job.
 * @param nb_arg Nombre d'arguments.
 * @param job_list_head Pointeur vers la tête de la liste de jobs.
 * @return L'identifiant du job créé.
 */
int ajouter_job_background(pid_t pid, char **arg, int nb_arg, job_t **job_list_head);

#endif /* TEST_FIN_PID_BACKGROUND_H */
