/* klib对外接口 */

#ifndef _MYOS_KLIB_H_
#define _MYOS_KLIB_H_

    #include "type.h"

    EXTERN void disp_str(char * str);
    EXTERN void disp_color_str(char * str, u16_t color);
    EXTERN char * itoa(char * str, int num);
    EXTERN void disp_int(int input);
    
    PUBLIC void delay_loop(int scale);
    PUBLIC void delay_ms(int milli_second);
    
#endif /* _MYOS_KLIB_H_ */