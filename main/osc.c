#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#include "osc.h"

#include "lo/lo.h"

static lo_address t;

#define EV_SIZE 200

static struct osc_event eventsbuf[EV_SIZE];
static struct osc_event *evhead, *evtail;

void osc_connect(const char * 	host)
{
	t = lo_address_new(host, "9000");

}
void osc_send_f(const char * 	path,float num)
{
	if (lo_send(t, path, "f", num) == -1) {
		printf("OSC error %d: %s\n", lo_address_errno(t),
				lo_address_errstr(t));
	}

}
void osc_send_s(const char * 	path,const char * value)
{
	if (lo_send(t, path, "s", value) == -1) {
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
		int argc, void *data, void *user_data)
{
	int i;



	const char delimiters[] = "/";
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
		if(strcmp("multipush1",token)==0)
		{
			token = strsep (&running, delimiters);
			osc_a = atoi(token);
			token = strsep (&running, delimiters);
			osc_b = atoi(token);
			osc_value = argv[0]->f;
			osc_type = 1;
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
//if (rxhead0==rxtail0) return 0;
//*c = *rxtail0;
//if (++rxtail0 == (rxbuf0 + UART_RXBUFSIZE)) rxtail0 = rxbuf0;

void osc_start_server(void)
{
	evhead = evtail = eventsbuf;

	lo_server_thread st = lo_server_thread_new("8000", error);
	lo_server_thread_add_method(st, NULL, NULL, generic_handler, NULL);
	lo_server_thread_start(st);
}

void osc_setMatrix(int x, int y, int r, int g, int flash)
{
	char path[200];

	sprintf(path, "/1/multipush1/%i/%i",x+1,y+1);

	printf("send %s\n",path);

	osc_send_f(path,1.0f);
}

void osc_update_queue_label(uint16_t idx,const char * value)
{
	char path[200];
	sprintf(path, "/1/q_%i_label",idx+1);
	osc_send_s(path,value);
}
void osc_update_queue_entry_label(uint16_t idx,uint16_t entry_idx,const char * value)
{
	char path[200];
	sprintf(path, "/1/q_%i_%i_label",idx+1,entry_idx+1);
	osc_send_s(path,value);
}

