/*=========================================================================*
 *	     					  		文件说明	                            *
 *	内核主函数,组织成可支持多平台的结构，主函数部分保证架构无关
 *=========================================================================*/
#include "const.h"
#include "global.h"
#include "archproto.h"
#include "interrupt.h"
#include "proc.h"
#include "proto.h"
#include "string.h"
#include "syscall.h"
#include "type.h"

/*======================================================================*
 *	     					   内核主函数体                               *
 *======================================================================*/
PUBLIC void kernel_main()
{
	disp_str("now in kmain.\n");

	init_process();

	init_syscall_table();

	init_keyboard();

	init_clock();

	restart();

	while (1)
		;
}
