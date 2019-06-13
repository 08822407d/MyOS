#include "type.h"
#include "const.h"
#include "interrupt.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "tty.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"


/*****************************************************************************
 *                                task_sys
 *****************************************************************************/
/**
 * <Ring 1> The main loop of TASK SYS.
 * 
 *****************************************************************************/
PUBLIC void Task_sys()
{
	MESSAGE_t msg;
	while (1) {
		send_recv(RECEIVE, ANY, &msg);
		int src = msg.source;

		switch (msg.type) {
		case GET_TICKS:
			msg.RETVAL = ticks;
			send_recv(SEND, src, &msg);
			break;
		default:
			panic("unknown msg type");
			break;
		}
	}
}