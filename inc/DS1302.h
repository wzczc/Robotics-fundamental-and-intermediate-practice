#ifndef __DS1302_H
#define __DS1302_H

#include "msp.h"
#include "Clock.h"

struct ds1302time
{
	int	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	week;
	unsigned char	hour;
	unsigned char	minute;
	unsigned char	second;
};

void DS1302_read_time( struct ds1302time *p );  // ��ȡʱ��
void DS1302_write_time( struct ds1302time *p );  // д��ʱ��
int getWeekdayByYearday( int iY, int iM, int iD );  // ��ȡ���ڼ�
//void DS1302_Read_Time( unsigned char *ptr );


#endif

