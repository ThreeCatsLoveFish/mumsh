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
    int num = 1;

    for (cmd[0] = 0; cmd[num] != NULL; num++) {
        if (strcmp(cmd[num], ">") == 0 ||
            strcmp(cmd[num], "<") == 0 ||
            strcmp(cmd[num], "<<") == 0 ||
            strcmp(cmd[num], ">>") == 0 ||
            strcmp(cmd[num], "|") == 0) {
                break;
            }
    }
    if (num == 1) {
        mumsh_error(WRONG_REDIRECTION);
    }
    return num - 1;
}

/**
 * Handles redirection of in.
 * 
 * @param   filename    Filename
 * @param   fd_in       File descriptor for input
 */
static void
mumsh_redirection_in(const char* filename, int fd_in)
{
    int file;

    file = open(filename, O_RDONLY);
    dup2(file, fd_in);
    close(file);
}

/**
 * Handles redirection of out.
 * 
 * @param   filename    Filename
 * @param   fd_out      File descriptor for output
 */
static void
mumsh_redirection_out(const char* filename, int fd_out)
{
    int file;

    file = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0777);
    dup2(file, fd_out);
    close(file);
}

/**
 * Handles redirection of append.
 * 
 * @param   filename    Filename
 * @param   fd_out      File descriptor for output
 */
static void
mumsh_redirection_append(const char* filename, int fd_out)
{
    int file;

    file = open(filename, O_WRONLY|O_CREAT|O_APPEND, 0777);
    dup2(file, fd_out);
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
    int fd[2]  = {STDIN_FILENO, STDOUT_FILENO};
    int re_in  = 0;
    int re_out = 0;
    int re_app = 0;

    for (int i = 0; i < size; i++) {
        if (strcmp(argv[i], "<") == 0) {
            re_in = i + mumsh_redirection_argc(&argv[i]);
        } else if (strcmp(argv[i], ">") == 0) {
            mumsh_redirection_argc(&argv[i]);
            re_out = i + 1;
        } else if (strcmp(argv[i], ">>") == 0) {
            mumsh_redirection_argc(&argv[i]);
            re_app = i + 1;
        }
    }
    if (!re_in && !re_out && !re_app) {
        mumsh_exec_cmd(argv);
    }
    if (re_in) {
        mumsh_redirection_in(argv[re_in], fd[0]);
    }
    if (re_out) {
        mumsh_redirection_out(argv[re_out], fd[1]);
    } else if (re_app) {
        mumsh_redirection_append(argv[re_app], fd[1]);
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
