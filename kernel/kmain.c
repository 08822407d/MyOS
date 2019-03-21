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
#include "syscall.h"

/*======================================================================*
 *	     					   内核主函数体                               *
 *======================================================================*/
PUBLIC void kernel_main()
{
	disp_str("now in kmain.\n");

	process_init();

	init_syscall_table();

	init_keyboard();
	init_clock();

	restart();

	while(1);
}
