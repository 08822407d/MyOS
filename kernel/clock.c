#include "global.h"
#include "archproto.h"
#include "klib.h"

void clock_handler()
{   
    ++ticks;
    
    p_num++;
    p_proc_ready = PCB + (p_num % NR_PROCS);
}