/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "command.h"
#include "error_status.h"
#include "io_utils.h"
#include "sign.h"
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

int
mumsh_chdir(char* cmd)
{
    if (strncmp(cmd, "cd\n", 3) != 0 && strncmp(cmd, "cd ", 3) != 0) {
        return 1;
    } else {
        char*   found = NULL;
        char*   path  = NULL;
        int     argc  = 0;

        while ((found = strsep(&cmd, " \n")) != NULL) {
            if (*found == 0) {
                continue;
            }
            if (argc++ == 1) {
                path = found;
            } else if (argc >= 2) {
                mumsh_wrong_cd_args();
                return NORMAL_EXIT;
            }
        }
        if (argc == 1) {
            char*   home;

            home = getenv("HOME");
            chdir(home);
        } else {
            int error;

            error = chdir(path);
            if (error != 0) {
                mumsh_wrong_cd_path(path);
            }
        }
        return NORMAL_EXIT;
    }
}

/**
 * Handles built-in `pwd` command.
 *
 * @param   cmd     Command
 */
static void
mumsh_pwd(const char* cmd)
{
    if (strncmp(cmd, "pwd", 3) == 0) {
        char*   pwd = NULL;

        pwd = getcwd(pwd, buffer_size);
        printf("%s\n", pwd);
        free(pwd);
        exit(NORMAL_EXIT);
    }
}

/**
 * Decodes special signs.
 *
 * @param   cmd     Command
 */
static void
mumsh_decode(char* cmd)
{
    for (size_t i = 0; cmd[i] != '\0'; i++) {
        sign_decode(&cmd[i]);
    }
}

/**
 * Decodes special signs.
 *
 * @param   cmd     Command
 */
static void
mumsh_decode_cmd(char** cmd)
{
    for (size_t i = 0; cmd[i] != 0; i++) {
        for (size_t j = 0; cmd[i][j] != '\0'; j++) {
            sign_decode(&cmd[i][j]);
        }
    }
}

/**
 * Executes command.
 *
 * @param   cmd     Command and arguments
 */
