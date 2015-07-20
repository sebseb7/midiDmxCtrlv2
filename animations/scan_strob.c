#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint8_t tick_0(void) 
{
	setCh(28,255);
	return 1;
}

static uint8_t tick_1(void) 
{
	setCh(28,220);
	return 1;
}

static uint8_t tick_2(void) 
{
	setCh(28,200);
	return 1;
}

static uint8_t tick_3(void) 
{
	setCh(28,180);
	return 1;
}

static uint8_t tick_4(void) 
{
	setCh(28,140);
	return 1;
}

static uint8_t tick_5(void) 
{
	setCh(28,0);
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
	registerAnimation(init,tick_0,deinit, SCAN_STR,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_1,deinit, SCAN_STR,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_2,deinit, SCAN_STR,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_3,deinit, SCAN_STR,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_4,deinit, SCAN_STR,TYPE_NORMAL,10, 3);
	registerAnimation(init,tick_5,deinit, SCAN_STR,TYPE_NORMAL,10, 3);
	queueInitialization(SCAN_STR,0,1,1);
}


