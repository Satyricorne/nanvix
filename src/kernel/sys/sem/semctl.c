#include "sys/sem.h"
#define MAX_SIZE

int sys_semctl(int semid, int cmd, int val){

  struct semaphore * sem = getSem(semid);

  for(int i = 0 ; i < MAX_SIZE ; i++){
    if(sem->list->key == semid){
      if(cmd == GETVAL){
        return list_sem->compteur;
      }else if(cmd == SETVAL){
        list_sem->compteur = val;
        return 0;
      }else if(cmd == IPC_RMID){
        list_sem->compteur = NULL;
        return 0;
      }else{
        return -1;
      }
    }
    sem->list = sem->list_pro->next;
  }
}
