#include "s5pv210.h"

void Rtc_Init(void)
{
	int wYear, wMonth,wDay,wDayOfWeek,wHour,wMinute,wSecond;

    wYear = 2008;
    wMonth = 8;
    wDay = 1;
    wDayOfWeek = 5;
    wHour= 0;
    wMinute = 0;
    wSecond = 0;
	
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = 13;	//年
    rBCDMON  = (unsigned char)TO_BCD(wMonth);		//月
    rBCDDAY	 = (unsigned char)TO_BCD(wDay);			//日	
	rBCDDATE = wDayOfWeek+1;						//星期
	rBCDHOUR = (unsigned char)TO_BCD(wHour);		//小时
	rBCDMIN  = (unsigned char)TO_BCD(wMinute);		//分
	rBCDSEC  = (unsigned char)TO_BCD(wSecond);		//秒
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}

