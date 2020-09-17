/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "command.h"
#include "error_status.h"
#include "io_utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
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
 * Counts the number of arguments behind redirection sign.
 * 
 * @param   cmd     Command and arguments
 * @return          Number of arguments
 */
static int
mumsh_redirection_argc(char** cmd)
{
    int num = 0;

    for (; cmd[num] != NULL; num++) {
        if (strcmp(cmd[num], ">") == 0 ||
            strcmp(cmd[num], "<") == 0 ||
            strcmp(cmd[num], "<<") == 0 ||
            strcmp(cmd[num], ">>") == 0 ||
            strcmp(cmd[num], "|") == 0) {
                break;
            }
    }
    return num;
}

/**
 * Handles redirection of in.
 * 
 * @param   arg     Arguments
 * @param   fd_in   File descriptor for input
 */
static void
mumsh_redirection_in(const char* arg, int fd_in)
{
    int file;

    file = open(arg, O_RDONLY);
    dup2(file, fd_in);
    close(file);
}

/**
 * Handles overall redirection.
 * 
 * @param   argv    Command and arguments
 * @param   size    Number of arguments
 */
static void
mumsh_redirection(char** argv, const int size)
{
    int fd[2] = {STDIN_FILENO, STDOUT_FILENO};
    int re_in = 0;

    for (int i = 0; i < size; i++) {
        if (strcmp(argv[i], "<") == 0) {
            int argc;

            argv[i] = NULL;
            argc = mumsh_redirection_argc(&argv[i + 1]);
            if (argc <= 0) {
                mumsh_error(WRONG_REDIRECTION);
            }
            re_in = i + argc;
        }
    }
    /* TODO: Redirection for out and append. */
    if (!re_in) {
        mumsh_exec_cmd(argv);
    }
    if (re_in) {
        mumsh_redirection_in(argv[re_in], fd[0]);
    }
    mumsh_exec_cmd(argv);
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
