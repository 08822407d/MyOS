#include "archconst.h"
#include "archproto.h"
#include "interrupt.h"
#include "proto.h"
#include "klib.h"
#include "syscall.h"

#include "keyboard.h"
#include "keymap.h"

void disp_printable_key(u32_t);

PRIVATE struct kbd_s kbd_in;

PRIVATE int code_with_E0 = 0;
PRIVATE int shift_l;
PRIVATE int shift_r;
PRIVATE int alt_l;
PRIVATE int alt_r;
PRIVATE int ctrl_l;
PRIVATE int ctrl_r;
PRIVATE int caps_lock;
PRIVATE int num_lock;
PRIVATE int scroll_lock;

PRIVATE int column;

void keyborad_handler()
{
    u8_t scan_code = in_b(KBD_IO);

    if (kbd_in.count < KBDUFF_SIZE)
    {
        *(kbd_in.p_head) = scan_code;
        kbd_in.p_head++;
        kbd_in.p_head = ((u32_t)(kbd_in.p_head) & (KBDUFF_SIZE-1)) +
                        kbd_in.buf;
        kbd_in.count++;
    }
}

u8_t read_kbuf()
{
    u8_t scan_code = 0;

    while (kbd_in.count <= 0)
    {}

    disable_intr();
    scan_code = *(kbd_in.p_tail);
    kbd_in.p_tail++;
    kbd_in.p_tail = ((u32_t)(kbd_in.p_tail) & (KBDUFF_SIZE - 1)) +
                    kbd_in.buf;
    kbd_in.count--;
    enable_intr();

    return scan_code;
}

PUBLIC void read_keyboard(TTY_t* tty_ptr)
{
    char output[2];
    u32_t make;
    u32_t key = 0;
    u32_t *keyrow;
    u8_t scan_code = 0;

    if (kbd_in.count > 0)
    {
        scan_code = read_kbuf();

        if (scan_code == 0xE1)
        {
            int i;
            u8_t pausebrk_scode[] = {0xE1, 0x1D, 0x45,
                                     0xE1, 0x9D, 0xC5};
            bool is_pausebrk = TRUE;
            for ( i = 1; i < 6; i++)
            {
                if (read_kbuf() != pausebrk_scode[i])
                {
                    is_pausebrk = FALSE;
                    break;
                }
            }
            if (is_pausebrk)
            {
                key = PAUSEBREAK;
            }
        }

        else if (scan_code == 0xE0)
        {
            scan_code = read_kbuf();
            /* printscreen按下 */
            if (scan_code = 0x2A)
            {
                if (read_kbuf() == 0xE0)
                {
                    if (read_kbuf() == 0x37)
                    {
                        key = PRINTSCREEN;
                        make = 1;
                    }
                }
            }
            scan_code = read_kbuf();
            /* printscreen弹起 */
            if (scan_code = 0xB7)
            {
                if (read_kbuf() == 0xE0)
                {
                    if (read_kbuf() == 0xAA)
                    {
                        key = PRINTSCREEN;
                        make = 0;
                    }
                }
            }
            /* printscreen以外的键 */
            if (key == 0)
            {
                key = code_with_E0;
            }
        }

        if ((key != PAUSEBREAK) && (key != PRINTSCREEN))
        {
            make = (scan_code & FLAG_BREAK ? FALSE : TRUE);
            keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];

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
            case SHIFT_L:
                shift_l = make;
                key = 0;
                break;
            case SHIFT_R:
                shift_r = make;
                key = 0;
                break;
            case CTRL_L:
                ctrl_l = make;
                key = 0;
                break;
            case CTRL_R:
                ctrl_r = make;
                key = 0;
                break;
            case ALT_L:
                alt_l = make;
                key = 0;
                break;
            case ALT_R:
                alt_r = make;
                key = 0;
                break;
            default:
                break;
            }

            if (make)
            {
                key |= shift_l  ?   FLAG_SHIFT_L : 0;
                key |= shift_r  ?   FLAG_SHIFT_R : 0;
                key |= ctrl_l   ?   FLAG_CTRL_L  : 0;
                key |= ctrl_r   ?   FLAG_CTRL_R  : 0;
                key |= alt_l    ?   FLAG_ALT_L   : 0;
                key |= alt_r    ?   FLAG_ALT_R   : 0;

                in_process(tty_ptr, key);
            }
        }
    }
}

void disp_printable_key(u32_t key)
{
    unsigned char output[2] = {0,0};
    if (!(key & FLAG_EXT))
    {
        output[0] = key & 0xFF;
        disp_str(output);
    }
}

PUBLIC void init_keyboard()
{
    kbd_in.p_head = kbd_in.p_tail = kbd_in.buf;
    kbd_in.count = 0;

    put_irq_handler(KEYBOARD_IRQ, keyborad_handler);
    irq_i8259A_unmask(KEYBOARD_IRQ);
}
