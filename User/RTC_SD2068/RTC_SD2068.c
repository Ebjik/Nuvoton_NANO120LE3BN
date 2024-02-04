#include "RTC_SD2068.h"

uint8_t unlock = 0;

void RTC_SD2068_Unlock(void){
	
	uint8_t reg[2]={0x84,0x80};
//	I2C0_Master_Write(0x0F,reg,RTC_SD2068_ADDR,2);
	unlock = 1;
}
void RTC_SD2068_SetTime(uint8_t sec,uint8_t min,uint8_t hour,uint8_t week,
												uint8_t day,uint8_t month,uint8_t year){
												
	uint8_t data[7];
	data[0]=HEX2BCD(sec);
	data[1]=HEX2BCD(min);
	data[2]=HEX2BCD(hour);
	data[3]=HEX2BCD(week);
	data[4]=HEX2BCD(day);
	data[5]=HEX2BCD(month);
	data[6]=HEX2BCD(year);
	
	if(!unlock)RTC_SD2068_Unlock();
//	I2C0_Master_Write(0x00,data,RTC_SD2068_ADDR,7);
}