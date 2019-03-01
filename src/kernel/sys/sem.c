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
	
	struct semaphore sem;
	(&sem)->key = compteurKey++;
	(&sem)->compteur = n;
	(&sem)->list = NULL;
	(&sem)->nextSem = NULL;

	struct semaphore * tmp = list_sem;
	while(tmp->nextSem != NULL){
		tmp = tmp->nextSem;
	}
	tmp->nextSem = &sem;
	return((&sem)->key);
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
			/*struct listProc * tmp = sem->list;
			while((tmp->nextProc) != NULL)
				tmp = tmp->nextProc;
			tmp->nextProc->proc = &p;*/
			sleep(&(sem->list->proc),0);
		}
		return 0;
	}
	return -1;
}

int destroy(int key) {

	struct semaphore * tmp = list_sem;
	struct semaphore * oldTmp = NULL;

	while(tmp->key != key || tmp->nextSem != NULL){
		oldTmp = tmp;
		tmp = tmp->nextSem;
	}
	if (tmp->nextSem == NULL) {			// Semaphore introuvable
		return(-1);
	} else if (oldTmp == NULL) {		// Semaphore en debut de chaine
		list_sem = tmp->nextSem;
	} else {							// Semaphore en milieu/fin de chaine
		oldTmp->nextSem = tmp->nextSem;
	}
	return(0);
}

