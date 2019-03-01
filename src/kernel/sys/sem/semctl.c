#include "sys/sem.h"
#define MAX_SIZE 100

int sys_semctl(int semid, int cmd, int val){

  struct semaphore * sem = getSem(semid);

  if(cmd == GETVAL){
    return sem->compteur;
  }else if(cmd == SETVAL){
    sem->compteur = val;
    return 0;
  }else if(cmd == IPC_RMID){
    destroy(semid);
    return 0;
  }else{
    return -1;
  }
}
