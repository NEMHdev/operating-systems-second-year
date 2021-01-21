#include <lib.h>
#include <unistd.h>
#include <errno.h>

int printchildpids(pid_t pid) {

    message m;      // Minix uses message to pass parameters to a system call
   
    m.m1_i1 = i;    // set first integer of message to i
 
    return _syscall(PM_PROC_NR, PRINTCHILDPIDS, &m);
	
}

int getnchildren(pid_t pid) {
    
    message m;      // Minix uses message to pass parameters to a system call
   
    m.m1_i1 = i;    // set first integer of message to i
 
	return _syscall(PM_PROC_NR, GETNCHILDREN, &m);
}
        
int getchildpids(pid_t pid, int nchildren, pid_t *childpids) {
       message m;      // Minix uses message to pass parameters to a system call
   
    m.m1_i1 = i;    // set first integer of message to i
	m.m1_i2 = j;
 
		pid_t userarray[j];
 
	m.m1_p1 = (char*)userarray;
 
 
    return _syscall(PM_PROC_NR, GETCHILDPIDS, &m);
	
}
