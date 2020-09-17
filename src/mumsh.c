/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include <sys/wait.h>
#include <unistd.h>
#include "command.h"
#include "error_status.h"
#include "io_utils.h"

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
            /* TODO: Execute the command. */
            return 0;
        }
        
        /* Parent waits until child killed. */
        int status;
        if (wait(&status) != pid) {
            mumsh_error(WRONG_CHILD);
        }
        
    }
}
