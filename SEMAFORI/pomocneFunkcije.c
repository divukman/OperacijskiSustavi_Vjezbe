#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int SemId;   /* identifikacijski broj skupa semafora */

void SemGet(int n)
{  /* dobavi skup semafora sa ukupno n semafora */
   SemId = semget(IPC_PRIVATE, n, 0600);
   if (SemId == -1) {
      printf("Nema semafora!\n");
      exit(1);
   }
}

int SemSetVal(int SemNum, int SemVal)
{  /* postavi vrijednost semafora SemNum na SemVal */
   return semctl(SemId, SemNum, SETVAL, SemVal);
}

int SemOp(int SemNum, int SemOp)
{  /* obavi operaciju SemOp sa semaforom SemNum */
   struct sembuf SemBuf;
   SemBuf.sem_num = SemNum;
   SemBuf.sem_op  = SemOp;
   SemBuf.sem_flg = 0;
   return semop(SemId, & SemBuf, 1);
}

void SemRemove(void)
{  /* uništi skup semafora */
   (void) semctl(SemId, 0, IPC_RMID, 0);
}

int main() {
	return 0;
}
