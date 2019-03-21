#include "global.h"
#include "archproto.h"
#include "klib.h"

void schedule(void);

void clock_handler()
{   
    ++ticks;
    p_proc_ready->ticks--;

    if (k_reenter != 0)
    {
        return;
    }

    if (p_proc_ready->ticks != 0)
    {
        return;
    }

    schedule();
}

void schedule()
{
    struct proc_s* p;
    unsigned int biggest_ticks = 0;

    while(!biggest_ticks)
    {
        for (p = PCB; p < PCB + NR_PROCS; p++)
        {
            if (p->ticks > biggest_ticks)
            {
                biggest_ticks = p->ticks;
                p_proc_ready = p;
            }   
        }

        if (!biggest_ticks)
        {
            for (p = PCB; p < PCB + NR_PROCS; p++)
            {
                p->ticks = p->priority;
            }
            
        }
    }
}