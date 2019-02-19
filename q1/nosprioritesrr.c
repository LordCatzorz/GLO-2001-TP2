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

} Parametres;

void change_scheduler(int scheduler)
{
	struct sched_param Param;
	int maxPriority = sched_get_priority_max(scheduler);
	int minPriority = sched_get_priority_min(scheduler);
	int meanPriority = (maxPriority + minPriority) / 2;
	Param.sched_priority = meanPriority;

	sched_setscheduler(0, scheduler, &Param);
	int ThreadID = syscall(SYS_gettid);
	printf("Code retour de errno pour changement d'ordonnanceur(%d) pour processus %d : %s. \n", scheduler, ThreadID, strerror(errno));
}

void *work(void *data)
{
	Parametres *pParam = (Parametres *)data;

	printf("Je suis le thread %d et je demarre !!! \n", pParam->ThreadNum);

	change_scheduler(SCHED_RR);
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
