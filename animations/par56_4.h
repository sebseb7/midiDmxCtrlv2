#include "et_devices.h"
#include <stdio.h>
#include <stdlib.h>

#include "sini.h"

static int e;

static int last_la;

static uint8_t tick_5(void) 
{

	float var = 0.5f+((sini(e*80)/64535.0f)*0.7f);

	setpar(last_la,0,0);
	setpar(last_la,1,0);
	setpar(last_la,2,0);
//	setpar(5-last_la,2,0);
	last_la = sini(e*450*var)/65535.0f*6;
	setpar(last_la,0,255);
	setpar(last_la,1,255);
	setpar(last_la,2,255);
//	setpar(5-last_la,2,255);
	
	e++;
	return 1;
}

