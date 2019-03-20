#include "type.h"
#include "global.h"
#include "klib.h"
#include "syscall.h"

/* ===============================系统调用函数================================ */
PUBLIC unsigned int sc_get_ticks()
{
    return ticks;
}

/* ===============================初始化系统调用================================ */
PUBLIC void put_syscall(u32_t SYSCALL, systcall_f systemcall)
{
    syscall_table[SYSCALL] = systemcall;
}

PUBLIC void init_syscall_table()
{
    put_syscall(SC_GET_TICKS, sc_get_ticks);
}

