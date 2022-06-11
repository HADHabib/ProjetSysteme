#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <sys/wait.h>
#include "job.h"
#include <signal.h>
#include <errno.h>
#include <string.h>


/* A process is a single process.  */
typedef struct process
{
  struct process *next;       /* next process in pipeline */
  char **argv;                /* for exec */
  pid_t pid;                  /* process ID */
  char completed;             /* true if process has completed */
  char stopped;               /* true if process has stopped */
  int status;                 /* reported status value */
} process;

process *NewProcess(char *commande);



void launch_process (process *p, pid_t pgid, int infile, int outfile, int errfile,int foreground,int shell_is_interactive,int shell_terminal);

int mark_process_status (pid_t pid, int status);

void update_status (void);
#endif //PROCESS_H