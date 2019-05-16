#include "archproto.h"
#include "const.h"
#include "global.h"
#include "klib.h"
#include "syscall.h"

#define DELAYSCALE_MS 5000
#define DELAYSCALE 1000

void TestA()
{
	int i = 0x0;
	while (1)
	{

		//disp_color_str("A", RED);
		//disp_str(".");
		//delay_loop(DELAYSCALE);
	}
}

void TestB()
{
	int i = 0x1000;
	while (1)
	{
		//disp_color_str("B", GREEN);
		//disp_str(".");
		//delay_loop(DELAYSCALE);
	}
}

void TestC()
{
	int i = 0x2000;
	while (1)
	{
		//disp_color_str("C", BLUE);
		//disp_str(".");
		//delay_loop(DELAYSCALE);
	}
}
