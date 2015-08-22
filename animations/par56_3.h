#include "et_devices.h"
#include <stdio.h>
#include <stdlib.h>

static int d;


int curr_l;
int curr_c;

int curr_l2;
int curr_c2;

static uint8_t tick_4(void) 
{
	
	int a = d % 3;


	if(a==0)
	{
		curr_l = rand()%6;
		curr_c = rand()%3;

		setpar(curr_l,curr_c,255);
	}
	if(a==1)
	{
		setpar(curr_l,curr_c,0);
		
		
		curr_l2 = rand()%6;
		curr_c2 = rand()%3;

		setpar(curr_l2,curr_c2,255);
	}
	if(a==2)
	{
		setpar(curr_l2,curr_c2,0);
	}



	
	d++;
	return 1;
}

