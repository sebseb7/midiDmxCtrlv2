#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dmx_devices.h"



static uint8_t tick(void) 
{
	int x =getFader(5);
	int y =getFader(6);


	setCh(4,x);
	setCh(5,y);
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
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,5, 5);
}


