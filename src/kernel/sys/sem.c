#include sem.h;

int create(int key, int n) {
	if (list_sem[key] != NULL)
	{
		return(-1);
	}
	struct semaphore sem;
	sem->key = key;
	sem->compteur = n;
	sem->proc = NULL;

	list_sem[key] = sem;

	return(0);
}

int up(int key) {
	if(list_sem[key]->compteur == 0) {
		list_sem[key]->compteur++;
	} else {
		wait();
	}
}

int down(int key) {
	if(list_sem[key]->compteur > 0) {
		list_sem[key]->compteur--;
	} else {
		list_sem[key]->
	}
}

int destroy(int key) {
	free(list_sem[key]);
	list_sem[key] = NULL;
}