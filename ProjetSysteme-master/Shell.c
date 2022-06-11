#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "job.h"
#include "process.h"
#include <string.h>
#include <sys/wait.h>
#include <errno.h>


pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;



/* Make sure the shell is running interactively as the foreground job
   before proceeding. */

void init_shell ()
{

  /* See if we are running interactively.  */
  shell_terminal = STDIN_FILENO;
  setshell_terminal(shell_terminal);
  shell_is_interactive = isatty (shell_terminal);
  setshell_is_interactive(shell_is_interactive);
  if (shell_is_interactive)
    {
      /* Loop until we are in the foreground.  */
      while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
        setshell_pgid(shell_pgid);
        kill (- shell_pgid, SIGTTIN);

      /* Ignore interactive and job-control signals.  */
      signal (SIGINT, SIG_IGN);
      signal (SIGQUIT, SIG_IGN);
      signal (SIGTSTP, SIG_IGN);
      signal (SIGTTIN, SIG_IGN);
      signal (SIGTTOU, SIG_IGN);
      signal (SIGCHLD, SIG_IGN);

      /* Put ourselves in our own process group.  */
      shell_pgid = getpid ();
      if (setpgid (shell_pgid, shell_pgid) < 0)
        {
          perror ("Couldn't put the shell in its own process group");
          exit (1);
        }

      /* Grab control of the terminal.  */
      tcsetpgrp (shell_terminal, shell_pgid);
      
      /* Save default terminal attributes for shell.  */
      tcgetattr (shell_terminal, &shell_tmodes);
      setshell_tmodes(shell_tmodes);
    }
    
}
char *read_command()
{
  char *line=NULL;
  ssize_t line_size;
  getline(&line, &line_size, stdin);
  line[line_size-1]=0;
  line[line_size-2]=0;
  return line;
}



void shelloop(){
    int status=1;
    do{
        char* help=read_command();
        if(strcmp(help,"exit\n")==0){
            status=0;
        }
        else{
            launch_job(NewJob(help),1,shell_terminal,shell_is_interactive);
        }
    }while (status);

    
}
int main(int argc, char *argv[])
{
    init_shell();
    shelloop();
    return 0;
}
