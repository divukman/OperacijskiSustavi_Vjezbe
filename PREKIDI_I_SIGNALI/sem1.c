#define _XOPEN_SOURCE 500 // X/Open 5, incorporating POSIX 1995

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define N 8    /* broj razina prekida */

int OZNAKA_CEKANJA[N] = {0};
int PRIORITET[N] = {0};
int TEKUCI_PRIORITET = 0;


void obrada_prekida(int j)
{
   printf ("Poceo obradu prekida %d\n", j);
   
   /* obrada se simulira trošenjem vremena - 10 s */
     for (int i = 0; i < 10; i++) { 
		sleep(1);
		printf("Obrada prekida %d. %d/%ds.\n",j, i+1, 10);		
	}
	
   printf ("Zavrsio obradu prekida %d\n", j);
}

void prekidna_rutina (int sig)
{
   int i, x,j,z;
   time_t t;

   time(&t);
   printf("Prekidna rutina pozvana u: %s", ctime(&t));

   printf ("Upisi razinu prekida ");
   scanf ("%d", &i);  
   
   OZNAKA_CEKANJA[i] = 1;
   
   j = i;
   while (OZNAKA_CEKANJA[j] != 0 && j > TEKUCI_PRIORITET) {	      	

		   OZNAKA_CEKANJA[j] = 0;
		   PRIORITET[j] = TEKUCI_PRIORITET;
		   TEKUCI_PRIORITET = j;
		   sigrelse(SIGINT);
		   obrada_prekida(j);		   
		   sighold(SIGINT);		   		   		  		 
		   TEKUCI_PRIORITET = PRIORITET[j];  
	   
		  for (z = 0; z < N; z++) {
			if (OZNAKA_CEKANJA[z] != 0) {
				j = z;						
			}
	  }
		     
   }
   
   sigrelse(SIGINT);
 /*  do {
	   x = 0;
	   
	
	   for (j = TEKUCI_PRIORITET + 1; j < N; j++) {
			if (OZNAKA_CEKANJA[j] != 0) {
				x = j;
			}
		}
		
	
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
	*/      
}

int main (void)
{
   sigset (SIGINT, prekidna_rutina);

   printf ("Poceo osnovni program\n");
   
   /* troši vrijeme da se ima šta prekinuti - 10 s */
   for (int i = 0; i < 10; i++) {		
		sleep(1);
		printf("Main funkcija. %d/%ds.\n", i+1, 10);		
	}
	
   printf ("Zavrsio osnovni program\n");

   return 0;
}
