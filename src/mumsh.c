/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "command.h"
#include "error_status.h"
#include "io_utils.h"
#include "jobs.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#define buffer_size 1026

/* A single-linked list, dealing with jobs. */
extern jobs_t* head;

int
main(void)
{
    head = jobs_init();
    if (!head) {
        mumsh_error(NO_MEMORY);
    }
    while (1) {
        __pid_t  pid;
        char     buffer[buffer_size];

        /* Handle CTRL-C for parent. */
        signal(SIGINT, interrupt_parent);

        /* Prompts user for input. */
        mumsh_prompt(buffer);

        /* Exit mumsh normally. */
        mumsh_exec_exit(buffer);

        /* Change working directory. */
        if (mumsh_chdir(buffer) == 0) {
            continue;
        }

        /* Execute `jobs` command. */
        if (mumsh_exec_jobs(buffer)) {
            continue;
        }

        /* Handle CTRL-C for child. */
        signal(SIGINT, interrupt_child);

        /* Create child process. */
        if ((pid = fork()) < 0) {
            mumsh_error(FAIL_FORK);
        } else if (pid == 0) {
            mumsh_parse(buffer);
        }
        
        /* Handle background jobs. */
        if (mumsh_exec_bg(buffer)) {
            jobs_insert(buffer, pid);
        } else {
            /* Parent waits until child killed. */
            if (waitpid(pid, NULL, 0) != pid) {
                mumsh_error(WRONG_CHILD);
            }
        }
    }
}
