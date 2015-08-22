#ifndef _ET_DEVICES_H
#define _ET_DEVICES_H

#define ADDR_PAR1 29
#define ADDR_PAR2 35
#define ADDR_PAR3 41
#define ADDR_PAR4 47
#define ADDR_PAR5 53
#define ADDR_PAR6 58

int parmap[] = {ADDR_PAR5+1,ADDR_PAR1,ADDR_PAR2,ADDR_PAR3,ADDR_PAR4,ADDR_PAR6+1};
void setpar(int lamp,int ch,int val)
{
	if(lamp < 6)
		setCh(parmap[lamp]+ch,val);
}

#endif
