#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dmx_devices.h"

static int length = 2;
static int idx = 1;
static int a = 0;
static int list[] = 
{
	128,80,
	84,54,
};

static uint8_t tick(void) 
{
	a++;

	if(a%10 == 0)
		idx++;
	if(idx > length) 
		idx=1;


	setCh(4,list[(idx-1)*2]);
	setCh(5,list[(idx-1)*2+1]);
	return 1;
}

static void init(void)
{
	a=0;
	idx=1;
}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,5, 5);
}


