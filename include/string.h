/* string的对外接口 */

#ifndef _MYOS_STRING_H_
#define _MYOS_STRING_H_

    #include "type.h"

    EXTERN void memcpy(void * dest, void * src, u32_t size);
    EXTERN void memset(void * dest, char content, u32_t size);
    EXTERN char* strcpy(char* p_dst, char* p_src);

#endif /* _MYOS_STRING_H_ */