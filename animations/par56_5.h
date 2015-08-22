#include "et_devices.h"
#include <stdio.h>
#include <stdlib.h>

#include "sini.h"

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
		if(colmap[i]>5) colmap[i]-=5;
		if(colmap[i]<6) colmap[i]=0;
	}


	for(int i = 0;i<6;i++)
		for(int j=0;j<3;j++)
			if((colmap[i*3+j]>130)||(f%2==0))
				setpar(i,j,colmap[i*3+j]);
			else
				setpar(i,j,0);


	return 1;
}

