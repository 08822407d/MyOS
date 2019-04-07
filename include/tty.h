#ifndef _MYOS_TTY_H_
#define _MYOS_TTY_H_

	#define TTY_IN_BUF_SIZE 256

	#include "type.h"

	struct tty_s
	{
		u32_t		inbuff[TTY_IN_BUF_SIZE];
		u32_t*		inbuff_head_ptr;
		u32_t*		inbuff_tail_ptr;
		int			inbuff_count;
		CONSOLE_t*	console_ptr;
	};
	typedef struct tty_s TTY_t;

	struct console_s
	{
		unsigned int	current_display_pos;
		unsigned int	startpos_in_vgaram;
		unsigned int	current_console_memsize;
		unsigned int	cursor_pos;
	};
	typedef struct console_s CONSOLE_t;

#endif /* _MYOS_TTY_H_ */
