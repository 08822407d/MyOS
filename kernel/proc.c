#include "const.h"
#include "global.h"
#include "const.h"
#include "proc.h"
#include "klib.h"
#include "archtypes.h"
#include "archproto.h"

void arch_proc_reset(PROC_t* p_proc, PROC_TABLE_t* p_table)
{	
	p_proc->ldt_sele = SELECTOR_LDT0;
	//设置gdt中ldt0和tss0两项
	init_tssdesc(INDEX_TSS0, &tss0);
	init_ldtdesc(INDEX_LDT0, &(p_proc->ldt[0]));

	//设置ldt
	init_segdesc(&(p_proc->ldt[1]), 0x0, SIZE_4GB, DA_C | p_table->proc_type<<DPL_SHIFT);
	init_segdesc(&(p_proc->ldt[2]), 0x0, SIZE_4GB, DA_DRW | p_table->proc_type<<DPL_SHIFT);

	u32_t selector_cs = SELECTOR_CS_LOCAL;
	u32_t selector_ds = SELECTOR_DS_LOCAL;
	u32_t psw	  	  = PROC_PSW;
	if (p_table->proc_type == TASK_PROC)
	{
		selector_cs = SELECTOR_CS_TASK;
		selector_ds = SELECTOR_DS_TASK;
		psw			= TASK_PSW;
	}
	//设置stack_frame
	p_proc->p_reg.cs  = selector_cs;
	p_proc->p_reg.ds  =
	p_proc->p_reg.es  =
	p_proc->p_reg.fs  =
	p_proc->p_reg.ss  = selector_ds;
	p_proc->p_reg.gs  = SELECTOR_VGARAM;
	p_proc->p_reg.eax =
	p_proc->p_reg.ebp =
	p_proc->p_reg.ecx =
	p_proc->p_reg.edx =
	p_proc->p_reg.esi =
	p_proc->p_reg.edi = 0;
	p_proc->p_reg.esp = p_stacktop;
	p_proc->p_reg.eip = p_table->initial_eip;
	p_proc->p_reg.psw = TASK_PSW;

	p_stacktop -= p_table->stacksize;
}

void init_process()
{
    PROC_t* p_proc = PCB;

	for (int i = 0; i < NR_TASKS + NR_PROCS; i++)
	{
		arch_proc_reset(&p_proc[i],&user_proc_map[i]);

		p_proc->pid = i;
	}

	PCB[0].ticks = PCB[0].priority = 10;
	PCB[1].ticks = PCB[1].priority = 1;
	PCB[2].ticks = PCB[2].priority = 1;
	PCB[3].ticks = PCB[3].priority = 1;

	tss0.ss0 = SELECTOR_DS_KRNL;
	tss0.sp0 = PCB + sizeof(STACK_FRAME_t);
	tss0.iobase = sizeof(TSS_t);
// 汇编代码中，进程切换功能所使用的指针，指向下一个进程的进程表
	x86_ltr(SELECTOR_TSS0);
}



