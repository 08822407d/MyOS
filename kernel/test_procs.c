#include "global.h"
#include "const.h"
#include "archproto.h"
#include "klib.h"
#include "syscall.h"

#define DELAYSCALE_MS 1000

void TestA()
{
	int i = 0x0;
	while(1){
		
		disp_color_str("ticks:#", GREEN);
		disp_int(get_ticks());
		disp_color_str("# ", GREEN);
		delay_ms(DELAYSCALE_MS);
	}
}

void TestB()
{
	int i = 0x1000;
	while(1){
		disp_str("B");
		disp_int(i++);
		disp_str("...");
		delay_ms(DELAYSCALE_MS);
	}
}

void TestC()
{
	int i = 0x2000;
	while(1){
		disp_str("C");
		disp_int(i++);
		disp_str("...");
		delay_ms(DELAYSCALE_MS);
	}
}
