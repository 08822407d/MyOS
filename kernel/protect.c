/*=========================================================================*
 *	     					  		文件说明	                            *
 *	包含初始化386保护模式的相关函数
 *=========================================================================*/
#include "const.h"
#include "global.h"
#include "archproto.h"
#include "archtypes.h"


/*===========================================================================*
 *								设置描述符				     				  *
 *===========================================================================*/

/* --------------------------部分初始化一个段描述符----------------------------- */
PUBLIC void set_segdesc(SEG_DESC_t* seg_dp, u32_t base, u32_t size)
{
	/* 初始化段描述符的一部分，默认基址0，根据size设置gran属性 */
	seg_dp->base_low    = base;
	seg_dp->base_middle = base >> BASE_MIDDLE_SHIFT;
	seg_dp->base_high   = base >> BASE_HIGH_SHIFT;

	--size; /* 把段尺寸换算为段界限值 */
	if (size > BYTE_GRAN_MAX)
	{
		seg_dp->limit_low   = size >> PAGE_GRAN_SHIFT;
		seg_dp->granularity = GRANULAR | (size >>
										  (PAGE_GRAN_SHIFT + GRANULARITY_SHIFT));
	}
	else
	{
		seg_dp->limit_low   = size;
		seg_dp->granularity = size >> GRANULARITY_SHIFT;
	}
	
	seg_dp->granularity |= DEFAULT; /* 段上限默认为4GB */
}

/* --------------------------设置一个系统段描述符------------------------------ */
PUBLIC void init_segdesc(SEG_DESC_t* seg_dp, u32_t base, u32_t size, u32_t attr)
{
	set_segdesc(seg_dp, base, size);
	seg_dp->access |= attr;
}

/* 设置tss描述符 */
PUBLIC void init_tssdesc(u32_t index, u32_t base)
{
	set_segdesc(&gdt[index], base, sizeof(TSS_t));
	gdt[index].access |= DA_386TSS;
}

/* 设置ldt描述符 */
PUBLIC void init_ldtdesc(u32_t index, u32_t base)
{
	set_segdesc(&gdt[index], base, sizeof(TSS_t));
	gdt[index].access |= DA_LDT;
}

/* --------------------------设置一个全局段描述符------------------------------ */
PUBLIC void init_param_dataseg(SEG_DESC_t* seg_dp,
							   u32_t base, u32_t size, u32_t privilege)
{
	/* 设置一个数据段的描述符 */
	set_segdesc(seg_dp, base, size);
	seg_dp->access = (privilege << DPL_SHIFT) |
					 (PRESENT | SEGMENT | WRITEABLE | ACCESSED);
	/* EXECUTABLE = 0, EXPAND_DOWN = 0, ACCESSED = 0 
			不可执行、向上扩展、权限级别0						*/
}

PUBLIC void init_param_codeseg(SEG_DESC_t* seg_dp,
							   u32_t base, u32_t size, u32_t privilege)
{
	/* 设置一个数据段的描述符 */
	set_segdesc(seg_dp, base, size);
	seg_dp->access = (privilege << DPL_SHIFT) |
					 (PRESENT | SEGMENT | EXECUTABLE | READABLE);
	/* EXECUTABLE = 0, EXPAND_DOWN = 0, ACCESSED = 0 
			可执行、向上扩展、权限级别0						*/
}

/* 设置flat数据段描述符的函数 */
PUBLIC void init_dataseg(u32_t index, u32_t privilege)
{
	/* 设置一个数据段的描述符,默认基址0，尺寸4GB */
	init_param_dataseg(&gdt[index], 0, SIZE_4GB, privilege);
}

/* 设置flat代码段描述符的函数 */
PUBLIC void init_codeseg(u32_t index, u32_t privilege)
{
	/* 设置一个代码段描述符,默认基址0，尺寸4GB */
	set_segdesc(&gdt[index], 0, SIZE_4GB);
	gdt[index].access = (privilege << DPL_SHIFT) |
						(PRESENT | SEGMENT | EXECUTABLE | READABLE);
	/* CONFORMING = 0, ACCESSED = 0
			非一致代码段，权限级别0		*/
}

/* 设置空段 */
PUBLIC void init_dummyseg(u32_t index)
{
	gdt[index].access      =
	gdt[index].base_high   =
	gdt[index].base_middle =
	gdt[index].granularity = 0;

	gdt[index].base_low    =
	gdt[index].limit_low   = 0;
}

/* -------------------------------初始化TSS------------------------------- */
PUBLIC void tss_init(u8_t *kernel_stack)
{
	TSS_t* 		t = &tss0;
	int 		index = INDEX_TSS0;
	SEG_DESC_t*	tssgdt;

	tssgdt = &gdt[index];

	init_param_dataseg(tssgdt, (u32_t)t,
					   sizeof(TSS_t), KRNL_PRIVILEGE);
	tssgdt->access = PRESENT | (KRNL_PRIVILEGE << DPL_SHIFT) | DA_386TSS;

	/* Build TSS. */
	memset(t, 0, sizeof(*t));
	t->ds = t->es = t->fs = t->gs = t->ss0 = SELECTOR_DS_KRNL;
	t->cs = SELECTOR_CS_KRNL;
	t->iobase = sizeof(TSS_t); /* 无io-map */
}
