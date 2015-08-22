#include "et_devices.h"
#include <stdio.h>
#include <stdlib.h>

#include "sini.h"

static int e;

static int last_la;

static uint8_t tick_5(void) 
{
	setpar(last_la,1,0);
	setpar(5-last_la,2,0);
	last_la = sini(e*1000*get_custom(0))/65535.0f*6;
	setpar(last_la,1,255);
	setpar(5-last_la,2,255);
	
	e++;
	return 1;
}

