/** 
 * Course: 2020FA VE482
 * Author: Zhimin Sun
 */
#ifndef MUMSH_JOBS_H
#define MUMSH_JOBS_H

typedef struct jobs jobs_t;

/* A single-linked list, stroing jobs. */
jobs_t* head;

/** 
 * Initializes the single-linked list.
 * 
 * @return          Head pointer of linked list
 */
jobs_t* jobs_init(void);

/**
 * Frees the single-linked list.
 */
void jobs_clean(void);

/**
 * Inserts a new job.
 *
 * @param   cmd     Command string
 * @param   pid     Process ID
 */
void jobs_insert(char* cmd, int pid);

/**
 * Traverse jobs.
 */
void jobs_traverse(void);

#endif // MUMSH_JOBS_H
