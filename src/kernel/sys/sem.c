#include <sys/sem.h>
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

struct semaphore create(int n) {
	
	struct semaphore sem = malloc(sizeof(struct semaphore));
	sem.key = compteurKey++;
	sem.compteur = n;
	sem->listProc = NULL;

	struct tmp = list_sem;
	while(tmp->nextSem != NULL){
		tmp = tmp->nextSem;
	}
	tmp->nextSem = sem;
}

int up(int key) {
	struct semaphore * sem = getSem(key);
	if(sem != NULL) {
		if(sem.compteur == 0){
			wakeup(sem->listProc);
			sem->listProc = sem->listProc->nextProc;
		}else{
			sem.compteur++;
		}
	} else {
		return -1;
	}
	return(0);
}

int down(int key) {
	struct semaphore * sem = getSem(key);
	if(sem != NULL){
		if(sem->compteur > 0)
			sem->compteur--;
		else if(sem->compteur == 0){
			sleep(listProc,0);
		}
		return 0;

	}
	return -1;
}

int destroy(int key) {

	struct tmp = list_sem;
	while(tmp->key != key && tmp->nextSem != NULL){
		tmp = tmp->nextSem;
	}

}

