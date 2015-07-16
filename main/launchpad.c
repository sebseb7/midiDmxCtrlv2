

#include "keyboard.h"
#include "launchpad.h"


static const uint8_t lpmap_side[8] = {
	8,24,40,56,72,88,104,120
};

static const uint8_t lpmap[64] = {
	0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,
	16,17,18,19,20,21,22,23,
	32,33,34,35,36,37,38,39,
	48,49,50,51,52,53,54,55,
	64,65,66,67,68,69,70,71,
	80,81,82,83,84,85,86,87,
	96,97,98,99,100,101,102,103,
	112,113,114,115,116,117,118,119

};
	
static MidiObj midi_launch;

void launchpad_init(void)
{
	keyboard_init(&midi_launch,"Launchpad Mini");

	
	for(int i =0;i<8;i++)
	{
		keyboard_send(&midi_launch,176,104+i,12);
		keyboard_send(&midi_launch,144,lpmap_side[i],12);	
	}

	for(int i = 0; i<64;i++)
		keyboard_send(&midi_launch,144,lpmap[i],12);	

	
	keyboard_send(&midi_launch,176,0,40);
}

void launchpad_setMatrix(int x, int y, int r, int g, int flash)
{
	int flag=0;
	if(flash)
	{
		flag=8;
	}
	else
	{
		flag=12;
	}
	keyboard_send(&midi_launch,144,lpmap[x*8+y],g*16+flag+r);	
}
void launchpad_setSide(int x, int r, int g, int flash)
{
	int flag=0;
	if(flash)
	{
		flag=8;
	}
	else
	{
		flag=12;
	}
	keyboard_send(&midi_launch,144,lpmap_side[x],g*16+flag+r);	
}
void launchpad_setTop(int y, int r, int g, int flash)
{
	int flag=0;
	if(flash)
	{
		flag=8;
	}
	else
	{
		flag=12;
	}
	keyboard_send(&midi_launch,176,104+y,g*16+flag+r);
}

int launchpad_poll(KeyboardEvent* e)
{
	return keyboard_poll(&midi_launch,e);
}
