#define _XOPEN_SOURCE 500 // X/Open 5, incorporating POSIX 1995

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int broj_poziva = 0;

/* 
 * Funkcija za obradu prekida. 
 */
void sigIntHandler(int signal) {
	int broj = 0;
	
	int brPoziva = broj_poziva + 1;
	broj_poziva = broj_poziva + 1; /* ili jednostavno brPoziva = ++broj_poziva */
	
	
	/* Ispisi neku poruku. */
	printf ("\nFunkcija sigIntHandler. \n");
	
	/* Ucitaj brojcanu vrijednost. */
	printf ("Upisite broj 1 - 10. \n");
	scanf("%d", &broj);
	
	/* Ispisi unesenu vrijednost. */
	printf("Upisali ste broj: %d.\n", broj);
	
	/* Spavaj N(broj) sekundi. */
	for (int i = 0; i < broj; i++) {
		sleep(1);
		printf("Spavam %d/%d sekundi u sigIntHandler (%d) funkciji. \n", i+1, broj, brPoziva);
	}
	
}


/**
 * Main funkcija.
 */
int main (void) {
	/* Postavi funkciju za obradu SIGINT signala. */
	sigset (SIGINT, sigIntHandler);

	/* Ispisi neku poruku. */
	printf ("Main funkcija. \n");

	/* Spavaj 10 sekundi. */
	for (int i = 0; i < 10; i++) {
		sleep(1);
		printf("Spavam %d/%d sekundi u main funkciji. \n", i+1, 10);		
	}

	/* Program zavrsen, vrati 0. */
	return 0;
}
