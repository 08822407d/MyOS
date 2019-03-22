#include "global.h"
#include "klib.h"
#include "proc.h"
#include "type.h"

PUBLIC void Task_tty()
{
    while (1)
    {
        parse_scancode();
        delay_loop(50);
    }
}