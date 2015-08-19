#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "osc.h"

#include "lo/lo.h"

static lo_address t;
static lo_server s;
//static lo_bundle current_bundle;
//static uint16_t bundle_size;
#define EV_SIZE 200

static struct osc_event eventsbuf[EV_SIZE];
static struct osc_event *evhead, *evtail;

void osc_connect(const char * 	host)
{
	s = lo_server_new(NULL,NULL);
	t = lo_address_new(host, "9000");
//	current_bundle = lo_bundle_new(LO_TT_IMMEDIATE);
//	bundle_size=0;

}
void osc_send_flush(void)
{
	//lo_send_bundle_from(t,s,current_bundle);
//	lo_send_bundle(t,current_bundle);
//	lo_bundle_free(current_bundle);
//	current_bundle = lo_bundle_new(LO_TT_IMMEDIATE);
//	bundle_size=0;

};
void osc_send_f(const char * 	path,float num)
{
//	lo_message mess = lo_message_new();

//	lo_message_add_float(mess,num);
	
//	lo_bundle_add_message(current_bundle,path,mess);
//	bundle_size++;
//	if(bundle_size > 2)
//		osc_send_flush();
	//if (lo_send(t, path, "f", num) == -1) {
	//	printf("OSC error %d: %s\n", lo_address_errno(t),
	//			lo_address_errstr(t));
	//}
	if (lo_send_from(t,s,LO_TT_IMMEDIATE, path, "f", num) == -1) {
		printf("OSC error %d: %s\n", lo_address_errno(t),
				lo_address_errstr(t));
	}

}
void osc_send_s(const char * 	path,const char * value)
{
//	lo_message mess = lo_message_new();

//	lo_message_add_string(mess,value);
	
//	lo_bundle_add_message(current_bundle,path,mess);
//	bundle_size++;
//	if(bundle_size > 2)
//		osc_send_flush();
	//if (lo_send(t, path, "s", value) == -1) {
	if (lo_send_from(t,s,LO_TT_IMMEDIATE, path, "s", value) == -1) {
		printf("OSC error %d: %s\n", lo_address_errno(t),
				lo_address_errstr(t));
	}

}

void error(int num, const char *msg, const char *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}

int generic_handler(const char *path, const char *types, lo_arg ** argv,
		int argc, __attribute__((unused)) void *data, __attribute__((unused)) void *user_data)
{
	int i;



	const char delimiters[] = "/_";
	char *running;
	char *copy;
	char *token;

	running = strdup(path);
	copy=running;
	token = strsep (&running, delimiters);
	token = strsep (&running, delimiters);


	int osc_type = 0;
	int osc_a;
	int osc_b;
	float osc_value;
	
	if(strcmp("1",token)==0)
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
	}
	free(copy);



	printf("path: <%s>\n", path);
	for (i = 0; i < argc; i++) {
		printf("arg %d '%c' ", i, types[i]);
		lo_arg_pp((lo_type)types[i], argv[i]);
		printf("\n");
	}
	printf("\n");
	fflush(stdout);
	
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

	lo_server_thread st = lo_server_thread_new("8000", error);
	lo_server_thread_add_method(st, NULL, NULL, generic_handler, NULL);
	lo_server_thread_start(st);
}

void osc_update_queue_label(uint16_t idx,const char * value)
{
	char path[200];
	sprintf(path, "/1/q_%i_label",idx+1);
	osc_send_s(path,value);
}
void osc_update_queue_active(uint16_t idx,uint16_t value)
{
	char path[200];
	sprintf(path, "/1/q_%i_en/1/1",idx+1);
	osc_send_f(path,value);
}
void osc_update_queue_entry_label(uint16_t idx,uint16_t entry_idx,const char * value)
{
	char path[200];
	sprintf(path, "/1/q_%i_%i_label",idx+1,entry_idx+1);
	osc_send_s(path,value);
}
void osc_update_queue_entry_led(uint16_t idx,uint16_t entry_idx, uint16_t led,uint16_t value)
{
	char path[200];
	sprintf(path, "/1/q_%i_l_%i_%i",idx+1,entry_idx+1,led);
	osc_send_f(path,value);
}
void osc_update_queue_entry_button(uint16_t idx,uint16_t entry_idx,uint16_t value)
{
	char path[200];
	sprintf(path, "/1/q_%i_toggle/1/%i",idx+1,entry_idx+1);
	osc_send_f(path,value);
}
void osc_update_queue_ctrl(uint16_t idx,uint16_t ctrl_idx,uint16_t value)
{
	char path[200];
	sprintf(path, "/1/q_%i_ctrl/1/%i",idx+1,ctrl_idx);
	osc_send_f(path,value);
}

