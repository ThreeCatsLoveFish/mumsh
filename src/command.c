/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "command.h"
#include "io_utils.h"
#include "error_status.h"
#include <string.h>

#define buffer_size 1026

void
mumsh_exec_exit(const char* cmd)
{
    if (strncmp(cmd, "exit", 4) == 0) {
        mumsh_error(NORMAL_EXIT);
    }
}
