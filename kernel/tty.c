#include "global.h"
#include "klib.h"
#include "proc.h"
#include "type.h"
#include "tty.h"
#include "proto.h"
#include "archconst.h"
#include "archproto.h"
#include "keyboard.h"
#include "syscall.h"

#define TTY_FIRST   (tty_table)
#define TTY_END     (tty_table + NR_CONSOLES)

void init_tty();
void tty_do_read(TTY_t*);
void tty_do_write(TTY_t*);

/*======================================================================*
				        Task_tty
 *======================================================================*/
PUBLIC void Task_tty()
{
    TTY_t* tty_ptr;

    for (tty_ptr = TTY_FIRST; tty_ptr < TTY_END; tty_ptr++)
    {
        init_tty(tty_ptr);
    }
    
    select_console(0);
    while (1)
    {
        for (tty_ptr = TTY_FIRST; tty_ptr < TTY_END; tty_ptr++)
        {
            tty_do_read(tty_ptr);
            tty_do_write(tty_ptr);
        };
    }
}

/*======================================================================*
				        init_tty
 *======================================================================*/
void init_tty(TTY_t* tty_ptr)
{
    tty_ptr->inbuff_count = 0;
    tty_ptr->inbuff_head_ptr =
    tty_ptr->inbuff_tail_ptr = tty_ptr->inbuff;

    init_screen(tty_ptr);
}

/*======================================================================*
				        tty_do_read
 *======================================================================*/
void tty_do_read(TTY_t* tty_ptr)
{
    if (is_current_console(tty_ptr->console_ptr))
    {
        read_keyboard(tty_ptr);
    }
}

/*======================================================================*
				        tty_do_write
 *======================================================================*/
void tty_do_write(TTY_t *tty_ptr)
{
    if (tty_ptr->inbuff_count)
    {
        char ch = *(tty_ptr->inbuff_tail_ptr);
        tty_ptr->inbuff_tail_ptr++;
        if (tty_ptr->inbuff_tail_ptr == tty_ptr->inbuff + TTY_IN_BUF_SIZE)
        {
            tty_ptr->inbuff_tail_ptr = tty_ptr->inbuff;
        }
        tty_ptr->inbuff_count--;

        out_char(tty_ptr->console_ptr, ch);
    }
}

/*======================================================================*
				        put_key
 *======================================================================*/
void put_key(TTY_t* tty_ptr, u32_t key)
{
    if (tty_ptr->inbuff_count < TTY_IN_BUF_SIZE)
    {
        *(tty_ptr->inbuff_head_ptr) = key;
        tty_ptr->inbuff_head_ptr++;
        if (tty_ptr->inbuff_head_ptr == tty_ptr->inbuff + TTY_IN_BUF_SIZE)
        {
            tty_ptr->inbuff_head_ptr = tty_ptr->inbuff;
        }
        tty_ptr->inbuff_count++;
    }
}

/*======================================================================*
				        in_process
 *======================================================================*/
PUBLIC void in_process(TTY_t *tty_ptr, u32_t key)
{
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT))
    {
        put_key(tty_ptr, key);
    }
    else
    {
        int raw_code = key & MASK_RAW;
        switch (raw_code)
        {
        case ENTER:
            put_key(tty_ptr, '\n');
            break;
        case BACKSPACE:
            put_key(tty_ptr, '\b');
            break;
        case UP:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                scroll_screen(tty_ptr->console_ptr, SCROLL_DOWN);
            }
            break;
        case DOWN:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                scroll_screen(tty_ptr->console_ptr, SCROLL_UP);
            }
            break;
        case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
		case F11:
		case F12:
			/* Alt + F1~F12 */
			if ((key & FLAG_ALT_L) || (key & FLAG_ALT_R))
			{
				select_console(raw_code - F1);
			}
			break;
        default:
            break;
        }
    }
}

PUBLIC void tty_write(TTY_t* tty_ptr, char* buf, int length)
{
    char* p =buf;
    int  i = length;

    while (i)
    {
        out_char(tty_ptr->console_ptr, *p++);
        i--;
    }
}

/*======================================================================*
                              sys_printx
*======================================================================*/
PUBLIC int sys_printx(int _unused1, int _unused2, char* s, PROC_t* p_proc)
{
	const char * p;
	char ch;

	char reenter_err[] = "? k_reenter is incorrect for unknown reason";
	reenter_err[0] = MAG_CH_PANIC;

	/**
	 * @note Code in both Ring 0 and Ring 1~3 may invoke printx().
	 * If this happens in Ring 0, no linear-physical address mapping
	 * is needed.
	 *
	 * @attention The value of `k_reenter' is tricky here. When
	 *   -# printx() is called in Ring 0
	 *      - k_reenter > 0. When code in Ring 0 calls printx(),
	 *        an `interrupt re-enter' will occur (printx() generates
	 *        a software interrupt). Thus `k_reenter' will be increased
	 *        by `kernel.asm::save' and be greater than 0.
	 *   -# printx() is called in Ring 1~3
	 *      - k_reenter == 0.
	 */
	if (k_reenter == 0)  /* printx() called in Ring<1~3> */
		p = va2la(proc2pid(p_proc), s);
	else if (k_reenter > 0) /* printx() called in Ring<0> */
		p = s;
	else	/* this should NOT happen */
		p = reenter_err;

	/**
	 * @note if assertion fails in any TASK, the system will be halted;
	 * if it fails in a USER PROC, it'll return like any normal syscall
	 * does.
	 */
	if ((*p == MAG_CH_PANIC) ||
	    (*p == MAG_CH_ASSERT && p_proc_ready < &PCB[NR_TASK_PROCS])) {
		disable_intr();
		char * v = (char*)VGARAM_BASE;
		const char * q = p + 1; /* +1: skip the magic char */

		while (v < (char*)(VGARAM_BASE + VGARAM_SIZE)) {
			*v++ = *q++;
			*v++ = RED;
			if (!*q) {
				while (((int)v - VGARAM_BASE) % (SCREEN_WIDTH * 16)) {
					/* *v++ = ' '; */
					v++;
					*v++ = GRAY;
				}
				q = p + 1;
			}
		}

		__asm__ __volatile__("hlt");
	}

	while ((ch = *p++) != 0) {
		if (ch == MAG_CH_PANIC || ch == MAG_CH_ASSERT)
			continue; /* skip the magic char */

		out_char(tty_table[p_proc->tty_idx].console_ptr, ch);
	}

	return 0;
}