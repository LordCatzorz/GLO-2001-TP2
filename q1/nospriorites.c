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
#define CHOIX_PRIORITE 2 //Changer ce paramètre pour modifier la priorité des threads.

int arr_thread_priorite[4][5] = {
	{0,0,0,0,0},
	{1,2,3,4,5},
	{9,7,5,3,1},
	{0,-4,-2,3,4}
};

typedef struct
{
	int ThreadNum;
} Parametres;

void *work(void *data)
{
	Parametres *pParam = (Parametres *)data;

	printf("Je suis le thread %d et je demarre !!! \n", pParam->ThreadNum);

	int ThreadID = syscall(SYS_gettid);
	int ret;
	ret = setpriority(PRIO_PROCESS, ThreadID, arr_thread_priorite[CHOIX_PRIORITE][pParam->ThreadNum]);

	printf("Code retour de setpriority() pour processus %d : %d. \n", pParam->ThreadNum, ret);
	printf("Code retour de errno pour processus %d : %s. \n", pParam->ThreadNum, strerror(errno));

	while (1)
		;

	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[N_THREADS];
	Parametres myParam[N_THREADS];
	int i;
	for (i = 0; i < N_THREADS; i++)
	{
		printf("creation thread %d! \n", i);
		myParam[i].ThreadNum = i;
		pthread_create(&threads[i], NULL, work, (void *)&myParam[i]);
	}

	for (i = 0; i < N_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}

	exit(0);
}