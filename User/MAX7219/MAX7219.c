#include "MAX7219.h"


void Write_7219(uint8_t add,uint8_t data){//write value in reg(0x01-0x0F)
	
	PC->DOUT &= ~(1<<0);//CS LOW
	
	uint16_t temp = 0;
	temp = ((add<<8) | data);
	Write_spi(temp);

	PC->DOUT |= (1<<0);//CS HIGH

}

void Init_7219(void){
	Write_7219(0x0F, 0x00);//Display-Test Register
	Write_7219(0x0C, 0x01);//Shutdown Register,Normal Operation
	Write_7219(0x09, 0xFF);//Decode-Mode Register,decode for digits 7–0
	Write_7219(0x0A, 1);//Intensity Register
	Write_7219(0x0B, 0x07);//Scan-Limit Register,Display digits 0 1 2 3 4 5 6 7
	
	Clear_7219();
}

void Clear_7219(void){
	for(uint8_t i = 0;i<8;i++){
		Write_7219(i+1,0x0F);//Blank code
	}
}

void Number_7219(uint32_t num,uint8_t position)
{
	
	while (num)
	{		
		Write_7219(position,num%10);
		position++;
		num/=10;		
	}
	
}

void RTC_7219(uint8_t * sec,uint8_t * min,uint8_t * hour){
	
		Write_7219(1,*sec%10);
		Write_7219(2,*sec/10);
		
		Write_7219(4,*min%10);
		Write_7219(5,*min/10);
		
		Write_7219(7,*hour%10);
		Write_7219(8,*hour/10);		
		

}
