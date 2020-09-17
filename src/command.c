/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "command.h"
#include "io_utils.h"
#include "error_status.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define buffer_size 1026

void
mumsh_exec_exit(const char* cmd)
{
    if (strncmp(cmd, "exit", 4) == 0) {
        mumsh_error(NORMAL_EXIT);
    }
}

/**
 * Executes command.
 *
 * Child process deals with command.
 *
 * @param   cmd     Command and arguments
 */
static void
mumsh_exec_cmd(char** cmd)
{
    int error;

    error = execvp(cmd[0], cmd);
    if (error < 0) {
        mumsh_wrong_cmd(cmd[0]);
    }
}

/**
 * Handles overall redirection.
 * 
 * @param   argv    Command and arguments
 * @param   size    Number of arguments
 * @return          Number of redirection needed
 */
static int
mumsh_redirection(char** argv, const int size)
{
    int num = 0;

    for (int i = 0; i < size; i++) {
        /* TODO: Redirection for out and append. */
    }
    mumsh_exec_cmd(argv);
    return num;
}

void
mumsh_parse_cmd(char* cmd)
{
    char*   found = NULL;
    char*   pos[buffer_size] = {0};
    int     index = 0;

    while ((found = strsep(&cmd, " \n")) != NULL) {
        if (*found == 0) {
            continue;
        }
        pos[index++] = found;
    }
    pos[index] = 0;

    if (index > 1) {
        mumsh_redirection(pos, index);
    } else {
        mumsh_exec_cmd(pos);
    }
}
