#ifndef _OSC_H
#define _OSC_H


struct osc_event {
	uint16_t type;
	uint16_t a;
	uint16_t b;
	float value;
};

void osc_connect(const char * 	host);
void osc_send_f(const char * 	path,float num);
void osc_send_s(const char * 	path,const char * value);
void osc_send_flush(void);
void osc_start_server(void);

void osc_setMatrix(int x, int y, int r, int g, int flash);
				
				
				
void osc_update_queue_label(uint16_t idx,const char * value);
void osc_update_queue_active(uint16_t idx,uint16_t value);
void osc_update_queue_entry_label(uint16_t idx,uint16_t entry_idx, const char * value);
void osc_update_queue_entry_led(uint16_t idx,uint16_t entry_idx, uint16_t led,uint16_t value);
void osc_update_queue_entry_button(uint16_t idx,uint16_t entry_idx, uint16_t value);

#endif


