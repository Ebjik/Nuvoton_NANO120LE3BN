#include "Timer.h"


void Timer_init(void){
	
	CLK->APBCLK |= CLK_APBCLK_TMR0_EN_Msk;//Timer0 Clock Enable
	CLK->CLKSEL1 &= ~(0x07 << CLK_CLKSEL1_TMR0_S_Pos);//Timer0 Clock Source (000-HXT,001-LXT,010-LIRC,011-external pin,1xx-HIRC)
	TIMER0->CTL |= (0x01 << TIMER_CTL_MODE_SEL_Pos);//The timer is operating in the periodic mode.
	TIMER0->PRECNT |= 0x00;//Timer x Pre-Scale Counter Register
	TIMER0->CMPR = 2400000;//Timer x Compare Register 0xB71B00-1 2400000
	TIMER0->IER |= TIMER_IER_TMR_IE_Msk;//Enable Interrupt 
	//TIMER0->IER |= TIMER_IER_TCAP_IE_Msk;
	TIMER0->CTL |= TIMER_CTL_TMR_EN_Msk;//Starts counting
	NVIC_EnableIRQ(TMR0_IRQn);
	
}