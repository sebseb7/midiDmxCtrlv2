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
static uint8_t in[512];
static uint8_t poti[8];

static int running = 1;

void intHandler(int dummy) 
{
	running = 0*dummy;
}

#define MAX_CUEUES 20
#define MAX_ANI_PER_CUEUE 30

static int cueuecount = 0;
static uint32_t update_ui = 1;

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


struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	uint16_t active;
	uint32_t duration;
	uint32_t timing;
};


struct cueue {
	struct animation list[MAX_ANI_PER_CUEUE];
	tick_fun test;
	tick_fun off;
	uint8_t test_available;
	uint8_t off_available;
	uint8_t length;
	uint8_t visible;
	uint8_t active;
	uint8_t random;
	uint8_t paused;
	uint8_t in_test;
	uint8_t in_off;
	uint8_t active_item;
	uint8_t active_elements;
	uint32_t tick;
	uint32_t last_frame;
} cueues[MAX_CUEUES];

uint16_t cueueidx[MAX_CUEUES];



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

int addToCueue(const uint16_t cueue,const uint8_t cueue_type,const init_fun init,const tick_fun tick, const deinit_fun deinit,const uint32_t d, const uint32_t t)
{
	if(cueues_initialized == 0)
	{
		for(int i = 0; i < MAX_CUEUES; i++)
		{
			cueues[i].length = 0;
			cueues[i].active_elements = 0;
			cueues[i].off_available=0;
			cueues[i].test_available=0;
			cueues[i].tick=0;
			cueues[i].last_frame=0;
			cueues[i].active=1;
			cueues[i].paused=0;
			cueues[i].random=0;
			cueues[i].in_test=0;
			cueues[i].in_off=0;
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

		cueues[cidx].list[cueues[cidx].length].init_fp = init;
		cueues[cidx].list[cueues[cidx].length].tick_fp = tick;
		cueues[cidx].list[cueues[cidx].length].deinit_fp = deinit;
		cueues[cidx].list[cueues[cidx].length].duration = d;
		cueues[cidx].list[cueues[cidx].length].timing = t;
		cueues[cidx].list[cueues[cidx].length].active = 1;
		cueues[cidx].length++;
		cueues[cidx].active_elements++;
	}

	if(cueue_type == TYPE_OFF)
	{
		cueues[cidx].off = tick;
		cueues[cidx].off_available = 1;
	}

	if(cueue_type == TYPE_TEST)
	{
		cueues[cidx].test = tick;
		cueues[cidx].test_available = 1;
	}

	return cidx;


}

void queueInitialization(uint8_t cueue_type,int active,int visible,int paused)
{
	int cidx = cueueidx[cueue_type]-1;
	cueues[cidx].active=active;
	cueues[cidx].visible=visible;
	cueues[cidx].paused=paused;
}
void queueAniActive(uint8_t cueue_type,int item,int active)
{
	int cidx = cueueidx[cueue_type]-1;

	if(cueues[cidx].length > item)
	{
		if((cueues[cidx].list[item].active == 0)&&(active==1))
		{
			cueues[cidx].list[item].active=1;
			cueues[cidx].active_elements++;
			update_ui=1;
		}
		if((cueues[cidx].list[item].active == 1)&&(active==0))
		{
			if(cueues[cidx].active_elements>0)
			{
				cueues[cidx].list[item].active=0;
				cueues[cidx].active_elements--;
				update_ui=1;
			}
		}
	}
}

void registerAnimation(const init_fun init,const tick_fun tick, const deinit_fun deinit,const uint16_t cueue,const uint8_t cueue_type,const uint16_t t, const float count)
{
	if(cueue_type == TYPE_NORMAL)
	{
		addToCueue(cueue,cueue_type,init,tick,deinit,count*t,1000000/t);
	}
	else
	{
		addToCueue(cueue,cueue_type,init,tick,deinit,0,0);
	}
}


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {

#ifdef LAUNCHPAD
	launchpad_init();


	launchpad_setSide(5,2,2,0);
	int display_mode = 0;
	int queue_setup = 0;
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

	srand(time(NULL));

	signal(SIGINT, intHandler);

	poti[0] = 127;
	poti[1] = 127;


	ch[0]=0;

	for(int cidx=0;cidx < cueuecount;cidx++)
	{
		cueues[cidx].list[cueues[cidx].active_item].init_fp();
	}

	uint32_t tick_count_ui = 0;

	unsigned long long last_frame_ui=0;

	struct timeval tv;




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
						if((e.type == 144)&&(e.x==lpmap[cidx*8])&&(e.y==127))
						{
							update_ui=1;
							if(cueues[cidx_l].active==1)
							{
								cueues[cidx_l].active=0;
							}
							else
							{
								cueues[cidx_l].active=1;
							}
						}
						if((e.type == 144)&&(e.x==lpmap[cidx*8+1])&&(e.y==127))
						{
							update_ui=1;
							if(queue_setup==cidx_l+1)
							{
								queue_setup=0;
							}
							else
							{
								queue_setup=cidx_l+1;
							}
						}


						if((cueues[cidx_l].active == 1)&&(cueues[cidx_l].in_test==0)&&(cueues[cidx_l].in_off==0))
						{


							int new_idx =  cueues[cidx_l].active_item;

							if(queue_setup != cidx_l+1)
							{
								if((e.type == 144)&&(e.y == 127)&&(e.x >= lpmap[(cidx*8)+2])&&(e.x <= lpmap[(cidx*8)+7]))
								{
									int new_idx_tmp = e.x-(2+(cidx*16));
									if(new_idx_tmp < cueues[cidx_l].length)
									{
										new_idx = new_idx_tmp;
									}
								}
								if((e.type == 144)&&(e.y == 127)&&(e.x >= lpmap[(cidx*8)+8])&&(e.x <= lpmap[(cidx*8)+15]))
								{
									printf("ssdsdsd: %i\n",e.x);
									int new_idx_tmp = e.x-(10+(cidx*16));
									if(new_idx_tmp < cueues[cidx_l].length)
									{
										new_idx = new_idx_tmp;
									}
								}
							}
							else
							{
								if((e.type == 144)&&(e.y == 127)&&(e.x >= lpmap[(cidx*8)+2])&&(e.x <= lpmap[(cidx*8)+7]))
								{
									int new_idx_tmp = e.x-(2+(cidx*16));
									if(new_idx_tmp < cueues[cidx_l].length)
									{
										update_ui=1;
										if(cueues[cidx_l].list[new_idx_tmp].active==0)
										{
											cueues[cidx_l].list[new_idx_tmp].active=1;
											cueues[cidx_l].active_elements++;
										}
										else
										{
											if(cueues[cidx_l].active_elements > 1)
											{
												cueues[cidx_l].list[new_idx_tmp].active=0;
												cueues[cidx_l].active_elements--;
											}
										}
									}
								}
								if((e.type == 144)&&(e.y == 127)&&(e.x >= lpmap[(cidx*8)+8])&&(e.x <= lpmap[(cidx*8)+15]))
								{
									int new_idx_tmp = e.x-(10+(cidx*16));
									if(new_idx_tmp < cueues[cidx_l].length)
									{
										update_ui=1;
										if(cueues[cidx_l].list[new_idx_tmp].active==0)
										{
											cueues[cidx_l].list[new_idx_tmp].active=1;
											cueues[cidx_l].active_elements++;
										}
										else
										{
											if(cueues[cidx_l].active_elements > 1)
											{
												cueues[cidx_l].list[new_idx_tmp].active=0;
												cueues[cidx_l].active_elements--;
											}
										}
									}
								}
							}

							int error = 0;
							while(new_idx != cueues[cidx_l].active_item)
							{
								error++;
								if(error > 20)
								{
									printf("error1 %i %i %i\n",new_idx,cidx_l,cueues[cidx_l].active);
									exit(0);
								}
								update_ui=1;

								cueues[cidx_l].list[cueues[cidx_l].active_item].deinit_fp();

								cueues[cidx_l].active_item++;
								if(cueues[cidx_l].length == cueues[cidx_l].active_item)
									cueues[cidx_l].active_item=0;

								cueues[cidx_l].tick=0;
								cueues[cidx_l].list[cueues[cidx_l].active_item].init_fp();
							}	
						}
						cidx++;
						if(cueues[cidx_l].length>6)
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

			if(queue_setup != 0)
			{
				if((e.type == 176)&&(e.x==104)&&(e.y==127))
				{
					update_ui=1;
					if(cueues[queue_setup-1].paused==1)
					{
						cueues[queue_setup-1].paused=0;
					}
					else
					{
						cueues[queue_setup-1].paused=1;
					}
				}
				if((e.type == 176)&&(e.x==105)&&(e.y==127))
				{
					update_ui=1;
					if(cueues[queue_setup-1].random==1)
					{
						cueues[queue_setup-1].random=0;
					}
					else
					{
						cueues[queue_setup-1].random=1;
					}
				}
				if((e.type == 176)&&(e.x==106)&&(e.y==127))
				{
					if(cueues[queue_setup-1].off_available == 1)
					{
						update_ui=1;
						if(cueues[queue_setup-1].in_off==1)
						{
							cueues[queue_setup-1].in_off=0;
						}
						else
						{
							cueues[queue_setup-1].in_off=1;
							cueues[queue_setup-1].in_test=0;
						}
					}
				}
				if((e.type == 176)&&(e.x==107)&&(e.y==127))
				{
					if(cueues[queue_setup-1].test_available == 1)
					{
						update_ui=1;
						if(cueues[queue_setup-1].in_test==1)
						{
							cueues[queue_setup-1].in_test=0;
						}
						else
						{
							cueues[queue_setup-1].in_test=1;
							cueues[queue_setup-1].in_off=0;
						}
					}
				}





			}


			if((e.type == 144)&&(e.x==88)&&(e.y==127))
			{
				update_ui=1;
				if(display_mode != 1)
				{
					queue_setup=0;
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
			printf("%d %d %d\n", e.x, e.y, e.type);
			if((e.type == 176)&&(e.x>=16)&&(e.x<24))
			{
				//poti[16-e.x] = e.y;
			}
			if((e.type == 176)&&(e.x<8))
			{
				poti[e.x] = e.y;
			}
		}
#endif


		if(update_ui)
		{
			update_ui=0;
#ifdef LAUNCHPAD

			if(queue_setup == 0)
			{
				for(int i = 0;i<8;i++)
					launchpad_setTop(i,0,0,0);
			}

			if(display_mode==0)
			{

				int cidx=0;
				for(int cidx_l=0;cidx_l < cueuecount;cidx_l++)
				{
					if(cueues[cidx_l].visible)
					{
						if(cueues[cidx_l].active == 1)
						{
							launchpad_setMatrix(cidx,0,0,3,0);
						}
						else
						{
							launchpad_setMatrix(cidx,0,1,0,0);
						}
						if(queue_setup == cidx_l+1)
						{
							launchpad_setMatrix(cidx,1,3,0,1);


							if(cueues[cidx_l].paused)
							{
								launchpad_setTop(0,1,0,0);
							}
							else
							{
								launchpad_setTop(0,0,3,0);
							}
							if(cueues[cidx_l].random)
							{
								launchpad_setTop(1,0,3,0);
							}
							else
							{
								launchpad_setTop(1,1,0,0);
							}
							if(cueues[queue_setup-1].off_available == 1)
							{
								if(cueues[cidx_l].in_off)
								{
									launchpad_setTop(2,0,3,0);
								}
								else
								{
									launchpad_setTop(2,1,0,0);
								}
							}
							else
							{
								launchpad_setTop(2,0,0,0);
							}
							if(cueues[queue_setup-1].test_available == 1)
							{
								if(cueues[cidx_l].in_test)
								{
									launchpad_setTop(3,0,3,0);
								}
								else
								{
									launchpad_setTop(3,1,0,0);
								}	
							}
							else
							{
								launchpad_setTop(3,0,0,0);
							}

							for(int i =0;i<cueues[cidx_l].length;i++)
							{
								if(cueues[cidx_l].list[i].active==1)
								{
									launchpad_setMatrix(cidx,2+i,0,3,0);
								}
								else
								{
									launchpad_setMatrix(cidx,2+i,1,0,0);
								}
							}
						}
						else
						{
							launchpad_setMatrix(cidx,1,1,0,0);
							for(int i =0;i<cueues[cidx_l].length;i++)
							{
								if((cueues[cidx_l].active==0)||(cueues[cidx_l].in_off==1)||(cueues[cidx_l].in_test==1))
								{
									launchpad_setMatrix(cidx,2+i,1,0,0);
								}
								else
								{
									if(cueues[cidx_l].active_item == i)
										launchpad_setMatrix(cidx,2+i,0,3,0);
									else
										if(cueues[cidx_l].paused==1)
										{
											launchpad_setMatrix(cidx,2+i,1,0,0);
										}
										else if(cueues[cidx_l].list[i].active==0)
										{
											launchpad_setMatrix(cidx,2+i,1,0,0);
										}
										else
										{
											launchpad_setMatrix(cidx,2+i,1,1,0);
										}
								}
							}
						}


						cidx++;
						if(cueues[cidx_l].length>6)
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

			if(cueues[cidx].in_test == 1)
			{
				cueues[cidx].test();
			}
			else if(cueues[cidx].in_off == 1)
			{
				cueues[cidx].off();
			}
			else if(((uint32_t)time_diff > cueues[cidx].list[cueues[cidx].active_item].timing)&&(cueues[cidx].active == 1))
			{
				//printf("fps %i : %f\n",cidx,1.0f/time_diff*1000000.0f);

				cueues[cidx].list[cueues[cidx].active_item].tick_fp();
				if(cueues[cidx].paused==0) cueues[cidx].tick++;
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


		for(int i = 1;i<9;i++)
		{
//			ch[i]=poti[i-1]*2;
		}

//			ch[29]=poti[0]*2;
//		ch[30]=poti[1]*2;
//			ch[4]=poti[2]*2;
//		ch[5]=poti[3]*2;

		ret = ftdi_write_data(ftdi, ch, 65);
		if (ret < 0)
		{
			fprintf(stderr,"write failed , error %d (%s)\n",ret, ftdi_get_error_string(ftdi));
		}
#endif
		usleep(3000);





		for(int cidx=0;cidx < cueuecount;cidx++)
		{

			if((cueues[cidx].tick >= cueues[cidx].list[cueues[cidx].active_item].duration)&&(cueues[cidx].active == 1)&&(cueues[cidx].paused == 0)&&(cueues[cidx].in_test == 0)&&(cueues[cidx].in_off == 0))
			{
				update_ui = 1;

				cueues[cidx].list[cueues[cidx].active_item].deinit_fp();
								

				int error = 0;
				do
				{
								error++;
								if(error > 20)
								{
									printf("error2\n");
									exit(0);
								}
					cueues[cidx].active_item++;
					if(cueues[cidx].length == cueues[cidx].active_item)
						cueues[cidx].active_item=0;
				}
				while(cueues[cidx].list[cueues[cidx].active_item].active==0);

				cueues[cidx].tick=0;

				cueues[cidx].list[cueues[cidx].active_item].init_fp();
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

