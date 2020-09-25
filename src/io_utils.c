/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "io_utils.h"
#include "error_status.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

/**
 * Outputs prefix for mumsh.
 */
static void
mumsh_prefix(void)
{
    printf("mumsh $ ");
    fflush(stdout);
}

void
interrupt_parent(int signal)
{
    if (signal != SIGINT) return;
    printf("\n");
    mumsh_prefix();
}

void
interrupt_child(int signal)
{
    if (signal != SIGINT) return;
    printf("\n");
    fflush(stdout);
}

/**
 * Handles input and determines EOF.
 */
static void
mumsh_getcmd(char* buffer)
{
    for (int i = 0, ch = 0; (ch = getchar()) != 0;) {
        if (ch == EOF) {
            if (i == 0) {
                mumsh_error(NORMAL_EXIT);
            } else {
                clearerr(stdin);
                continue;
            }
        } else {
            buffer[i++] = ch;
            if (ch == '\n') {
                buffer[i] = '\0';
                return;
            }
        }
    }
}

void
mumsh_prompt(char* buffer)
{
    mumsh_prefix();
    mumsh_getcmd(buffer);
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

void
mumsh_wrong_cd_args(void)
{
    printf("mumsh: cd: too many arguments\n");
}

void
mumsh_wrong_path(const char* path)
{
    printf("mumsh: cd: %s: No such file or directory\n", path);
}
