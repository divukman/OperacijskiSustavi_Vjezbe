#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>
#include <unistd.h>


/* Funkcija koju izvodi svaki proces. Nema sinkronizacije. */
void Proces(int i) {
	for(int k=1; k<=5; k++){
		for(int m=1; m<=10; m++){
				printf("Proces %d- K.O - Br. %d - (%d/3)\n",i+1,k,m);				
		}
	}
}

   
int main() {
	const int N = 5;
	
	/* Pokreni 5 procesa. Nema medjusobnog iskljucivanja. */
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
		(void) wait NULL;
	}


	return 0;
}
