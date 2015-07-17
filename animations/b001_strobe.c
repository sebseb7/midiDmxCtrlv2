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
	queueAniActive(CHASE,0,0);
	a=0;
}


static void deinit(void)
{
	queueAniActive(CHASE,0,1);
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, PAR36,TYPE_NORMAL,40, 8);
	queueInitialization(PAR36,0,0);
}


