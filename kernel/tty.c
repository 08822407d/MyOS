#include "global.h"
#include "klib.h"
#include "proc.h"
#include "type.h"
#include "tty.h"

void tty_do_read();
void tty_do_write();

PUBLIC void Task_tty()
{
    while (1)
    {
        parse_scancode();

        delay_loop(50);
    }
}

void tty_do_read()
{

}

void tty_do_write()
{

}
