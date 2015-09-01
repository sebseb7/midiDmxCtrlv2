#include "et_devices.h"
#include <stdio.h>
#include <stdlib.h>

#include "sini.h"
#include "led_gamma.h"

static int f;

static int colmap[6*3];


static int last_lamp;
static int last_lamp2;

static uint8_t tick_6(void) 
{
	f++;

	if((f%((int)(12*get_custom(1)))) == 0)
	{
		int lamp;
		int col = rand()%3;
		do
		{
			lamp = rand()%6;
		}while((lamp == last_lamp)||(lamp == last_lamp2));

		last_lamp2=last_lamp;
		last_lamp=lamp;

		colmap[lamp*3]=0;
		colmap[lamp*3+1]=0;
		colmap[lamp*3+2]=0;
		colmap[lamp*3+col]=255;
	}

	
	for(int i = 0;i<(6*3);i++)
	{
		if(colmap[i]>130) colmap[i]-=4;
		else if(colmap[i]>15) colmap[i]-=4;
		else if(colmap[i]<16) colmap[i]=0;
	}


	for(int i = 0;i<6;i++)
		for(int j=0;j<3;j++)
			if((colmap[i*3+j]>180)||(f%2==0))
				setpar(i,j,led_gamma(colmap[i*3+j]));
			else
				setpar(i,j,0);


	return 1;
}

