#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint8_t tick(void) 
{
	setCh(25,0);//color red 16
	return 1;
}

static void init(void)
{
}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_COL,TYPE_NORMAL,10, 10);
	queueInitialization(SCAN_COL,0,1,1);
}


