#include "global.h"
#include "archproto.h"
#include "klib.h"

void clock_handler()
{   
    ++ticks;

    p_proc_ready = PCB + ((ticks-1) % NR_PROCS);
}