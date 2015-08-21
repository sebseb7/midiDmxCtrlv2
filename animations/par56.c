#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint8_t tick_0(void) 
{
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
	registerAnimation(init,tick_0,deinit, PAR56,TYPE_NORMAL,10, 14,"x");
	queueInitialization(PAR56,0,1,1,"PAR56");
	queuePageSet(PAR56,0,1);
}


