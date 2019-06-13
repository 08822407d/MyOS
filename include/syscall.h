#ifndef _SYSCALL_H_
#define _SYSCALL_H_

    #include "type.h"
    
    #define INTVEC_SYSCALL  0x80
    #define NR_SYSCALL      0x02
        
    EXTERN  void            sys_call();
    EXTERN  void            init_syscall_table();
    
    /* 系统调用 - 系统级 */
    /* proc.c */
    PUBLIC	int	sys_sendrec(int function, int src_dest, MESSAGE_t* m, PROC_t* p);
    PUBLIC	int	sys_printx(int _unused1, int _unused2, char* s, PROC_t* p_proc);

    /* 系统调用 - 用户级 */
    EXTERN	int	sendrec(int function, int src_dest, MESSAGE_t* p_msg);
    EXTERN	int	printx(char* str);

    #define SC_SENDRECV     0x00   
    #define SC_PRINTX       0x01

#endif /* _SYSCALL_H_ */