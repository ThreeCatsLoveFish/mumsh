/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "io_utils.h"
#include "error_status.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void
mumsh_prefix(void)
{
    printf("mumsh $ ");
    fflush(stdout);
}

static void
interrupt(int signal)
{
    if (signal != SIGINT) return;
    printf("\n");
    mumsh_prefix();
}

void
mumsh_prompt(char* buffer)
{
    signal(SIGINT, interrupt);
    
    mumsh_prefix();
    fgets(buffer, 1026, stdin);
}

void
mumsh_error(int error)
{
    switch (error) {
    case NORMAL_EXIT:
        printf("exit\n");
        break;

    case FAIL_FORK:
        printf("mumsh: Fork fail\n");
        break;

    case FAIL_PIPE:
        printf("mumsh: Pipe wrong\n");
        break;

    case WRONG_CHILD:
        printf("mumsh: Child error\n");
        break;

    case WRONG_REDIRECTION:
        printf("mumsh: Redirection arguments wrong\n");
        break;

    default:
        break;
    }
    exit(error);
}

void
mumsh_wrong_cmd(const char* cmd)
{
    printf("%s: command not found\n", cmd);
    mumsh_error(WRONG_COMMAND);
}
