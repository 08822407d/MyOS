/* 全局变量和数据结构 */
#include "type.h"
#include "const.h"
#include "archtypes.h"
#ifdef _GLOBAL_VAR_
#undef EXTERN
#define EXTERN
#endif

EXTERN	u16_t 				disp_pos;

EXTERN	struct desctabptr_s gdt_desc,
							idt_desc;
EXTERN	struct segdesc_s 	gdt[GDT_SIZE];
EXTERN	struct gatedesc_s 	idt[IDT_SIZE];

EXTERN	unsigned char 		k_Stack[K_STACK_SIZE];
#define k_Stacktop			(u32_t)k_Stack+K_STACK_SIZE

EXTERN  struct tss_s 		tss0;

//extern  struct proc_s * 	PCB;
//extern	unsigned char *		proc_Stack;
//extern  struct proctable_s*	proc_map;

//extern	void TestA(void);
//extern	void TestB(void);
//extern	void TestC(void);

/* 宏 */
/* 线性地址 → 物理地址 */
#define vir2phys(seg_base, vir)	(u32_t)(((u32_t)seg_base) + (u32_t)(vir))