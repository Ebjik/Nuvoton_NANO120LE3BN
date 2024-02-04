#include "SHT20.h"

//-40-125 Operating Range
int8_t SHT20_ReadT(){
	
	uint8_t raw[2];
	uint16_t tmp;
	if( I2C_TxRx(I2C1,THOLD,raw,SHT20_ADDR,2) != 0)return 0;
	tmp = (raw[0]<<8);
	tmp |= raw[1];
	return -46.85 + 175.72 / 65536 * tmp;//calculating temp in Celsius 
	
}
//Operating Range 0-100 %
uint16_t SHT20_ReadRH(){
	
	uint8_t raw[2];
	uint16_t tmp;
	if( I2C_TxRx(I2C1,RHHOLD,raw,SHT20_ADDR,2) != 0)return 0;
	tmp = (raw[0]<<8);
	tmp |= raw[1];
	return	(125*tmp)/0xFFFF-6;
	
	
}
//    float tempRH = rawHumidity * (125.0 / 65536.0);
//    float rh = tempRH - 6.0;