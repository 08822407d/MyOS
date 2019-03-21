#include "interrupt.h"
#include "archproto.h"
#include "archconst.h"
#include "syscall.h"
#include "klib.h"

void keyborad_handler()
{
    u8_t scan_code = in_b(KBD_IO);
    disp_int(scan_code);
}

PUBLIC void init_keyboard()
{
    put_irq_handler(KEYBOARD_IRQ, keyborad_handler);	
	irq_i8259A_unmask(KEYBOARD_IRQ);
}