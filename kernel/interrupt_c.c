/*=========================================================================*
 *	     					  		文件说明	                            *
 *	包含初始化异常/硬件中断相关系统数据结构的函数
 *=========================================================================*/
// DEBUG日记
// 2018.01.01补
// 最开始的时候，写完设置中断门表的部分，居然忘了设置idt_ptr，导致不触发中断处理

#include "global.h"
#include "type.h"
#include "proto.h"
#include "syscall.h"
#include "string.h"
#include "archtypes.h"
#include "archproto.h"
#include "interrupt.h"

extern char * itoa(char * str, int num);
extern void disp_int(int input);


/*======================================================================*
 							中断相关数据结构
 *======================================================================*/
/* 以下两个表用于IDT初始化函数填充中断门 */

/*------------------------------硬件中断句柄表-----------------------------*/
static GATE_TABLE_t gate_table_pic[] =
{
	{ hwint00, 	VECTOR( 0), 	KRNL_PRIVILEGE },
	{ hwint01, 	VECTOR( 1), 	KRNL_PRIVILEGE },
	{ hwint02, 	VECTOR( 2), 	KRNL_PRIVILEGE },
	{ hwint03, 	VECTOR( 3), 	KRNL_PRIVILEGE },
	{ hwint04, 	VECTOR( 4), 	KRNL_PRIVILEGE },
	{ hwint05, 	VECTOR( 5), 	KRNL_PRIVILEGE },
	{ hwint06, 	VECTOR( 6), 	KRNL_PRIVILEGE },
	{ hwint07, 	VECTOR( 7), 	KRNL_PRIVILEGE },
	{ hwint08, 	VECTOR( 8), 	KRNL_PRIVILEGE },
	{ hwint09, 	VECTOR( 9), 	KRNL_PRIVILEGE },
	{ hwint10, 	VECTOR(10), 	KRNL_PRIVILEGE },
	{ hwint11, 	VECTOR(11), 	KRNL_PRIVILEGE },
	{ hwint12, 	VECTOR(12), 	KRNL_PRIVILEGE },
	{ hwint13, 	VECTOR(13), 	KRNL_PRIVILEGE },
	{ hwint14, 	VECTOR(14), 	KRNL_PRIVILEGE },
	{ hwint15, 	VECTOR(15), 	KRNL_PRIVILEGE },
	{ NULL, 	0, 				0			   }
};

/*-------------------------------异常句柄表-------------------------------*/
static GATE_TABLE_t gate_table_exceptions[] =
{
	{ divide_error, 			INTVEC_DIVIDE, 			KRNL_PRIVILEGE },
	{ single_step_exception,	INTVEC_DEBUG, 			KRNL_PRIVILEGE },
	{ nmi,						INTVEC_NMI, 			KRNL_PRIVILEGE },
	{ breakpoint_exception,		INTVEC_BREAKPOINT, 		USER_PRIVILEGE },
	{ overflow, 				INTVEC_OVERFLOW, 		USER_PRIVILEGE },
	{ bounds_check, 			INTVEC_BOUNDS, 			KRNL_PRIVILEGE },
	{ inval_opcode, 			INTVEC_INVAL_OP, 		KRNL_PRIVILEGE },
	{ copr_not_available,	 	INTVEC_COPROC_NOT, 		KRNL_PRIVILEGE },
	{ double_fault, 			INTVEC_DOUBLE_FAULT,	KRNL_PRIVILEGE },
	{ copr_seg_overrun, 		INTVEC_COPROC_SEG, 		KRNL_PRIVILEGE },
	{ inval_tss, 				INTVEC_INVAL_TSS, 		KRNL_PRIVILEGE },
	{ segment_not_present, 		INTVEC_SEG_NOT, 		KRNL_PRIVILEGE },
	{ stack_exception,	 		INTVEC_STACK_FAULT, 	KRNL_PRIVILEGE },
	{ general_protection, 		INTVEC_PROTECTION, 		KRNL_PRIVILEGE },
	{ page_fault, 				INTVEC_PAGE_FAULT, 		KRNL_PRIVILEGE },
	{ copr_error, 				INTVEC_COPROC_ERR, 		KRNL_PRIVILEGE },
	{ alignment_check, 			INTVEC_ALIGNMENT_CHECK, KRNL_PRIVILEGE },
	{ machine_check, 			INTVEC_MACHINE_CHECK, 	KRNL_PRIVILEGE },
	{ simd_exception, 			INTVEC_SIMD_EXCEPTION, 	KRNL_PRIVILEGE },

/*	{ ipc_entry_softint_orig, IPC_VECTOR_ORIG, USER_PRIVILEGE },
	{ kernel_call_entry_orig, KERN_CALL_VECTOR_ORIG, USER_PRIVILEGE },
	{ ipc_entry_softint_um, IPC_VECTOR_UM, USER_PRIVILEGE },
	{ kernel_call_entry_um, KERN_CALL_VECTOR_UM, USER_PRIVILEGE },		*/
	{ sys_call,					INTVEC_SYSCALL,			USER_PRIVILEGE },

	{ NULL, 					0, 						0			   }
};

