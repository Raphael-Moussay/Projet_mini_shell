#ifndef PARSER_CMD_H
#define PARSER_CMD_H

typedef struct {
    char * name;
    char ** arg;
    int nb_arg;
}command;


void parser_cmd(char *input_cmd, command *cmd_parsee);

void free_cmd(command * cmd);

#endif // PARSER_CMD_H