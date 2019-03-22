#include "const.h"
#include "global.h"
#include "const.h"
#include "proc.h"
#include "klib.h"
#include "archtypes.h"
#include "archproto.h"

void arch_proc_reset(struct proc_s *p_proc, struct proctable_s *p_table)
{	
	p_proc->ldt_sele = SELECTOR_LDT0;
	//设置gdt中ldt0和tss0两项
	init_tssdesc(INDEX_TSS0, &tss0);
	init_ldtdesc(INDEX_LDT0, &(p_proc->ldt[0]));

	//设置ldt
	init_segdesc(&(p_proc->ldt[1]), 0x0, SIZE_4GB, DA_C | p_table->proc_type<<DPL_SHIFT);
	init_segdesc(&(p_proc->ldt[2]), 0x0, SIZE_4GB, DA_DRW | p_table->proc_type<<DPL_SHIFT);

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
	p_proc->p_reg.psw = INIT_PSW;

	p_stacktop -= p_table->stacksize;
}

void process_init()
{
    struct proc_s* p_proc = PCB;

	for (int i = 0; i < NR_PROCS; i++)
	{
		arch_proc_reset(&p_proc[i],&proc_map[i]);

		p_proc->pid = i;
	}

	PCB[0].ticks = PCB[0].priority = 1;
	PCB[1].ticks = PCB[1].priority = 2;
	PCB[2].ticks = PCB[2].priority = 3;
	PCB[3].ticks = PCB[3].priority = 10;

	tss0.ss0 = SELECTOR_DS_KRNL;
	tss0.sp0 = PCB + sizeof(struct stackframe_s);
	tss0.iobase = sizeof(struct tss_s);
// 汇编代码中，进程切换功能所使用的指针，指向下一个进程的进程表
	x86_ltr(SELECTOR_TSS0);
}



