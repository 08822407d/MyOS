#ifndef _SYSCALL_H_
#define _SYSCALL_H_

    #include "type.h"
    
    #define INTVEC_SYSCALL  0x80
    #define NR_SYSCALL      0x02
        
    EXTERN  void            sys_call();
    EXTERN  void            init_syscall_table();
    
    EXTERN  void            sendrec();
    EXTERN  void            printx();

    #define SC_SENDRECV     0x00   
    #define SC_PRINTX       0x01

#endif /* _SYSCALL_H_ */