#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"
#include "scan_limits.h"


static uint16_t a;

static uint8_t tick(void) 
{
	a++;
	
	
	//setCh(4,50+rand()%125);
	//setCh(5,100+rand()%70);
	setCh(4,0+rand()%255);
	setCh(5,SCAN_MIN_X+rand()%(255-(SCAN_MAX_X-SCAN_MIN_X)));
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


