#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "osc.h"
#include "main.h"

#include "lo/lo.h"

static int fader_init[16] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void osc_page_flip(void);

__attribute__((unused)) static lo_address t;
__attribute__((unused)) static lo_server s;
__attribute__((unused)) static lo_bundle current_bundle;
__attribute__((unused)) static uint16_t bundle_size;
#define EV_SIZE 200

static struct osc_event eventsbuf[EV_SIZE];
static struct osc_event *evhead, *evtail;

void osc_connect(__attribute__((unused)) const char * 	host)
{
#ifdef OSC_OUT
	s = lo_server_new(NULL,NULL);
	t = lo_address_new(host, "9000");
	current_bundle = lo_bundle_new(LO_TT_IMMEDIATE);
	bundle_size=0;
#endif

}
void osc_send_flush(void)
{
#ifdef OSC_OUT
	if(bundle_size>0)
	{
		lo_send_bundle_from(t,s,current_bundle);
		lo_bundle_free(current_bundle);
		current_bundle = lo_bundle_new(LO_TT_IMMEDIATE);
		bundle_size=0;
	}
#endif

};
void osc_send_f(__attribute__((unused)) const char * 	path,__attribute__((unused)) float num)
{
#ifdef OSC_OUT
	lo_message mess = lo_message_new();

	lo_message_add_float(mess,num);

	char * newpath =  strdup(path);
	lo_bundle_add_message(current_bundle,newpath,mess);
	bundle_size++;
	if(bundle_size > 200)
		osc_send_flush();

#endif
}
void osc_send_ff(__attribute__((unused)) const char * 	path,__attribute__((unused)) float num,__attribute__((unused)) float num2)
{
#ifdef OSC_OUT
	lo_message mess = lo_message_new();

	lo_message_add_float(mess,num);
	lo_message_add_float(mess,num2);

	char * newpath =  strdup(path);
	lo_bundle_add_message(current_bundle,newpath,mess);
	bundle_size++;
	if(bundle_size > 200)
		osc_send_flush();
#endif

}
void osc_send_s(__attribute__((unused)) const char * 	path,__attribute__((unused)) const char * value)
{
#ifdef OSC_OUT
	lo_message mess = lo_message_new();

	lo_message_add_string(mess,value);
	
	char * newpath =  strdup(path);
	lo_bundle_add_message(current_bundle,newpath,mess);
	bundle_size++;
	if(bundle_size > 200)
		osc_send_flush();

#endif

}

void error(int num, const char *msg, const char *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}

