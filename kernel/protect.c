/*=========================================================================*
 *	     					  		文件说明	                            *
 *	包含初始化386保护模式的相关函数
 *=========================================================================*/
#include "const.h"
#include "global.h"
#include "archtypes.h"
#include "archproto.h"


/*===========================================================================*
 *								设置描述符				     				  *
 *===========================================================================*/

/* --------------------------部分初始化一个段描述符----------------------------- */
PUBLIC void set_segdesc( struct segdesc_s* seg_dp, u32_t base,u32_t size)
{
/* 初始化段描述符的一部分，默认基址0，根据size设置gran属性 */
	seg_dp->base_low 	= base;
	seg_dp->base_middle = base >> BASE_MIDDLE_SHIFT;
	seg_dp->base_high 	= base >> BASE_HIGH_SHIFT;

	--size;			/* 把段尺寸换算为段界限值 */
  	if (size > BYTE_GRAN_MAX)
	{
		seg_dp->limit_low 	= size >> PAGE_GRAN_SHIFT;
		seg_dp->granularity = GRANULAR | (size >>
			    			(PAGE_GRAN_SHIFT + GRANULARITY_SHIFT));
	}
	else
	{
		seg_dp->limit_low 	= size;
		seg_dp->granularity = size >> GRANULARITY_SHIFT;
	}
  	seg_dp->granularity |= DEFAULT;	/* 段上限默认为4GB */
}

/* ----------------------通用的设置一个数据段描述符的函数-------------------------- */
PUBLIC void init_param_dataseg(register struct segdesc_s *seg_dp,
								u32_t base, u32_t size,	u32_t privilege)
{
	/* 设置一个数据段的描述符 */
	set_segdesc(seg_dp, base, size);
	seg_dp->access = (privilege << DPL_SHIFT) | 
						(PRESENT | SEGMENT | WRITEABLE | ACCESSED);
	/* EXECUTABLE = 0, EXPAND_DOWN = 0, ACCESSED = 0 
			不可执行、向上扩展、权限级别0						*/
}
/* -------------------------设置flat数据段描述符的函数---------------------------- */
PUBLIC void init_dataseg(u32_t index, u32_t privilege)
{
	/* 设置一个数据段的描述符,默认基址0，尺寸4GB */
	init_param_dataseg(&gdt[index], 0, 0xFFFFFFFF, privilege);
}

/* ------------------------设置flat代码段描述符的函数------------------------- */
PRIVATE void init_codeseg(u32_t index, u32_t privilege)
{
	/* 设置一个代码段描述符,默认基址0，尺寸4GB */
	set_segdesc(&gdt[index], 0, 0xFFFFFFFF);
	gdt[index].access = (privilege << DPL_SHIFT)
	        | (PRESENT | SEGMENT | EXECUTABLE | READABLE);
		/* CONFORMING = 0, ACCESSED = 0
			非一致代码段，权限级别0		*/
}

/* -------------------------------初始化TSS------------------------------- */
PUBLIC void tss_init(u8_t * kernel_stack)
{
	struct tss_s * t = &tss;
	int index = INDEX_TSS0;
	struct segdesc_s *tssgdt;

	tssgdt = &gdt[index];
  
	init_param_dataseg(tssgdt, (u32_t) t,
			sizeof(struct tss_s), KRNL_PRIVILEGE);
	tssgdt->access = PRESENT | (KRNL_PRIVILEGE << DPL_SHIFT) | DA_386TSS;

	/* Build TSS. */
	memset(t, 0, sizeof(*t));
	t->ds = t->es = t->fs = t->gs = t->ss0 = SELECTOR_DS_KRNL;
	t->cs = SELECTOR_CS_KRNL;
	t->iobase = sizeof(struct tss_s);	/* 无io-map */
}

PUBLIC void init_dummyseg(u32_t index)
{
	gdt[index].access = 0;
	gdt[index].base_high = 0;
	gdt[index].base_low = 0;
	gdt[index].base_middle = 0;
	gdt[index].granularity = 0;
	gdt[index].limit_low = 0;
}

/* -------------------------------增加一个段描述符------------------------------- */
PUBLIC void add_desc_gdt(u32_t segtype, u32_t privilege)
{
	u16_t gdt_currentsize = gdt_desc.limit + 1;
	if (gdt_currentsize == 1)
		gdt_currentsize--;

	switch (segtype)
	{
		case CODE_SEG :
			init_codeseg(gdt_currentsize / 8, privilege);
			break;
		case DATA_SEG :
		case TSS_SEG  :
		case TR_SEG   :
			init_dataseg(gdt_currentsize / 8, privilege);
			break;
		default : 
			init_dummyseg(gdt_currentsize / 8);

	}
	gdt_currentsize += 8;
	gdt_desc.limit = gdt_currentsize - 1;
}

PUBLIC void del_desc_gdt(void)
{
	u16_t gdt_currentindex;
	if (gdt_desc.limit + 1 > 0 )
	{
		gdt_currentindex = (gdt_desc.limit + 1) / 8;

		gdt[gdt_currentindex].access = 0;
		gdt[gdt_currentindex].base_high = 0;
		gdt[gdt_currentindex].base_low = 0;
		gdt[gdt_currentindex].base_middle = 0;
		gdt[gdt_currentindex].granularity = 0;
		gdt[gdt_currentindex].limit_low = 0;

		gdt_desc.limit -= 8;
	}
}
