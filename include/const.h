#ifndef	_MyOS_CONST_H_
#define	_MyOS_CONST_H_
    #include "proc.h"

    #define HZ              20
    #define K_STACK_SIZE    0x4000

    #define NR_CONSOLES     4

    #define	STR_DEFAULT_LEN	1024

/*======================================================================*
                            一些系统参数
 *======================================================================*/
    #define GDT_SIZE    1024
    #define IDT_SIZE    256
    #define LDT_SIZE    3

/*======================================================================*
                            task相关参数
 *======================================================================*/
    /* 注意 TASK_XXX 的定义要与 global.c 中对应 */
    #define INVALID_DRIVER -20
    #define INTERRUPT -10
    #define TASK_TTY 0
    #define TASK_SYS 1
    /* #define TASK_WINCH	2 */
    /* #define TASK_FS	3 */
    /* #define TASK_MM	4 */
    #define ANY (NR_TASK_PROCS + NR_USER_PROCS + 10)
    #define NO_TASK (NR_TASK_PROCS + NR_USER_PROCS + 20)

    /* system call */
    #define NR_SYS_CALL 3

    /* ipc */
    #define SEND 1
    #define RECEIVE 2
    #define BOTH 3 /* BOTH = (SEND | RECEIVE) */
    /* Process */
    #define SENDING   0x02	/* set when proc trying to send */
    #define RECEIVING 0x04	/* set when proc trying to recv */

    /* magic chars used by `printx' */
    #define MAG_CH_PANIC '\002'
    #define MAG_CH_ASSERT '\003'

    /* @enum msgtype @brief MESSAGE types */
    enum msgtype
    {
        /* when hard interrupt occurs, a msg (with type==HARD_INT)
	        will be sent to some tasks */
        HARD_INT = 1,
        /* SYS task */
        GET_TICKS,
    };

    #define RETVAL u.m3.m3i1

#endif /* _MyOS_CONST_H_ */
