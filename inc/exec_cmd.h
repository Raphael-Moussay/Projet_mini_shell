#ifndef EXEC_CMD_H
#define EXEC_CMD_H
#include "parser_cmd.h"
#include "test_fin_pid_background.h"

typedef enum exec_mode{ 
    FOREGROUND,
    BACKGROUND
} exec_mode;

void exec_cmd(command *cmd, job_t **job_list_head);

void exec_cmd_foreground(command *cmd);

void exec_cmd_background(command *cmd, job_t **job_list_head);

void exec_cmd_pipe(command *cmd, int nombre_pipe, job_t **job_list_head, exec_mode mode);

void exec_cmd_arrow(command *cmd);

#endif /* EXEC_CMD_H */
