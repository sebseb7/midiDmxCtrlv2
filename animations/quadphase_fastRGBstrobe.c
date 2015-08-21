#include "main.h"
#include <stdio.h>

// ch1 : 1-16 red
// 		 17-33 green
// 		 34-50 blue


// ch2 : 0-9 no rot
//       10-120 clockwise
//       121-134 no
//       135-245 counter
//       246-249 no 
//       250-255 music

uint16_t a;
uint16_t b;

static uint8_t tick(void) 
{
	b++;

	if(b == 30)
	{
		setCh(11,1);
		setCh(14,255);
		setCh(15,1);
		setCh(18,255);
	}
	if(b == 32)
	{
		setCh(14,0);
		setCh(18,0);
	}
	if(b == 60)
	{
		setCh(11,18);
		setCh(14,255);
		setCh(15,18);
		setCh(18,255);
	}
	if(b == 62)
	{
		setCh(14,0);
		setCh(18,0);
	}
	if(b == 90)
	{
		setCh(11,35);
		setCh(14,255);
		setCh(15,35);
		setCh(18,255);
	}
	if(b == 92)
	{
		setCh(14,0);
		setCh(18,0);
		b=0;
	}

	setCh(12,60);
	setCh(13,0);
	setCh(16,60);
	setCh(17,0);

	return 1;
}

static void init(void)
{
	a=0;
	b=0;
}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, QUADPHASE, TYPE_NORMAL,80,2,"RGBStrobe");
	queueInitialization(QUADPHASE,1,0,0,"Quadphase");
	queuePageSet(QUADPHASE,1,1);
}


