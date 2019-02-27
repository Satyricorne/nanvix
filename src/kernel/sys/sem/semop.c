#include <sys/sem.h>

PUBLIC int sys_semop(int semid, int op){

	if(op<0)
		return down(semid);
	else if(op>=0)
		return up(semid);
	else
		return -1;

}
