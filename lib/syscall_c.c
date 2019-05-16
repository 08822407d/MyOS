#include "type.h"
#include "global.h"
#include "klib.h"
#include "syscall.h"

/* ===============================系统调用函数================================ */
PUBLIC unsigned int sc_get_ticks()
{
    return ticks;
}

PUBLIC void sc_ajust_millisecond()
{
    unsigned int loop_count = 0xFFFF;
    unsigned int t_start = get_ticks();

    while (loop_count--) {}
    
    unsigned int t_end = get_ticks();

    whileloops_per_ms = (u32_t)(0xFFFF / (t_end - t_start)) /
                        (1000 / HZ);
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

PUBLIC void calc_whileloop_per_ms()
{
    unsigned int loop_count = 0xFFFF;
    unsigned int t_start = get_ticks();

    while (loop_count--) {}
    
    unsigned int t_end = get_ticks();

    whileloops_per_ms = (u32_t)(0xFFFF / (t_end - t_start)) /
                        (1000 / HZ);
}