#ifndef _MYOS_PROTO_H_
#define _MYOS_PROTO_H_

    #include "type.h"
    #include "tty.h"
    
    void spurious_irq(u32_t irq);

    void init_process(void);

    void in_process(TTY_t* tty_ptr, u32_t key);
    void out_char(CONSOLE_t* console_ptr, char ch);
    void set_cursor(u32_t cursor_pos);
    bool is_current_console(CONSOLE_t* console_ptr);
    void tty_write(TTY_t* tty_ptr, char* buf, int lenght);

    /* printf.c */
    int printf(const char *fmt, ...);
    /* vsprintf.c */
    int vsprintf(char *buf, const char *fmt, va_list args);
    
#endif /* _MYOS_PROTO_H_ */