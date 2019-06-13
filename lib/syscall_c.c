#include "type.h"
#include "global.h"
#include "proto.h"
#include "klib.h"
#include "syscall.h"
#include "tty.h"

/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	MESSAGE_t msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}

/*****************************************************************************
 *                                校准时间
 *****************************************************************************/
PUBLIC void calc_whileloop_per_ms()
{
    unsigned int loop_count = 0xFFFF;
    unsigned int t_start = get_ticks();

    while (loop_count--) {}
    
    unsigned int t_end = get_ticks();

    whileloops_per_ms = (u32_t)(0xFFFF / (t_end - t_start)) /
                        (1000 / HZ);
}