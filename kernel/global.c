#define _GLOBAL_VAR_

#include "global.h"
#include "proc.h"

PUBLIC  u32_t    k_Stacktop = k_Stack + K_STACK_SIZE;

PUBLIC  int      k_reenter = 0;
PUBLIC  u32_t    p_num = 0;
PUBLIC  u8_t*    p_stacktop = proc_Stack + STACK_SIZE_TOTAL;

PUBLIC	irq_handler			irq_table[NR_IRQS];

PUBLIC  struct proc_s* 		p_proc_ready = PCB;
PUBLIC  struct  proctable_s proc_map[NR_PROCS] = 
    {
        {TestA, STACK_SIZE_TESTA, USER_PROC},
		{TestB, STACK_SIZE_TESTB, USER_PROC},
		{TestC, STACK_SIZE_TESTC, USER_PROC}
    };
