#include "../../../include/sys/sem.h"
#define MAX_SIZE

int sys_semctl(int semid, int cmd, int val){
    struct semaphore * sem = getSem(semid);
    if(sem != NULL){
      if(cmd == GETVAL){
        return sem.compteur;
      }else if(cmd == SETVAL){
        sem.compteur = val;
        return 0;
      }else if(cmd == IPC_RMID){
        sem.compteur = NULL;
        return 0;
      }else{
        return -1;
      }
    }else{
      return -1;
    }
}
