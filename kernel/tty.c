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