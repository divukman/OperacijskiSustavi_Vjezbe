#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

/*
Kompajlirati sa naredbom:
	gcc -D_REENTRANT -lpthread zadatak1.c -o zadatak1
*/


/* Deklaracija. Zajednicka, globalna, varijabla A. */
int A;

/* Deklaracija. Zajednicka, globalna, varijabla N. */
int N;

/* Funkcija koju ce izvrsavati svaki thread. */
void * PosaoDretve(void * x) {
  for (int i = 0; i < N; i ++) {
	  printf("Thread output: A = %d\n", (* (int*)x));
	  (*(int*)x)++;
  }
}

/* Main funkcija. */
int main(void) {
   int A = 1, D = 0;
   pthread_t * thr_id;   
  
  /* Prvo obavimo unos podataka. Alternativa je koristiti argumente main funkcije.*/ 
   printf("Upisite broj dretvi D: ");
   scanf("%d", &D);
   
   printf("Upisite broj N: ");
   scanf("%d", &N);
   
   printf("Upisali ste: D = %d, N = %d\n", D, N);
   printf("*************************************\n");

	/* Moramo alocirati memoriju koja ce cuvati ID-ove threadova. */
	thr_id = (pthread_t *) malloc(D * sizeof(pthread_t));

   /* Pokretanje threadova. */
   for (int i = 0; i < D; i ++) {
	    if (pthread_create(&thr_id[i], NULL, PosaoDretve, &A) != 0) {
		  printf("Greska pri stvaranju dretve!\n");
		  exit(1);
	   }
   }	
	
  /* Cekanje na threadove. */
  for (int i = 0; i < D; i ++) { 
	  printf ("Cekam na thread sa ID-om: %d\n", (int)thr_id[i]);
	  pthread_join(thr_id[i], NULL);
  }
  
  sleep(2);
  
  /* Ispis vrijednosti varijable A. */
  printf("Svi threadovi su zavrsili, vrijednost varijable A je: A = %d\n", A);
   

   return 0;
}
