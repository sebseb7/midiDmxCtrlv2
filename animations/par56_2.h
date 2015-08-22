#include "et_devices.h"

static int c;



static uint8_t tick_3(void) 
{

	if((c%20)==0)
	{
		for(int i = 0;i<6;i++)
			for(int j=0;j<3;j++)
				setpar(i,j,255);
	}
	else
	{
		for(int i = 0;i<6;i++)
			for(int j=0;j<3;j++)
				setpar(i,j,0);
	}

	



	c++;


	return 1;
}

