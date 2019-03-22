#include "archproto.h"
#include "global.h"
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
    struct proc_s *p;
    unsigned int biggest_ticks = 0;

    while (!biggest_ticks)
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

PUBLIC void init_clock()
{
    out_b(TIMER_MODE, RATE_GENERATOR);
    out_b(TIMER0, (u8_t)(TIMER_FREQ / HZ));
    out_b(TIMER0, (u8_t)((TIMER_FREQ / HZ) >> 8));

    put_irq_handler(CLOCK_IRQ, clock_handler);
    irq_i8259A_unmask(CLOCK_IRQ);
}