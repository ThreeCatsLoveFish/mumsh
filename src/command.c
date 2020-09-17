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

void
mumsh_exec_cmd(char* cmd)
{
    char*   found = NULL;
    char*   pos[buffer_size] = {0};
    int     error = 0;
    int     index = 0;

    while ((found = strsep(&cmd, " \n")) != NULL) {
        if (*found == 0) {
            continue;
        }
        pos[index++] = found;
    }
    pos[index] = 0;
    error = execvp(pos[0], pos);
    if (error < 0) {
        mumsh_wrong_cmd(pos[0]);
    }
}
