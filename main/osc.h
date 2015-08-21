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
void osc_send_ff(const char * 	path,float num,float num2);
void osc_send_s(const char * 	path,const char * value);
void osc_send_flush(void);
void osc_start_server(void);


int poll_osc_event(struct osc_event* event_ref);
				
				
void osc_update_queue_label(uint16_t idx,const char * value);
void osc_update_queue_active(uint16_t idx,uint16_t value);
void osc_update_queue_entry_label(uint16_t idx,uint16_t entry_idx, const char * value);
void osc_update_queue_entry_led(uint16_t idx,uint16_t entry_idx, uint16_t led,uint16_t value);
void osc_update_queue_entry_button(uint16_t idx,uint16_t entry_idx, uint16_t value);
void osc_update_queue_ctrl(uint16_t idx,uint16_t ctrl_idx, uint16_t value);
void osc_update_page(uint16_t idx);
void osc_update_fader(uint16_t slot,uint16_t ch,uint16_t value);	
void osc_update_xy(uint16_t x,uint16_t y);
void osc_update_manual_state(uint16_t slot,uint16_t state);

#endif


