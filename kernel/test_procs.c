#include "archproto.h"
#include "const.h"
#include "global.h"
#include "klib.h"
#include "syscall.h"
#include "proto.h"

#define DELAYSCALE_MS 5000
#define DELAYSCALE 1000

void TestA()
{
	int i = 0x0;
	while (1)
	{

		printf("<Ticks,%x>", get_ticks());
		delay_loop(DELAYSCALE);
		//delay_ms(1000);
	}
}

void TestB()
{
	int i = 0x1000;
	while (1)
	{
		printf("..B..");
		delay_loop(DELAYSCALE);
		//delay_ms(5000);
	}
}

void TestC()
{
	int i = 0x2000;
	while (1)
	{
		printf("..C..");
		delay_loop(DELAYSCALE);
		//delay_ms(5000);
	}
}
