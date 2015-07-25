#include "main.h"
#include <stdio.h>

#include "dmx_devices.h"
#include "sini.h"

static uint16_t a;

static uint8_t tick(void) 
{
	a+=200;

//	float b = getFader(0)/256.0f;
//	float c = getFader(1)/256.0f;
	float b = 0.273f;
	float c = 0.263f;

	setCh(4,96+(sini(a)/(3000.0f*b)));
	setCh(5,96+(sini(a+4192)/(3000.0f*c)));
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
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,20, 7);
}


