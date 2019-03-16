#include "const.h"
#include "global.h"
#include "const.h"
#include "proc.h"
#include "klib.h"
#include "archtypes.h"
#include "archproto.h"

int k_reenter = 0;

void process_init()
{
	struct proc_s *p_proc = PCB;
	
	disp_int(p_proc);
	disp_str("\n");
	
	p_proc->ldt_sele = SELECTOR_LDT0;
	//设置gdt中ldt0和tss0两项
	init_tssdesc(INDEX_TSS0, &tss0);
	init_ldtdesc(INDEX_LDT0, &(p_proc->ldt[0]));

	//设置ldt
	init_segdesc(&(p_proc->ldt[1]), 0x0, SIZE_4GB, DA_C);
	init_segdesc(&(p_proc->ldt[2]), 0x0, SIZE_4GB, DA_DRW);

	//设置stack_frame
	p_proc->p_reg.cs = SELECTOR_CS_LOCAL;
	p_proc->p_reg.ds =
	p_proc->p_reg.es =
	p_proc->p_reg.fs =
	p_proc->p_reg.ss = SELECTOR_DS_LOCAL;
	p_proc->p_reg.gs = SELECTOR_VGARAM;
	p_proc->p_reg.eax =
	p_proc->p_reg.ebp =
	p_proc->p_reg.ecx =
	p_proc->p_reg.edx =
	p_proc->p_reg.esi =
	p_proc->p_reg.edi = 0;
	p_proc->p_reg.esp = proc_Stack + STACK_SIZE_TOTAL;
	p_proc->p_reg.eip = (u32_t)TestA;
	p_proc->p_reg.psw = INIT_PSW;

	tss0.ss0 = SELECTOR_DS_KRNL;
	tss0.sp0 = &PCB[0] + sizeof(struct stackframe_s);
	tss0.iobase = sizeof(struct tss_s);
	x86_ltr(SELECTOR_TSS0);

	restart();
}

/*
void process_init()
{
    struct  proc_s* p_proc	= PCB;

	for (int i = 0; i < NR_PROCS; i++)
	{
		arch_proc_reset(&p_proc[i],&proc_map[i]);

		p_proc->pid = i;
	}

	int k_reenter = -1;

// 汇编代码中，进程切换功能所使用的指针，指向下一个进程的进程表
	struct proc_s* p_proc_ready = PCB; 
	  
    restart();

	while(1){}
}
*/

struct proctable_s proc_map[] = 
{
	{TestA, STACK_SIZE_TESTA, USER_PROC},
	{TestB, STACK_SIZE_TESTB, USER_PROC},
	{TestC, STACK_SIZE_TESTC, USER_PROC}
};

// 测试用的进程体
void TestA()
{
	int i = 0x0;
	while(1){
		disp_str("A");
		disp_int(i++);
		disp_str(".");
		delay(1);
	}
}

void TestB()
{
	int i = 0x1000;
	while(1){
		disp_str("B");
		disp_int(i++);
		disp_str(".");
		delay(1);
	}
}

void TestC()
{
	int i = 0x2000;
	while(1){
		disp_str("C");
		disp_int(i++);
		disp_str(".");
		delay(1);
	}
}

