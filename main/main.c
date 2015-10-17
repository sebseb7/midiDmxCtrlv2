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


#include "osc.h"

#ifdef SDL_OUTPUT
#include <SDL/SDL.h>
#endif

static float custom_master[8] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};

float get_custom(int ch)
{
	if(ch < 8)
		return custom_master[ch];
	
	return 1.0f;
}

static uint8_t ch[512];
static uint8_t in[512];
static uint8_t out[512];
static uint8_t manual[512];
static uint8_t override[512];
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

static int osc_manual_ch_offset_fine = 0;
static int osc_manual_ch_offset = 0;

static uint8_t page_matrix[8*32] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};

struct animation {
	init_fun init_fp;
	tick_fun tick_fp;
	deinit_fun deinit_fp;
	uint16_t initialized;
	uint16_t active;
	uint16_t active_remote;
	uint32_t duration;
	uint32_t timing;
	char * name;
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
	uint8_t active_remote;
	uint8_t random;
	uint8_t paused;
	uint8_t in_test;
	uint8_t in_off;
	uint8_t active_item;
	uint8_t active_elements;
	uint32_t tick;
	uint32_t last_frame;
	char * name;
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
uint8_t getFader(uint8_t chan)
{
	return poti[chan]*2;
}


static int cueues_initialized = 0;

int addToCueue(const uint16_t cueue,const uint8_t cueue_type,const init_fun init,const tick_fun tick, const deinit_fun deinit,const uint32_t d, const uint32_t t, char * name)
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
			cueues[i].active_remote=0;
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
		cueues[cidx].list[cueues[cidx].length].initialized = 0;
		cueues[cidx].list[cueues[cidx].length].active_remote = 0;
		cueues[cidx].list[cueues[cidx].length].name = name;
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

void queueInitialization(uint8_t cueue_type,int active,int visible,int paused, char * name)
{
	int cidx = cueueidx[cueue_type]-1;
	cueues[cidx].active=active;
	cueues[cidx].visible=visible;
	cueues[cidx].paused=paused;
	cueues[cidx].name=name;
}
void queueRemoteActive(uint8_t cueue_type,int active, int entry)
{
	int cidx = cueueidx[cueue_type]-1;
	

	if(active == 1)
	{
		cueues[cidx].active_remote=1;
		if(cueues[cidx].active==0)
		{
			cueues[cidx].list[cueues[cidx].active_item].init_fp();
		}
		for(int x =0;x<cueues[cidx].length;x++)
		{
			if(x == entry)
			{
				cueues[cidx].list[x].active_remote=1;
			}
			else
			{
				cueues[cidx].list[x].active_remote=0;
			}
		}
	}
	else
	{
		cueues[cidx].active_remote=0;
		if(cueues[cidx].active==0)
		{
			cueues[cidx].list[cueues[cidx].active_item].deinit_fp();
		}
		for(int x =0;x<cueues[cidx].length;x++)
		{
			cueues[cidx].list[x].active_remote=0;
		}
	}
}
void queuePageSet(uint8_t cueue_type,int page,int active)
{
	int cidx = cueueidx[cueue_type]-1;

	page_matrix[page+cidx*8] = active;
}


void registerAnimation(const init_fun init,const tick_fun tick, const deinit_fun deinit,const uint16_t cueue,const uint8_t cueue_type,const uint16_t t, const float count,char * name)
{
	if(cueue_type == TYPE_NORMAL)
	{
		addToCueue(cueue,cueue_type,init,tick,deinit,count*t,1000000/t,name);
	}
	else
	{
		addToCueue(cueue,cueue_type,init,tick,deinit,0,0,name);
	}
}


int main(int argc __attribute__((__unused__)), char *argv[] __attribute__((__unused__))) {


	osc_connect("10.0.1.2");
	//osc_connect("169.254.112.29");

	osc_start_server();
	uint16_t osc_current_page = 0;
	uint16_t osc_current_tab = 1;


#ifdef SDL_OUTPUT
	SDL_Surface* screen;
	screen = SDL_SetVideoMode(500,500,32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	int last_scan_x=128;
	int last_scan_y=128;
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
		if(cueues[cidx].active == 1)
			cueues[cidx].list[cueues[cidx].active_item].init_fp();
	}

	uint32_t tick_count_ui = 0;

	unsigned long long last_frame_ui=0;
	unsigned long long last_frame_fader_ui=0;

	struct timeval tv;




	while(running) {




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


	struct osc_event oscev;

	while(poll_osc_event(&oscev))
	{
		//printf("EVENT %i %i %i %f\n",oscev.type,oscev.a,oscev.b,oscev.value);
		if(oscev.type == 4)
		{
			update_ui=1;
			osc_current_page = oscev.a-1;
		}
		else if(oscev.type == 6)
		{
			if(manual[oscev.a+osc_manual_ch_offset]==1)
			{
				manual[oscev.a+osc_manual_ch_offset]=0;
			}
			else
			{
				manual[oscev.a+osc_manual_ch_offset]=1;
			}
		}
		else if(oscev.type == 7)
		{
			override[oscev.a+osc_manual_ch_offset] = 255.0f * oscev.value;
		}
		else if(oscev.type == 10)
		{
			custom_master[oscev.a] = oscev.value;
		}
		else if(oscev.type == 8)
		{
			if((oscev.a == 1)&&(osc_manual_ch_offset < 239))
			{
				osc_manual_ch_offset_fine++;
			}
			else if((oscev.a == 0)&&(osc_manual_ch_offset > 0))
			{
				osc_manual_ch_offset_fine--;
			}
			osc_manual_ch_offset = osc_manual_ch_offset_fine>>3;
		}
		else if(oscev.type == 9)
		{
			if((oscev.a == 1)&&(osc_manual_ch_offset < 238))
			{
				osc_manual_ch_offset_fine+=16;
			}
			else if((oscev.a == 0)&&(osc_manual_ch_offset > 1))
			{
				osc_manual_ch_offset_fine-=16;
			}
			osc_manual_ch_offset = osc_manual_ch_offset_fine>>3;
		}
		else if(oscev.type == 5)
		{
			osc_current_tab=oscev.a;
		}
		else
		{
			int line = oscev.a-1;

			int qidx = -1;

			for(int i = 0; i < 32;i++)
			{
				if(line == 0)
				{
					qidx = i;
				}
				if(page_matrix[osc_current_page+i*8] == 1)
				{
					line--;
				}
			}
			
			if((qidx != -1)&&(qidx < cueuecount))
			{
				if(oscev.type == 1)
				{
					if(oscev.b-1 < cueues[qidx].length)
					{
						update_ui=1;

						if(cueues[qidx].paused==1)
						{


							int new_idx =  oscev.b-1;

							// why this loop (relict from linked-list times?)
							int error = 0;
							while(new_idx != cueues[qidx].active_item)
							{
								error++;
								if(error > 20)
								{
									printf("error1a\n");
									exit(0);
								}
								update_ui=1;

								cueues[qidx].list[cueues[qidx].active_item].deinit_fp();

								cueues[qidx].active_item++;
								if(cueues[qidx].length == cueues[qidx].active_item)
									cueues[qidx].active_item=0;

								cueues[qidx].tick=0;
								cueues[qidx].list[cueues[qidx].active_item].init_fp();
							}	
						}
						else
						{
							if(cueues[qidx].list[oscev.b-1].active==0)
							{
								cueues[qidx].list[oscev.b-1].active=1;
								cueues[qidx].active_elements++;
							}
							else
							{
								if(cueues[qidx].active_elements > 1)
								{
									cueues[qidx].list[oscev.b-1].active=0;
									cueues[qidx].active_elements--;
								}
							}
						}
					}
				}
				else if(oscev.type == 2)
				{
					update_ui=1;
					if(cueues[qidx].active==1)
					{
						cueues[qidx].active=0;
						cueues[qidx].list[cueues[qidx].active_item].deinit_fp();
					}
					else
					{
						cueues[qidx].active=1;
						cueues[qidx].list[cueues[qidx].active_item].init_fp();
					}
				}
				else if(oscev.type == 3)
				{
					update_ui=1;
					if(oscev.b == 1)
					{
						if(cueues[qidx].paused==1)
						{
							cueues[qidx].paused=0;
						}
						else
						{
							cueues[qidx].paused=1;
						}
					}
					else if(oscev.b ==2)
					{
						if(cueues[qidx].random==1)
						{
							cueues[qidx].random=0;
						}
						else
						{
							cueues[qidx].random=1;
						}
					}
					else if(oscev.b ==3)
					{
					}
					else if(oscev.b ==4)
					{
					}

				}
			}
		}
	}



	if(update_ui)
	{
		update_ui=0;

		osc_update_page(osc_current_page);


		int i = 0;
		while((i<32)&&(page_matrix[osc_current_page+i*8]==0)) i++;


		for(uint16_t j = 0;j < 6;j++)
		{

			if((i < cueuecount)&&(page_matrix[osc_current_page+i*8]==1))
			{


				osc_update_queue_label(j,cueues[i].name);
				if(cueues[i].active == 1)
				{
					osc_update_queue_active(j,1);
				}
				else
				{
					osc_update_queue_active(j,0);
				}
				if(cueues[i].paused == 0)
				{
					osc_update_queue_ctrl(j,1,1);
				}
				else
				{
					osc_update_queue_ctrl(j,1,0);
				}
				if(cueues[i].active_remote == 1)
				{
					osc_update_queue_ctrl(j,4,1);
				}
				else
				{
					osc_update_queue_ctrl(j,4,0);
				}

				for(int x =0;x<16;x++)
				{
					if(x < cueues[i].length)
					{
						osc_update_queue_entry_label(j,x,cueues[i].list[x].name);
						osc_update_queue_entry_led(j,x,1,1);
						osc_update_queue_entry_led(j,x,2,0);
						if(cueues[i].active_item == x)
						{
							osc_update_queue_entry_led(j,x,3,1);
						}
						else
						{
							osc_update_queue_entry_led(j,x,3,0);
						}
						if(cueues[i].list[x].active==1)
						{
							osc_update_queue_entry_button(j,x,1);
						}
						else
						{
							osc_update_queue_entry_button(j,x,0);
						}
						if(cueues[i].list[x].active_remote==1)
						{
							osc_update_queue_entry_led(j,x,2,1);
						}
						else
						{
							osc_update_queue_entry_led(j,x,2,0);
						}
					}
					else
					{
						osc_update_queue_entry_label(j,x,"");
						osc_update_queue_entry_led(j,x,1,0);
						osc_update_queue_entry_led(j,x,2,0);
						osc_update_queue_entry_led(j,x,3,0);
						osc_update_queue_entry_button(j,x,0);
					}

				}
			}
			else 
			{
				osc_update_queue_label(j,"");
				osc_update_queue_active(j,0);
				osc_update_queue_ctrl(j,1,0);
				osc_update_queue_ctrl(j,2,0);
				osc_update_queue_ctrl(j,3,0);
				osc_update_queue_ctrl(j,4,0);

				for(int x =0;x<16;x++)
				{
					osc_update_queue_entry_label(j,x,"");
					osc_update_queue_entry_led(j,x,1,0);
					osc_update_queue_entry_led(j,x,2,0);
					osc_update_queue_entry_led(j,x,3,0);
					osc_update_queue_entry_button(j,x,0);
				}
			}
			do
			{
				i++;
			}
			while((i<32)&&(page_matrix[osc_current_page+i*8]==0));


		}
		osc_send_flush();

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
		else if(((uint32_t)time_diff > cueues[cidx].list[cueues[cidx].active_item].timing)&&((cueues[cidx].active == 1)||(cueues[cidx].active_remote == 1)))
		{
			//printf("fps %i : %f\n",cidx,1.0f/time_diff*1000000.0f);

			cueues[cidx].list[cueues[cidx].active_item].tick_fp();
			if(cueues[cidx].paused==0) cueues[cidx].tick++;
			gettimeofday(&tv,NULL);
			cueues[cidx].last_frame = tv.tv_usec ;

		}



	}
	
	
	for(int i = 0;i<512;i++)
	{
		if(manual[i] == 0)
		{
			out[i]=ch[i];
		}
		else
		{
			out[i]=override[i];
		}
	}

	{
		unsigned long long current_time;
		int32_t time_diff;
		gettimeofday(&tv,NULL);
		current_time = tv.tv_usec;
		time_diff = current_time - last_frame_fader_ui;

		if(time_diff < 0 )
		{
			time_diff+=1000000;
		}

		if((uint32_t)time_diff > 50000) // 20Hz
		{
			time_diff -= 50000;
			gettimeofday(&tv,NULL);
			last_frame_fader_ui = tv.tv_usec - time_diff;


			if(osc_current_tab == 2)
			{
				for(int i=0;i<16;i++)
				{
					osc_update_fader(i,i+osc_manual_ch_offset,out[1+i+osc_manual_ch_offset]);	
					osc_update_manual_state(i,manual[1+i+osc_manual_ch_offset]);	
				}
				osc_update_xy(out[4],out[5]);	
				osc_send_flush();
			}

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
		SDL_Rect rect10 = { 50, 0, 300, 50 };
		SDL_FillRect(
				screen, 
				&rect10, 
				SDL_MapRGB(screen->format,100,100,100)
				);
		SDL_Rect rect8 = { 55, 5, 40, 40 };
		SDL_FillRect(
				screen, 
				&rect8, 
				SDL_MapRGB(screen->format,out[54],out[55],out[56])
				);
		SDL_Rect rect = { 105, 5, 40, 40 };
		SDL_FillRect(
				screen, 
				&rect, 
				SDL_MapRGB(screen->format,out[29],out[30],out[31])
				);
		SDL_Rect rect2 = { 155, 5, 40, 40 };
		SDL_FillRect(
				screen, 
				&rect2, 
				SDL_MapRGB(screen->format,out[35],out[36],out[37])
				);
		SDL_Rect rect3 = { 205, 5, 40, 40 };
		SDL_FillRect(
				screen, 
				&rect3, 
				SDL_MapRGB(screen->format,out[41],out[42],out[43])
				);
		SDL_Rect rect4 = { 255, 5, 40, 40 };
		SDL_FillRect(
				screen, 
				&rect4, 
				SDL_MapRGB(screen->format,out[47],out[48],out[49])
				);
		SDL_Rect rect9 = { 305, 5, 40, 40 };
		SDL_FillRect(
				screen, 
				&rect9, 
				SDL_MapRGB(screen->format,out[59],out[60],out[61])
				);

		/*SDL_Rect rect5 = { 100, 100, 300, 300 };
		  SDL_FillRect(
		  screen, 
		  &rect5, 
		  SDL_MapRGB(screen->format,0,0,0)
		  );*/
		SDL_Rect rect6 = { 100+last_scan_x, 100+last_scan_y, 10, 10 };
		SDL_FillRect(
				screen, 
				&rect6, 
				SDL_MapRGB(screen->format,0,55,55)
				);
		last_scan_x = ch[4];
		last_scan_y = ch[5];
		SDL_Rect rect7 = { 100+out[4], 100+out[5], 10, 10 };
		SDL_FillRect(
				screen, 
				&rect7, 
				SDL_MapRGB(screen->format,255,255,255)
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

	ret = ftdi_write_data(ftdi, out, 75);
	if (ret < 0)
	{
		fprintf(stderr,"write failed , error %d (%s)\n",ret, ftdi_get_error_string(ftdi));
	}
#endif
	usleep(3000);





	for(int cidx=0;cidx < cueuecount;cidx++)
	{

		if((cueues[cidx].tick >= cueues[cidx].list[cueues[cidx].active_item].duration)&&(cueues[cidx].active == 1)&&(cueues[cidx].active_remote == 0)&&(cueues[cidx].paused == 0)&&(cueues[cidx].in_test == 0)&&(cueues[cidx].in_off == 0))
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
		else if((cueues[cidx].list[cueues[cidx].active_item].active_remote!=1)&&(cueues[cidx].active_remote == 1)&&(cueues[cidx].in_test == 0)&&(cueues[cidx].in_off == 0))
		{
			update_ui = 1;

			cueues[cidx].list[cueues[cidx].active_item].deinit_fp();


			int error = 0;
			do
			{
				error++;
				if(error > 20)
				{
					printf("error2b\n");
					exit(0);
				}
				cueues[cidx].active_item++;
				if(cueues[cidx].length == cueues[cidx].active_item)
					cueues[cidx].active_item=0;
			}
			while(cueues[cidx].list[cueues[cidx].active_item].active_remote==0);

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

