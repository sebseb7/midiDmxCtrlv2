#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"


static uint16_t a;

static uint8_t tick(void) 
{
	a++;
	setCh(4,rand()%255);
	setCh(5,rand()%255);
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
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,20, 6);
}


