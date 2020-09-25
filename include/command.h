/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#ifndef MUMSH_COMMAND_H
#define MUMSH_COMMAND_H

/**
 * Handles normal exit.
 *
 * @param  cmd     String of command
 */
void mumsh_exec_exit(const char* cmd);

/**
 * Handles directory change.
 *
 * @param  cmd     String of command
 * @return         0 on success, 1 for no cd command
 */
int mumsh_chdir(char* cmd);

/**
 * Parses the command.
 *
 * @param  cmd     String of command
 */
void mumsh_parse(char* cmd);

#endif // MUMSH_COMMAND_H