int generic_handler(const char *path, __attribute__((unused)) const char *types, lo_arg ** argv,
		__attribute__((unused)) int argc, __attribute__((unused)) void *data, __attribute__((unused)) void *user_data)
{



	const char delimiters[] = "/_";
	char *running;
	char *copy;
	char *token;

	running = strdup(path);
	copy=running;
	token = strsep (&running, delimiters);
	token = strsep (&running, delimiters);


	int osc_type = 0;
	int osc_a=0;
	int osc_b=0;
	float osc_value=0.0f;

	if(strcmp("1",token)==0)
	{
		printf("%s\n",running);
		if(running != NULL)
		{
			token = strsep (&running, delimiters);
			if(strcmp("q",token)==0)
			{
				token = strsep (&running, delimiters);
				osc_a = atoi(token);
				token = strsep (&running, delimiters);

				if(strcmp("toggle",token)==0)
				{
					osc_type = 1;
					token = strsep (&running, delimiters);
					if(strcmp("1",token)==0)
					{
						token = strsep (&running, delimiters);
						osc_b = atoi(token);
						osc_value = argv[0]->f;
					}
				}
				if(strcmp("ctrl",token)==0)
				{
					osc_type = 3;
					token = strsep (&running, delimiters);
					if(strcmp("1",token)==0)
					{
						token = strsep (&running, delimiters);
						osc_b = atoi(token);
						osc_value = argv[0]->f;
					}
				}
				else if(strcmp("en",token)==0)
				{
					osc_type = 2;
					osc_value = argv[0]->f;
				}
			}
			else if(strcmp("pageflip",token)==0)
			{
				token = strsep (&running, delimiters);
				token = strsep (&running, delimiters);
				osc_a = atoi(token);
				osc_type = 4;
				osc_page_flip();
			}
		}
		else
		{
			osc_type=5;
			osc_a=1;
		}
	}
	else if(strcmp("2",token)==0)
	{
		if(running != NULL)
		{
			token = strsep (&running, delimiters);
			if(strcmp("override",token)==0)
			{
				token = strsep (&running, delimiters);
				token = strsep (&running, delimiters);
				osc_a = atoi(token);
				osc_type = 6;
			}
			else if(strcmp("fader",token)==0)
			{
				token = strsep (&running, delimiters);
				osc_a = atoi(token);
				osc_value = argv[0]->f;
				fader_init[osc_a-1]=-1;
				osc_type = 7;
			}
			else if(strcmp("encoder1",token)==0)
			{
				osc_a = argv[0]->f;
				osc_type = 8;
			}
			else if(strcmp("encoder2",token)==0)
			{
				osc_a = argv[0]->f;
				osc_type = 9;
			}
		}
		else
		{
			osc_type=5;
			osc_a=2;
		}

	}
	else if(strcmp("3",token)==0)
	{
		if(running != NULL)
		{
		}
		else
		{
			osc_type=5;
			osc_a=3;
		}

	}

	else if(strcmp("4",token)==0)
	{
		if(running != NULL)
		{
			token = strsep (&running, delimiters);
			if(strcmp("multifader",token)==0)
			{
				token = strsep (&running, delimiters);
				osc_a = atoi(token)-1;
				osc_value = argv[0]->f * 2;
				osc_type = 10;
			}

		}
		else
		{
			osc_type=5;
			osc_a=4;
		}

	}




	int i;
	printf("path: <%s>\n", path);
	for (i = 0; i < argc; i++) {
		printf("arg %d '%c' ", i, types[i]);
		lo_arg_pp((lo_type)types[i], argv[i]);
		printf("\n");
	}
	printf("\n");
	fflush(stdout);


	free(copy);


	if(osc_type != 0)
	{
		int diff;
		diff = evhead - evtail;
		if (diff < 0) diff += EV_SIZE;
		if (diff < EV_SIZE -1)
		{
			evhead->type = osc_type;
			evhead->a = osc_a;
			evhead->b = osc_b;
			evhead->value = osc_value;
			++evhead;
			if (evhead == (eventsbuf + EV_SIZE)) evhead = eventsbuf;
		}
	}

	return 1;
}

int poll_osc_event(struct osc_event* event_ref)
{
	if (evhead==evtail) return 0;
	event_ref->type =  evtail->type;
	event_ref->a =  evtail->a;
	event_ref->b =  evtail->b;
	event_ref->value =  evtail->value;
	if (++evtail == (eventsbuf + EV_SIZE)) evtail = eventsbuf;
	return 1;
}

void osc_start_server(void)
{
	evhead = evtail = eventsbuf;

#ifdef OSC_OUT
	lo_server_thread st = lo_server_thread_new("8000", error);
	lo_server_thread_add_method(st, NULL, NULL, generic_handler, NULL);
	lo_server_thread_start(st);
#endif
}




static int label_init[6] = {
	-1,-1,-1,-1,-1,-1 };

void osc_update_queue_label(uint16_t idx,const char * value)
{
	if(label_init[idx] == 1) return;
	char path[200];
	sprintf(path, "/1/q_%i_label",idx+1);
	osc_send_s(path,value);
	label_init[idx]=1;
}




static int active_init[6] = {
	-1,-1,-1,-1,-1,-1 };

void osc_update_queue_active(uint16_t idx,uint16_t value)
{
	if(active_init[idx] == value) return;
	char path[200];
	sprintf(path, "/1/q_%i_en/1/1",idx+1);
	osc_send_f(path,value);
	active_init[idx]=value;
}




