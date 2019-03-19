#include "global.h"
#include "archproto.h"
#include "klib.h"

void clock_handler()
{   
    disp_str("#");
    
    p_num++;
    p_proc_ready = PCB + (p_num % NR_PROCS);
}