#include <sys/sem.h>
#include <stdlib.h>

int compteurKey = 0;

struct semaphore * getSem(int key){
	struct semaphore * l = list_sem;
	while (l != NULL){
		if(l->key == key)
			return l;
		else
			l = l->nextSem;
	}
	return NULL;
}

int create(int n) {
	
	struct semaphore * sem = malloc(sizeof(struct semaphore));
	sem->key = compteurKey++;
	sem->compteur = n;
	sem->list = NULL;
	sem->nextSem = NULL;

	struct semaphore * tmp = list_sem;
	while(tmp->nextSem != NULL){
		tmp = tmp->nextSem;
	}
	tmp->nextSem = sem;
	return(0);
}	

int up(int key) {
	struct semaphore * sem = getSem(key);
	if(sem != NULL) {
		if(sem->compteur == 0){
			wakeup(&(sem->list->proc));
			sem->list = sem->list->nextProc;
		}else{
			sem->compteur++;
		}
	} else {
		return(-1);
	}
	return(0);
}

int down(int key) {
	struct semaphore * sem = getSem(key);
	if(sem != NULL){
		if(sem->compteur > 0)
			sem->compteur--;
		else if(sem->compteur == 0){
			sleep(&(sem->list->proc),0);
		}
		return 0;

	}
	return -1;
}

int destroy(int key) {

	struct semaphore * tmp = list_sem;
	while(tmp->key != key && tmp->nextSem != NULL){
		tmp = tmp->nextSem;
	}
	return(0);
}

