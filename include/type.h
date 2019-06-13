#ifndef _MyOS_TYPE_H_
#define _MyOS_TYPE_H_

/*+++++++++++++++++++++++++++++++定义一些类型++++++++++++++++++++++++++++++++++++++*/
/* 符号类型 */
    #define EXTERN extern
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

    typedef	char *			va_list;

    typedef	void    (*idt_handler_f)();             // 中断、异常处理函数原型
    typedef	void	(*irq_handler_f)(u32_t irq);    // 中断处理函数
    typedef void    *systcall_f;                    // 系统调用
    typedef	void	(*proc_f)();                    // 进程体入口

    /* the assert macro */
    #define ASSERT
    #ifdef ASSERT
    void assertion_failure(char *exp, char *file, char *base_file, int line);
    #define assert(exp)  if (exp) ; \
        else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
    #else
    #define assert(exp)
    #endif

    /**
    * MESSAGE mechanism is borrowed from MINIX
    */
    struct mess1
    {
        int m1i1;
        int m1i2;
        int m1i3;
        int m1i4;
    };
    struct mess2
    {
        void *m2p1;
        void *m2p2;
        void *m2p3;
        void *m2p4;
    };
    struct mess3
    {
        int m3i1;
        int m3i2;
        int m3i3;
        int m3i4;
        u64_t m3l1;
        u64_t m3l2;
        void *m3p1;
        void *m3p2;
    };
    typedef struct
    {
        int source;
        int type;
        union {
            struct mess1 m1;
            struct mess2 m2;
            struct mess3 m3;
        } u;
    } MESSAGE_t;

    #define NULL 0

    #define bool uint32_t
    #ifndef uint32_t
    typedef unsigned int    bool;
    #endif
    #define FALSE           0
    #define TRUE            1
    
#endif /* _MyOS_TYPE_H_ */