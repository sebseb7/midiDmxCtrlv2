#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


static __attribute__((unused)) uint8_t tick_0(void) 
{
	setCh(1,0);
	return 1;
}


/*static __attribute__((unused)) uint8_t tick_1(void) 
{
	setCh(1,22);
	return 1;
}
*/
static __attribute__((unused)) uint8_t tick_2(void) 
{
	setCh(1,44);
	return 1;
}

static __attribute__((unused)) uint8_t tick_3(void) 
{
	setCh(1,60);
	return 1;
}

static __attribute__((unused)) uint8_t tick_4(void) 
{
	setCh(1,80);
	return 1;
}
static __attribute__((unused)) uint8_t tick_5(void) 
{
	setCh(1,100);
	return 1;
}
static __attribute__((unused)) uint8_t tick_6(void) 
{
	setCh(1,110);
	return 1;
}
static __attribute__((unused)) uint8_t tick_7(void) 
{
	setCh(1,134);
	return 1;
}
static __attribute__((unused)) uint8_t tick_8(void) 
{
	setCh(1,148);
	return 1;
}
static __attribute__((unused)) uint8_t tick_9(void) 
{
	setCh(1,174);
	return 1;
}
static __attribute__((unused)) uint8_t tick_10(void) 
{
	setCh(1,186);
	return 1;
}

static __attribute__((unused)) uint8_t tick_11(void) 
{
	setCh(1,214);
	return 1;
}
static __attribute__((unused)) uint8_t tick_12(void) 
{
	setCh(1,226);
	return 1;
}
static __attribute__((unused)) uint8_t tick_13(void) 
{
	setCh(1,242);
	return 1;
}
static __attribute__((unused)) uint8_t tick_14(void) 
{
	setCh(1,254);
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
	registerAnimation(init,tick_0,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4,"weiss");
//	registerAnimation(init,tick_1,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_2,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4,"gelb");
//	registerAnimation(init,tick_3,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
//	registerAnimation(init,tick_4,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_5,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4,"orange");
	registerAnimation(init,tick_6,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4,"cyan");
	registerAnimation(init,tick_7,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4,"rose");
//	registerAnimation(init,tick_8,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
//	registerAnimation(init,tick_9,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
	registerAnimation(init,tick_10,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4,"rot");
//	registerAnimation(init,tick_11,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4,"col7");
//	registerAnimation(init,tick_12,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
//	registerAnimation(init,tick_13,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
//	registerAnimation(init,tick_14,deinit, SCAN_2_COL,TYPE_NORMAL,10, 4);
	queueInitialization(SCAN_2_COL,1,1,0,"scan250_col");
	queuePageSet(SCAN_2_COL,0,1);
}


