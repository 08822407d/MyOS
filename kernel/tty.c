#include "global.h"
#include "klib.h"
#include "proc.h"
#include "type.h"
#include "tty.h"
#include "proto.h"
#include "archconst.h"
#include "archproto.h"
#include "keyboard.h"

#define TTY_FIRST   (tty_table)
#define TTY_END     (tty_table + NR_CONSOLES)

void init_tty();
void tty_do_read(TTY_t*);
void tty_do_write(TTY_t*);

PUBLIC void Task_tty()
{
    for (TTY_t* tty_ptr = TTY_FIRST; tty_ptr < TTY_END; tty_ptr++)
    {
        init_tty(tty_ptr);
    }
    
    int idx_thistty = 0;
    while (1)
    {
        TTY_t* this_tty = TTY_FIRST + (0x03 & idx_thistty);

        tty_do_read(this_tty);
        tty_do_write(this_tty);
        idx_thistty++;
    }
}

void init_tty(TTY_t* tty_ptr)
{
    tty_ptr->inbuff_count = 0;
    tty_ptr->inbuff_head_ptr =
    tty_ptr->inbuff_tail_ptr = tty_ptr->inbuff;

    init_screen(tty_ptr);
}

void tty_do_read(TTY_t* tty_ptr)
{
    if (is_current_console(tty_ptr->console_ptr))
    {
        read_keyboard(tty_ptr);
    }
}

void tty_do_write(TTY_t *tty_ptr)
{
    if (tty_ptr->inbuff_count > 0)
    {
        char ch = *(tty_ptr->inbuff_tail_ptr);
        tty_ptr->inbuff_tail_ptr++;
        tty_ptr->inbuff_tail_ptr == ((u32_t)tty_ptr->inbuff_tail_ptr & (TTY_IN_BUF_SIZE - 1)) +
                                    tty_ptr->inbuff;
        tty_ptr->inbuff_count--;

        out_char(tty_ptr->console_ptr, ch);
    }
}

PUBLIC void in_process(TTY_t *tty_ptr, u32_t key)
{
    char output[2] = {'\0', '\0'};

    if (!(key & FLAG_EXT))
    {
        if (tty_ptr->inbuff_count < TTY_IN_BUF_SIZE)
        {
            *(tty_ptr->inbuff_head_ptr) = key;
            tty_ptr->inbuff_head_ptr++;
            tty_ptr->inbuff_head_ptr == ((u32_t)tty_ptr->inbuff_head_ptr & (TTY_IN_BUF_SIZE - 1)) +
                                        tty_ptr->inbuff;
            tty_ptr->inbuff_count++;
        }
    }
    else
    {
        int raw_code = key & MASK_RAW;

        switch (raw_code)
        {
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