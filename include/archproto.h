#ifndef _MYOS_ARCHPROTO_H_
#define _MYOS_ARCHPROTO_H_

    #include "type.h"
    #include "archtypes.h"

/* --------------------------打包一些汇编指令----------------------------- */
    extern  u8_t    in_b(u16_t  port);
    extern  void    out_b(u16_t port, u8_t    value);
    extern  u16_t   in_w(u16_t  port);
    extern  void    out_w(u16_t port, u16_t   value);

    extern  void    x86_lgdt(DESCTAB_PTR_t*);
    extern  void    x86_lidt(DESCTAB_PTR_t*);
    extern  void    x86_lltr(u16_t  sele_ldt);
    extern  void    x86_ltr(u16_t   sele_tss);

    extern  void    disable_intr(void);
    extern  void    enable_intr(void);
    
/* --------------------------中断和异常句柄声明----------------------------- */
    extern	void	divide_error();
    extern	void	single_step_exception();
    extern	void	nmi();
    extern	void	breakpoint_exception();
    extern	void	overflow();
    extern	void	bounds_check();
    extern	void	inval_opcode();
    extern	void	copr_not_available();
    extern	void	double_fault();
    extern	void	copr_seg_overrun();
    extern	void	inval_tss();
    extern	void	segment_not_present();
    extern	void	stack_exception();
    extern	void	general_protection();
    extern	void	page_fault();
    extern	void	copr_error();
    extern	void	alignment_check();
    extern	void	machine_check();
    extern	void	simd_exception();

    extern	void    hwint00();
    extern	void    hwint01();
    extern	void    hwint02();
    extern	void    hwint03();
    extern	void    hwint04();
    extern	void    hwint05();
    extern	void    hwint06();
    extern	void    hwint07();
    extern	void    hwint08();
    extern	void    hwint09();
    extern	void    hwint10();
    extern	void    hwint11();
    extern	void    hwint12();
    extern	void    hwint13();
    extern	void    hwint14();
    extern	void    hwint15();

/* --------------------------iintel 8259A----------------------------- */
    void init_i8259A(void);
    void irq_i8259A_unmask(const u32_t irq);
    void irq_i8259A_mask(const u32_t irq);
    void irq_i8259A_eoi(u32_t);
    void i8259A_disable(void);
    void irq_i8259A_disable(u32_t irq);
    
/* --------------------------X86架构相关的操作----------------------------- */
    void init_segdesc(SEG_DESC_t* seg_dp, u32_t base, u32_t size, u32_t attr);
    void init_param_dataseg(SEG_DESC_t* seg_dp, u32_t base, u32_t size, u32_t privilege);
    void init_codeseg(u32_t index, u32_t privilege);
    void init_dataseg(u32_t index, u32_t privilege);
    void init_dummyseg(u32_t);
    void tss_init(u8_t *);

    void init_sysseg(u32_t index, u32_t base,u32_t size);
    void init_tssdesc(u32_t index, u32_t base);
    void init_ldtdesc(u32_t index, u32_t base);

    void set_int_gate_idt(u32_t, u32_t, u32_t);
    void idt_veccpy_exception(void);
    void idt_veccpy_pic(void);

    void process_init(void);

/* --------------------------IRQ_HANDLER----------------------------- */
    void init_irqhandler_table(void);
    void put_irq_handler(int irq, irq_handler_f handler);

/* --------------------------TIME----------------------------- */
    void init_clock(void);
    void calc_whileloop_per_ms(void);

/* --------------------------TIME----------------------------- */
    void init_keyboard(void);

    void* va2la(int pid, void* va);

#endif