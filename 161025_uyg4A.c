#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SIZE 1024

struct stat attr;
struct stat oldTime;/*It keeps a previous version of last modification time and oldTime.st_mtime! = required to compare attr.st_mtime with times.*/
char *pathInput;/*it keeps the path of the input file entered from the monitor.*/
int pfd[2];
long int var;
void *funcChecker (void *pathInp);
//int alarmFlag = 0;
void alarmControl (int arg);
void alarmHandler (int arg );
int main (int argc, char **argv)
{
  // only one argument can be entered
  if (argc == 1 || argc > 2)
    {
      printf ("./161025 must be executed with one additional argument(pathInputFile):\nAborted automatically...\n\n");
      exit (1);
    }

  pathInput = argv[1];

  if (-1 == pipe (pfd))
    {
      perror ("Pipe failed\n");
      exit (3);
    }
  printf ("Pipe was created succesfully...\n");
    
  int pid;
  int faculty, departments, year;
  pthread_t checker_thread;
  if (0 > (pid = fork ()))
    {
      perror ("Fork failed\n");
      exit (4);
    }

  if (0 == pid)
    {				// child
      printf ("Child\t->My PID is %d, PPID is %d\n", getpid (), getppid ());
      pthread_create (&checker_thread, NULL, funcChecker, (void *) pathInput);
      pthread_join (checker_thread, NULL);
    }
  else
    {				// parent
      printf ("Parent\t->My PID is %d, PPID is %d\n", getpid (), getppid ());
      long int bytesread;
      close (pfd[1]);/* close unused end */
      while (1)
	{
	  sleep (1);
	  if ((read(pfd[0], &var, sizeof (var)) )!= 0)
	    {

	      printf ("\t%s:%ld\n", pathInput, var);
	      long int tmp = var;
	      int digits = 0;
	      while (tmp)
		{
		  digits++;
		  tmp /= 10;
		}

	      if (digits < 12)
		{
		  printf ("\t ..wrong Input! few digits..\n");
                 
		}
	      else if (digits >= 13)
		{
		  printf ("\t ..wrong Input! number of digits overflowed..\n");
               
		}
	      else
		{
		  faculty = (var / 10000000000);//15
		  departments = (var / 100000000);//1521
		  departments = departments % 100;//21
		  year = (var / 10000);	//15212016
		  year = year % 10000;	//2016
		  printf ("\t->Parsing and printing Student ID\n");
		  if (15 == faculty)
		    {
		      printf ("\tFaculty:	Engineering Faculty \n");
		    }
		  else if (30 == faculty)
		    {
		      printf ("\tFaculty:	Out University \n");
		    }
		  else
		    {
		      printf ("\tFaculty:	Other Faculty \n");
		    }

		  if (21 == departments)
		    {
		      printf ("\tDepartment:	Computer Engineering\n");
		    }
		  else if (12 == departments)
		    {
		      printf ("\tDepartment:	Electrical Engineering \n");
		    }
		  else
		    {
		      printf ("\tDepartment:	Other \n");
		    }
		  printf ("\tEntrance Year:\t%d\n", year);

		  printf ("\tThe message is being send. Warning type constraint!\n");
		  printf ("\tSleeping 4 sec.s\n");
		  sleep (4);
		  printf ("\tIncoming message from child is:%ld\n", var);
		  printf("\tIncoming message size is fixed 8 bytes for this application\n");
		  /*if faculty and department is Engineering faculty and Computer engineering, The parent wait to continue the process until child will complete the process*/
		  if (15 == faculty && 21 == departments)
		    {
		      printf ("\tChild process is being waited.\n");
		      sleep (4);
		      printf ("\tSlept 4 sec.s\n");
		      printf ("Child\t->Terminating PID:%d, PPID:%d\n ", pid,getpid ());
		      printf ("\tWaiting is done.\n");
		      printf ("Parent\t->Terminating PID:%d, PPID:%d\n ", getpid (), getppid ());
		      exit (5);
		    }
		  /*If the entry year is 2017 or later, it waits for a 4-second alarm, and then the child process is terminated.*/
		  else if (2017 <= year)
		    {
		      printf ("\tI'll kill you when your normal time is up.\n");
		      kill (pid, SIGKILL);
		      printf("\tAlarm clock signal was scheduled in 4 seconds...\n");
		      alarm (4);/*Schedule an alarm signal in 4 seconds */
		      signal (SIGALRM, alarmHandler); /*Install signal Handler*/
		      sleep (4);
		      printf ("\tSlept 4 sec.s\n");
		      printf ("\tChild is killed. Or not?\n");
		      printf ("Parent\t->Terminating PID:%d, PPID:%d\n",getpid (), getppid ());
		      exit (6);
		    }
		  /*If the Faculty is not Faculty of Engineering, the parent process completes the process after waiting for the 6-second alarm mechanism.*/
		  else if (15 != faculty)
		    {
		      printf ("\tAlarm clock signal was scheduled in 10 seconds...\n");
		      printf ("\tOnly the God can judge me! (freedom for 10 seconds).\n");

		      printf ("Child\t->Terminating PID:%d, PPID:%d\n", pid, getppid ());
		      alarm (10);/*Schedule an alarm signal in 10 seconds */
		      signal (SIGALRM, alarmHandler);/*Install signal Handler*/
		      signal (SIGINT, alarmControl); /*Ignore Ctrl-C*/ 
		      signal (SIGTSTP, alarmControl); /*Ignore Ctrl-Z*/ 
		      //exit (7);
		    }
		  //d
		  else
		    { 
            printf ("\tSlept 4 sec.s\n");
		      printf ("Child\t->Terminating PID:%d, PPID:%d\n", pid, getppid ());
		      printf ("Parent\t->Terminating PID:%d, PPID:%d\n ", getpid (), getppid ());
		      exit (8);
		    }

		}
	    }
	}
      close (pfd[0]);/* close used end */
    }

  return 0;
}
/*This function compares modification times. If last modification time has changed, child process sends the content of the file received from the monitor to the parent process.*/
void *funcChecker (void *pathInp)
{

  FILE *fp;
 
  stat ((char *) pathInp, &attr);

  oldTime.st_mtime = attr.st_mtime;
  printf ("\n\t->Last modification time: %s", ctime (&attr.st_mtime));
  /*child, writer*/ 
  close (pfd[0]);/* close unused end */
  while (1)
    {
      sleep (1);
      stat ((char *) pathInp, &attr);

      if (attr.st_mtime > oldTime.st_mtime)
	{
	  printf ("\t->Last updated time: %s", ctime (&attr.st_mtime));
          /*Assigns the content of the file on the monitor to the var variable.*/
          fp = fopen ((char *) pathInp, "r");
	  fscanf (fp, "%ld", &var);
	  fclose (fp);
          /*The child sends the value in the input file to the parent. Then Sleeping 4 sec.s*/
	  write (pfd[1], &var, sizeof (var));
          sleep(4);
	  oldTime.st_mtime = attr.st_mtime;
	}
    }
  close (pfd[1]);/* close used end */
}
/*This function is for handling CTRL + C and CTRL + Z signals.*/
void alarmControl (int arg)
{
  if (arg == SIGINT)
    {
      signal(SIGINT,alarmControl);
      printf(" Sig. Handler\t-> I'll always get suffering (Protection from Ctrl+C | Ctrl+Z)!!!\n");
      printf ("Handler\t-> Alarm clock signal has been received.\n");
     

    }
  if( arg == SIGTSTP)
    {
      signal(SIGTSTP,alarmControl);
      printf(" Sig. Handler\t-> I'll always get suffering (Protection from Ctrl+C | Ctrl+Z)!!!\n");
      printf ("Handler\t-> Alarm clock signal has been received.\n");
    
    }
}
/*This function is for handling alarm signals.*/
void alarmHandler(int arg)
{
  printf ("Handler\t->Alarm clock signal has been received.\n");
  exit(0);
}
