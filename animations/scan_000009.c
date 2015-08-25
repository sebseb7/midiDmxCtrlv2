#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"
#include "sini.h"
#include "et_devices.h"



static uint16_t a;
static uint16_t b;
static uint16_t c;
static uint16_t d;

static uint8_t tick(void) 
{
	a+=80;
	b+=85;
	c+=93;
	d+=110;

	setallscan(1,0);//color red 16
	setallscan(2,160);
	setallscan(3,148);//gobo
	if((a%6000)<2000)
	{
		setscan(1,4,255);
	}
	else
	{
		setscan(1,4,0);
	}
	if((b%6000)<2000)
	{
		setscan(1,4,255);
	}
	else
	{
		setscan(1,4,0);
	}
	if((c%6000)<2000)
	{
		setscan(2,4,255);
	}
	else
	{
		setscan(2,4,0);
	}
	if((d%6000)<2000)
	{
		setscan(3,4,255);
	}
	else
	{
		setscan(3,4,0);
	}
	if((a%6000)<1500)
	{
		setscan(0,5,64+(sini(a*3)>>9));
		setscan(0,6,64+(sini(a+4192)>>9));
	}
	if((b%6000)<1500)
	{
		setscan(1,5,64+(sini(b*3)>>9));
		setscan(1,6,64+(sini(b+4192)>>9));
	}
	if((c%6000)<1500)
	{
		setscan(2,5,64+(sini(c*3)>>9));
		setscan(2,6,64+(sini(c+4192)>>9));
	}
	if((d%6000)<1500)
	{
		setscan(3,5,64+(sini(d*3)>>9));
		setscan(3,6,64+(sini(d+4192)>>9));
	}



	return 1;
}

static void init(void)
{
	a=0;
	b=0;
	d=0;
	c=0;
	setallscan(1,0);
	setallscan(2,0);
	setallscan(3,0);
	setallscan(4,0);
	setallscan(5,0);
	setallscan(6,0);
	setallscan(7,0);

}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN,TYPE_NORMAL,50, 20,"ecke");
}


