#include sem.h;

int create(int key) {
	if (list_sem[key] != NULL)
	{
		return(-1);
	}
	struct semaphore sem;
	sem->key = key;
	sem->compteur;
	sem->proc = NULL;

	list_sem = (key)*malloc(sizeof(struct semaphore));
	list_sem[key] = sem;

	return(0);
}

int up(int key) {
	if(list_sem[key]->compteur == 0) {
		
	}
}

int down(int key) {

}

int destroy(int key) {

}