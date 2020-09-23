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
    WRONG_CHILD       = 3,
    WRONG_COMMAND     = 4,
    WRONG_PWD         = 5,
    WRONG_REDIRECTION = 6,
} error_t;

#endif // MUMSH_ERROR_STATUS_H
