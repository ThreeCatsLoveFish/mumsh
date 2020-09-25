/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#ifndef MUMSH_ERROR_STATUS_H
#define MUMSH_ERROR_STATUS_H

typedef enum error {
    NORMAL_EXIT       = 0,
    FAIL_FORK         = 1,
    FAIL_PIPE         = 2,
    WRONG_CD_ARGS     = 3,
    WRONG_CD_PATH     = 4,
    WRONG_CHILD       = 5,
    WRONG_COMMAND     = 6,
    WRONG_REDIRECTION = 7,
} error_t;

#endif // MUMSH_ERROR_STATUS_H
