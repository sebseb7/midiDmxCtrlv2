#include "main.h"
#include <stdio.h>
#include "dmx_devices.h"

static uint8_t tick(void) 
{
	set_quadphase(ADDR_QF1, 0,0,0,0);
	set_par56(ADDR_LED1, 0,0,0);


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
	registerAnimation(init,tick,deinit, QUADPHASE,TYPE_OFF,0, 0);
}


