#ifndef EXEC_CMD_H
#define EXEC_CMD_H
#include "parser_cmd.h"
#include "test_fin_pid_background.h"

typedef enum { 
    FOREGROUND,
    BACKGROUND,
    PIPE
} exec_mode;

void exec_cmd(command *cmd, job_t **job_list_head);

#endif /* EXEC_CMD_H */
