#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"


uint16_t a;
uint16_t col;
uint16_t gobo;

static uint8_t tick(void) 
{
	a+=100;
	setCh(25,col);
	setCh(27,gobo);
	setCh(28,255);
	setCh(29,64+(sini(a*3)>>9));
	setCh(30,64+(sini(a+4192)>>9));



	return 1;
}

static void init(void)
{
	col=random(255);
	gobo=random(255);
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
	registerAnimation(init,tick,deinit, QUADPHASE,TYPE_NORMAL,50, 10);
}


