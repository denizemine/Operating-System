 #include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <time.h> 
//usleep((int)(100000)); for 0.1 sec.s
#include <sys/stat.h>
#include <stdbool.h>

void *funcChecker (void *arg);
void *funcPrintTimeAndChanges (void *arg);
char *pathRep;
struct stat attr;
struct stat oldTime;   //last modification time ın bir önceki halini tutar ve oldTime.st_mtime!=attr.st_mtime ile zamanlar karşılaştırılabilmesi için gereklidir.

int main (int argc, char **argv)
{
   pthread_t threadChecker, threadWorker_TimeAndChanges;
  
  if (argc == 1)
    {

	printf("./161025 must be executed with exactly two additional arguments[(pathRep,pathSub)]\n Type count is 1\n Aborted automaticalyy...\n");
	exit (-1);
    }

  if (argc == 2)
    {
	printf("./161025 must be executed with exactly two additional arguments[(pathRep,pathSub)]\n Type count is 2\n Aborted automaticalyy...\n");
	exit (-1);

    }
  if (argc > 3)
    {
	printf("./161025 must be executed with exactly two additional arguments[(pathRep,pathSub)]\n Type count is 3\n Aborted automaticalyy...\n");
	exit (-1);

    }

	printf ("main function -> \n");
	//path rep ve patsubı yazdıracaksdın
	char *pathSub;	
	pathRep=argv[1];
	pathSub=argv[2];
  
	printf("pathRep: %s\n",pathRep);
	printf("pathSub: %s\n\n",pathSub);

	

	pthread_create (&threadChecker, NULL, funcChecker, (void*)pathSub);
	pthread_create (&threadWorker_TimeAndChanges, NULL, funcPrintTimeAndChanges, (void*)pathSub);



	pthread_join (threadWorker_TimeAndChanges, NULL);
	pthread_join (threadChecker, NULL);



  return 0;


}

//Bu fonksiyon modification zamanlarını karşılaştırır eğer değiştirilmişse submissionı repository'e kopyalar. log işlemlerini yapar...
void *funcPrintTimeAndChanges(void *arg)
{	
     	printf("FuncPrintTimeAndChanges ->\n");
	char *pathSub=(char *)arg;
        printf("pathChk: %s\n\n", pathSub);
        oldTime.st_mtime=attr.st_mtime; //last modification time'ın ilk değeri oldtime'a atanır.

	unsigned char pathLogFile[400];
	sprintf(pathLogFile, " %s161025_uyg3A_repHist.log", pathRep);

        unsigned char CPY[400];
        sprintf(CPY,"cp -r %s/. %s/ ",pathSub,pathRep); //bir dizinin içeriginin tümünü başka bir dizinekopyalamak için cp -r ../. ../ 

        unsigned char LS[400];
        sprintf(LS,"ls -sR  %s/ ",pathSub); // -s -> her bir dosyanın ayrılan boyutunu bloklar  halinde yazdırır. -R diziin içerigindeki dosyaların içerigini yazdırır.
       
	unsigned char STR_pathLogFile[400];
        sprintf(STR_pathLogFile,"date >> %s   && %s >> %s &&  echo ------------------------------------------------------- >> %s  ",pathLogFile,LS,pathLogFile,pathLogFile);
       


        while(true){
         
        	if(attr.st_mtime>oldTime.st_mtime) //last modification time değiştirilmişse dizinin repository kopyalnması gerekir. 
		{
                  printf("Last modified time: %s\n", ctime(&attr.st_mtime)); //If you need some ctime comparison, use (a.st_mtime == b.st_mtime)
	        
                 system(CPY);
                 system(STR_pathLogFile);

                 oldTime.st_mtime=attr.st_mtime; //dizin kopyalandıktan sonra last modification oldtime ve attr eşitlenmeli.
		 system(LS);
                 //system(pathLogFile);
         	 printf("-------------------------------------------------------\n\n");
           
		}
	printf("\r\033[1A%c",  system("date")); // \r: set cursor to the first char of the line; \033[1A: move cursor up 1 line;
	fflush(stdout);	//clear output buffer
	usleep((int)(100000)); // 100 ms. bekleme aralıklarıyla Saat/tarih bilgisini ekrana basar
                  
        }


	
	printf("This line should never be executed. Server program must always be running!\n");
}



//Fonksiyonun görevi sadece 1 sn aralıklarla &attr güncellemek.Tüm yazdırma işlemleri funcPrintTimeAndChanges yapılacak
void *funcChecker(void *arg)
{       
	printf("\nfuncChecker ->\n");
	char *pathSub=(char *)arg;
        printf("pathChk:%s\n",pathSub);
	    
	
        if(stat(pathSub, &attr)==-1)
        {
          perror("stat");
          exit(EXIT_FAILURE);

        }

	while(true){
	
        stat(pathSub, &attr);
	sleep(1); 
            
	}
     
  	printf("This line should never be executed. Server program must always be running!\n");
}
