#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <signal.h>

#include "main.h"
#include <string.h>
#include<sys/time.h>

#ifdef LIBFTDI
#include "libftdi1/ftdi.h"
#endif


#include "keyboard.h"
#include "launchpad.h"

#ifdef SDL_OUTPUT
#include <SDL/SDL.h>
#endif

static uint8_t ch[512];
static uint8_t toggle[5];
static uint8_t in[512];
static uint8_t poti[8];

static int running = 1;

void intHandler(int dummy) 
{
	running = 0*dummy;
}

#define MAX_ANIMATIONS 200
#define MAX_CUEUES 20

int animationcount = 0;
int cueuecount = 0;

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



struct cueue {
	uint16_t first_in_cueue;
	uint16_t last_in_cueue;
	uint16_t active_in_cueue;
	int16_t test_in_cueue;
	int16_t off_in_cueue;
	uint8_t length;
	uint8_t visible;
	uint8_t playmode; //1=play,2=pause;3=off,4=testi,(0 = manual)
	uint8_t active_item;
	uint32_t tick;
	uint32_t last_frame;
} cueues[MAX_CUEUES];

uint16_t cueueidx[MAX_CUEUES];

struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	uint16_t cueue;
	uint16_t next_in_cueue;
	uint32_t duration;
	uint32_t timing;
} animations[MAX_ANIMATIONS];


void setCh(uint8_t chan, uint8_t value)
{
	ch[chan] = value;
}

void setIn(uint8_t chan, uint8_t value)
{
	in[chan] = value;
}
uint8_t getIn(uint8_t chan)
{
	return in[chan];
}


static int cueues_initialized = 0;

void addToCueue(const uint16_t cueue,const uint8_t cueue_type,const uint16_t animation_position)
{
	if(cueues_initialized == 0)
	{
		for(int i = 0; i < MAX_CUEUES; i++)
		{
			cueues[i].length = 0;
			cueues[i].off_in_cueue=-1;
			cueues[i].test_in_cueue=-1;
			cueues[i].tick=0;
			cueues[i].last_frame=0;
			cueues[i].playmode=1;
			cueues[i].visible=1;
			cueues[i].active_item=0;
			cueueidx[i]=0;
		}
		cueues_initialized=1;
	}

	int cidx=0;
	if(cueueidx[cueue]==0)
	{
		cidx=cueuecount;
		cueuecount++;
		cueueidx[cueue]=cidx+1;
	}
	else
	{
		cidx = cueueidx[cueue]-1;
	}



	if(cueue_type == TYPE_NORMAL)
	{
		if(cueues[cidx].length == 0)
		{
			cueues[cidx].active_in_cueue=animation_position;
			cueues[cidx].first_in_cueue=animation_position;
		}
		else
		{
			animations[cueues[cidx].last_in_cueue].next_in_cueue = animation_position;
		}

		animations[animation_position].next_in_cueue = cueues[cidx].first_in_cueue;
		cueues[cidx].last_in_cueue=animation_position;
		cueues[cidx].length++;
	}
	
	if(cueue_type == TYPE_OFF)
		cueues[cidx].off_in_cueue=animation_position;
	
	if(cueue_type == TYPE_TEST)
		cueues[cidx].test_in_cueue=animation_position;
		

}

void queueInitialization(uint8_t cueue_type,int playmode,int visible)
{
	int cidx = cueueidx[cueue_type]-1;
	cueues[cidx].playmode=playmode;
	cueues[cidx].visible=visible;
}

