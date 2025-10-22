#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decouper_cmd.h"



command **decouper_cmd(char *cmd, int *nb_cmd)
{
    char *token;


    int nb_et_ou = 0;
    token = strtok(cmd, " "); // premier while pour compter le nombre de & ou de |
    while (token != NULL) 
    {
        if (strcmp(token, "|")!=0 && strcmp(token, "&")!=0)
        {
            nb_et_ou++;
        }
        token = strtok(NULL, " ");
    }
    int * tab_count = malloc((nb_et_ou + 1)* sizeof(int)); // indice des & et |
    tab_count[0] = -1; // permet ensuite de faire le calcul du nombre d'arguments par commande
    int count = 0;
    token = strtok(cmd, " "); // deuxieme while pour compter le nombre de token et stocker les indices des & ou des |
    int i = 1;
    while (token != NULL) 
    {
        if (strcmp(token, "|")!=0 && strcmp(token, "&")!=0)
        {
            tab_count[i] = count;
            i++;
        }
        count++;
        token = strtok(NULL, " ");
    }


    int nombre_cmd = (tab_count[nb_et_ou - 1]==count-1) ? nb_et_ou : nb_et_ou + 1; // on détermine le nombre de commandes
    command *ens_des_cmd[nombre_cmd];  // on créé un tableau contenant les commandes découpées
    for (int j = 0; j < nombre_cmd; j++) {
        ens_des_cmd[j] = malloc(sizeof(command));
        ens_des_cmd[j]->arg = malloc((tab_count[j + 1] - tab_count[j]) * sizeof(char *)); // on alloue la mémoire pour les arguments de chaque commande
    }
    int indice_arg=0;
    int its_an_cmd=0;
    int indice_cmd=0;
    count = 0;
    token = strtok(cmd, " ");
    ens_des_cmd[0]->name = token;
    ens_des_cmd[0]->arg[0] = token;
    while (token != NULL) 
    {
        count++;
        token = strtok(NULL, " ");
        if (token!=NULL)
        {
            if ((strcmp(token, "|") == 0)||(strcmp(token, "&") == 0)) // si on tombe sur un & ou un |, on passe à la commande suivante
            {
                ens_des_cmd[indice_cmd]->arg[indice_arg + 1] = NULL; // on met le dernier argument à NULL
                indice_cmd++;
                its_an_cmd=1;
                indice_arg=0;
            }
            else if (its_an_cmd) // si c'est une commande, on ajoute l'argument
            {
                ens_des_cmd[indice_cmd]->arg[indice_arg] = token;
                ens_des_cmd[indice_cmd]->name=token;
                indice_arg++;
            } else if (!its_an_cmd)
            {
                ens_des_cmd[indice_cmd]->arg[indice_arg] = token;
                indice_arg++;
            }

        }
    }
    for (int j=0; j< nombre_cmd; j++)
    {
        ens_des_cmd[j]->arg[tab_count[j + 1] - tab_count[j]] = NULL;
        ens_des_cmd[j]->nb_arg = tab_count[j + 1] - tab_count[j]+1;
    }
    *nb_cmd=nombre_cmd;
    free(tab_count);
    return ens_des_cmd;
}

