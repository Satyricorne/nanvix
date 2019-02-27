#include <sys/sem.h>


PUBLIC int sys_semget(unsigned int key){
	semaphore * sem = getSem(key);
	if(sem == NULL){
		create((int)key);
		return key;
	}else if (sem != NULL){
		return key;
	}else{
		return -1; // Error
	}	
}