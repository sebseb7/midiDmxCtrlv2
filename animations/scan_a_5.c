#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"


static uint16_t a;

static int32_t x;
static int32_t y;

static int32_t c_r;


//static int x_min = 100;
//static int x_max = 160;

//static int y_max = 200;
//static int y_min = 50;

//static int step = 23;


static int x_min = 0;
static int x_max = 255;

static int y_max = 255;
static int y_min = 0;

static int step = 40;

static uint8_t tick(void) 
{
	a++;


	int x_scale = x_max-x_min;
	int y_scale = y_max-y_min;


	


	if(a%2 == 0)
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
	
	
	if(c_r == 0) x+=step;
	if(c_r == 1) y+=step;
	if(c_r == 2) x-=step;
	if(c_r == 3) y-=step;

	if(x < 0){ x=(step-(x*-1))+step;c_r+=2;};
	if(y < 0){ y=(step-(y*-1))+step;c_r+=2;};
	if(x > x_scale){ x=x_scale-((step-(x-x_scale))+step);c_r+=2;};
	if(y > y_scale){ y=y_scale-((step-(y-y_scale))+step);c_r+=2;};
	if(c_r > 3){c_r = c_r-4;};

}



	setCh(4,x+x_min);
	setCh(5,y+y_min);
	return 1;
}

static void init(void)
{
	x=10;
	y=10;
	c_r=0;
	a=0;

}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,7, 5);
}


