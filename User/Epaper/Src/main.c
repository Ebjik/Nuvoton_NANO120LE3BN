#include "Nano100Series.h"
#include "Epaper_lib.h"
#include "SHT20.h"
#include "RTC_SD2068.h"
#include "stdio.h"



#define LED_RED 13
#define LED_YEL 14
#define LED_GREEN 15



//void TMR0_IRQHandler(void){

//}

//void SysTick_Handler(void){
//	//PA->DOUT ^= (HIGH<<LED_GREEN);	
//}

int main (void)
{
	CLK_Init();
	Systick_Init();
	SPI0_init();
	I2C_Init(I2C1,0);
	Epaper_init();
	
	PA->PMD |= (OUTPUT<<GP_PMD_PMD13_Pos);
	PA->PMD |= (OUTPUT<<GP_PMD_PMD14_Pos);
	PA->PMD |= (OUTPUT<<GP_PMD_PMD15_Pos);

	PA->DOUT &= ~(HIGH<<LED_RED);
	PA->DOUT &= ~(HIGH<<LED_YEL);
	PA->DOUT &= ~(HIGH<<LED_GREEN);
	
	uint8_t count = 15;

	//RTC_SD2068_SetTime(0,57,13,1,25,1,24);
	char arr[15];
	
	sprintf(arr,"T:%d",SHT20_ReadT());
	DrawStringAt(0,0,arr,1);	
	
	sprintf(arr,"RH:%d",SHT20_ReadRH());
	DrawStringAt(0,25,arr,1);


//	I2C0_TxRx(0x00,arr,0x64,7);
//	arr[0]=BCD2HEX(arr[0]);
//	arr[1]=BCD2HEX(arr[1]);
//	arr[2]=BCD2HEX(arr[2]&0x7F);
//	sprintf(arr,"%d:%d:%d",arr[2],arr[1],arr[0]);
//	DrawStringAt(0,50,arr,1);
		
//	I2C0_TxRx(0x04,arr,0x65,3);
//	arr[0]=BCD2HEX(arr[0]);
//	arr[1]=BCD2HEX(arr[1]);
//	arr[2]=BCD2HEX(arr[2]);
//	sprintf(arr,"%d-%d-20%d",arr[0],arr[1],arr[2]);
//	DrawStringAt(0,75,arr,1);
	
	//char * Wday[]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};

	Displayimage();



while(1){
	
	PA->DOUT ^= (HIGH<<count);
	if(--count<13)count=15;
	Delay(200);

}


}





