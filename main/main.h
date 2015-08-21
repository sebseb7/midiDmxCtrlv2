#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

#define KORG_CTRL
#define LAUNCHPAD


#define LIBFTDI

enum {
	QUADPHASE = 0,
	SCAN_2,
	SCAN_2_COL,
	SCAN_2_GOBO,
	SCAN_2_STROBE,
	SCAN_2_MULTI,
	SCAN_COL,
	SCAN_GOBO,
	SCAN_GOBO_R,
	SCAN_STR,
	SCAN_MOVE,
	PAR36_UV,
	PAR56,
	PAR36,
	CHASE,


	CUEUE_ENUMLENGTH
};

enum {
	TYPE_NORMAL = 1,
	TYPE_OFF,
	TYPE_TEST,
	TYPE_SIMPLE
};


typedef void (*init_fun)(void);
typedef void (*deinit_fun)(void);
typedef uint8_t (*tick_fun)(void);


void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t cueue, uint8_t cueue_type, uint16_t t, float duration,char * name);
void queueInitialization(uint8_t cueue_type,int active,int visible,int paused,char * name);
void queueRemoteActive(uint8_t cueue_type,int active,int entry);
void queuePageSet(uint8_t cueue_type,int page, int active);


void setLedX(uint8_t x, uint8_t r,uint8_t g,uint8_t b);
void setCh(uint8_t chan, uint8_t value);
uint8_t getIn(uint8_t chan);
uint8_t getFader(uint8_t chan);
void invLedX(uint8_t x);
void Delay(uint16_t t);
void getLedX(uint8_t x, uint8_t* red, uint8_t* green, uint8_t* blue);


#endif

