#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint8_t tick_0(void) 
{
	setCh(3,255);
	return 1;
}


static __attribute__((unused)) uint8_t tick_1(void) 
{
	setCh(3,240);
	return 1;
}

static __attribute__((unused)) uint8_t tick_3(void) 
{
	setCh(3,200);
	return 1;
}

static uint8_t tick_5(void) 
{
	setCh(3,150);
	return 1;
}
static __attribute__((unused)) uint8_t tick_6(void) 
{
	setCh(3,100);
	return 1;
}
static __attribute__((unused)) uint8_t tick_7(void)
{
	return 1;
}

static void init(void)
{
	setCh(6,255);
}


static void deinit(void)
{
	setCh(3,0);
	setCh(6,0);
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick_0,deinit, SCAN_2_STROBE,TYPE_NORMAL,10, 14,"on");
//	registerAnimation(init,tick_1,deinit, SCAN_2_STROBE,TYPE_NORMAL,10, 4);
//	registerAnimation(init,tick_3,deinit, SCAN_2_STROBE,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_5,deinit, SCAN_2_STROBE,TYPE_NORMAL,10, 4,"fast");
//	registerAnimation(init,tick_6,deinit, SCAN_2_STROBE,TYPE_NORMAL,10, 4);
//	registerAnimation(init,tick_7,deinit, SCAN_2_STROBE,TYPE_NORMAL,10, 4);
	queueInitialization(SCAN_2_STROBE,0,1,0,"scan250_strob");
	queuePageSet(SCAN_2_STROBE,0,1);
}


