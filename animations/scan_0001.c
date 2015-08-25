#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"
#include "et_devices.h"
#include "sini.h"

#include <stdlib.h>


static uint16_t a;
static uint16_t col;
static uint16_t gobo;

static uint8_t tick(void) 
{
	a+=100;
	setallscan(1,col);
	setallscan(3,gobo);
	setallscan(4,255);
	setallscan(5,64+(sini(a*3)>>9));
	setallscan(6,64+(sini(a+4192)>>9));



	return 1;
}

static void init(void)
{
	col=rand()%255;
	gobo=rand()%255;
	a=0;
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
	registerAnimation(init,tick,deinit, SCAN,TYPE_NORMAL,50, 10,"circ2");
}