void registerAnimation(const init_fun init,const tick_fun tick, const deinit_fun deinit,const uint16_t cueue,const uint8_t cueue_type,const uint16_t t, const float count)
{
	if(animationcount == MAX_ANIMATIONS)
		return;
	animations[animationcount].init_fp = init;
	animations[animationcount].tick_fp = tick;
	animations[animationcount].deinit_fp = deinit;
	animations[animationcount].cueue = cueue;

	if(cueue_type == TYPE_NORMAL)
	{
		animations[animationcount].duration = count*t;
		animations[animationcount].timing = 1000000/t;
	}

	addToCueue(cueue,cueue_type,animationcount);

	animationcount++;

}


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {
		
#ifdef LAUNCHPAD
	launchpad_init();


	launchpad_setSide(5,2,2,0);
	int display_mode = 0;
#endif

#ifdef KORG_CTRL
	MidiObj midi_korg;
	keyboard_init(&midi_korg,"nanoKONTROL");
#endif

#ifdef SDL_OUTPUT
	SDL_Surface* screen;
	screen = SDL_SetVideoMode(100,100,32, SDL_SWSURFACE | SDL_DOUBLEBUF);
#endif


	for(uint16_t i = 0; i < 512;i++)
	{
		ch[i]=0;
	}

	toggle[0]=1;			
	toggle[1]=1;			
	toggle[2]=1;			
	toggle[3]=1;			
	toggle[4]=1;			
#ifdef KORG_CTRL
	keyboard_send(&midi_korg,176,43,toggle[0]*127);
	keyboard_send(&midi_korg,176,44,toggle[1]*127);
	keyboard_send(&midi_korg,176,42,toggle[2]*127);
	keyboard_send(&midi_korg,176,41,toggle[3]*127);
	keyboard_send(&midi_korg,176,45,toggle[4]*127);
#endif
	//	FILE *input;

	/*	input = popen ("../traktorMidiClock/traktorMidiClock", "r");
		if (!input)
		{
		fprintf (stderr,
		"incorrect parameters or too many files.\n");
		return EXIT_FAILURE;
		}
		*/

#ifdef LIBFTDI

	int ret;
	struct ftdi_context *ftdi;
	if ((ftdi = ftdi_new()) == 0)
	{
		fprintf(stderr, "ftdi_new failed\n");
		return EXIT_FAILURE;
	}
	if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0)
	{
		fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return EXIT_FAILURE;
	}
	// Read out FTDIChip-ID of R type chips
	if (ftdi->type == TYPE_R)
	{
		unsigned int chipid;
		printf("ftdi_read_chipid: %d\n", ftdi_read_chipid(ftdi, &chipid));
		printf("FTDI chipid: %X\n", chipid);
	}
	ret = ftdi_set_line_property(ftdi, 8, STOP_BIT_2, NONE);
	if (ret < 0)
	{
		fprintf(stderr, "unable to set line parameters: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		exit(-1);
	}
	ret = ftdi_set_baudrate(ftdi, 250000);
	if (ret < 0)
	{
		fprintf(stderr, "unable to set baudrate: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		exit(-1);
	}
#endif

#ifdef KORG_CTRL
	for(uint8_t i = 32;i <= 39;i++)
	{
		keyboard_send(&midi_korg,176,i,0);
		keyboard_send(&midi_korg,176,i+16,0);
		keyboard_send(&midi_korg,176,i+32,0);
	}
	keyboard_send(&midi_korg,176,32,127);
#endif
	srand(time(NULL));

	signal(SIGINT, intHandler);

	poti[0] = 127;
	poti[1] = 127;


	ch[0]=0;
		
	for(int cidx=0;cidx < cueuecount;cidx++)
	{
		animations[cueues[cidx].active_in_cueue].init_fp();
	}

	uint32_t tick_count_ui = 0;

	unsigned long long last_frame_ui=0;

	struct timeval tv;

	uint32_t update_ui = 1;



	while(running) {


		KeyboardEvent e;
		
		
		
#ifdef SDL_OUTPUT
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.type) {
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						default: break;
					}
				default: break;
			}
		}
#endif



