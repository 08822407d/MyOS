#define _GLOBAL_VAR_

#include "global.h"
#include "proc.h"
#include "syscall.h"
#include "tty.h"

PUBLIC u32_t    k_Stacktop = k_Stack + K_STACK_SIZE;

PUBLIC u32_t    whileloops_per_ms = 0;

PUBLIC int      k_reenter = 0;
PUBLIC u32_t    p_count = 0;
PUBLIC u8_t *   p_stacktop = proc_Stack + STACK_SIZE_TOTAL;

PUBLIC irq_handler_f    irq_table[NR_IRQS];
PUBLIC systcall_f       syscall_table[NR_SYSCALL];

PUBLIC PROC_t*          p_proc_ready = PCB;
PUBLIC PROC_TABLE_t     task_proc_map[NR_TASK_PROCS] =
    {
        {Task_tty, STACK_SIZE_TASKTTY, ""}
    };
PUBLIC PROC_TABLE_t     user_proc_map[NR_USER_PROCS] =
    {
        {TestA, STACK_SIZE_TESTA, ""},
        {TestB, STACK_SIZE_TESTB, ""},
        {TestC, STACK_SIZE_TESTC, ""}
    };

PUBLIC TTY_t        tty_table[NR_CONSOLES];
PUBLIC CONSOLE_t    console_table[NR_CONSOLES];
PUBLIC int          current_console_idx = 0;
/* =============================系统变量============================== */

PUBLIC unsigned int ticks = 0;

/* =============================一次性变量=============================== */
PUBLIC unsigned int flag_for_millisecond_ajust = 2;

/* =============================一次性变量=============================== */