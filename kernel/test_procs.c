#include "global.h"
#include "archproto.h"
#include "klib.h"

#define DELAY_SCALE 4

void TestA()
{
	int i = 0x0;
	while(1){
		disp_str("A");
		disp_int(i++);
		disp_str("...");
		delay(DELAY_SCALE);
	}
}

void TestB()
{
	int i = 0x1000;
	while(1){
		disp_str("B");
		disp_int(i++);
		disp_str("...");
		delay(DELAY_SCALE);
	}
}

void TestC()
{
	int i = 0x2000;
	while(1){
		disp_str("C");
		disp_int(i++);
		disp_str("...");
		delay(DELAY_SCALE);
	}
}
