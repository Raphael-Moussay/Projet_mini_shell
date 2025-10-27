#ifndef TEST_FIN_PID_BACKGROUND_H
#define TEST_FIN_PID_BACKGROUND_H

typedef enum {
    JOB_RUNNING,
    JOB_DONE,
    JOB_STOPPED
} job_status_t;

typedef struct job_t {
    int id;       // Numéro du job ([1], [2], ...)
    pid_t pid;    // PID du processus enfant
    char ** arg;   // Nom de la commande et arg(ex: "sleep 10 &")
    int nb_arg; // Nombre d'arguments
    job_status_t status;   // État (actif, terminé, stoppé)
    struct job_t *next;  // Pointeur vers le prochain job dans la liste
} job_t;

void free_job_list(job_t **job_list_head);

void test_fin_pid_background(job_t **job_list_head);

int ajouter_job_background(pid_t pid, char **arg, int nb_arg, job_t **job_list_head);

#endif /* TEST_FIN_PID_BACKGROUND_H */
