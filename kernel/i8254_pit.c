#include "const.h"
#include "archconst.h"
#include "archproto.h"

PUBLIC void init_i8254A()
{
    out_b(TIMER_MODE, RATE_GENERATOR);
    out_b(TIMER0, (u8_t)(TIMER_FREQ / HZ));
    out_b(TIMER0, (u8_t)((TIMER_FREQ / HZ) >> 8));
}