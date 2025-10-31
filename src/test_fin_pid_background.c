#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "test_fin_pid_background.h"

/**
 * @file test_fin_pid_background.c
 * @brief Gestion et suivi des processus lancés en arrière-plan.
 *
 * Implémente l'ajout de jobs en arrière-plan, la vérification de leur
 * terminaison et la libération de la liste des jobs.
 */

void free_job_list(job_t **job_list_head)
{
    job_t *current = *job_list_head;
    
    while (current != NULL) 
    {
        job_t *next = current->next;
        // Libérer les arguments dupliqués
        for (int i = 0; i < current->nb_arg; i++) 
        {
            free(current->arg[i]);
        }
        free(current->arg);
        free(current);
        current = next;
    }
}

void test_fin_pid_background(job_t **job_list_head)
{
    int status;

    pid_t pid = waitpid(-1, &status, WNOHANG); // vérifier si un processus enfant s'est terminé (-1 veut dire pour tous les enfants) (WNOHANG pour ne pas bloquer)
    
    // parcours de la liste des jobs pour trouver le job correspondant au pid
    job_t *current = *job_list_head;

    while (current != NULL)
    {
        if (current->pid == pid)
        {
            current->status = JOB_DONE;
            printf("[%d]\tFini\t\t\t", current->id);
            for (int i = 0; i < current->nb_arg; i++)
            {
                printf("%s ", current->arg[i]);
            }
            printf("\n");
            break;
        }
        current = current->next;
    }
    int test_all_done = JOB_DONE;
    current = *job_list_head;
    while (current != NULL)
    {
        if (current->status == JOB_RUNNING)
        {
            test_all_done = JOB_RUNNING;
            break;
        }
        current = current->next;
    }
    if (test_all_done == JOB_DONE)
    {
        // si tous les jobs sont terminés, on libère la liste
        free_job_list(job_list_head);
        *job_list_head = NULL;
    }
}

int ajouter_job_background(pid_t pid, char **arg, int nb_arg, job_t **job_list_head)
{
    
    job_t *new_job = malloc(sizeof(job_t));

    if (new_job == NULL) // gestion d'erreur malloc
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    if (*job_list_head == NULL)
    {
        new_job->id = 1;
    } else
    {
        new_job->id = (*job_list_head)->id + 1; // incrémenter l'ID du job
    }
    new_job->pid = pid;
    // Dupliquer les arguments pour que le job ait sa propre copie
    new_job->arg = malloc((nb_arg + 1) * sizeof(char *));
    if (new_job->arg == NULL) // gestion d'erreur malloc
    {
        perror("malloc");
        free(new_job);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nb_arg; i++) 
    {
        new_job->arg[i] = strdup(arg[i]);
    }
    new_job->arg[nb_arg] = NULL; // terminer le tableau d'arguments par NULL
    new_job->nb_arg = nb_arg;
    new_job->status = JOB_RUNNING;
    new_job->next = *job_list_head;
    *job_list_head = new_job;

    return new_job->id;
}