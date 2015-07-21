#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"
#include "sini.h"


uint16_t a;
uint16_t col;
uint16_t gobo;

static uint8_t tick(void) 
{
	a+=80;
	setCh(25,0);//color red 16
	setCh(26,160);
	setCh(27,148);//gobo
	if((a%6000)<2000)
	{
		setCh(28,255);
	}
	else
	{
		setCh(28,0);
	}
	if((a%6000)<1500)
	{
		setCh(29,64+(sini(a*3)>>9));
		setCh(30,64+(sini(a+4192)>>9));
	}



	return 1;
}

static void init(void)
{
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
//	queueInitialization(QUADPHASE,0,1,1);
}


