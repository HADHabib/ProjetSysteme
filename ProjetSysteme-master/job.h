#ifndef JOB_H
#define JOB_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include "process.h"
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

//Import process structure from process.h
typedef struct process process;

/* A job is a pipeline of processes.  */
typedef struct job
{
  struct job *next;           /* next active job */
  char *command;              /* command line, used for messages */
  process *first_process;     /* list of processes in this job */
  pid_t pgid;                 /* process group ID */
  char notified;              /* true if user told about stopped job */
  struct termios tmodes;      /* saved terminal modes */
  int stdin, stdout, stderr;  /* standard i/o channels */
} job;

job *get_first_job (void);

job *NewJob(char *command);


//Here are some utility functions that are used for operating on job objects.

/* Find the active job with the indicated pgid.  */
job * find_job (pid_t pgid);


/* Return true if all processes in the job have stopped or completed.  */
int job_is_stopped (job *j);


/* Return true if all processes in the job have completed.  */
int job_is_completed (job *j);

int
mark_process_status (pid_t pid, int status);

/* Check for processes that have status information available,
   blocking until all processes in the given job have reported.  */

void wait_for_job (job *j);

/* Format information about job status for the user to look at.  */

void
format_job_info (job *j, const char *status);

/* Notify the user about stopped or terminated jobs.
   Delete terminated jobs from the active job list.  */

void
do_job_notification ();

void
launch_job (job *j, int foreground,int shell_terminal,
int shell_is_interactive);

void free_job (job *j);

void
put_job_in_foreground (job *j, int cont);

void
put_job_in_background (job *j, int cont);

#endif  //JOB_H



