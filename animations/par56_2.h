#include "et_devices.h"

static int c;



static uint8_t tick_3(void) 
{

	

	setpar(c%((int)(16*get_custom(2))),0,255);
	
	setpar((c+((int)(16*get_custom(2)))-2)%((int)(16*get_custom(2))),0,0);
	



	c++;


	return 1;
}

