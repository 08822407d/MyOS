#ifndef	_MyOS_CONST_H_
#define	_MyOS_CONST_H_

    #define K_STACK_SIZE        0x4000
/*======================================================================*
                            一些系统参数
 *======================================================================*/
    #define GDT_SIZE    1024
    #define IDT_SIZE    256
    #define LDT_SIZE    3
/* 段类型 */
    #define DUMMY_SEG   0
    #define CODE_SEG    1
    #define DATA_SEG    2
    #define TSS_SEG     3
    #define TR_SEG      4

/* 全局描述符 */
    #define INDEX_DUMMY         0x00
    #define INDEX_VGARAM        0x01
    #define INDEX_CS_KRNL       0x02
    #define INDEX_DS_KRNL       0x03
    #define INDEX_CS_SYSCALL    0x04
    #define INDEX_DS_SYSCALL    0x05
    #define INDEX_TSS0          0x06
    #define INDEX_LDT0          0x07

    #define SEG_SELECTOR(i)     ((i)*8)
    #define SELECTOR_DUMMY      SEG_SELECTOR(INDEX_DUMMY)
    #define SELECTOR_VGARAM     SEG_SELECTOR(INDEX_VGARAM)|USER_PRIVILEGE
    #define SELECTOR_CS_KRNL    SEG_SELECTOR(INDEX_CS_KRNL)|KRNL_PRIVILEGE
    #define SELECTOR_DS_KRNL    SEG_SELECTOR(INDEX_DS_KRNL)|KRNL_PRIVILEGE
    #define SELECTOR_CS_SYSCALL SEG_SELECTOR(INDEX_CS_SYSCALL)|TASK_PRIVILEGE
    #define SELECTOR_DS_SYSCALL SEG_SELECTOR(INDEX_CS_SYSCALL)|TASK_PRIVILEGE
    #define SELECTOR_TSS0       SEG_SELECTOR(INDEX_TSS0)|KRNL_PRIVILEGE
    #define SELECTOR_LDT0       SEG_SELECTOR(INDEX_LDT0)|KRNL_PRIVILEGE

/*======================================================================*
                        一些i386架构的系统常量
 *======================================================================*/
// 先从minix复制过来，之后慢慢整理
    /* Access-byte bits. */
    /* Access-byte and type-byte bits. */
    #define PRESENT             0x80	/* set for descriptor present */
    #define DPL                 0x60	/* descriptor privilege level mask */
    #define DPL_SHIFT           5
    #define SEGMENT             0x10	/* set for segment-type descriptors */
    #define EXECUTABLE          0x08	/* set for executable segment */
    #define CONFORMING          0x04	/* set for conforming segment if executable */
    #define EXPAND_DOWN         0x04	/* set for expand-down segment if !executable*/
    #define READABLE            0x02	/* set for readable segment if executable */
    #define WRITEABLE           0x02	/* set for writeable segment if !executable */
    #define TSS_BUSY            0x02	/* set if TSS descriptor is busy */
    #define ACCESSED            0x01	/* set if segment accessed */
    #define BASE_MIDDLE_SHIFT   16	/* shift for base --> base_middle */
    #define BASE_HIGH_SHIFT     24  /* shift for base --> base_high */
    #define BYTE_GRAN_MAX       0xFFFFFL   /* maximum size for byte granular segment */
    #define GRANULARITY_SHIFT   16  /* shift for limit --> granularity */
    #define OFFSET_HIGH_SHIFT   16  /* shift for (gate) offset --> offset_high */
    #define PAGE_GRAN_SHIFT     12  /* extra shift for page granular limits */
/* Granularity byte. */
    #define GRANULAR  	        0x80	/* set for 4K granularilty */
    #define DEFAULT   	        0x40	/* set for 32-bit defaults (executable seg) */
    #define BIG       	        0x40	/* set for "BIG" (expand-down seg) */
    #define AVL        	        0x10	/* 0 for available */
    #define LIMIT_HIGH   	    0x0F	/* mask for high bits of limit */
//------------------------------------
/* 权限 */
    #define	KRNL_PRIVILEGE		0
    #define	TASK_PRIVILEGE		1
    #define	USER_PRIVILEGE		3
/* 描述符类型值说明 */
    #define	DA_32				0x4000	/* 32 位段				 */
    #define	DA_LIMIT_4K			0x8000	/* 段界限粒度为 4K 字节	   */
    #define	DA_DPL0				0x00	/* DPL = 0				*/
    #define	DA_DPL1				0x20	/* DPL = 1				*/
    #define	DA_DPL2				0x40	/* DPL = 2				*/
    #define	DA_DPL3				0x60	/* DPL = 3				*/
/* 存储段描述符类型值说明 */
    #define	DA_DR				0x90	/* 存在的只读数据段类型值		   */
    #define	DA_DRW				0x92	/* 存在的可读写数据段属性值		   */
    #define	DA_DRWA				0x93	/* 存在的已访问可读写数据段类型值   */
    #define	DA_C				0x98	/* 存在的只执行代码段属性值	       */
    #define	DA_CR				0x9A	/* 存在的可执行可读代码段属性值	    */
    #define	DA_CCO				0x9C	/* 存在的只执行一致代码段属性值		*/
    #define	DA_CCOR				0x9E	/* 存在的可执行可读一致代码段属性值	 */
/* 系统段描述符类型值说明 */
    #define	DA_LDT				0x82	/* 局部描述符表段类型值		 */
    #define	DA_TaskGate			0x85	/* 任务门类型值				*/
    #define	DA_386TSS			0x89	/* 可用 386 任务状态段类型值 */
    #define	DA_386CGate			0x8C	/* 386 调用门类型值			*/
    #define	DA_386IGate			0x8E	/* 386 中断门类型值			*/
    #define	DA_386TGate			0x8F	/* 386 陷阱门类型值			*/
/* 选择子类型值说明 */
/* 其中, SA_ : Selector Attribute */
    #define	SA_RPL_MASK	0xFFFC
    #define	SA_RPL0		0
    #define	SA_RPL1		1
    #define	SA_RPL2		2
    #define	SA_RPL3		3

    #define	SA_TI_MASK	0xFFFB
    #define	SA_TIG		0
    #define	SA_TIL		4

    #define VGAROM_BASE     0xB8000
    #define VGAROM_LENTH    0x8000
#endif
