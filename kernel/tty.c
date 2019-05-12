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
        disp_str("-");
        disp_int(key);
        disp_str("-");
        disp_int(raw_code);
        disp_str("-");
        switch (raw_code)
        {
        case UP:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                disable_intr();
                out_b(CRTC_ADDR_REG, START_ADDR_H);
                out_b(CRTC_DATA_REG, ((80 * 15) >> 8) & 0xFF);
                out_b(CRTC_ADDR_REG, START_ADDR_L);
                out_b(CRTC_DATA_REG, (80 * 15) & 0xFF);
                enable_intr();
            }
            break;
        case DOWN:
            if ((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R))
            {
                /* Shift+Down, do nothing */
            }
            break;
        default:
            break;
        }
    }
}