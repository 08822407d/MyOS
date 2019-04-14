#ifndef _MYOS_PROTO_H_
#define _MYOS_PROTO_H_

    #include "type.h"
    #include "tty.h"
    
    void spurious_irq(u32_t irq);

    void in_process(TTY_t* p_tty, u32_t key);
    
#endif /* _MYOS_PROTO_H_ */