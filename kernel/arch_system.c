#include "type.h"
#include "proc.h"
#include "global.h"
#include "archconst.h"

void arch_proc_reset(PROC_t* p_table, PROC_TABLE_t* p_map)
{
	u16_t segtype_cs,segtype_ds;
	u16_t ldttype;

	unsigned char*  p_proc_stack 	= proc_Stack + STACK_SIZE_TOTAL;
	p_proc_stack -= p_map.stacksize;

	if (p_map->proc_type == TASK_PROC)
	{
		segtype_cs = SELECTOR_CS_TASK;
		segtype_ds = SELECTOR_DS_TASK;
		ldttype	   = SELECTOR_LDT_TASK;
	}
	else
	{
		segtype_cs = SELECTOR_CS_USER;
		segtype_ds = SELECTOR_DS_USER;
		ldttype	   = SELECTOR_LDT_USER;
	}

    p_table->p_reg.psw 	= INIT_PSW;

	p_table->p_reg.cs 	= segtype_cs;
	p_table->p_reg.gs 	= 
	p_table->p_reg.fs 	= 
	p_table->p_reg.ss 	= 
	p_table->p_reg.es 	= 
	p_table->p_reg.ds 	= segtype_ds;
	p_proc->p_reg.eip	= (u32_t)proc_map.initial_eip;
	p_proc->p_reg.esp	= (u32_t)p_proc_stack;

	p_table->ldt_sele	= ldttype;
}