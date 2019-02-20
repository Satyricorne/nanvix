#include "../../../include/sys/sem.h"

int sys_semctl(int semid, int cmd, int val){
  if(cmd == GETVAL){
    return list_sem[semid].compteur;
  }else if(cmd == SETVAL){
    list_sem[semid].compteur = val;
    return 0;
  }else if(cmd == IPC_RMID){
    list_sem[semid].compteur = NULL;
    return 0;
  }else{
    return -1;
  }
}
