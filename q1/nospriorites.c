#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/syscall.h> 
#include <sys/resource.h>
#include <sched.h> 

#define N_THREADS 5

typedef struct
{
	int ThreadNum;
	
}Parametres;

void *FonctionThread(void *data)
{
	Parametres *pParam = (Parametres *)data;
	
	printf("Je suis le thread %d et je demarre !!! \n", pParam->ThreadNum);
	
	int ThreadID = syscall(SYS_gettid);
               int ret;
	switch(pParam->ThreadNum)
      {
                 case 0:
			ret = setpriority(PRIO_PROCESS,ThreadID,0);
			break;
                 case 1:
                 	
			ret = setpriority(PRIO_PROCESS,ThreadID,-3);
             		break;         
                 case 2:
			ret = setpriority(PRIO_PROCESS,ThreadID,-2);
			break;
                 case 3:
                     ret = setpriority(PRIO_PROCESS,ThreadID,3);
                      break;                 
                 case 4:
                     ret = setpriority(PRIO_PROCESS,ThreadID,4);
                     break;
        }


	printf("Code retour de setpriority() pour processus %d : %d. \n", pParam->ThreadNum, ret);
	printf("Code retour de errno pour processus %d : %s. \n",  pParam->ThreadNum,strerror(errno));

	while(1);


	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[N_THREADS];
	Parametres myParam[N_THREADS];
	int i;
	for(i=0; i<N_THREADS;i++)
	{
		printf("creation thread %d! \n", i);
		myParam[i].ThreadNum = i;
		pthread_create(&threads[i],NULL,FonctionThread,(void *)&myParam[i]);
	} 
	
	for (i=0;i<N_THREADS;i++)
	{
		pthread_join(threads[i],NULL);
	} 

	exit(0);
}