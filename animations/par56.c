#include "main.h"
#include <stdio.h>
#include <math.h>

#include "dmx_devices.h"
#include "et_devices.h"

#include "par56_5.h"
#include "par56_4.h"
#include "par56_3.h"
#include "par56_2.h"
#include "par56_1.h"


static int a;

static uint8_t tick_0(void) 
{
	if(a < 12)
	{
		setCh(ADDR_PAR1,255/(12.0f*12.0f)*a*a);
		setCh(ADDR_PAR2,255/(12.0f*12.0f)*a*a);
		setCh(ADDR_PAR3,255/(12.0f*12.0f)*a*a);
		setCh(ADDR_PAR4,255/(12.0f*12.0f)*a*a);
		setCh(ADDR_PAR5+1,255/(12.0f*12.0f)*a*a);
		setCh(ADDR_PAR6+1,255/(12.0f*12.0f)*a*a);
	}
	else if(a < 24)
	{
		setCh(ADDR_PAR1,255/(12.0f*12.0f)*((a-12)*(a-12)));
		setCh(ADDR_PAR2,255/(12.0f*12.0f)*((a-12)*(a-12)));
		setCh(ADDR_PAR3,255/(12.0f*12.0f)*((a-12)*(a-12)));
		setCh(ADDR_PAR4,255/(12.0f*12.0f)*((a-12)*(a-12)));
		setCh(ADDR_PAR5+1,255/(12.0f*12.0f)*((a-12)*(a-12)));
		setCh(ADDR_PAR6+1,255/(12.0f*12.0f)*((a-12)*(a-12)));
	}
	else
	{
		setCh(ADDR_PAR1,0);
		setCh(ADDR_PAR2,0);
		setCh(ADDR_PAR3,0);
		setCh(ADDR_PAR4,0);
		setCh(ADDR_PAR5+1,0);
		setCh(ADDR_PAR6+1,0);
	}


	a++;

	if(a==50)a=0;


	return 1;
}


static void init(void)
{
	a=0;
	setCh(ADDR_PAR1,0);
	setCh(ADDR_PAR1+1,0);
	setCh(ADDR_PAR1+2,0);
	setCh(ADDR_PAR1+3,0);
	setCh(ADDR_PAR1+4,0);
	setCh(ADDR_PAR1+5,0);
	setCh(ADDR_PAR2,0);
	setCh(ADDR_PAR2+1,0);
	setCh(ADDR_PAR2+2,0);
	setCh(ADDR_PAR2+3,0);
	setCh(ADDR_PAR2+4,0);
	setCh(ADDR_PAR2+5,0);
	setCh(ADDR_PAR3,0);
	setCh(ADDR_PAR3+1,0);
	setCh(ADDR_PAR3+2,0);
	setCh(ADDR_PAR3+3,0);
	setCh(ADDR_PAR3+4,0);
	setCh(ADDR_PAR3+5,0);
	setCh(ADDR_PAR4,0);
	setCh(ADDR_PAR4+1,0);
	setCh(ADDR_PAR4+2,0);
	setCh(ADDR_PAR4+3,0);
	setCh(ADDR_PAR4+4,0);
	setCh(ADDR_PAR4+5,0);
	setCh(ADDR_PAR5,255);
	setCh(ADDR_PAR5+1,0);
	setCh(ADDR_PAR5+2,0);
	setCh(ADDR_PAR5+3,0);
	setCh(ADDR_PAR5+4,0);
	setCh(ADDR_PAR5+5,0);
	setCh(ADDR_PAR6,255);
	setCh(ADDR_PAR6+1,0);
	setCh(ADDR_PAR6+2,0);
	setCh(ADDR_PAR6+3,0);
	setCh(ADDR_PAR6+4,0);
	setCh(ADDR_PAR6+5,0);
}


static void deinit(void)
{
}



static void constructor(void) CONSTRUCTOR_ATTRIBUTES
void constructor(void) {
	registerAnimation(init,tick_6,deinit, PAR56,TYPE_NORMAL,35, 20,"fd_out");
	registerAnimation(init,tick_1,deinit, PAR56,TYPE_NORMAL,25, 3,"fd_in2");
	registerAnimation(init,tick_4,deinit, PAR56,TYPE_NORMAL,15, 20,"randFla");
	registerAnimation(init,tick_3,deinit, PAR56,TYPE_NORMAL,130, 5,"strobe");
	registerAnimation(init,tick_5,deinit, PAR56,TYPE_NORMAL,20, 20,"Sin");
	registerAnimation(init,tick_0,deinit, PAR56,TYPE_NORMAL,25, 3,"fd_in");
	queueInitialization(PAR56,1,1,0,"PAR56");
	queuePageSet(PAR56,0,1);
}


