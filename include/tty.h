#ifndef _MYOS_TTY_H_
#define _MYOS_TTY_H_

	#define TTY_IN_BUF_SIZE 256
	#define SCREEN_WIDTH    80
    #define SCREEN_SIZE     (SCREEN_WIDTH * 25)
	#define	SCROLL_UP		1
	#define	SCROLL_DOWN		-1

	#define DEFAULT_TEXT_COLOR WHITE

	#include "type.h"

	struct console_s
	{
		//vga属性：当前显示起始地址（在vgaram中位置）
		unsigned int	this_video_startpos;
		//console参数：当前console在vgaram中的位置
		unsigned int	this_console_startpos;
		//console参数：当前console被分配的内存大小
		unsigned int	this_memsize;
		//vga属性：光标位置（在vgaram中的位置）
		unsigned int	this_cursor;
	};
	typedef struct console_s CONSOLE_t;

	struct tty_s
	{
		u32_t		inbuff[TTY_IN_BUF_SIZE];
		u32_t*		inbuff_head_ptr;
		u32_t*		inbuff_tail_ptr;
		int			inbuff_count;
		CONSOLE_t*	console_ptr;
	};
	typedef struct tty_s TTY_t;

#endif /* _MYOS_TTY_H_ */
