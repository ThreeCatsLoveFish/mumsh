/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define buffer_size 1026

typedef struct jobs {
    char*           cmd;
    int             pid;
    size_t          id;
    struct jobs*    next;
} jobs_t;

jobs_t*
jobs_init(void)
{
    jobs_t* job;

    job = calloc(1, sizeof(jobs_t));
    if (job == NULL) {
        return NULL;
    }
    job->cmd = calloc(buffer_size, sizeof(char));
    if (job->cmd == NULL) {
        free(job);
        return NULL;
    }
    job->id   = 0;
    job->next = NULL;
    return job;
}

void
jobs_clean(void)
{
    jobs_t* job = head;

    while (job) {
        jobs_t* tmp = job->next;

        free(job->cmd);
        free(job);
        job = tmp;
    }
    head = NULL;
}

void
jobs_insert(char* cmd, int pid)
{
    jobs_t* job = head;
    jobs_t* tmp = jobs_init();

    while (job->next) {
        job = job->next;
    }
    strcpy(tmp->cmd, cmd);
    tmp->pid = pid;
    tmp->id  = job->id + 1;
    printf("[%zu] %s", tmp->id, tmp->cmd);
    job->next = tmp;
}

void
jobs_traverse(void)
{
    jobs_t* job = head->next;
    jobs_t* tmp;

    for (; job != NULL; job = tmp) {
        tmp = job->next;
        if (waitpid(job->pid, NULL, WNOHANG) != 0) {
            printf("[%zu] done %s", job->id, job->cmd);
        } else {
            printf("[%zu] running %s", job->id, job->cmd);
        }
    }
}
