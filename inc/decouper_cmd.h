#ifndef DECOUPER_CMD_H
#define DECOUPER_CMD_H


typedef struct {
    char * name;
    char ** arg;
    int nb_arg;
}command;

command **decouper_cmd(char *cmd, int *nb_cmd);

#endif /* DECOUPER_CMD_H */
