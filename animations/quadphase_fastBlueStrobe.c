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

static uint8_t tick(void) 
{
	a++;

	//printf("%i\n",a);
	if(((a % 20) == 0)||((a % 20) == 10))
	{
		setCh(14,255);
		setCh(18,255);
	}
	if(((a % 20) == 1)||((a % 20) == 11))
	{
		setCh(14,0);
		setCh(18,0);
	}

	setCh(11,35);
	setCh(12,20);
	setCh(13,0);

	setCh(15,35);
	setCh(16,20);
	setCh(17,0);


	return 1;
}

static void init(void)
{
	a=0;
}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, QUADPHASE,TYPE_NORMAL,60,2,"redBlue");
}


