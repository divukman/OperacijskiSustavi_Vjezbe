/**
 * Operacijski sustavi - Aspira
 * Vjezba: prekidi i signali
 * @author dimitar vukman (divukman (at) gmail.com)
 */

#define _XOPEN_SOURCE 500 // X/Open 5, incorporating POSIX 1995

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define N 8    /* broj razina prekida */

#define TIMEOUT 10

int OZNAKA_CEKANJA[N] = {0};
int PRIORITET[N] = {0};
int TEKUCI_PRIORITET = 0;

/**
 * Funkcija ceka TIMEOUT sekundi i ispisuje stanje svake sekunde.
 * @param char* Ime pozivatelja.
 */
void trosi_vrijeme(char* ime) {
	int timer = 0;
	while (timer++ < TIMEOUT) {
		sleep(1);
		printf ("%s. Proteklo vrijeme: %d/%ds.\n", ime,  timer, TIMEOUT);
	}
}

/**
 * Funkcija za obradu prekida.
 */
void obrada_prekida(int j) {
	int timer = 0;
	char strPrekid[20]; //@todo optimizirati

	sprintf(strPrekid, "Obrada prekida %d", j);

	printf ("Poceo obradu prekida %d\n", j);

	/* obrada se simulira trošenjem vremena - 10 s */
	trosi_vrijeme(strPrekid);

	printf ("Zavrsio obradu prekida %d\n", j);
}

/**
 * Funkcija za prekidnu rutinu.
 */
void prekidna_rutina (int sig)
{
	int i, x, j;
	time_t t;

	time(&t);
	printf("Prekidna rutina pozvana u: %s", ctime(&t));

	printf ("Upisi razinu prekida ");
	sighold(SIGINT);
	scanf ("%d", &i);
	sigrelse(SIGINT);
	OZNAKA_CEKANJA[i] = 1;

	do {
		x = 0;
		
		// Pretrazi postoji li prekid veceg prioriteta na cekanju
		for (j = TEKUCI_PRIORITET + 1; j < N; j++) {
			if (OZNAKA_CEKANJA[j] > 0) {
				x = j;
			}
		}

		// Ako postoji prekid veceg prioriteta, obradi njega
		if (x > 0) {
			OZNAKA_CEKANJA[x] = 0;
			PRIORITET[x] = TEKUCI_PRIORITET;
			TEKUCI_PRIORITET = x;
			sigrelse(SIGINT);
			obrada_prekida(x);
			sighold(SIGINT);
			TEKUCI_PRIORITET = PRIORITET[x];

		}

	} while (x > 0);


}

/**
 * Main funkcija.
 */
int main (void)
{
	sigset (SIGINT, prekidna_rutina);

	printf ("Poceo osnovni program\n");

	/* troši vrijeme da se ima šta prekinuti - 10 s */
	trosi_vrijeme("Main funkcija");
	printf ("Zavrsio osnovni program\n");

	return 0;
}