#ifdef LAUNCHPAD
	while(launchpad_poll(&e)) 
		{
			if(display_mode == 0)
			{
				int cidx=0;
				for(int cidx_l=0;cidx_l < cueuecount;cidx_l++)
				{
					if(cueues[cidx_l].visible)
					{
						for(int pm =1;pm < 5 ; pm++)
							if((e.type == 144)&&(e.x==lpmap[(cidx*16+pm)-1])&&(e.y==127))
							{
								if(cueues[cidx_l].playmode == pm)
								{
									cueues[cidx_l].playmode = 0;
									update_ui=1;
								}
								else
								{
									if((pm == 4)&&(cueues[cidx_l].test_in_cueue != -1))
									{
										cueues[cidx_l].playmode = pm;
										update_ui=1;
									}
									else if((pm == 3)&&(cueues[cidx_l].off_in_cueue != -1))
									{
										cueues[cidx_l].playmode = pm;
										update_ui=1;
									}
									else if(pm < 3)
									{
										cueues[cidx_l].playmode = pm;
										update_ui=1;
									}
								}
							}
					
						if((cueues[cidx_l].playmode == 1)||(cueues[cidx_l].playmode==2))
						{
							int new_idx =  cueues[cidx_l].active_item;
							if((e.type == 144)&&(e.y == 127)&&(e.x >= lpmap[(cidx*16)+4])&&(e.x <= lpmap[(cidx*16)+7]))
							{
								int new_idx_tmp = e.x-(4+(cidx*32));
								if(new_idx_tmp < cueues[cidx_l].length)
								{
									new_idx = new_idx_tmp;
								}
							}
							if((e.type == 144)&&(e.y == 127)&&(e.x >= lpmap[(cidx*16)+8])&&(e.x <= lpmap[(cidx*16)+15]))
							{
								printf("ssdsdsd: %i\n",e.x);
								int new_idx_tmp = e.x-(12+(cidx*32));
								if(new_idx_tmp < cueues[cidx_l].length)
								{
									new_idx = new_idx_tmp;
								}
							}
						
							while(new_idx != cueues[cidx_l].active_item)
							{
								update_ui=1;
						
								animations[cueues[cidx_l].active_in_cueue].deinit_fp();
						
								cueues[cidx_l].active_item++;
								if(cueues[cidx_l].length == cueues[cidx_l].active_item)
									cueues[cidx_l].active_item=0;

								cueues[cidx_l].active_in_cueue=animations[cueues[cidx_l].active_in_cueue].next_in_cueue;
								cueues[cidx_l].tick=0;
								animations[cueues[cidx_l].active_in_cueue].init_fp();
							}	
						}
						cidx++;
					}
				}
			}
			else if(display_mode == 1)
			{
				if((e.type == 144)&&(e.y == 127)&&((e.x%16) < 8))
				{
					int key = ((e.x-(e.x%8))/2)+e.x%8;

					if(key < cueuecount)
					{
						if(cueues[key].visible==0)
						{
							cueues[key].visible=1;
							update_ui=1;
						}
						else
						{
							cueues[key].visible=0;
							update_ui=1;
						}
					}


				}
			}


			if((e.type == 176)&&(e.x==104)&&(e.y==127))
			{
				toggle[0] ^= 1;
				update_ui=1;
			}
			if((e.type == 176)&&(e.x==105)&&(e.y==127))
			{
				toggle[1] ^= 1;
				update_ui=1;
			}
			if((e.type == 176)&&(e.x==106)&&(e.y==127))
			{
				toggle[2] ^= 1;
				update_ui=1;
			}
			if((e.type == 176)&&(e.x==107)&&(e.y==127))
			{
				toggle[3] ^= 1;
				update_ui=1;
			}
			if((e.type == 176)&&(e.x==108)&&(e.y==127))
			{
				toggle[4] ^= 1;
				update_ui=1;
			}
			if((e.type == 144)&&(e.x==88)&&(e.y==127))
			{
				update_ui=1;
				if(display_mode != 1)
				{
					display_mode = 1;
					launchpad_setSide(5,2,2,1);
				}
				else
				{
					display_mode = 0;
					launchpad_setSide(5,2,2,0);
				}
			}
			
			printf("%d %d %d\n", e.x, e.y, e.type);
		}

#endif
#ifdef KORG_CTRL
		while(keyboard_poll(&midi_korg,&e)) 
		{
			if((e.type == 176)&&(e.x>=16)&&(e.x<24))
			{
				poti[e.x-16] = e.y;
			}
		}
