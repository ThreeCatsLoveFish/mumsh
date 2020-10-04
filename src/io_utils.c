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
mumsh_multi_prompt(char* buffer)
{
    printf("> ");
    fflush(stdout);
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
        fputs("mumsh: Fork fail\n", stderr);
        break;

    case FAIL_PIPE:
        fputs("mumsh: Pipe wrong\n", stderr);
        break;

    case WRONG_PROGRAM:
        fputs("error: missing program\n", stderr);
        break;

    case WRONG_CHILD:
        fputs("mumsh: Child error\n", stderr);
        break;

    case WRONG_DUP_REDIRECT_IN:
        fputs("error: duplicated input redirection\n", stderr);
        break;

    case WRONG_DUP_REDIRECT_OUT:
        fputs("error: duplicated output redirection\n", stderr);
        break;

    default:
        break;
    }
    exit(error);
}

void
mumsh_wrong_cmd(const char* cmd)
{
    fputs(cmd, stderr);
    fputs(": command not found\n", stderr);
    mumsh_error(WRONG_COMMAND);
}

void
mumsh_wrong_cd_args(void)
{
    fputs("mumsh: cd: too many arguments\n", stderr);
}

void
mumsh_wrong_cd_path(const char* path)
{
    fputs(path, stderr);
    fputs(": No such file or directory\n", stderr);
}

void
mumsh_wrong_redirect_in(const char* path)
{
    fputs(path, stderr);
    fputs(": No such file or directory\n", stderr);
    mumsh_error(WRONG_REDIRECT_IN);
}

void
mumsh_wrong_redirect_out(const char* path)
{
    fputs(path, stderr);
    fputs(": Permission denied\n", stderr);
    mumsh_error(WRONG_REDIRECT_OUT);
}

void
mumsh_wrong_redirect_syntax(char sign)
{
    fputs("syntax error near unexpected token `", stderr);
    fputc(sign, stderr);
    fputs("'\n", stderr);
    mumsh_error(WRONG_REDIRECT_OUT);
}