static int entry_label_init[6*16] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

void osc_update_queue_entry_label(uint16_t idx,uint16_t entry_idx,const char * value)
{
	if(entry_label_init[(idx)*16+(entry_idx)] == 1) return;
	char path[200];
	sprintf(path, "/1/q_%i_%i_label",idx+1,entry_idx+1);
	osc_send_s(path,value);
	entry_label_init[(idx)*16+(entry_idx)]=1;
}



static int entry_led_init[3*6*16] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void osc_update_queue_entry_led(uint16_t idx,uint16_t entry_idx, uint16_t led,uint16_t value)
{
	if(entry_led_init[(idx*(16*3))+(entry_idx*3)+(led-1)] == value) return;
	char path[200];
	sprintf(path, "/1/q_%i_l_%i_%i",idx+1,entry_idx+1,led);
	osc_send_f(path,value);
	entry_led_init[(idx*(16*3))+(entry_idx*3)+(led-1)] = value;
}



static int button_init[6*16] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };

void osc_update_queue_entry_button(uint16_t idx,uint16_t entry_idx,uint16_t value)
{
	if(button_init[(idx)*16+(entry_idx)] == value) return;
	char path[200];
	sprintf(path, "/1/q_%i_toggle/1/%i",idx+1,entry_idx+1);
	osc_send_f(path,value);
	button_init[(idx)*16+(entry_idx)]=value;
}



static int ctrl_init[6*4] = {
	-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1, 
	-1,-1,-1,-1,-1,-1};

void osc_update_queue_ctrl(uint16_t idx,uint16_t ctrl_idx,uint16_t value)
{
	if(ctrl_init[(idx)*4+(ctrl_idx-1)] == value) return;
	char path[200];
	sprintf(path, "/1/q_%i_ctrl/1/%i",idx+1,ctrl_idx);
	osc_send_f(path,value);
	ctrl_init[(idx)*4+(ctrl_idx-1)] = value;
}

void osc_page_flip(void)
{
	for(int i = 0;i<6;i++) label_init[i]=-1;
	for(int i = 0;i<(6*16);i++) entry_label_init[i]=-1;
}

static int page_init = -1;
void osc_update_page(uint16_t idx)
{
	if(page_init == idx) return;
	char path[200];
	if(page_init != -1) sprintf(path, "/1/pageflip/1/%i",page_init+1);
	osc_send_f(path,0.0f);
	sprintf(path, "/1/pageflip/1/%i",idx+1);
	osc_send_f(path,1.0f);
	page_init = idx;
}




// this needs buffering!
static int fader_num_init[16] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void osc_update_fader(uint16_t slot,uint16_t ch,uint16_t value)
{
	
	if(fader_init[slot] != value)
	{

		fader_init[slot]=value;
	
		char path[200];
		char label[200];
	
		sprintf(path, "/2/val_%i",slot+1);
		sprintf(label, "%i",value);
		osc_send_s(path,label);

		sprintf(path, "/2/fader/%i",slot+1);
		osc_send_f(path,value/255.0f);
	}
	
	if(fader_num_init[slot] != ch)
	{
		fader_num_init[slot]=ch;
		char path[200];
		sprintf(path, "/2/nr_%i",slot+1);
		char label[200];
		sprintf(label, "%i",ch+1);
		osc_send_s(path,label);
	}
	


}

// this needs buffering!
void osc_update_xy(uint16_t x,uint16_t y)
{
	//osc_send_ff("/2/xy1",x/255.0f,y/255.0f);
}


static int manual_state_init[16] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void osc_update_manual_state(uint16_t slot,uint16_t state)
{
	if(manual_state_init[slot] == state) return;
	manual_state_init[slot]=state;
	char path[200];
	sprintf(path, "/2/override/1/%i",slot+1);
	osc_send_f(path,state);
}

