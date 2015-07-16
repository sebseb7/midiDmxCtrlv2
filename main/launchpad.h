#ifndef _LAUNCHPAD_H
#define _LAUNCHPAD_H

#include "main.h"


void launchpad_init(void);
void launchpad_setMatrix(int x, int y, int r, int g, int flash);
void launchpad_setSide(int x,  int r, int g, int flash);
void launchpad_setTop(int y, int r, int g, int flash);
int launchpad_poll(KeyboardEvent* e);



#endif