#endif


		if(update_ui)
		{
			update_ui=0;
#ifdef LAUNCHPAD

			if(display_mode==0)
			{

				int cidx=0;
				for(int cidx_l=0;cidx_l < cueuecount;cidx_l++)
				{
					if(cueues[cidx_l].visible)
					{
						for(int pm =1;pm < 5 ; pm++)
							if(cueues[cidx_l].playmode == pm)
								launchpad_setMatrix(cidx*2,pm-1,3,3,0);
							else
							{
								launchpad_setMatrix(cidx*2,pm-1,0,0,0);
								if((pm == 4)&&(cueues[cidx_l].test_in_cueue != -1))
									launchpad_setMatrix(cidx*2,pm-1,3,0,0);
								if((pm == 3)&&(cueues[cidx_l].off_in_cueue != -1))
									launchpad_setMatrix(cidx*2,pm-1,1,0,0);
								if(pm < 3)
									launchpad_setMatrix(cidx*2,pm-1,2,0,0);
							}
				
						for(int i =0;i<cueues[cidx_l].length;i++)
						{
							if((cueues[cidx_l].playmode==0)||(cueues[cidx_l].playmode>2))
							{
								launchpad_setMatrix(cidx*2,4+i,1,0,0);
							}
							else
							{
								if(cueues[cidx_l].active_item == i)
									launchpad_setMatrix(cidx*2,4+i,0,3,0);
								else
									if(cueues[cidx_l].playmode==2)
									{
										launchpad_setMatrix(cidx*2,4+i,1,0,0);
									}
									else
									{
										launchpad_setMatrix(cidx*2,4+i,1,1,0);
									}
							}
						}
				
						cidx++;
					}
				}
			}
			else if(display_mode==1)
			{
				for(int i=0;i<64;i++)
				{
					if(i < cueuecount)
					{
						if(cueues[i].visible==1)
						{
							launchpad_setMatrix((i-(i%8))/8,i%8,0,3,0);
						}
						else
						{
							launchpad_setMatrix((i-(i%8))/8,i%8,1,0,0);
						}
					}
					else
					{
						launchpad_setMatrix((i-(i%8))/8,i%8,0,0,0);
					}
				}
				printf("\n");
			}
#endif
		}


		for(int cidx=0;cidx < cueuecount;cidx++)
		{
			unsigned long long current_time;
			int32_t time_diff;
			gettimeofday(&tv,NULL);
			current_time = tv.tv_usec;
			time_diff = current_time - cueues[cidx].last_frame;

			if(time_diff < 0 )
			{
				time_diff+=1000000;
			}

			if(cueues[cidx].playmode == 4)
			{
				animations[cueues[cidx].test_in_cueue].tick_fp();
			}
			else if(cueues[cidx].playmode == 3)
			{
				animations[cueues[cidx].off_in_cueue].tick_fp();
			}
			else if(((uint32_t)time_diff > animations[cueues[cidx].active_in_cueue].timing)&&(cueues[cidx].playmode > 0))
			{
				//printf("fps %i : %f\n",cidx,1.0f/time_diff*1000000.0f);

				animations[cueues[cidx].active_in_cueue].tick_fp();
				if(cueues[cidx].playmode==1) cueues[cidx].tick++;
				gettimeofday(&tv,NULL);
				cueues[cidx].last_frame = tv.tv_usec ;
			
			}
				
			

		}

		{
			unsigned long long current_time;
			int32_t time_diff;
			gettimeofday(&tv,NULL);
			current_time = tv.tv_usec;
			time_diff = current_time - last_frame_ui;

			if(time_diff < 0 )
			{
				time_diff+=1000000;
			}

			if((uint32_t)time_diff > 50000) // 20Hz
			{
				time_diff -= 50000;
				tick_count_ui++;
				gettimeofday(&tv,NULL);
				last_frame_ui = tv.tv_usec - time_diff;
			}
#ifdef SDL_OUTPUT			
		SDL_Rect rect = { 0, 0, 50, 50 };
		SDL_FillRect(
			screen, 
			&rect, 
			SDL_MapRGB(screen->format,ch[16],ch[17],ch[18])
		);
		SDL_Rect rect2 = { 0, 50, 50, 50 };
		SDL_FillRect(
			screen, 
			&rect2, 
			SDL_MapRGB(screen->format,ch[22],ch[23],ch[24])
		);
		SDL_Rect rect3 = { 50, 0, 50, 50 };
		SDL_FillRect(
			screen, 
			&rect3, 
			SDL_MapRGB(screen->format,ch[28],ch[29],ch[30])
		);
		SDL_Rect rect4 = { 50, 50, 50, 50 };
		SDL_FillRect(
			screen, 
			&rect4, 
			SDL_MapRGB(screen->format,ch[34],ch[35],ch[36])
		);
		SDL_Flip(screen);
#endif
		}

#ifdef LIBFTDI
		ret = ftdi_set_line_property2(ftdi, 8, STOP_BIT_2, NONE,BREAK_ON);
		if (ret < 0)
		{
			fprintf(stderr, "unable to set line parameters: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
			exit(-1);
		}
		usleep(100);
		ret = ftdi_set_line_property2(ftdi, 8, STOP_BIT_2, NONE,BREAK_OFF);
		if (ret < 0)
		{
			fprintf(stderr, "unable to set line parameters: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
			exit(-1);
		}
		unsigned char c=0;

		ret = ftdi_write_data(ftdi, &c, 0);
		if (ret < 0)
		{
			fprintf(stderr,"write failed , error %d (%s)\n",ret, ftdi_get_error_string(ftdi));
		}
		usleep(10);

		ret = ftdi_write_data(ftdi, ch, 65);
		if (ret < 0)
		{
			fprintf(stderr,"write failed , error %d (%s)\n",ret, ftdi_get_error_string(ftdi));
		}
#endif
		usleep(2000);
			




		for(int cidx=0;cidx < cueuecount;cidx++)
		{

			if((cueues[cidx].tick >= animations[cueues[cidx].active_in_cueue].duration)&& cueues[cidx].playmode == 1)
			{
				update_ui = 1;

				animations[cueues[cidx].active_in_cueue].deinit_fp();

				cueues[cidx].active_item++;
				if(cueues[cidx].length == cueues[cidx].active_item)
					cueues[cidx].active_item=0;

				cueues[cidx].active_in_cueue=animations[cueues[cidx].active_in_cueue].next_in_cueue;

				cueues[cidx].tick=0;

				animations[cueues[cidx].active_in_cueue].init_fp();
			}
		}
	}
	printf("exiting\n");
#ifdef LIBFTDI
	if ((ret = ftdi_usb_close(ftdi)) < 0)
	{
		fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return EXIT_FAILURE;
	}
	ftdi_free(ftdi);
#endif


#ifdef SDL_OUTPUT
	SDL_Quit();
#endif

	return 0;
}

