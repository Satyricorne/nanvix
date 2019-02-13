#include <sem.h>

int semget(unsigned key){
	if(!(list_sem[key].key == key)){
		struct semaphore sem;
		sem.key = key;
		sem.compteur = 0;
		list_sem[key] = sem;
	}
	return key;
}