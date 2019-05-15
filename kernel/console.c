#include "type.h"
#include "proto.h"
#include "global.h"
#include "archconst.h"
#include "archproto.h"
#include "tty.h"

/*======================================================================*
                           	flush
*======================================================================*/
void flush(CONSOLE_t* console_ptr)
{
    set_cursor(console_ptr->this_cursor);
    set_video_start_pos(console_ptr->this_video_startpos);
}

/*======================================================================*
			   				out_char
 *======================================================================*/
PUBLIC void out_char(CONSOLE_t* console_ptr, char ch)
{
    u8_t* video_ptr = (u8_t*)(VGARAM_BASE + console_ptr->this_cursor * 2);

	switch(ch)
	{
	case '\n':
		if (console_ptr->this_cursor < console_ptr->this_console_startpos +
		    	console_ptr->this_memsize - SCREEN_WIDTH)
		{
			console_ptr->this_cursor = console_ptr->this_console_startpos + SCREEN_WIDTH * 
				((console_ptr->this_cursor - console_ptr->this_console_startpos) /
				 SCREEN_WIDTH + 1);
		}
		break;
	case '\b':
		if (console_ptr->this_cursor > console_ptr->this_console_startpos)
		{
			console_ptr->this_cursor--;
			*(video_ptr-2) = ' ';
			*(video_ptr-1) = DEFAULT_TEXT_COLOR;
		}
		break;
	default:
		if (console_ptr->this_cursor <
		    	console_ptr->this_console_startpos + console_ptr->this_memsize - 1)
		{
			*video_ptr++ = ch;
			*video_ptr++ = DEFAULT_TEXT_COLOR;
			console_ptr->this_cursor++;
		}
		break;
	}

	while (console_ptr->this_cursor >= console_ptr->this_video_startpos + SCREEN_SIZE) {
		scroll_screen(console_ptr, SCROLL_DOWN);
	}

	flush(console_ptr);
}

/*======================================================================*
			  		set_video_start_addr
 *======================================================================*/
void set_video_start_pos(u32_t start_pos)
{
	disable_intr();
	out_b(CRTC_ADDR_REG, START_ADDR_H);
	out_b(CRTC_DATA_REG, (start_pos >> 8) & 0xFF);
	out_b(CRTC_ADDR_REG, START_ADDR_L);
	out_b(CRTC_DATA_REG, start_pos & 0xFF);
	enable_intr();
}

/*======================================================================*
			    		set_cursor
 *======================================================================*/
PUBLIC void set_cursor(u32_t cursor_pos)
{
    disable_intr();
    out_b(CRTC_ADDR_REG, CURSOR_H);
    out_b(CRTC_DATA_REG, (cursor_pos >> 8) & 0xFF);
    out_b(CRTC_ADDR_REG, CURSOR_L);
    out_b(CRTC_DATA_REG, cursor_pos & 0xFF);
    enable_intr();
}

/*======================================================================*
			   		is_current_console
*======================================================================*/
PUBLIC bool is_current_console(CONSOLE_t* console_ptr)
{
    return (console_ptr == &console_table[current_console_idx]);
}

/*======================================================================*
			   			init_screen
 *======================================================================*/
PUBLIC void init_screen(TTY_t* tty_ptr)
{
	int idx_thistty = tty_ptr - tty_table;
    tty_ptr->console_ptr = console_table + idx_thistty;

	int vgaram_size = VGARAM_SIZE >> 1;	//以word计算显存大小

	int con_v_mem_size                   		= vgaram_size / NR_CONSOLES;
	tty_ptr->console_ptr->this_console_startpos	= idx_thistty * con_v_mem_size;
	tty_ptr->console_ptr->this_memsize			= con_v_mem_size;
	tty_ptr->console_ptr->this_video_startpos	= tty_ptr->console_ptr->this_console_startpos;

	/* 初始化光标位置在最开始处 */
	tty_ptr->console_ptr->this_cursor			= tty_ptr->console_ptr->this_console_startpos;

	if (idx_thistty == 0)
	{
		/* 第一个控制台沿用原来的光标位置 */
		tty_ptr->console_ptr->this_cursor = disp_pos / 2;
		disp_pos = 0;
	}
	else
	{
		out_char(tty_ptr->console_ptr, idx_thistty + '1');
		out_char(tty_ptr->console_ptr, '$');
	}

	set_cursor(tty_ptr->console_ptr->this_cursor);
}

/*======================================================================*
			   			select_console
 *======================================================================*/
PUBLIC void select_console(int idx_console)	/* 0 ~ (NR_CONSOLES - 1) */
{
	if ((idx_console < 0) || (idx_console >= NR_CONSOLES)) {
		return;
	}

	current_console_idx = idx_console;

	set_cursor(console_table[idx_console].this_cursor);
	set_video_start_pos(console_table[idx_console].this_console_startpos);
}

/*======================================================================*
			   			scroll_screen
 *----------------------------------------------------------------------*
 滚屏.
 *----------------------------------------------------------------------*
 direction:
	SCR_UP	: 向上滚屏
	SCR_DN	: 向下滚屏
	其它	: 不做处理
 *======================================================================*/
PUBLIC void scroll_screen(CONSOLE_t* console_ptr, int direction)
{
	if (direction == SCROLL_UP) {
		if (console_ptr->this_video_startpos > console_ptr->this_console_startpos) {
			console_ptr->this_video_startpos -= SCREEN_WIDTH;
		}
	}
	else if (direction == SCROLL_DOWN) {
		if (console_ptr->this_video_startpos + SCREEN_SIZE <
		    console_ptr->this_console_startpos + console_ptr->this_memsize) {
			console_ptr->this_video_startpos += SCREEN_WIDTH;
		}
	}
	else{
	}

	set_video_start_pos(console_ptr->this_video_startpos);
	set_cursor(console_ptr->this_cursor);
}