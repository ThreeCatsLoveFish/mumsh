/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "command.h"
#include "error_status.h"
#include "io_utils.h"
#include <sys/wait.h>
#include <unistd.h>

#define buffer_size 1026

int
main()
{
    while (1) {
        __pid_t  pid;
        char     buffer[buffer_size];

        /* Prompts user for input. */
        mumsh_prompt(buffer);

        /* Exit mumsh normally. */
        mumsh_exec_exit(buffer);
        
        /* Create child process. */
        if ((pid = fork()) < 0) {
            mumsh_error(FAIL_FORK);
        } else if (pid == 0) {
            mumsh_parse(buffer);
        }
        
        /* Parent waits until child killed. */
        if (wait(NULL) != pid) {
            mumsh_error(WRONG_CHILD);
        }
    }
}
