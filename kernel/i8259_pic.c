#include "archproto.h"
#include "interrupt.h"
#include "type.h"

#define ICW1_AT         0x11	/* edge triggered, cascade, need ICW4 */
#define ICW1_PC         0x13	/* edge triggered, no cascade, need ICW4 */
#define ICW1_PS         0x19	/* level triggered, cascade, need ICW4 */
#define ICW4_AT_SLAVE   0x01	/* not SFNM, not buffered, normal EOI, 8086 */
#define ICW4_AT_MASTER  0x05	/* not SFNM, not buffered, normal EOI, 8086 */
#define ICW4_PC_SLAVE   0x09	/* not SFNM, buffered, normal EOI, 8086 */
#define ICW4_PC_MASTER  0x0D	/* not SFNM, buffered, normal EOI, 8086 */
#define ICW4_AT_AEOI_SLAVE   0x03 /* not SFNM, not buffered, auto EOI, 8086 */
#define ICW4_AT_AEOI_MASTER  0x07 /* not SFNM, not buffered, auto EOI, 8086 */
#define ICW4_PC_AEOI_SLAVE   0x0B /* not SFNM, buffered, auto EOI, 8086 */
#define ICW4_PC_AEOI_MASTER  0x0F /* not SFNM, buffered, auto EOI, 8086 */

/*======================================================================*
 *	     					  设置i8259A的函数                            *
 *======================================================================*/

/*----------------------------初始化i8259A-------------------------------*/
PUBLIC void init_i8259A()
{
	out_b( INT_CTL,			ICW1_AT);
	out_b( INT_CTLMASK,		IRQ0_VECTOR);			/* ICW2 for master */
	out_b( INT_CTLMASK,		(1 << CASCADE_IRQ));	/* ICW3 tells slaves */
	out_b( INT_CTLMASK,		ICW4_AT_MASTER);

	out_b( INT2_CTL,		ICW1_AT);
	out_b( INT2_CTLMASK,	IRQ8_VECTOR);			/* ICW2 for slave */
	out_b( INT2_CTLMASK,	CASCADE_IRQ);			/* ICW3 is slave nr */
	out_b( INT2_CTLMASK,	ICW4_AT_SLAVE);

	out_b( INT_CTLMASK,		~(1 << CASCADE_IRQ));	/* IRQ 0-7 mask */
	out_b( INT2_CTLMASK,	~0);					/* IRQ 8-15 mask */
}


PUBLIC void irq_i8259A_unmask(const u32_t irq)
{
	const u8_t ctl_mask = irq < 8 ? INT_CTLMASK : INT2_CTLMASK;
	out_b(ctl_mask, in_b(ctl_mask) & ~(1 << (irq & 0x7)));
}

PUBLIC void irq_i8259A_mask(const u32_t irq)
{
	const u8_t ctl_mask = irq < 8 ? INT_CTLMASK : INT2_CTLMASK;
	out_b(ctl_mask, in_b(ctl_mask) | (1 << (irq & 0x7)));
}

/*------------------------------屏蔽所有中断-----------------------------*/
PUBLIC void i8259A_disable(void)
{
	out_b(INT2_CTLMASK, 0xFF);
	out_b(INT_CTLMASK, 0xFF);
	in_b(INT_CTLMASK);
}
/*---------------------------EOI(中断结束恢复接收)-------------------------*/
PUBLIC void irq_i8259A_eoi(u32_t irq)
{
	if (irq < 8)
		eoi_i8259A_master();
	else
		eoi_i8259A_slave();
}

void eoi_i8259A_master()
{
	out_b(INT_CTL, END_OF_INT);
}

void eoi_i8259A_slave()
{
	out_b(INT_CTL, END_OF_INT);
	out_b(INT2_CTL, END_OF_INT);
}