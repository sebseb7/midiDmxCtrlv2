#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dmx_devices.h"


static uint16_t a;

static uint8_t x;
static uint8_t y;

static int angle=0;

static int length=12;

static uint8_t tick(void) 
{
	a++;

//	length=getFader(2)/6;


	if(a%2 == 0)
	
	{
		int new_angle;
		int new_x;
		int new_y;
		int spread=70;
		do
		{
			int angle_var = (rand()%(spread*2))-spread;
			spread+=10;

			new_angle = angle +angle_var;

			if(new_angle > 360) new_angle=new_angle-360;
			if(new_angle < 0) new_angle=new_angle+360;

			new_x = x+sin(new_angle * M_PI / 180.0f)*4*length;
			new_y = y+cos(new_angle * M_PI / 180.0f)*4*length;

		//	printf("%i %i %i\n",new_angle,new_x,new_y);
		}
		while((new_x<0)||(new_y<0)||(new_x > 255)||(new_y>255));
		angle = new_angle;

	}

	x+=sin(angle * M_PI / 180.0f)*length;
	y+=cos(angle * M_PI / 180.0f)*length;

//	if(x < 0){ x=x*-1;angle+=180;};
//	if(y < 0){ y=y*-1;angle+=180;};
//	if(x > 255){ x=255-(x-255);angle+=180;};
//	if(y > 255){ y=255-(y-255);angle+=180;};
//	if(angle > 360){angle = angle-180;};

	setCh(4,x);
	setCh(5,y);
	return 1;
}

static void init(void)
{
	x=128;
	y=128;
	angle=0;
	a=0;


}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick,deinit, SCAN_2,TYPE_NORMAL,5, 5);
}


