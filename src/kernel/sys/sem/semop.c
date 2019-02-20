#include "sem.h"

int sys_semop(int semid, int op){

  if(op<0){
    down(semid);
  }else{
    up(semid);
  }
}