static void
mumsh_exec_cmd(char** cmd)
{
    int error;

    mumsh_pwd(cmd[0]);
    mumsh_decode_cmd(cmd);
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
mumsh_redirection_in(char* filename, int fd_in)
{
    int file;

    mumsh_decode(filename);
    file = open(filename, O_RDONLY);
    if (file < 0) {
        mumsh_wrong_redirect_in(filename);
    }
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
mumsh_redirection_out(char* filename, int fd_out)
{
    int file;

    mumsh_decode(filename);
    file = open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0777);
    if (file < 0) {
        mumsh_wrong_redirect_out(filename);
    }
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
mumsh_redirection_append(char* filename, int fd_out)
{
    int file;

    mumsh_decode(filename);
    file = open(filename, O_WRONLY|O_CREAT|O_APPEND, 0777);
    if (file < 0) {
        mumsh_wrong_redirect_out(filename);
    }
    dup2(file, fd_out);
    close(file);
}

/**
 * Handles the redirection of the command.
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
    if (repos[1] != NULL) {
        mumsh_redirection_out(repos[1], fd[1]);
    }
    if (repos[2] != NULL) {
        mumsh_redirection_append(repos[2], fd[1]);
    }
    mumsh_exec_cmd(argv);
}

/**
 * Parses a single command.
 *
 * Handles the redirection signs in the command.
 *
 * @param  cmd     String of command
 * @param  id      ID of command, 1 for in, 2 for out, 0 for both, -1 for no
 */
static void
mumsh_parse_cmd(char* cmd, int id)
{
    char*   found            = NULL;
    char*   pos[buffer_size] = {0};
    char*   repos[3]         = {NULL, NULL, NULL};
    int     count            = 0;

    /* Finds the position of redirector. */
    found = strchr(cmd, '<');
    if (found) {
        repos[0] = ++found;
    }
    found = strchr(cmd, '>');
    if (found) {
        if (*(++found) == '>') {
            *found   = ' ';
            repos[2] = ++found;
        } else {
            repos[1] = found;
        }
    }

    /* Fixes offset of position and judge error. */
    for (size_t i = 0; i < 3; i++) {
        if (repos[i] == NULL) continue;
        while (*repos[i] == ' ') repos[i]++;
        if (*repos[i] == '<' || *repos[i] == '>') {
            mumsh_wrong_redirect_syntax(*repos[i]);
        }
    }
    for (size_t i = 0; i < 3; i++) {
        if (repos[i] == NULL || *repos[i] != '\0') {
            continue;
        }
        if (id == 0 || id == 2) {
            mumsh_wrong_redirect_syntax('\0');
        } else {
            mumsh_wrong_redirect_syntax('|');
        }
    }
    
    if (repos[0] && strchr(repos[0], '<')) {
        mumsh_error(WRONG_DUP_REDIRECT_IN);
    }
    if (repos[1] && strchr(repos[1], '>')) {
        mumsh_error(WRONG_DUP_REDIRECT_OUT);
    }
    if (repos[2] && strchr(repos[2], '>')) {
        mumsh_error(WRONG_DUP_REDIRECT_OUT);
    }

    /* Parses the string, replace special characters with '\0`. */
    for (int repeat = 0; (found = strsep(&cmd, " <>|\n")) != NULL;) {
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
        pos[count++] = found;
    }

    /* Executes the command. */
    if (repos[0] || repos[1] || repos[2]) {
        if (pos[0] == 0) {
            mumsh_error(WRONG_PROGRAM);
        }
        if (repos[0] && id != 0 && id != 1) {
            mumsh_error(WRONG_DUP_REDIRECT_IN);
        }
        if ((repos[1] || repos[2]) && id != 0 && id != 2) {
            mumsh_error(WRONG_DUP_REDIRECT_OUT);
        }
        mumsh_redirection(pos, repos);
    } else {
        if (id == -1 && pos[0] == 0) {
            mumsh_error(WRONG_PROGRAM);
        }
        mumsh_exec_cmd(pos);
    }
}

/**
 * Handles the pipe in the command.
 *
 * @param  pos     Strings of command
 * @param  count   Number of command
 */
static void
mumsh_pipe_cmd_handle(char** pos, size_t count)
{
    __pid_t pid;
    int     fd_prev[2] = {0};

    /* No pipe. */
    if (count == 1) {
        mumsh_parse_cmd(pos[0], 0);
    }
    /* With pipe. */
    for (size_t i = 0; i < count; i++) {
        int fd_curr[2];

        if (i != count - 1) {
            if (pipe(fd_curr) == -1) {
                mumsh_error(FAIL_PIPE);
            }
            fd_prev[1] = fd_curr[1];
        } else {
            fd_prev[1] = STDOUT_FILENO;
        }
        if ((pid = fork()) < 0) {
            mumsh_error(FAIL_FORK);
        } else if (pid == 0) {
            if (fd_prev[0] != STDIN_FILENO) {
                dup2(fd_prev[0], STDIN_FILENO);
                close(fd_prev[0]);
            }
            if (fd_prev[1] != STDOUT_FILENO) {
                dup2(fd_prev[1], STDOUT_FILENO);
                close(fd_prev[1]);
            }
            if (i != count - 1) {
                close(fd_curr[0]);
            }
            if (i == 0) {
                mumsh_parse_cmd(pos[i], 1);
            } else if (i == count - 1) {
                mumsh_parse_cmd(pos[i], 2);
            } else {
                mumsh_parse_cmd(pos[i], -1);
            }
        }
        if (fd_prev[0] != STDIN_FILENO) {
            close(fd_prev[0]);
        }
        if (fd_prev[1] != STDOUT_FILENO) {
            close(fd_prev[1]);
        }
        fd_prev[0] = fd_curr[0];
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    for (size_t i = 0; i < count; i++) {
        wait(NULL);
    }
    exit(NORMAL_EXIT);
}

/**
 * Handles the error in the command.
 *
 * @param  pos     Strings of command
 * @param  cmd     String of command
 * @param  count   Number of command
 */
static void
mumsh_error_cmd_handle(char** pos, char* cmd, size_t count)
{
    for (size_t i = 0, dup = 0; i < count; i++) {
        if (pos[i][0] == '|') {
            mumsh_error(WRONG_PROGRAM);
        }
        for (size_t j = 0, empty = 1; pos[i][j] != '|'; j++, empty = 0) {
            if (pos[i][j] == '\n') {
                if (i > 0 && i == count - 1 && empty) {
                    /* Extra input. */
                    mumsh_multi_prompt(&pos[i][j]);
                    mumsh_parse(cmd);
                    return;
                } else {
                    break;
                }
            }
            while (pos[i][j] == ' ') {
                j++;
            }
            if (pos[i][j] == '|') {
                if (empty == 1) {
                    mumsh_error(WRONG_PROGRAM);
                } else {
                    break;
                }
            } else if (pos[i][j] == '\n' && i > 0 && i == count - 1 && empty) {
                /* Extra input. */
                mumsh_multi_prompt(&pos[i][j]);
                mumsh_parse(cmd);
                return;
            }
            if (pos[i][j] == '<') {
                while (pos[i][++j] == ' ');
                if (pos[i][j] == '>' || pos[i][j] == '<' || pos[i][j] == '|') {
                    mumsh_wrong_redirect_syntax(pos[i][j]);
                } else if (pos[i][j] == '\n' && i == count - 1) {
                    /* Extra input. */
                    mumsh_multi_prompt(&pos[i][j]);
                    mumsh_parse(cmd);
                    return;
                } else if (i > 0) {
                    dup = 1;
                }
            }
            if (pos[i][j] == '>') {
                if (pos[i][j + 1] == '>') {
                    j++;
                }
                while (pos[i][++j] == ' ');
                if (pos[i][j] == '>' || pos[i][j] == '<' || pos[i][j] == '|') {
                    mumsh_wrong_redirect_syntax(pos[i][j]);
                } else if (pos[i][j] == '\n' && i == count - 1) {
                    /* Extra input. */
                    mumsh_multi_prompt(&pos[i][j]);
                    mumsh_parse(cmd);
                    return;
                } else if (i < count - 1) {
                    dup = 2;
                }
            }
        }
        if (i == count - 1 && dup == 1) {
            mumsh_error(WRONG_DUP_REDIRECT_IN);
        } else if (i == count - 1 && dup == 2) {
            mumsh_error(WRONG_DUP_REDIRECT_OUT);
        }
    }
    while (strsep(&cmd, "|"));
}

/**
 * Handles the error in the command.
 *
 * @param  cmd     String of command
 */
static void
mumsh_parse_pipe(char* cmd)
{
    char*   pos[buffer_size] = {0};
    size_t  count            = 0;

    pos[count++] = cmd;
    for (char* found = cmd; (found = strchr(found, '|')) != NULL;) {
        pos[count++] = ++found;
    }

    mumsh_error_cmd_handle(pos, cmd, count);
    mumsh_pipe_cmd_handle(pos, count);
}

/**
 * Encodes special signs.
 *
 * @param   cmd     Command
 */
static void
mumsh_encode(char* cmd)
{
    char*   dquote;
    char*   next;
    char*   squote;
    char*   start;

    squote = strchr(cmd, '\'');
    dquote = strchr(cmd, '\"');
    if (!squote && !dquote) {
        mumsh_parse_pipe(cmd);
        return;
    } else if (!squote) {
        start = dquote;
        next = strchr(dquote + 1, '\"');
    } else if (!dquote) {
        start = squote;
        next = strchr(squote + 1, '\'');
    } else {
        if (squote < dquote) {
            start = squote;
            next = strchr(squote + 1, '\'');
        } else {
            start = dquote;
            next = strchr(dquote + 1, '\"');
        }
    }
    if (!next) {
        mumsh_multi_prompt(strchr(cmd, '\0'));
        mumsh_parse(cmd);
        return;
    } else {
        for (; start + 1 != next; start++) {
            *start = *(start + 1);
            sign_encode(start);
        }
        for (; *start != '\0'; start++) {
            *start = *(start + 2);
        }
    }
    mumsh_parse(cmd);
    return;
}

void
mumsh_parse(char* cmd)
{
    mumsh_encode(cmd);
}
