#define _XOPEN_SOURCE 500 // X/Open 5, incorporating POSIX 1995

#include <stdio.h>
#include <signal.h>
#include <time.h>

#define N 8


int oznaka_cekanja[N]={0};
int prioritet[N]={0};
int TP=0;


void obrada_prekida (int p){
	int j;
	printf("Poceo obradu prekida: %d\n", p);
	for(j=1; j < 11; ++j){
		printf("Obrada prekida: %d/10\n", j);
		usleep(1000000);
	}
	printf("Zavrsio obradu prekida razine: %d\n\n", p);
}

void prekidna_rutina (int sig){
	int i, x, j;
	time_t t;

	time(&t);
	printf("Prekidna rutina pozvana u: %s", ctime(&t));


	printf("Upisi razinu prekida: ");
	sighold(14);
	scanf("%d", &i);
	sigrelse(14);

	oznaka_cekanja[i] = 1;
	do{

		x=0;
		for(j=TP+1; j<N; j++){
			if(oznaka_cekanja[j] != 0)
				x=j;
		}

		if(x>0){
			oznaka_cekanja[x] = 0;
			prioritet[x] = TP;
			TP = x;
			sigrelse(SIGINT);
			obrada_prekida(x);
			sighold(SIGINT);
			TP = prioritet[x];
		}
	}while(x>0);



}
int main (void)
{
	int j;
	sigset (SIGINT, prekidna_rutina);
	printf ("Poceo osnovni program\n");
	for(j=1; j < 11; ++j){
		printf("Glavni program: %d/10 \n", j);
		usleep(1000000);
	}
	printf ("Zavrsio osnovni program\n");

	return 0;
}
