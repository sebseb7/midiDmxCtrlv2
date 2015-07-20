#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint16_t a;

static uint8_t tick(void) 
{
	a++;
	setCh(4,random(255));
	setCh(5,random(255));
	return 1;
}

static void init(void)
{
	a=0;
	setCh(1,0);
	setCh(2,0);
	setCh(3,155);
	setCh(6,255);


}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,20, 6);
}


