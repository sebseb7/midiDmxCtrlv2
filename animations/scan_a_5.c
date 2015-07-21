#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"


static uint16_t a;

static int32_t x;
static int32_t y;

static int32_t c_r;


static uint8_t tick(void) 
{
	a++;


	if(a%5 == 0)
	{
		int r = rand()%2;

		if(r==0)
		{
			c_r--;
		}
		else
		{
			c_r++;
		}
		if(c_r > 3) c_r=0;
		if(c_r < 0) c_r=3;
	}


	if(c_r == 0) x+=10;
	if(c_r == 1) y+=10;
	if(c_r == 2) x-=10;
	if(c_r == 3) y-=10;

	if(x < 0){ x=x*-1;c_r+=2;};
	if(y < 0){ y=y*-1;c_r+=2;};
	if(x > 255){ x=255-(x-255);c_r+=2;};
	if(y > 255){ y=255-(y-255);c_r+=2;};
	if(c_r > 3){c_r = c_r-4;};

	setCh(4,x);
	setCh(5,y);
	return 1;
}

static void init(void)
{
	x=128;
	y=128;
	c_r=0;
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
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,10, 5);
}


