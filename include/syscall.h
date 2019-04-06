#ifndef _SYSCALL_H_
#define _SYSCALL_H_

    #include "type.h"
    
    #define INTVEC_SYSCALL  0x80
    #define NR_SYSCALL      0x01
        
    EXTERN  void            sys_call();
    EXTERN  void            init_syscall_table();

    #define SC_GET_TICKS    0x00   
    extern  unsigned int    get_ticks();

#endif /* _SYSCALL_H_ */