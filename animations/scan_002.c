#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"
#include "et_devices.h"

#include <stdlib.h>


static uint16_t a;

static uint8_t tick(void) 
{
	a++;
	setallscan(1,28);
	setallscan(3,126);
	setallscan(4,224);
	setscan(0,5,rand()%255);
	setscan(0,6,rand()%255);
	setscan(1,5,rand()%255);
	setscan(1,6,rand()%255);
	setscan(2,5,rand()%255);
	setscan(2,6,rand()%255);
	setscan(3,5,rand()%255);
	setscan(3,6,rand()%255);
	return 1;
}

static void init(void)
{
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
	registerAnimation(init,tick,deinit, SCAN,TYPE_NORMAL,10, 15,"rand2");
}


