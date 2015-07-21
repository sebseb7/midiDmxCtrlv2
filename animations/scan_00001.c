#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#include "dmx_devices.h"
#include "sini.h"


uint16_t a;
uint16_t col;
uint16_t gobo;
uint16_t rot;

static uint8_t tick(void) 
{
	a+=800;
	setCh(25,col);
	setCh(26,rot);
	setCh(27,gobo);
	setCh(28,255);
	setCh(29,64+(sini(a)>>11));
	setCh(30,64+(sini(a+4192)>>11));



	return 1;
}

static void init(void)
{
	col=rand()&255;
	rot=rand()&255;
	gobo=rand()&255;
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
	registerAnimation(init,tick,deinit, QUADPHASE,TYPE_NORMAL,20, 10);
}


