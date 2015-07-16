#include "main.h"
#include <stdio.h>
#include "dmx_devices.h"

static uint16_t a;

static uint8_t tick(void) 
{
	a++;



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
	registerAnimation(init,tick,deinit, PAR36,TYPE_NORMAL,40, 8);
}


