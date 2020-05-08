#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
void fork_r (int depth);

int main (int argc, char *argv[])
{
  int depth, parent_pid,status;
 
  // only one argument can be entered
  if (argc != 2)
    {
      printf ("Wrong number of arguments!!\n");
      exit (-1);
    }
  
  // depth minimum value would be assumed to be 2
  depth = atoi (argv[1]);
  if (depth <= 2)
    {
      printf ("Insert positive integer as argument!!\n");
      exit (EXIT_FAILURE);
    }


  parent_pid = getpid ();
  printf ("PID : %d, PPID : %d : parent...\n\n", parent_pid, getppid ());
  printf ("will create /logs folder 1 sec. later\n\n");
  sleep(1);
  system ("mkdir -p logs ; date >>logs/161025_uyg2A_logAll.log  ");
  sleep (1);
  /*The process tree was drawn after all the children were formed in the demo.
    But I am printing here because I got errors when I tried to put at the end of the function       fork_r. So my pstree doesn't show childs. But if you look at the pid ppids, I built the tree correctly.*/
  unsigned char STR[100];
  unsigned char LOG_STR[100];
  unsigned char LOG_PID_DEPTH_STR[100];
  unsigned char *pstree="pstree -p";
  unsigned char *pstree_log=">>logs/161025_uyg2A_logAll.log  ";
  unsigned char *pid_depth_log=">>logs/161025_uyg2A_logAll.log  ";
  unsigned char *pid_and_depth ="echo ";
  unsigned char PID_DEPTH_STR[100];

  sprintf(PID_DEPTH_STR,"%s PID: %d DEPTH: %d",pid_and_depth,parent_pid,depth); 
  sprintf(STR,"%s %d",pstree,parent_pid); 
  sprintf(LOG_PID_DEPTH_STR,"%s %s",PID_DEPTH_STR,pid_depth_log);
  sprintf(LOG_STR,"%s %s",STR,pstree_log);
  system (PID_DEPTH_STR);
  system (LOG_PID_DEPTH_STR);
  system (STR);
  system(LOG_STR);
  sleep(2);
 
 
  fork_r (depth + 1); //start from level one and go until depth
 

   
 return 0;
}

//Create process tree up to depth without left leaves.
void fork_r (int depth)
{
  int status;
  int temp = depth;
 
  while (depth > 0)
  {

    int child_pid = fork ();

    depth--;
    // handle error
    if (child_pid == -1)
	{ 
          printf("fork() failed.");
          exit(0);
	}
    //child process
    else if (child_pid == 0)
	{ 
	 sleep (2);
	 printf ("PID : %d, PPID : %d : forked...\n", getpid (), getppid ());
	
        // not to create the children on the left leaf. Child 2(left child)
        if (depth == temp - 1)
	    {
              int leafchild_pid=getpid();
	      printf
		("PID : %d, PPID : %d : printing log file content, caution! if the first run, there is nothing...\n", leafchild_pid, getppid ());
            
	      sleep (1);
	      printf ("----------------logs/logAll.log--------------\n\n");
              system ("cat logs/161025_uyg2A_logAll.log  \n");
	      printf ("---------------------------------------------\n\n");
              wait(&status);
              sleep(2);
	      break;
	    }
             sleep(2);          
    	}
    // parent process
    else
    	{ 
	    // to find the parent before termination and then to terminate it. Child 1 (right child)
	    if (depth == temp - 2)
	    {
	      wait (&status);
	      break;
	    }
	    // not to add the children on the left leaf to the process tree . Child 2
	   if (depth != temp - 1)
	    {
	      break;
	    }
      
    	}
   
  }
  wait (&status);
  
  


  // kill all the kids after all the processes are over.
  printf ("pid: %d terminated ...\n" ,getpid());
 

}

