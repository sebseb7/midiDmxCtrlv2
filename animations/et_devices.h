#ifndef _ET_DEVICES_H
#define _ET_DEVICES_H

#define ADDR_PAR1 29
#define ADDR_PAR2 35
#define ADDR_PAR3 41
#define ADDR_PAR4 47
#define ADDR_PAR5 53
#define ADDR_PAR6 58

#define ADDR_STROBE_A 63
#define ADDR_STROBE_B 65


#define ADDR_SCAN_1 1
#define ADDR_SCAN_2 8
#define ADDR_SCAN_3 15
#define ADDR_SCAN_4 22

__attribute__((unused)) static int parmap[] = {ADDR_PAR5+1,ADDR_PAR1,ADDR_PAR2,ADDR_PAR3,ADDR_PAR4,ADDR_PAR6+1};
__attribute__((unused)) static void setpar(int lamp,int ch,int val)
{
	if(lamp < 6)
		setCh(parmap[lamp]+ch,val);
}

__attribute__((unused)) static int scanmap[] = {ADDR_SCAN_1,ADDR_SCAN_2,ADDR_SCAN_3,ADDR_SCAN_4};
__attribute__((unused)) static void setscan(int lamp,int ch,int val)
{
	if(lamp < 4)
		setCh(scanmap[lamp]+ch-1,val);
}

__attribute__((unused)) static void setallscan(int ch,int val)
{
	for(int i = 0;i<4 ;i++)
		setCh(scanmap[i]+ch-1,val);
}

#endif
