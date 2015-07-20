#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint16_t a;

static uint8_t tick(void) 
{
	a++;
	setCh(2,0);
	setCh(3,255);
	setCh(4,32+random(120));
	setCh(5,120+random(60));
	return 1;
}

static void init(void)
{
	a=0;
	setCh(1,0);
	setCh(2,0);
	setCh(6,255);


}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,10,9);
}


