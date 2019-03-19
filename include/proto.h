#ifndef _PROTO_H_
#define _PROTO_H_

    #include "type.h"
    
    void spurious_irq(u32_t irq);
    void put_irq_handler(int irq, irq_handler handler);
    
#endif