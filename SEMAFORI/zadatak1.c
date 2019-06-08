#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#ifndef _SEMUN_H
#define _SEMUN_H
union semun {
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short int *array; /* array for GETALL, SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
};
#endif

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);
static int sem_id;

/* Funkcija koju izvodi svaki proces. */
void Proces(int i) { 
	for(int k=1; k<=5; k++) {
		if (!semaphore_p()) exit(EXIT_FAILURE);
		for(int m=1; m<=10; m++){
				printf("Proces %d- K.O - Br. %d - (%d/3)\n",i+1,k,m);				
		}
		if (!semaphore_v()) exit(EXIT_FAILURE);	
	}
}


int main(int argc, char *argv[]) {
	const int N = 2;
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
	
	if (!set_semvalue()) {
			fprintf(stderr, "Failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}
		
	for(int i=0; i<N; i++){
		switch(fork()){
			case 0: Proces(i);
					exit(0);
					break;					
			case -1: printf("Greska prilikom kreiranja procesa %d",i+1);
					 exit(1);
					 break;
		}
	}

	/* Pokupi procese. */
	for(int i=0; i<N; i++){
		(void) wait(NULL);
	}

	del_semvalue();	
	printf("\n%d − finished\n", getpid());
	exit(EXIT_SUCCESS);
}

static int set_semvalue(void)
{
	union semun sem_union;
	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
	return(1);
}

static void del_semvalue(void)
{
	union semun sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p(void)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_p failed\n");
		return(0);
	}
	return(1);
}

static int semaphore_v(void)
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_v failed\n");
		return(0);
	}
	return(1);
}
