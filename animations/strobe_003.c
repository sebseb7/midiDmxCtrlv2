#include "main.h"
#include <stdio.h>

#include "dmx_devices.h"
#include "et_devices.h"

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
		
	if(a%2)
	{
		setCh(ADDR_STROBE_A,a%255);
		setCh(ADDR_STROBE_B,a%255);
	}
	else
	{
		setCh(ADDR_STROBE_A,0);
		setCh(ADDR_STROBE_B,0);
	}
	return 1;
}

static void init(void)
{
	a=0;
	setCh(ADDR_STROBE_A+1,255);
	setCh(ADDR_STROBE_B+1,255);
}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, STROBE,TYPE_NORMAL,40, 5,"normal");
}


