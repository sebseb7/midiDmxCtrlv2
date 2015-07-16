#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define CONSTRUCTOR_ATTRIBUTES	__attribute__((constructor));

#define KORG_CTRL
#define LAUNCHPAD


//#define LIBFTDI

enum {
	QUADPHASE = 0,
	PAR36_UV,
	PAR56,
	PAR36,
	FOG,


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


void registerAnimation(init_fun init,tick_fun tick,deinit_fun deinit, uint16_t cueue, uint8_t cueue_type, uint16_t t, uint16_t duration);
void queueInitialization(uint8_t cueue_type,int playmode);

void setLedX(uint8_t x, uint8_t r,uint8_t g,uint8_t b);
void setCh(uint8_t chan, uint8_t value);
uint8_t getIn(uint8_t chan);
void invLedX(uint8_t x);
void Delay(uint16_t t);
void getLedX(uint8_t x, uint8_t* red, uint8_t* green, uint8_t* blue);


#endif

