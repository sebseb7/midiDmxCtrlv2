#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"


static uint16_t a;
static uint16_t col;
static uint16_t gobo;
static uint16_t rot;

static uint8_t tick(void) 
{
	a++;
	setCh(1,col);
	setCh(2,0);
	setCh(3,255);
	setCh(4,rand()%255);
	setCh(5,rand()%255);
	return 1;
}

static void init(void)
{
	a=0;
	col=rand()%255;
	rot=rand()%255;
	gobo=rand()%255;
	setCh(1,0);
	setCh(2,0);
	setCh(3,155);
	setCh(6,255);


}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,10, 7);
}


