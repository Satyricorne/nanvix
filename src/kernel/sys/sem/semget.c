#include <sem.h>
bool listContainKey(struct semaphore s[], unsigned k){
	int i = 0;
	int sizeOfTab = sizeof(s)/sizeof(s[0]);
	while(i<sizeOfTab){
		if(s[i++].key == k)
			return true;
	}
	return false;
}

int semget(unsigned key){
	if(!listContainKey(list_sem,key)){
		struct semaphore sem;
		sem.key = key;
		sem.compteur = 0;
		list_sem[key] = sem;
	}
	return key;
}