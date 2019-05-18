#ifndef _SYSCALL_H_
#define _SYSCALL_H_

    #include "type.h"
    
    #define INTVEC_SYSCALL  0x80
    #define NR_SYSCALL      0x02
        
    EXTERN  void            sys_call();
    EXTERN  void            init_syscall_table();

    #define SC_GET_TICKS    0x00   
    extern  unsigned int    get_ticks();
    #define SC_WRITE        0x01
    extern  unsigned int    write();

#endif /* _SYSCALL_H_ */