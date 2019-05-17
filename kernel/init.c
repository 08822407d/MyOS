#include "global.h"
#include "klib.h"
#include "proto.h"
#include "archproto.h"
#include "interrupt.h"


 //系统初始化
void sys_init()
{
    disp_str("init system.\n");

    protect_init();

    interrupt_init();

	init_irqtable();
}

/*===========================================================================*
 *								保护模式初始化				     				*
 *===========================================================================*/

/* -----------------------------保护模式初始化-------------------------------- */
void protect_init()
{
	/* 先把GDT和IDT的位置清理出来 */
	memset(gdt, 0, sizeof(gdt));
	memset(idt, 0, sizeof(idt));

	/* 为lgdt和lidt做准备工作 */
	gdt_desc.base = (u32_t) gdt;
	gdt_desc.limit = (u16_t) -1;
	idt_desc.base = (u32_t) idt;
	idt_desc.limit = sizeof(idt)-1;

	init_dummyseg(DUMMY_SEG);
	init_param_dataseg(&gdt[INDEX_VGARAM], VGARAM_BASE,
						VGARAM_SIZE, USER_PRIVILEGE);
	/* 2个内核段，一个VGA段，2个进程段 */
	init_codeseg(INDEX_CS_KRNL, KRNL_PRIVILEGE);
	init_dataseg(INDEX_DS_KRNL, KRNL_PRIVILEGE);
	init_codeseg(INDEX_CS_TASK, TASK_PRIVILEGE);
	init_dataseg(INDEX_DS_TASK, TASK_PRIVILEGE);

	gdt_desc.limit += 8 * sizeof(SEG_DESC_t);
	x86_lgdt(&gdt_desc);
}

/*--------------------------中断模块的总初始化函数--------------------------*/
void interrupt_init()
{
	/* 根据两个句柄表设置好IDT里的中断门 */
	idt_veccpy_exception();
	idt_veccpy_pic();
	/* 重新lidt一次确保idt有效 */
	x86_lidt(&idt_desc);

	/* 初始化好8259中断控制器，并屏蔽全部硬件中断 */
	init_i8259A();
}

void init_irqtable()
{
	for (int irq = 0; irq < NR_IRQS; irq++)
	{
		put_irq_handler(irq, spurious_irq);
	}
}