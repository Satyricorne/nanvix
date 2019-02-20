#include sem.h;

int create(int key, int n) {
	if (list_sem[key] != NULL)
	{
		return(-1);
	}
	struct semaphore sem;
	sem.key = key;
	sem.compteur = n;
	sem.compteurProcess = 0;
	sem.proc = NULL;

	list_sem[key] = sem;

	return(0);
}

int up(int key) {
	if(list_sem[key].compteur == 0) {
		list_sem[key].compteur++;
	} else {
		wait();
	}
	return 0;
}

int down(int key) {
	if(list_sem[key].compteur > 0) {
		list_sem[key].compteur--;
	} else {
		list_sem[key].proc[list_sem[key].compteurProcess];
	}
	return 0;
}

int destroy(int key) {
	free(list_sem[key]);
	list_sem[key] = NULL;
}