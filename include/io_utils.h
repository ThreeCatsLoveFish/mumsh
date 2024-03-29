/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#ifndef MUMSH_IO_UTILS_H
#define MUMSH_IO_UTILS_H

/**
 * Prompts user for input in mumsh.
 * 
 * @param  buffer   Pointer to buffer
 */
void mumsh_prompt(char* buffer);

/**
 * Prompts user for newline input in mumsh.
 * 
 * @param  buffer   Pointer to buffer
 */
void mumsh_multi_prompt(char* buffer);

/**
 * Handles the `SIGINT` signal for parent.
 * 
 * @param  signal  Input signal
 */
void interrupt_parent(int signal);

/**
 * Handles the `SIGINT` signal for child.
 * 
 * @param  signal  Input signal
 */
void interrupt_child(int signal);

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

/**
 * Prints error message and exits.
 */
void mumsh_wrong_cd_args(void);

/**
 * Prints wrong path and exits.
 *
 * @param  path    String of path
 */
void mumsh_wrong_cd_path(const char* path);

/**
 * Prints wrong path and exits.
 *
 * @param  path    String of path
 */
void mumsh_wrong_redirect_in(const char* path);

/**
 * Prints wrong path and exits.
 *
 * @param  path    String of path
 */
void mumsh_wrong_redirect_out(const char* path);

/**
 * Prints wrong sign and exits.
 *
 * @param  sign    String of sign
 */
void mumsh_wrong_redirect_syntax(char sign);

#endif // MUMSH_IO_UTILS_H
