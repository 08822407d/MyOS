#ifndef _MYOS_PROC_H_
#define _MYOS_PROC_H_

	#include "const.h"
	#include "type.h"
	#include "archtypes.h"
	
	#define	RPL_TASK	SA_RPL1
	#define INIT_PSW	0x1202
/* 一些常量 */
	#define NR_PROCS	3
	#define TASK_PROC	TASK_PRIVILEGE
	#define USER_PROC	USER_PRIVILEGE
	

	struct proc_s
	{
		struct stackframe_s p_reg;   /* process registers saved in stack frame */

		u16_t ldt_sele;                      /* gdt selector giving ldt base and limit */

//        	int ticks;
//        	int priority;

		u32_t pid;                          /* process id passed in from MM */
	};

	struct proctable_s
	{
		proc_f	initial_eip;
		int		stacksize;
		u16_t	proc_type;
	};


/* stacks of tasks */
	#define STACK_SIZE_TESTA	0x8000
	#define STACK_SIZE_TESTB	0x8000
	#define STACK_SIZE_TESTC	0x8000

	#define STACK_SIZE_TOTAL	(STACK_SIZE_TESTA + \
				            	STACK_SIZE_TESTB + \
				            	STACK_SIZE_TESTC)


#endif