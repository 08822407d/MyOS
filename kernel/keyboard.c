#include "archconst.h"
#include "archproto.h"
#include "interrupt.h"
#include "proto.h"
#include "klib.h"
#include "syscall.h"

#include "keyboard.h"
#include "keymap.h"

PRIVATE struct kbd_s kbd_in;

PRIVATE int code_with_E0 = 0;
PRIVATE int shift_l = 0;
PRIVATE int shift_r = 0;
PRIVATE int alt_l = 0;
PRIVATE int alt_r = 0;
PRIVATE int ctrl_l = 0;
PRIVATE int ctrl_r = 0;
PRIVATE int caps_lock = 0;
PRIVATE int num_lock = 0;
PRIVATE int scroll_lock = 0;

PRIVATE int column = 0;

/*======================================================================*
                        keyboard_handler
 *======================================================================*/
void keyborad_handler()
{
    u8_t scan_code = in_b(KBD_IO);

    if (kbd_in.count < KBDUFF_SIZE)
    {
        *(kbd_in.p_head) = scan_code;
        kbd_in.p_head++;
        if (kbd_in.p_head == kbd_in.buf + KBDUFF_SIZE)
            kbd_in.p_head = kbd_in.buf;
        kbd_in.count++;
    }
}


/*======================================================================*
				        kb_wait
 *======================================================================*/
void kbd_wait()	/* 等待 8042 的输入缓冲区空 */
{
	u8_t kbd_stat;

	do
    {
		kbd_stat = in_b(KBD_CTRL);
	}
    while (kbd_stat & 0x02);
}


/*======================================================================*
				        kb_ack
 *======================================================================*/
void kbd_ack()
{
	u8_t kbd_read;

	do
    {
		kbd_read = in_b(KBD_IO);
	}
    while (kbd_read =! KBD_ACK);
}

/*======================================================================*
				        set_leds
 *======================================================================*/
void set_leds()
{
	u8_t leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock;

	kbd_wait();
	out_b(KBD_IO, KBD_LED);
	kbd_ack();

	kbd_wait();
	out_b(KBD_IO, leds);
	kbd_ack();
}

/*======================================================================*
			            read_kbuf
 *======================================================================*/
u8_t read_kbuf()
{
    u8_t scan_code = 0;

    while (kbd_in.count <= 0)
    {}

    disable_intr();
    scan_code = *(kbd_in.p_tail);
    kbd_in.p_tail++;
    if (kbd_in.p_tail == kbd_in.buf + KBDUFF_SIZE)
        kbd_in.p_tail = kbd_in.buf;
    kbd_in.count--;
    enable_intr();
    
    return scan_code;
}

/*======================================================================*
                           read_keyboard
*======================================================================*/
PUBLIC void read_keyboard(TTY_t* tty_ptr)
{
    char output[2];
    u32_t make;
    u32_t key = 0;
    u32_t *keyrow;
    u8_t scan_code = 0;

    if (kbd_in.count > 0)
    {
        code_with_E0 = 0;

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
            if (scan_code == 0x2A)
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
            /* printscreen弹起 */
            if (scan_code == 0xB7)
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
                code_with_E0 = TRUE;
            }
        }

        if ((key != PAUSEBREAK) && (key != PRINTSCREEN))
        {
            make = (scan_code & FLAG_BREAK ? FALSE : TRUE);

            keyrow = &(keymap[(scan_code & 0x7F) * MAP_COLS]);

            column = 0;
			int caps = shift_l || shift_r;
			if (caps_lock)
            {
				if ((keyrow[0] >= 'a') && (keyrow[0] <= 'z'))
                {
					caps = !caps;
				}
			}
			if (caps)
            {
				column = 1;
			}

			if (code_with_E0)
            {
				column = 2;
			}
			key = keyrow[column];

            switch (key)
            {
            case SHIFT_L:
                shift_l = make;
                break;
            case SHIFT_R:
                shift_r = make;
                break;
            case CTRL_L:
                ctrl_l = make;
                break;
            case CTRL_R:
                ctrl_r = make;
                break;
            case ALT_L:
                alt_l = make;
                break;
            case ALT_R:
                alt_r = make;
                break;
            case CAPS_LOCK:
				if (make)
                {
					caps_lock   = !caps_lock;
					set_leds();
				}
				break;
			case NUM_LOCK:
				if (make)
                {
					num_lock    = !num_lock;
					set_leds();
				}
				break;
			case SCROLL_LOCK:
				if (make)
                {
					scroll_lock = !scroll_lock;
					set_leds();
				}
				break;
			default:
				break;
            }

            if (make)
            {
                int pad = 0;

				/* 首先处理小键盘 */
				if ((key >= PAD_SLASH) && (key <= PAD_9))
                {
					pad = 1;
					switch(key)
                    {
					case PAD_SLASH:
						key = '/';
						break;
					case PAD_STAR:
						key = '*';
						break;
					case PAD_MINUS:
						key = '-';
						break;
					case PAD_PLUS:
						key = '+';
						break;
					case PAD_ENTER:
						key = ENTER;
						break;
					default:
						if (num_lock &&
						    (key >= PAD_0) &&
						    (key <= PAD_9))
                        {
							key = key - PAD_0 + '0';
						}
						else if (num_lock &&
                                    (key == PAD_DOT))
                        {
							key = '.';
						}
                        else
                        {
							switch(key)
                            {
							case PAD_HOME:
								key = HOME;
								break;
							case PAD_END:
								key = END;
								break;
							case PAD_PAGEUP:
								key = PAGEUP;
								break;
							case PAD_PAGEDOWN:
								key = PAGEDOWN;
								break;
							case PAD_INS:
								key = INSERT;
								break;
							case PAD_UP:
								key = UP;
								break;
							case PAD_DOWN:
								key = DOWN;
								break;
							case PAD_LEFT:
								key = LEFT;
								break;
							case PAD_RIGHT:
								key = RIGHT;
								break;
							case PAD_DOT:
								key = DELETE;
								break;
							default:
								break;
							}
                        }
                        break;
                    }
                }

                key |= shift_l  ?   FLAG_SHIFT_L : 0;
                key |= shift_r  ?   FLAG_SHIFT_R : 0;
                key |= ctrl_l   ?   FLAG_CTRL_L  : 0;
                key |= ctrl_r   ?   FLAG_CTRL_R  : 0;
                key |= alt_l    ?   FLAG_ALT_L   : 0;
                key |= alt_r    ?   FLAG_ALT_R   : 0;
                key |= pad      ?   FLAG_PAD     : 0;

                //disp_str("--key:");
                //disp_int(key);
                in_process(tty_ptr, key);
            }
        }
    }
}

/*======================================================================*
                           init_keyboard
*======================================================================*/
PUBLIC void init_keyboard()
{
    kbd_in.p_head = kbd_in.p_tail = kbd_in.buf;
    kbd_in.count = 0;

    shift_l	= shift_r =
	alt_l	= alt_r   = 
	ctrl_l	= ctrl_r  = 0;

    num_lock    = 1;
    caps_lock   =
	scroll_lock = 0;

	set_leds();

    put_irq_handler(KEYBOARD_IRQ, keyborad_handler);
    irq_i8259A_unmask(KEYBOARD_IRQ);
}
