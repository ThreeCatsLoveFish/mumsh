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
 * Executes the command.
 *
 * Child process deals with command.
 *
 * @param  cmd     String of command
 */
void mumsh_exec_cmd(char* cmd);

#endif // MUMSH_COMMAND_H
