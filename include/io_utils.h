/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#ifndef MUMSH_IO_UTILS_H
#define MUMSH_IO_UTILS_H

/**
 * Prompts user for input in mumsh. 
 * 
 * This function handles the `SIGINT` signal.
 * 
 * @param  buffer   Pointer to buffer
 */
void mumsh_prompt(char* buffer);

/**
 * Prints error information and exits. 
 * 
 * @param  error    0 for normal exit, or an error code
 */
void mumsh_error(int error);

/**
 * Prints wrong command error and exits.
 *
 * @param  cmd     String of command
 */
void mumsh_wrong_cmd(const char* cmd);

#endif // MUMSH_IO_UTILS_H
