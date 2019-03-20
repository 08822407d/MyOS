#include "global.h"
#include "const.h"
#include "archproto.h"
#include "klib.h"
#include "syscall.h"

#define DELAYSCALE_MS	5000
#define DELAYSCALE		4

void TestA()
{
	int i = 0x0;
	while(1){
		
		disp_color_str("ticks:#", RED);
		disp_int(get_ticks());
		disp_color_str("# ", RED);
		delay_loop(DELAYSCALE);
	}
}

void TestB()
{
	int i = 0x1000;
	while(1){
		disp_color_str("B", GREEN);
		disp_int(i++);
		disp_color_str("...", GREEN);
		delay_loop(DELAYSCALE);
	}
}

void TestC()
{
	int i = 0x2000;
	while(1){
		disp_str("C");
		disp_int(i++);
		disp_str("...");
		delay_loop(DELAYSCALE);
	}
}
