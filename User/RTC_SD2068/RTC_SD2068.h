#include "Nano100Series.h"

#define RTC_SD2068_ADDR 0x64
#define RTC_DS1307_ADDR 0xD0

#define HEX2BCD(v)	((v) % 10 + (v) / 10 * 16)
#define BCD2HEX(v)	((v) % 16 + (v) / 16 * 10)

/* 0x00-Sec,0x01-Min,0x02-Hour,0x03-Week,0x04-Day,0x05-Month,0x06-Year *///Time
/* 0x07-Sec,0x08-Min,0x09-Hour,0x0A-Week,0x0B-Day,0x0C-Month,0x0D-Year,0x0E-? *///Calendar

//unlock write protection
void RTC_SD2068_Unlock(void);
void RTC_SD2068_SetTime(uint8_t sec,uint8_t min,uint8_t hour,uint8_t week,
												uint8_t day,uint8_t month,uint8_t year);
												
								