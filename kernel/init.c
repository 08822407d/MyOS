#include "global.h"
#include "klib.h"
#include "archproto.h"


 //系统初始化
void sys_init()
{
    disp_str("init system.\n");

    protect_init();

    interrupt_init();
}

/*===========================================================================*
 *								保护模式初始化				     				*
 *===========================================================================*/

/* -----------------------------保护模式初始化-------------------------------- */
PUBLIC void protect_init()
{
	/* 先把GDT和IDT的位置清理出来 */
	memset(gdt, 0, sizeof(gdt));
	memset(idt, 0, sizeof(idt));

	/* 为lgdt和lidt做准备工作 */
	gdt_desc.base = (u32_t) gdt;
	gdt_desc.limit = 0x0;
	idt_desc.base = (u32_t) idt;
	idt_desc.limit = sizeof(idt)-1;

	add_desc_gdt(DUMMY_SEG, 0);
	add_desc_gdt(DATA_SEG, USER_PRIVILEGE);
	init_param_dataseg(&gdt[INDEX_VGARAM], VGAROM_BASE,
						VGAROM_LENTH, USER_PRIVILEGE);
	/* 2个内核段，一个VGA段，2个进程段 */
	add_desc_gdt(CODE_SEG, KRNL_PRIVILEGE);
	add_desc_gdt(DATA_SEG, KRNL_PRIVILEGE);
	add_desc_gdt(CODE_SEG, TASK_PRIVILEGE);
	add_desc_gdt(DATA_SEG, TASK_PRIVILEGE);
	add_desc_gdt(CODE_SEG, USER_PRIVILEGE);
	add_desc_gdt(DATA_SEG, USER_PRIVILEGE);

	x86_lgdt(&gdt_desc);
/* 设置2个LDT，一个对应TASK两个段，一个对应USER两个段 
  	init_param_dataseg(&gdt[INDEX_LDT_TASK],&gdt[INDEX_CS_TASK],0x10,KRNL_PRIVILEGE); 
	gdt[INDEX_LDT_TASK].access = PRESENT | DA_LDT;
	init_param_dataseg(&gdt[INDEX_LDT_USER],&gdt[INDEX_CS_USER],0x10,KRNL_PRIVILEGE); 
	gdt[INDEX_LDT_USER].access = PRESENT | DA_LDT;
*/
}

/*--------------------------中断模块的总初始化函数--------------------------*/
PUBLIC void interrupt_init()
{
	/* 根据两个句柄表设置好IDT里的中断门 */
	idt_veccpy_exception();
	idt_veccpy_pic();
	/* 重新lidt一次确保idt有效 */
	x86_lidt(&idt_desc);

	/* 初始化好8259中断控制器，并屏蔽全部硬件中断 */
	init_i8259A();
}