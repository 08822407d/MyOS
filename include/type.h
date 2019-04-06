#ifndef _MyOS_TYPE_H_
#define _MyOS_TYPE_H_

/*+++++++++++++++++++++++++++++++定义一些类型++++++++++++++++++++++++++++++++++++++*/
    #define EXTERN extern

/* 函数类型 */
    #define	PUBLIC
    #define	PRIVATE	static
    
/* 数据类型 */
    #define u8_t            uint8_t
    #ifndef uint8_t
    typedef unsigned char   u8_t;
    #endif

    #define u16_t           uint16_t
    #ifndef uint16_t
    typedef unsigned short  u16_t;
    #endif

    #define u32_t           uint32_t
    #ifndef uint32_t
    typedef unsigned int    u32_t;
    #endif

    #define u64_t           uint64_t
    #ifndef uint64_t
    typedef unsigned long   u64_t;
    #endif

    typedef uint32_t        reg_t;

    typedef	void    (*idt_handler_f)();             // 中断、异常处理函数原型
    typedef	void	(*irq_handler_f)(u32_t irq);    // 中断处理函数
    typedef void    *systcall_f;                    // 系统调用
    typedef	void	(*proc_f)();                    // 进程体入口

    #define NULL    0

    #define bool            uint32_t
    #ifndef uint32_t
    typedef unsigned int    bool;
    #endif
    #define FALSE           0
    #define TRUE            1
    
#endif /* _MyOS_TYPE_H_ */