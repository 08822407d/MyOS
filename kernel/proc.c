#include "const.h"
#include "global.h"
#include "const.h"
#include "proc.h"
#include "klib.h"
#include "archtypes.h"
#include "archproto.h"

void init_task_pcb(PROC_t* p_proc, PROC_TABLE_t* p_table)
{	
	//设置stack_frame
	p_proc->p_reg.cs  = SELECTOR_CS_TASK;
	p_proc->p_reg.ds  =
	p_proc->p_reg.es  =
	p_proc->p_reg.fs  =
	p_proc->p_reg.ss  = SELECTOR_DS_TASK;
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

void init_user_pcb(PROC_t* p_proc, PROC_TABLE_t* p_table)
{	
	p_proc->ldt_sele = SELECTOR_LDT0;
	//设置gdt中ldt0和tss0两项
	init_tssdesc(INDEX_TSS0, &tss0);
	init_ldtdesc(INDEX_LDT0, &(p_proc->ldt[0]));

	//设置ldt
	init_segdesc(&(p_proc->ldt[1]), 0x0, SIZE_4GB, DA_C | USER_PROC<<DPL_SHIFT);
	init_segdesc(&(p_proc->ldt[2]), 0x0, SIZE_4GB, DA_DRW | USER_PROC<<DPL_SHIFT);

	//设置stack_frame
	p_proc->p_reg.cs  = SELECTOR_CS_LOCAL;
	p_proc->p_reg.ds  =
	p_proc->p_reg.es  =
	p_proc->p_reg.fs  =
	p_proc->p_reg.ss  = SELECTOR_DS_LOCAL;
	p_proc->p_reg.gs  = SELECTOR_VGARAM;
	p_proc->p_reg.eax =
	p_proc->p_reg.ebp =
	p_proc->p_reg.ecx =
	p_proc->p_reg.edx =
	p_proc->p_reg.esi =
	p_proc->p_reg.edi = 0;
	p_proc->p_reg.esp = p_stacktop;
	p_proc->p_reg.eip = p_table->initial_eip;
	p_proc->p_reg.psw = USER_PSW;

	p_stacktop -= p_table->stacksize;
}

void init_process()
{
    PROC_t* p_proc = PCB;
	int i = 0;
	for (int j =0 ; i < NR_TASK_PROCS; i++)
	{
		init_task_pcb(&p_proc[i],&task_proc_map[j]);
		p_proc->pid = i;
		j++;
	}
	for (int j =0 ; i < NR_TASK_PROCS + NR_USER_PROCS; i++)
	{
		init_user_pcb(&p_proc[i],&user_proc_map[j]);
		p_proc->pid = i;
		j++;
	}

	PCB[0].ticks = PCB[0].priority = 10;
	PCB[1].ticks = PCB[1].priority = 1;
	PCB[2].ticks = PCB[2].priority = 1;
	PCB[3].ticks = PCB[3].priority = 1;
	PCB[0].tty_idx = 0;
	PCB[1].tty_idx = 1;
	PCB[2].tty_idx = 2;
	PCB[3].tty_idx = 3;

	tss0.ss0 = SELECTOR_DS_KRNL;
	tss0.sp0 = PCB + sizeof(STACK_FRAME_t);
	tss0.iobase = sizeof(TSS_t);
// 汇编代码中，进程切换功能所使用的指针，指向下一个进程的进程表
	x86_ltr(SELECTOR_TSS0);
}