/*------------------------------异常信息字符串-----------------------------*/
char * excep_msg[20] =
	{	"#DE Divide Error",
	 	"#DB RESERVED",
		"—  NMI Interrupt",
		"#BP Breakpoint",
		"#OF Overflow",
		"#BR BOUND Range Exceeded",
		"#UD Invalid Opcode (Undefined Opcode)",
		"#NM Device Not Available (No Math Coprocessor)",
		"#DF Double Fault",
		"    Coprocessor Segment Overrun (reserved)",
		"#TS Invalid TSS",
		"#NP Segment Not Present",
		"#SS Stack-Segment Fault",
		"#GP General Protection",
		"#PF Page Fault",
		"—  (Intel reserved. Do not use.)",
		"#MF x87 FPU Floating-Point Error (Math Fault)",
		"#AC Alignment Check",
		"#MC Machine Check",
		"#XF SIMD Floating-Point Exception"
	};

/*======================================================================*
 							IDT设置相关的函数
 *======================================================================*/

/*-------------------------------设置中断门-------------------------------*/
void set_int_gate(GATE_TABLE_t* tab,
					u32_t vec_nr, u32_t offset, u32_t dpl_type)
{
/* 通用的中断门设置函数 */
  GATE_DESC_t* id_p;

  id_p = &(tab[vec_nr]);
  id_p->offset_low = (u16_t)offset;
  id_p->selector = SELECTOR_CS_KRNL;
  id_p->p_dpl_type = (u8_t)dpl_type;
  id_p->offset_high = (u16_t)(offset >> OFFSET_HIGH_SHIFT);
}

/*----------------------------设置IDT中的中断门---------------------------*/
void set_int_gate_idt(u32_t vec_nr, u32_t offset, u32_t dpl_type)
{
	set_int_gate(idt, vec_nr, offset, dpl_type);
}

/*--------------------按照给定的中断句柄表设置IDT中的中断门-------------------*/
void idt_veccpy(GATE_TABLE_t * first)
{
	GATE_TABLE_t* gatetab_p;
	for (gatetab_p = first; gatetab_p->gate != NULL; gatetab_p++)
	{
		set_int_gate(idt, gatetab_p->vec_nr,
					(u32_t) gatetab_p->gate,
					PRESENT | DA_386IGate | (gatetab_p->privilege << DPL_SHIFT));
	}
}

/*---------------------------初始化IDT中异常部分--------------------------*/
void idt_veccpy_exception(void)
{
	idt_veccpy(gate_table_exceptions);
}

/*---------------------------初始化IDT中IRQ部分--------------------------*/
void idt_veccpy_pic(void)
{
	idt_veccpy(gate_table_pic);
}

/*------------------------汇编lidt指令，重新加载IDT------------------------*/
void idt_reload(void)
{
	x86_lidt(&idt_desc);
}

/*======================================================================*
                           	(目前临时的)异常处理
 *======================================================================*/
void exception_handler(u32_t vec_nr,u32_t err_code,u32_t eip,u32_t cs,u32_t eflags)
{
	int i;
	int text_color = 0x1F; /* 蓝底高亮白字 */
	

/* 通过打印空格的方式清空屏幕的前五行，disp_pos置0 */
	disp_pos = 0;
	for(i=0;i<80*25;i++){
		disp_color_str(" ",text_color);
	}
	disp_pos = 0;

	disp_color_str("Exception! --> ", text_color);
	disp_color_str(excep_msg[vec_nr], text_color);
	disp_str("\n\n");
	disp_color_str("  EFLAGS:", text_color);
	disp_int(eflags);
	disp_color_str("  CS:", text_color);
	disp_int(cs);
	disp_color_str("  EIP:", text_color);
	disp_int(eip);

	if(err_code != 0xFFFFFFFF)
	{
		disp_color_str("  Error code:", text_color);
		disp_int(err_code);
	}
}


/*======================================================================*
                           	(目前临时的)硬中断处理
 *======================================================================*/
PUBLIC void spurious_irq(u32_t irq)
{
	disp_str("spurious_irq: ");
	disp_int(irq);
	disp_str("\n");
}

/*======================================================================*
                           put_irq_handler
 *======================================================================*/
PUBLIC void put_irq_handler(int irq, irq_handler_f handler)
{
	irq_i8259A_mask(irq);
	irq_table[irq] = handler;
}
 
