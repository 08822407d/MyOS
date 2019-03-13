/* klib对外接口 */

#ifndef _KLIB_H_
#define _KLIB_H_

    #include "type.h"

    EXTERN void disp_str(char * str);
    EXTERN void disp_color_str(char * str, u16_t color);
    EXTERN char * itoa(char * str, int num);
    EXTERN void disp_int(int input);
    PUBLIC void delay(int scale);

#endif