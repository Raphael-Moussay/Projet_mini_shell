#ifndef PARSER_CMD_H
#define PARSER_CMD_H

typedef struct command {
    char * name;
    char ** arg;
    int nb_arg;
    char separator;
    struct command * next;    
} command;

void free_cmd(command * cmd);

command * parser_cmd_multiple(char *input_cmd);

void afficher_cmd_list(command *cmd_head);

#endif // PARSER_CMD_H