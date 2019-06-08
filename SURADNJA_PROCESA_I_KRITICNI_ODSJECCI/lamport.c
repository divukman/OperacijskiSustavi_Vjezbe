#define _XOPEN_SOURCE 500 // X/Open 5, incorporating POSIX 1995

#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

/* Definicija broja procesa. */
#define N 5

/* ID-ovi segmenata. */
int ID_TRAZIM = 0;
int ID_BROJ = 0;

/* Pointeri na segmente. Zajednicka memorija. */ 
int * TRAZIM;
int * BROJ;

/* Funkcija pronalazi i vraca najveci broj u nizu.*/
int getMax(int arr[]) {
	int result =  arr[0]; //-1;
	
	for (int i = 1; i < N; i++) {
		if (arr[i] > result) {
			result = arr[i];
		}
	}
	
	return result;
}

/* Funkcija za usporedbu u Lamportovom algoritmu. */
int compare(const int i, const int j) {
	int result = 0;
	
	if ( (BROJ[j] < BROJ[i]) ||  ((BROJ[j] == BROJ[i]) && (j < i)) ) {
		result = 1;
	} 
	
	return result;
}

/* Funkcija koju izvodi svaki proces. Koristi Lamportov algoritam. */
void Lamport(int i) {
	for(int k=1; k<=5; k++) {
		
		TRAZIM[i] = 1;
		BROJ[i] = getMax(BROJ) + 1;
		TRAZIM[i] = 0;
		
		for (int j = 0; j < N; j++) {
			while(TRAZIM[j] != 0);
			while ( (BROJ[j] != 0) && compare(i,j));
		}
		for(int m=1; m<=10; m++) {
				printf("Proces %d- K.O - Br. %d - (%d/10)\n",i+1,k,m);				
		}
		
		BROJ[i] = 0;
	}
}

void brisi(int sig) { 
  shmdt((int *) TRAZIM);
  shmdt((int *) BROJ);
  shmctl(ID_TRAZIM, IPC_RMID, NULL);
  shmctl(ID_BROJ, IPC_RMID, NULL);
  exit(0);
}   
   
int main() {	
	/* Postavi SIGINT handler. */
	sigset(SIGINT, brisi);
		
	/* Alociraj memoriju, postavi pointere. */
	ID_TRAZIM = shmget(IPC_PRIVATE, sizeof(int)*N, 0600); 
	ID_BROJ = shmget(IPC_PRIVATE, sizeof(int)*N, 0600); 
	
	if (ID_TRAZIM == -1 || ID_BROJ == -1) {
		printf("Greska kod alokacije memorije!\n");
		exit(1);
	}
	
	printf("ID_TRAZIM = %d, ID_BROJ = %d\n", ID_TRAZIM, ID_BROJ);
	printf("Velicina memorije: %d B\n", (int)sizeof(int)*N);
	
	TRAZIM = (int *) shmat(ID_TRAZIM, NULL, 0);
	BROJ = (int *)  shmat(ID_BROJ, NULL, 0);
	
	
	printf("N = %d, sizeof(TRAZIM) = %d\n",N, (int)sizeof(TRAZIM));
	printf("N = %d, sizeof(BROJ) = %d\n",N, (int)sizeof(BROJ));
	
	
	for (int i = 0; i < N; i++) {
		TRAZIM[i] = 0;	
		BROJ[i] = 0;
	}
	
	for (int i = 0; i < N; i++) {		
		printf("TRAZIM[%d] = %d\n",i,TRAZIM[i]);
		printf("BROJ[%d] = %d\n",i,BROJ[i]);
	}

	
	/* Pokreni N procesa. Koristi Lamportov algoritam za sinkronizaciju. */
	for(int i=0; i<N; i++){
		switch(fork()){
			case 0: Lamport(i);
					exit(0);
					break;					
			case -1: printf("Greska prilikom kreiranja procesa %d",i+1);
					 exit(1);
					 break;
		}
	}

	/* Pokupi procese. */
	for(int i=0; i<N; i++){
		(void) wait NULL;
	}

	/* Ocisti memoriju. */
	brisi(0);

	/* Vrati status procesa, 0 (sve uspjesno). */
	return 0;
}
