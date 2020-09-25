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
 * Determines the end of file.
 */
static void
end_of_file(void)
{
    int c;

    if (feof(stdin)) {
        mumsh_error(NORMAL_EXIT);
    }
    c = getchar();
    if (c == EOF) {
        mumsh_error(NORMAL_EXIT);
    }
    ungetc(c, stdin);
}

void
mumsh_prompt(char* buffer)
{
    mumsh_prefix();
    end_of_file();
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

    case WRONG_PWD:
        printf("mumsh: Wrong parameter of directory\n");
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
