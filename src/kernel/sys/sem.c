#include <sys/sem.h>

struct semaphore create(int n) {
	
	struct semaphore sem;
	sem.compteur = n;
	sem->listProc = NULL;

	return(sem);
}

int up(int key) {
	if(list_sem[key].compteur == 0) {
		//wait();
	} else {
		list_sem[key].compteur++;
	}
	return(0);
}

int down(int key) {
	if(list_sem[key].compteur > 0) {
		list_sem[key].compteur--;
	} else {
		//list_sem[key].proc[list_sem[key].compteurProcess];
	}
	return(0);
}

int destroy(int key) {
	list_sem[key] = NULL;
	return(0);
}