#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"
#include "sini.h"



uint16_t a;

static uint8_t tick_0(void) 
{
	a+=80;
	setCh(29,64+(sini(a*2)>>9));
	setCh(30,64+(sini(a+4192)>>9));
	return 1;
}

static uint8_t tick_1(void) 
{
	a+=800;
	setCh(29,64+(sini(a)>>11));
	setCh(30,64+(sini(a+4192)>>11));
	return 1;
}

static uint8_t tick_2(void) 
{
	a+=300;
	setCh(29,64+(sini(a*3)>>9));
	setCh(30,64+(sini(a+4192)>>9));
	return 1;
}

static uint8_t tick_3(void) 
{
	setCh(29,rand()%255);
	setCh(30,rand()%255);
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
	registerAnimation(init,tick_0,deinit, SCAN_MOVE,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_1,deinit, SCAN_MOVE,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_2,deinit, SCAN_MOVE,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_3,deinit, SCAN_MOVE,TYPE_NORMAL,10, 3);
	queueInitialization(SCAN_MOVE,0,1,1);
}


