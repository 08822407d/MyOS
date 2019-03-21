#include "interrupt.h"
#include "archproto.h"
#include "archconst.h"
#include "syscall.h"
#include "klib.h"

#include "keyboard.h"
#include "keymap.h"

PRIVATE struct kbd_s    kbd_in;

PRIVATE int     code_with_E0 = 0;
PRIVATE int     shift_l;
PRIVATE int     shift_r;
PRIVATE int     alt_l;
PRIVATE int     alt_r;
PRIVATE int     ctrl_l;
PRIVATE int     ctrl_r;
PRIVATE int     caps_lock;
PRIVATE int     num_lock;
PRIVATE int     scroll_lock;

PRIVATE int     column;

void keyborad_handler()
{
    u8_t scan_code = in_b(KBD_IO);

    if (kbd_in.count < KBDUFF_SIZE)
    {
        *(kbd_in.p_head) = scan_code;
        kbd_in.p_head++;
        kbd_in.p_head = ((u32_t)(kbd_in.p_head) & KBDUFF_SIZE) +
                        kbd_in.buf;
        kbd_in.count++;
    }
}

PUBLIC void read_kbdbuff()
{
    u8_t    scan_code;
    char    output[2];
    u32_t   make;
    u32_t   key = 0;
    u32_t  *keyrow;

    if (kbd_in.count > 0)
    {
//        disable_intr();

        scan_code = *(kbd_in.p_tail);
        kbd_in.p_tail++;
        kbd_in.p_tail = ((u32_t)(kbd_in.p_tail) & KBDUFF_SIZE) +
                        kbd_in.buf;
        kbd_in.count--;

        if (scan_code == 0xE1)
        {

        }
        else if (scan_code == 0xE0)
        {

        }
        else
        {
            make = (scan_code & FLAG_BREAK ? FALSE : TRUE);

            keyrow = &keymap[(scan_code&0x7F)*MAP_COLS];

            column = 0;
            if (shift_l || shift_r)
            {
                column = 1;
            }
            if (code_with_E0)
            {
                column = 2;
                code_with_E0 = 0;
            }
            key = keyrow[column];

            switch (key)
            {
                case SHIFT_L :
                    shift_l = make;
                    key = 0;
                    break;
                case SHIFT_R :
                    shift_r = make;
                    key = 0;
                    break;
                case CTRL_L :
                    ctrl_l = make;
                    key = 0;
                    break;
                case CTRL_R :
                    ctrl_r = make;
                    key = 0;
                    break;
                case ALT_L :
                    alt_l = make;
                    key = 0;
                    break;
                case ALT_R :
                    alt_r = make;
                    key = 0;
                    break;
                default :
                    if (!make)
                    {
                        key = 0;
                    }
                    break;
            }

            if (key)
            {
                output[0] = key;
                disp_str(output);
            }
        }
    
//        enable_intr();
    }
}

PUBLIC void init_keyboard()
{
    kbd_in.p_head = kbd_in.p_tail = kbd_in.buf;
    kbd_in.count = 0;

    put_irq_handler(KEYBOARD_IRQ, keyborad_handler);	
	irq_i8259A_unmask(KEYBOARD_IRQ);
}
