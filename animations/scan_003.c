#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"


uint16_t a;

static uint8_t tick(void) 
{
	a++;
	setCh(27,100);
	setCh(28,160);
	setCh(29,rand()%255);
	setCh(30,rand()%255);
	return 1;
}

static void init(void)
{
	a=0;
	setCh(25,0);
	setCh(26,0);
	setCh(27,0);
	setCh(28,0);
	setCh(29,0);
	setCh(30,0);
	setCh(31,0);


}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, QUADPHASE,TYPE_NORMAL,10, 5);
}


