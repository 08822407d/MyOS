#define _GLOBAL_VAR_

#include "global.h"
#include "proc.h"

//PUBLIC  struct  proc_s      PCB[NR_PROCS];          // 进程表结构数组
//PUBLIC  struct  proctable_s proc_map[NR_PROCS];     // 进程体的信息结构数组，
                                                    // 用于存储进程切换必须的信息

//PUBLIC  char    task_stack[STACK_SIZE_TOTAL];       // 进程使用的栈
/*
PUBLIC  struct  proctable_s proc_map[NR_PROCS] = 
    {
        {TestA, STACK_SIZE_TESTA, USER_PROC},
		{TestB, STACK_SIZE_TESTB, USER_PROC},
		{TestC, STACK_SIZE_TESTC, USER_PROC}
    };
*/