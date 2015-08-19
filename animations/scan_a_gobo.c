#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static uint8_t tick_0(void) 
{
	setCh(2,0);
	return 1;
}


static __attribute__((unused)) uint8_t tick_1(void) 
{
	setCh(2,22);
	return 1;
}

static __attribute__((unused)) uint8_t tick_2(void) 
{
	setCh(2,44);
	return 1;
}

static __attribute__((unused)) uint8_t tick_3(void) 
{
	setCh(2,60);
	return 1;
}

static __attribute__((unused)) uint8_t tick_4(void) 
{
	setCh(2,80);
	return 1;
}
static __attribute__((unused)) uint8_t tick_5(void) 
{
	setCh(2,100);
	return 1;
}
static __attribute__((unused)) uint8_t tick_6(void) 
{
	setCh(2,110);
	return 1;
}
static __attribute__((unused)) uint8_t tick_7(void) 
{
	setCh(2,134);
	return 1;
}
static __attribute__((unused)) uint8_t tick_8(void) 
{
	setCh(2,148);
	return 1;
}
static __attribute__((unused)) uint8_t tick_9(void) 
{
	setCh(2,174);
	return 1;
}
static __attribute__((unused)) uint8_t tick_10(void) 
{
	setCh(2,186);
	return 1;
}

static __attribute__((unused)) uint8_t tick_11(void) 
{
	setCh(2,214);
	return 1;
}
static __attribute__((unused)) uint8_t tick_12(void) 
{
	setCh(2,226);
	return 1;
}
/*static uint8_t tick_13(void) 
{
	setCh(2,242);
	return 1;
}*/
static __attribute__((unused)) uint8_t tick_14(void) 
{
	setCh(2,254);
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
	registerAnimation(init,tick_0,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4,"open");
/*	registerAnimation(init,tick_1,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_2,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_3,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_4,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_5,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_6,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_7,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_8,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_9,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_10,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_11,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_12,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
//	registerAnimation(init,tick_13,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_14,deinit, SCAN_2_GOBO,TYPE_NORMAL,10, 4);
*/	queueInitialization(SCAN_2_GOBO,1,1,0,"scan250_gobo");
}


