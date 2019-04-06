/* IBM PC兼容机的中断相关常量 */

#ifndef _MYOS_INTERRUPT_H_
#define _MYOS_INTERRUPT_H_

//#if defined(__i386__)

    /* i8259A的端口 */
    #define INT_CTL                 0x20    /* 主片 */
    #define INT_CTLMASK             0x21
    #define INT2_CTL                0xA0    /* 从片 */
    #define INT2_CTLMASK            0xA1

    /* 中断向量 */
    #define	IRQ0_VECTOR			    0x20	/* 按LINUX的习惯16个硬中断号 */
    #define	IRQ8_VECTOR			    0x28    /* 重新设置为0x20到0x2F     */

    /* 一些魔数 */
    #define END_OF_INT              0x20	/* EOI，用于恢复中断信号接收 */

    #define ICW1_AT         	    0x11	/* edge triggered, cascade, need ICW4 */
    #define ICW1_PC         	    0x13	/* edge triggered, no cascade, need ICW4 */
    #define ICW1_PS         	    0x19	/* level triggered, cascade, need ICW4 */
    #define ICW4_AT_SLAVE   	    0x01	/* not SFNM, not buffered, normal EOI, 8086 */
    #define ICW4_AT_MASTER  	    0x05	/* not SFNM, not buffered, normal EOI, 8086 */
    #define ICW4_PC_SLAVE   	    0x09	/* not SFNM, buffered, normal EOI, 8086 */
    #define ICW4_PC_MASTER  	    0x0D	/* not SFNM, buffered, normal EOI, 8086 */
    #define ICW4_AT_AEOI_SLAVE	    0x03 	/* not SFNM, not buffered, auto EOI, 8086 */
    #define ICW4_AT_AEOI_MASTER	    0x07 	/* not SFNM, not buffered, auto EOI, 8086 */
    #define ICW4_PC_AEOI_SLAVE	    0x0B 	/* not SFNM, buffered, auto EOI, 8086 */
    #define ICW4_PC_AEOI_MASTER	    0x0F 	/* not SFNM, buffered, auto EOI, 8086 */

    /* CPU保留的(异常)中断号 */
    // 8086增加内容
    #define INTVEC_DIVIDE           0	    /* #DE  0除 */
    #define INTVEC_DEBUG            1	    /* #DB  单步中断 */
    #define INTVEC_NMI              2	    /* #NMI 不可屏蔽中断 */
    #define INTVEC_BREAKPOINT       3	    /* #BP  断点 */
    #define INTVEC_OVERFLOW         4	    /* #OF  溢出 */
    // 286增加内容
    #define INTVEC_BOUNDS           5       /* #BR  数组超界 */
    #define INTVEC_INVAL_OP         6       /* #UD  无效操作码 */
    #define INTVEC_COPROC_NOT       7       /* #NM  协处理器不可用 */
    #define INTVEC_DOUBLE_FAULT     8       /* #DF  双重错误 */
    #define INTVEC_COPROC_SEG       9       /*      协处理器段超界 */
    #define INTVEC_INVAL_TSS        10      /* #TS  无效的TSS */
    #define INTVEC_SEG_NOT          11      /* #NP  段不存在 */
    #define INTVEC_STACK_FAULT      12      /* #SS  栈段错误 */
    #define INTVEC_PROTECTION       13      /* #GP  常规保护 */
    // 386增加内容
    #define INTVEC_PAGE_FAULT       14      /* #PF  页错误 */
    #define INTVEC_COPROC_ERR       16      /* #MF  x87 FPU错误 */
    #define INTVEC_ALIGNMENT_CHECK  17      /* #AC  对齐检查 */
    #define INTVEC_MACHINE_CHECK    18      /* #MC  机器检查 */
    // 奔腾增加内容
    #define INTVEC_SIMD_EXCEPTION   19      /* #XM  SIMD浮点异常 */

    #define NR_IRQS                 16
    /* PC/AT的中断号 */
    #define CLOCK_IRQ               0
    #define KEYBOARD_IRQ            1
    #define CASCADE_IRQ             2	    /* cascade enable for 2nd AT controller */
    #define ETHER_IRQ               3	    /* default ethernet interrupt vector */
    #define SECONDARY_IRQ           3	    /* RS232 interrupt vector for port 2 */
    #define RS232_IRQ               4	    /* RS232 interrupt vector for port 1 */
    #define XT_WINI_IRQ             5	    /* xt winchester */
    #define FLOPPY_IRQ              6	    /* floppy disk */
    #define PRINTER_IRQ             7
    #define SPURIOUS_IRQ            7
    #define CMOS_CLOCK_IRQ          8
    #define KBD_AUX_IRQ             12	    /* AUX (PS/2 mouse) port in kbd controller */
    #define AT_WINI_0_IRQ           14	    /* at winchester controller 0 */
    #define AT_WINI_1_IRQ           15	    /* at winchester controller 1 */

    #define VECTOR(irq)    \
       (((irq) < 8 ? IRQ0_VECTOR : IRQ8_VECTOR) + ((irq) & 0x07))

//#endif /* (CHIP == INTEL) */
#endif /* _MYOS_INTERRUPT_H_ */
