#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>
#include <asm-generic/mman.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/wait.h>

// instruction pour compiler : gcc q2lock.c -o q2lock -lpthread
 
#define N_ITER 10000000
// variables globales (donc partagées entre threads)
long count = 0;

// ------------- Differents verrous pour le tp2 -------------
int lock = 0;       // Un verrou entier. 0 signifie qu'il est libre
int atomicLock = 0; // Le verrou atomique. 0 signifie qu'il est libre
pthread_mutex_t monMutex;

// http://en.wikipedia.org/wiki/Spinlock
// utilise la fonction assembleur xchgl
static inline int xchg(int *ptr, int x)
{
  asm volatile("lock\n" "xchgl %0,%1\n"
              :"=r" (x), "+m" (*(int*)ptr)
              :"0" (x)
              :"memory");
  return x;
}

// -------- à implémenter ----------
//2.1
void BadLock() {
}
	
void BadUnlock() {
}
//2.2
void AtomicLock() {

	while (xchg(&atomicLock,1));
	
}

void AtomicUnlock() {
	xchg(&atomicLock,0);
} 

void *CodeThread(void * a)
{
    long i;
    for(i = 0; i < N_ITER; i++)
    {
    	AtomicLock();
        count++;
	AtomicUnlock();
       	// La fonction de unlock à ajouter ici
    }
}


// -------- la fonction main ----------

int main(int argc, char **argv)
{
   pthread_t T1, T2;
   int iTrial;
   int correct = 0;
   for (iTrial = 0; iTrial < 10000; iTrial++) {
   	   count = 0;
	   pthread_create(&T1, 0, CodeThread, 0);
   	   pthread_create(&T2, 0, CodeThread, 0);
       pthread_join(T1, NULL);
       pthread_join(T2, NULL);
       printf("La valeur finale est %ld\n",count);
       if (count == 2*N_ITER) correct++;
       printf("%d/%d de correct!\n",correct,iTrial+1);   
   }
}
//20013A2018