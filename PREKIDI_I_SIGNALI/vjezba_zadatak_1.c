#define _XOPEN_SOURCE 500 // X/Open 5, incorporating POSIX 1995

#include <stdio.h>
#include <signal.h>
#include <unistd.h>


/* 
 * Funkcija za obradu prekida. 
 */
void sigIntHandler(int signal) {
	/* Ispisi neku poruku. */
	printf ("Funkcija sigIntHandler. \n");
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
		printf("Spavam 1 sekundu u main funkciji. \n");		
	}

	/* Program zavrsen, vrati 0. */
	return 0;
}