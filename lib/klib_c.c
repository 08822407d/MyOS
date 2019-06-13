#include "const.h"
#include "global.h"
#include "klib.h"
#include "string.h"
#include "type.h"

/*======================================================================*
                        	简易数转字符串 itoa
 *======================================================================*/
/* 转换为16进制的字符串表示，带0x头 */
PUBLIC char *itoa(char *str, int num)
{
	char *p = str;
	char ch;
	int i;
	int flag = 0;

	*p++ = '0';
	*p++ = 'x';

	if (num == 0)
	{
		*p++ = '0';
	}
	else
	{
		for (i = 28; i >= 0; i -= 4)
		{
			ch = (num >> i) & 0xF;
			if (flag || (ch > 0))
			{
				flag = 1;
				ch += '0';
				if (ch > '9')
				{
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}

/*======================================================================*
                        	显示数字 disp_int
 *======================================================================*/
/* 调用itoa的显示字符串，功能取决于itoa是否强大 */
PUBLIC void disp_int(int input)
{
	char output[16];
	itoa(output, input);
	disp_str(output);
}

PUBLIC void delay_ms(int milli_second)
{
	unsigned int t = get_ticks();

	while (((get_ticks() - t) * 1000 / HZ ) < milli_second)
	{}
}

PUBLIC void delay_loop(int scale)
{
	for (int i = 0; i < scale; i++)
	{
		for (int j = 0; j < 1000; j++)
		{}
	}
}

/*======================================================================*
                        	vsprintf()
 *======================================================================*/
int vsprintf(char *buf, const char *fmt, va_list args)
{
	char*	p;
	char	tmp[256];
	va_list	p_next_arg = args;

	for (p=buf;*fmt;fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}

		fmt++;

		switch (*fmt) {
		case 'x':
			itoa(tmp, *((int*)p_next_arg));
			strcpy(p, tmp);
			p_next_arg += 4;
			p += strlen(tmp);
			break;
		case 's':
			break;
		default:
			break;
		}
	}

	return (p - buf);
}

/*======================================================================*
                                 sprintf
 *======================================================================*/
int sprintf(char *buf, const char *fmt, ...)
{
	va_list arg = (va_list)((char*)(&fmt) + 4);        /* 4 是参数 fmt 所占堆栈中的大小 */
	return vsprintf(buf, fmt, arg);
}

/*======================================================================*
                                 printf
 *======================================================================*/
PUBLIC int printf(const char *fmt, ...)
{
	int i;
	char buf[256];

	va_list arg = (va_list)((char*)(&fmt) + 4); /*4是参数fmt所占堆栈中的大小*/
	i = vsprintf(buf, fmt, arg);
	printx(buf);

	return i;
}
