#include "interrupt.h"
#include "archproto.h"
#include "archconst.h"
#include "syscall.h"
#include "klib.h"

#include "keyboard.h"
#include "keymap.h"

PRIVATE struct kbd_s    kbd_in;

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

    if (kbd_in.count > 0)
    {
//        disable_intr();

        scan_code = *(kbd_in.p_tail);
        kbd_in.p_tail++;
        kbd_in.p_tail = ((u32_t)(kbd_in.p_tail) & KBDUFF_SIZE) +
                        kbd_in.buf;
        kbd_in.count--;

//        enable_intr();

        if (scan_code == 0xE1)
        {

        }
        else if (scan_code == 0xE0)
        {

        }
        else
        {
            make = (scan_code & FLAG_BREAK ? FALSE : TRUE);
            if (make)
            {
                output[0] = keymap[(scan_code&0x7F)*MAP_COLS];
                disp_str(output);
            }
        }
        
    }
}

PUBLIC void init_keyboard()
{
    kbd_in.p_head = kbd_in.p_tail = kbd_in.buf;
    kbd_in.count = 0;

    put_irq_handler(KEYBOARD_IRQ, keyborad_handler);	
	irq_i8259A_unmask(KEYBOARD_IRQ);
}
