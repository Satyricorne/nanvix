#include "../../../include/sys/sem.h"

int sys_semctl(int semid, int cmd, int val){

  if(cmd == GETVAL){
    return val;
  }else if(cmd == SETVAL || cmd == IPC_RMID){
    return 0;
  }else{
    return -1;
  }
}
