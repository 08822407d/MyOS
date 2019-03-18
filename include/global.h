/* 全局变量和数据结构 */
#include "type.h"
#include "const.h"
#include "archtypes.h"
#include "proc.h"
#ifdef _GLOBAL_VAR_
#undef EXTERN
#define EXTERN
#endif

EXTERN	u16_t 				disp_pos;

EXTERN	struct desctabptr_s gdt_desc,
							idt_desc;
EXTERN	struct segdesc_s 	gdt[GDT_SIZE];
EXTERN	struct gatedesc_s 	idt[IDT_SIZE];

EXTERN	u8_t 				k_Stack[K_STACK_SIZE];
extern 	u8_t*				k_Stacktop;

EXTERN  struct tss_s 		tss0;
EXTERN  struct proc_s	  	PCB[NR_PROCS];
EXTERN  u8_t 				proc_Stack[STACK_SIZE_TOTAL];

extern 	struct proc_s* 		p_proc_ready; 
extern 	int 				k_reenter;
extern	u32_t    			p_num;
extern  struct proctable_s	proc_map[];
extern 	unsigned char* 		p_stacktop;

void TestA(void);
void TestB(void);
void TestC(void);

/* 宏 */
/* 线性地址 → 物理地址 */
#define vir2phys(seg_base, vir)	(u32_t)(((u32_t)seg_base) + (u32_t)(vir))