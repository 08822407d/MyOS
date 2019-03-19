/*=========================================================================*
 *	     					  		文件说明	                            *
 *	内核主函数,组织成可支持多平台的结构，主函数部分保证架构无关
 *=========================================================================*/
#include "type.h"
#include "const.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "archproto.h"
#include "interrupt.h"

/*======================================================================*
 *	     					   内核主函数体                               *
 *======================================================================*/
PUBLIC void kernel_main()
{
	disp_str("now in kmain.\n");

	process_init();

	put_irq_handler(CLOCK_IRQ, clock_handler);
	irq_i8259A_unmask(0U);
	restart();

	while(1);
}
