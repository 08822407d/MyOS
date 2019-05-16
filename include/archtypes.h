
#ifndef _I386_TYPES_H_
#define _I386_TYPES_H_

#include "type.h"

  /* i386段描述符 */
  struct segdesc_s
  {
      u16_t limit_low;			  /* 段界限0-15位 */
      u16_t base_low;			    /* 段基址0-15位 */
      u8_t  base_middle;		  /* 段基址16-23位 */
      u8_t  access;				    /* 段属性1：P(1位) DPL(2位) DT(1位) TYPE(4位) */
      u8_t  granularity;	    /* 段属性2：G(1位) D(1位) 0(1位) AVL(1位) 段界限16-19位(4位) */
      u8_t  base_high;			  /* 段基址24-31位 */
  } __attribute__((packed));
  typedef struct segdesc_s SEG_DESC_t;

  /* i386门描述符 */
  struct gatedesc_s
  {
    u16_t offset_low;			    /* 段内偏移地址0-15位 */
    u16_t selector;			      /* 段选择符 */
    u8_t  pad;                /* |000|XXXXX|中断/陷阱门, |XXXXXXXX|任务门 */
    u8_t  p_dpl_type;				  /* 门属性：P(1位) DPL(2位) DT(1位) TYPE(4位) */
    u16_t offset_high;		    /* 段内偏移地址16-31位 */
  } __attribute__((packed));
  typedef struct gatedesc_s GATE_DESC_t;

  /* i386描述符表指针（GDT、IDT） */
  struct desctabptr_s
  {
    u16_t limit;                /* 表界限 */
    u32_t base;                 /* 表基址 */
  } __attribute__((packed));
  typedef struct desctabptr_s DESCTAB_PTR_t;

  /* prototype of an interrupt vector table entry */
  struct gate_table_s
  {
    void(*gate) (void);
	  u16_t vec_nr;
	  u16_t privilege;
  };
  typedef struct gate_table_s GATE_TABLE_t;

  struct cpu_info {
	  u8_t	vendor;
	  u8_t	family;
	  u8_t	model;
	  u8_t	stepping;
	  u32_t	freq;		            /* 单位：MHz */
	  u32_t	flags[2];
  };

  struct tss_s {
    reg_t backlink;
    reg_t sp0;                /* 中断时使用的0特权级别栈 */
    reg_t ss0;                /* 中断时使用的0特权级别栈 */
    reg_t sp1;
    reg_t ss1;
    reg_t sp2;
    reg_t ss2;
    reg_t cr3;
    reg_t ip;
    reg_t flags;
    reg_t ax;
    reg_t cx;
    reg_t dx;
    reg_t bx;
    reg_t sp;
    reg_t bp;
    reg_t si;
    reg_t di;
    reg_t es;
    reg_t cs;
    reg_t ss;
    reg_t ds;
    reg_t fs;
    reg_t gs;
    reg_t ldt;
    u16_t trap;
    u16_t iobase;
  /* u8_t iomap[0]; */
  } __attribute__((packed));
typedef struct tss_s TSS_t;

  struct stackframe_s
	{	                  /* proc_ptr points here		↑   Low			*/
		u32_t	gs;		      /* ┓						│			                */
		u32_t	fs;		      /* ┃						│			                */
		u32_t	es;		      /* ┃						│			                */
		u32_t	ds;		      /* ┃						│			                */
		u32_t	edi;		    /* ┃						│			                */
		u32_t	esi;		    /* ┣ pushed by save()		│			        */
		u32_t	ebp;		    /* ┃						│			                */
		u32_t	kernel_esp;	/* <- 'popad' will ignore it│			    */
		u32_t	ebx;		    /* ┃						↑   栈从高地址往低地址增长      */		
		u32_t	edx;		    /* ┃						│			                */
		u32_t	ecx;		    /* ┃						│			                */
		u32_t	eax;		    /* ┛						│			                */
		u32_t	retaddr;	  /* return address for      	│   assembly code save()	*/
		u32_t	eip;		    /* ┓						│			                */
		u32_t	cs;		      /* ┃						│			                */
		u32_t	psw;		    /* ┣ these are pushed by 	│   CPU during interrupt	*/
		u32_t	esp;		    /* ┃						│			                */
		u32_t	ss;		      /* ┛						┷   High			        */
	};
  typedef struct stackframe_s STACK_FRAME_t;

#endif /* #ifndef _I386_TYPES_H_ */