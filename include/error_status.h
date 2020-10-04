/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#ifndef MUMSH_ERROR_STATUS_H
#define MUMSH_ERROR_STATUS_H

typedef enum error {
    NORMAL_EXIT            = 0,
    FAIL_FORK              = 1,
    FAIL_PIPE              = 2,
    WRONG_CD_ARGS          = 3,
    WRONG_CD_PATH          = 4,
    WRONG_CHILD            = 5,
    WRONG_COMMAND          = 6,
    WRONG_DUP_REDIRECT_IN  = 7,
    WRONG_DUP_REDIRECT_OUT = 8,
    WRONG_REDIRECT_IN      = 9,
    WRONG_REDIRECT_OUT     = 10,
    WRONG_PROGRAM          = 11,
} error_t;

typedef enum redirection {
    NONE    = 0,
    BOTH    = 1,
    DUP_IN  = 2,
    DUP_OUT = 3,
    SIN_IN  = 4,
    SIN_OUT = 5,
} redirection_t;

#endif // MUMSH_ERROR_STATUS_H
