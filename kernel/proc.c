#include "const.h"
#include "global.h"
#include "const.h"
#include "proc.h"
#include "klib.h"
#include "archtypes.h"
#include "archproto.h"


struct proc_s  	PCB[NR_PROCS];
unsigned char 	proc_Stack[STACK_SIZE_TOTAL];

void process_init()
{
    struct  proc_s* p_proc			= PCB;

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

