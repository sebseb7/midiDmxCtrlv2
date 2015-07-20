#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint16_t a;

static int32_t x;
static int32_t y;

static uint8_t tick(void) 
{
	a++;

	x+=(random()%80)-50;
	y+=(random()%80)-30;

	if(x > 255)
		x = 255-(255-x);
	if(y > 255)
		y = 255-(255-y);
	if(x < 0)
	{
		x *= -1;
	}
	if(y < 0)
	{
		y *= -1;
	}

	setCh(4,x);
	setCh(5,y);
	return 1;
}

static void init(void)
{
	x=128;
	y=128;
	a=0;
	setCh(1,0);
	setCh(2,0);
	setCh(3,255);
	setCh(6,255);


}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,30, 5);
}


