#include <stdio.h>
#include "pm.h"             // for glo.h:  mp, call_nr, who_p etc.
#include "mproc.h"          // for proc table

// system call for library function printchildpids
int do_printchildpids() {
 pid_t pid = m_in.m1_i1;    
 
 printf("Searching for children of process: %d", pid );
 
 if ( pid > NR_PROCS || pid < 0) {
   return -1;
  }
    
 for (int i=0; i<NR_PROCS; i++){
  int parentidx = mproc[i].mp_parent;
  if(mproc[parentidx].mp_pid == pid && mproc[i].mp_pid != 0){
   printf("%d\n", mproc[i].mp_pid);
  }
 }
 return 0;
}

// system call for library function getnchildren
int do_getnchildren() {
   
  pid_t pid = m_in.m1_i1;  
 int count = 0;
// printf("Count of children of process\n");
 
 if ( pid > NR_PROCS || pid < 0 ) {
   return -1;
  }
    
 for (int i=0; i<NR_PROCS; i++){
  int parentidx = mproc[i].mp_parent;
  if(mproc[parentidx].mp_pid == pid && mproc[i].mp_pid != 0){
   
   count++;
   
  }
 }
 //printf("%d\n", count);
 return count;
}

// system call for library function getchildpids
int do_getchildpids() {
   
    pid_t pid = m_in.m1_i1;
    int nchildren = m_in.m1_i2;
    vir_bytes pointsTo = (vir_bytes)m_in.m1_p1;
   
 pid_t childpids[nchildren];
 vir_bytes pointer = (vir_bytes)childpids;
 
 
  if ( /*pid > NR_PROCS ||*/ pid < 0 ) {
   return -1;
  }
 // need a index to remember position in array int arrayPos = 0;
 int arrayPos = 0;
 
 for (int i=0; i<NR_PROCS && arrayPos < nchildren; i++){
  int parentidx = mproc[i].mp_parent;
  if(mproc[parentidx].mp_pid == pid && mproc[i].mp_pid != 0){
   childpids[arrayPos] = mproc[i].mp_pid;
   arrayPos++;
  } 
 }
 
 int result = sys_vircopy(SELF, pointer, who_e, pointsTo, sizeof(childpids));

 
 if(result != 0){
  return -1;
 }
 
 if ( sizeof(childpids) == 0 || pid < 1){
  return 0;
 }
 
 //printf("\n Number of Child Processes: %d \n", arrayPos);
 
 //printf("\n Child pids array:\n");
 
 for(int k=0; k < sizeof(childpids); k++){
 // printf("%d\n", childpids[k]);
 }
 
 //return the index of the array return arrayPos;
    return arrayPos;
}