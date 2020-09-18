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
 * Finds the command and executes.
 * 
 * @param   argv    Command and arguments
 * @param   repos   Parameters of redirection
 */
static void
mumsh_redirection(char** argv, char* repos[3])
{
    int fd[2] = {STDIN_FILENO, STDOUT_FILENO};
    
    if (repos[0] != NULL) {
        mumsh_redirection_in(repos[0], fd[0]);
    }
    if (repos[1] > repos[2]) {
        mumsh_redirection_out(repos[1], fd[1]);
    } else if (repos[2] != NULL) {
        mumsh_redirection_append(repos[2], fd[1]);
    }
    mumsh_exec_cmd(argv);
}

void
mumsh_parse_cmd(char* cmd)
{
    char*   found            = NULL;
    char*   pos[buffer_size] = {0};
    char*   repos[3]         = {NULL, NULL, NULL};

    /* Finds the position of redirector. */
    found = strchr(cmd, '<');
    if (found) {
        repos[0] = found + 1;
    }
    found = strchr(cmd, '>');
    if (found) {
        char*   tmp;
        tmp = strchr(found + 1, '>');
        if (tmp) {
            repos[2] = tmp + 1;
        } else {
            repos[1] = found + 1;
        }
    }

    /* Fixes offset of position. */
    for (size_t i = 0; i < 3; i++) {
        if (repos[i] == NULL) continue;
        while (*repos[i] == ' ') repos[i]++;
    }

    /* Parses the string, replace special characters with '\0`. */
    for (int i = 0, repeat = 0; (found = strsep(&cmd, " <>\n")) != NULL;) {
        if (*found == 0) {
            continue;
        }
        for (size_t j = 0; j < 3; j++) {
            if (found == repos[j]) {
                repeat = 1;
            }
        }
        if (repeat == 1) {
            repeat = 0;
            continue;
        }
        pos[i++] = found;
    }

    /* Executes the command. */
    if (repos[0] || repos[1] || repos[2]) {
        mumsh_redirection(pos, repos);
    } else {
        mumsh_exec_cmd(pos);
    }
}
