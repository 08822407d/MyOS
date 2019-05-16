/* 全局变量和数据结构 */
#include "type.h"
#include "const.h"
#include "archconst.h"
#include "archtypes.h"
#include "interrupt.h"
#include "proc.h"
#include "tty.h"

#ifdef _GLOBAL_VAR_
#undef EXTERN
#define EXTERN
#endif

#define HALT	while(1){}

EXTERN	u16_t 			disp_pos;
EXTERN  u32_t    		whileloops_per_ms;

EXTERN	DESCTAB_PTR_t	gdt_desc,
						idt_desc;
EXTERN	SEG_DESC_t	 	gdt[GDT_SIZE];
EXTERN	GATE_DESC_t	 	idt[IDT_SIZE];

EXTERN	u8_t 			k_Stack[K_STACK_SIZE];
extern 	u32_t			k_Stacktop;

extern	irq_handler_f	irq_table[];
extern	systcall_f      syscall_table[];

EXTERN  TSS_t		 	tss0;
EXTERN  PROC_t		  	PCB[NR_PROCS];
EXTERN  u8_t 			proc_Stack[STACK_SIZE_TOTAL];

extern 	PROC_t* 		p_proc_ready; 
extern 	int 			k_reenter;
extern	u32_t    		p_count;
extern  PROC_TABLE_t	proc_map[];
extern 	unsigned char* 	p_stacktop;

EXTERN	TTY_t        	tty_table[];
EXTERN	CONSOLE_t    	console_table[];
EXTERN	int          	current_console_idx;
/* =============================系统变量=============================== */
extern	unsigned int    ticks;

/* =============================一次性变量=============================== */
extern	unsigned int    flag_for_millisecond_ajust;
extern  unsigned int    start_millisecond_ajusting;
extern  unsigned int    end_millisecond_ajusting;
/* =============================一次性变量=============================== */
/* 宏 */
/* 线性地址 → 物理地址 */
#define vir2phys(seg_base, vir)	(u32_t)(((u32_t)seg_base) + (u32_t)(vir))

void 	TestA(void);
void 	TestB(void);
void 	TestC(void);