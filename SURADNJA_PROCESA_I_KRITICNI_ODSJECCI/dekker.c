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

int Id;

struct dekker {
       int zastavica[2];
	   int pravo;
} *dek;

void udji(int i, int j) {
	dek->zastavica[i] = 1;
	while (dek->zastavica[j] != 0) {
			if (dek->pravo == j) {
				dek->zastavica[i] = 0;
				while (dek->pravo == j);
				dek->zastavica[i] = 1;
			}
		}
}   

void izadji(int i, int j) {
	dek->pravo = j;
    dek->zastavica[i] = 0;
}


/* Funkcija koju izvodi svaki proces. Koristi Dekkerov algoritam. */
void Dekker(int i) {
	int j;
		
	if(i==0) j=1;
	if(i==1) j=0;
	
	for(int k=1; k<=5; k++){
		udji(i,j);
		for(int m=1; m<=3; m++){
				printf("Proces %d- K.O - Br. %d - (%d/3)\n",i+1,k,m);	
				sleep(1);			
		}
		izadji(i,j);
	}
}

void brisi(int sig) { 
  shmdt((char *) dek);
  shmctl(Id, IPC_RMID, NULL);
  exit(0);
}   
   
int main() {
	const int N = 2;
	
	sigset(SIGINT, brisi);
	Id = shmget(IPC_PRIVATE, sizeof(*dek)*100, 0600); 
	dek = shmat(Id, NULL, 0);
	dek->zastavica[0]=0;
	dek->zastavica[1]=0;
	dek->pravo=0;
	
	/* Pokreni 2 procesa. Koristi Dekkerov algoritam za sinkronizaciju. */
	for(int i=0; i<N; i++){
		switch(fork()){
			case 0: Dekker(i);
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


	brisi(0);

	return 0;
}
