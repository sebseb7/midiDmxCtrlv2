#include "et_devices.h"

static int b;

static uint8_t tick_1(void) 
{
	if(b < 12)
	{
		setCh(ADDR_PAR1,255/(12.0f*12.0f)*b*b);
		setCh(ADDR_PAR2,255/(12.0f*12.0f)*b*b);
		setCh(ADDR_PAR3,255/(12.0f*12.0f)*b*b);
		setCh(ADDR_PAR4,255/(12.0f*12.0f)*b*b);
		setCh(ADDR_PAR5+1,255/(12.0f*12.0f)*b*b);
		setCh(ADDR_PAR6+1,255/(12.0f*12.0f)*b*b);
	}
	else if(b < 24)
	{
		setCh(ADDR_PAR1,0);
		setCh(ADDR_PAR1+2,255/(12.0f*12.0f)*((b-12)*(b-12)));
		setCh(ADDR_PAR2,0);
		setCh(ADDR_PAR2+2,255/(12.0f*12.0f)*((b-12)*(b-12)));
		setCh(ADDR_PAR3,0);
		setCh(ADDR_PAR3+2,255/(12.0f*12.0f)*((b-12)*(b-12)));
		setCh(ADDR_PAR4,0);
		setCh(ADDR_PAR4+2,255/(12.0f*12.0f)*((b-12)*(b-12)));
		setCh(ADDR_PAR5+1,0);
		setCh(ADDR_PAR5+3,255/(12.0f*12.0f)*((b-12)*(b-12)));
		setCh(ADDR_PAR6+1,0);
		setCh(ADDR_PAR6+3,255/(12.0f*12.0f)*((b-12)*(b-12)));
	}
	else
	{
		setCh(ADDR_PAR1+2,0);
		setCh(ADDR_PAR2+2,0);
		setCh(ADDR_PAR3+2,0);
		setCh(ADDR_PAR4+2,0);
		setCh(ADDR_PAR5+3,0);
		setCh(ADDR_PAR6+3,0);
	}


	b++;

	if(b==50)b=0;


	return 1;
}

