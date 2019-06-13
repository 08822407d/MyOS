#ifndef _MYOS_PROC_H_
#define _MYOS_PROC_H_

	#include "const.h"
	#include "type.h"
	#include "archtypes.h"
	
	#define	RPL_TASK		SA_RPL1
	#define TASK_PSW		0x1202
	#define USER_PSW		0x0202
	/* 一些常量 */
	#define	NR_TASK_PROCS	2
	#define NR_USER_PROCS	3
	#define TASK_PROC		TASK_PRIVILEGE
	#define USER_PROC		USER_PRIVILEGE
	
	#define FIRST_PROC	PCB[0]
	#define LAST_PROC	PCB[NR_TASK_PROCS + NR_USER_PROCS - 1]

	struct proc_s
	{
		/* process registers saved in stack frame */
		STACK_FRAME_t p_reg;
		/* gdt selector giving ldt base and limit */
		u32_t ldt_sele;

		SEG_DESC_t ldt[LDT_SIZE];
		
        unsigned int ticks;
        unsigned int priority;
		/* process id passed in from MM */
		u32_t pid;
		/* name of the process */
		char name[16];
  		/* process flags.A proc is runnable iff p_flags==0 */
		int  p_flags;

		MESSAGE_t * p_msg;
		int p_recvfrom;
		int p_sendto;

		/* nonzero if an INTERRUPT occurred when the task
			is not ready to deal with it. */
		int has_int_msg;

		/* queue of procs sending messages to this proc */
		struct proc * q_sending;
		/* next proc in the sending queue (q_sending) */
		struct proc * next_sending;

		u32_t tty_idx;
	};
	typedef struct proc_s PROC_t;

	struct proctable_s
	{
		proc_f	initial_eip;
		int		stacksize;
		char*	proc_name;
	};
	typedef struct proctable_s PROC_TABLE_t;

	PUBLIC void TestA(void);
	PUBLIC void TestB(void);
	PUBLIC void TestC(void);

	PUBLIC void Task_tty();
	PUBLIC void Task_sys();

	PUBLIC void restart(void);

	/* stacks of tasks */
	#define STACK_SIZE_TESTA	0x8000
	#define STACK_SIZE_TESTB	0x8000
	#define STACK_SIZE_TESTC	0x8000
	#define STACK_SIZE_TASKTTY	0x8000
	#define STACK_SIZE_TASKSYS	0x8000

	#define STACK_SIZE_TOTAL	(STACK_SIZE_TESTA + \
				            	STACK_SIZE_TESTB + \
				            	STACK_SIZE_TESTC + \
								STACK_SIZE_TASKTTY + \
								STACK_SIZE_TASKSYS)

#endif /* _MYOS_PROC_H_ */