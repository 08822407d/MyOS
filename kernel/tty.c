#include "type.h"
#include "global.h"
#include "proc.h"
#include "klib.h"

PUBLIC void Task_tty()
{
	while (1)
    {
        read_kbdbuff();
        delay_loop(50);
    }
}