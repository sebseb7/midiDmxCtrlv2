#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"


static uint16_t a;

static int32_t x;
static int32_t y;

static uint8_t tick(void) 
{
	a++;


	if(a%4==0)
	{
		x=0;
		y=rand()%255;
	}
	else if(a%4==2)
	{
		x=255;
		y=rand()%266;
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


}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,5, 10);
	queueInitialization(SCAN_2,1,1,1);
}


