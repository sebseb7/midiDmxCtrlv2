#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint8_t tick_0(void) 
{
	setCh(27,0);
	return 1;
}


static uint8_t tick_2(void) 
{
	setCh(27,80);
	return 1;
}

static uint8_t tick_3(void) 
{
	setCh(27,120);
	return 1;
}

static uint8_t tick_4(void) 
{
	setCh(27,150);
	return 1;
}

static uint8_t tick_5(void) 
{
	setCh(27,190);
	return 1;
}
static uint8_t tick_6(void) 
{
	setCh(27,230);
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
	registerAnimation(init,tick_0,deinit, SCAN_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_2,deinit, SCAN_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_3,deinit, SCAN_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_4,deinit, SCAN_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_5,deinit, SCAN_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_6,deinit, SCAN_GOBO,TYPE_NORMAL,10, 4);
	queueInitialization(SCAN_GOBO,0,1,1);
}


