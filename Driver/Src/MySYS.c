#include "MySYS.h"

void Lock_Unlock_reg(uint8_t val){
	
	if(val){
	//The open sequence is to continue write the data “59h”, “16h” “88h” to the key controller address 0x5000_0100
	SYS->RegLockAddr = 0x59;
	SYS->RegLockAddr = 0x16;
	SYS->RegLockAddr = 0x88;//Unlock
	}
	else{
		SYS->RegLockAddr = 0x00;//Lock
	}
}

void CLK_Init(void){

	Lock_Unlock_reg(1);
	
	CLK->PWRCTL |= CLK_PWRCTL_HXT_EN_Msk;//HXT_EN 
	CLK->PWRCTL |= CLK_PWRCTL_LXT_EN_Msk;//LXT_EN
	
	CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);
	CLK_WaitClockReady(CLK_STATUS_LXTSTB_Msk);
	
	CLK->CLKSEL0 = 0x00;//HCLK Clock Source Selection (000-HXT,001-LXT,010-PLL clock,011-LIRC,111-HIRC)
	
	//CLK->PWRCTL &= ~(CLK_PWRCTL_HIRC_EN_Msk);//HIRC OFF
	//CLK->PWRCTL &= ~(CLK_PWRCTL_LIRC_EN_Msk);//LIRC OFF
	
	
	Lock_Unlock_reg(0);
	
	
}

uint32_t CLK_WaitClockReady(uint32_t u32ClkMask)
{
    int32_t i32TimeOutCnt = 2160000;
    uint32_t u32Ret = 1U;

    while((CLK->CLKSTATUS & u32ClkMask) != u32ClkMask)
    {
        if(i32TimeOutCnt-- <= 0)
        {
            u32Ret = 0U;
            break;
        }
    }
    return u32Ret;
}

void Systick_Init(void){//Init 1Khz = 1000ms = 1sec(12MHz source)
	
	SysTick->CTRL=0;
	SysTick->LOAD = 12000000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x05;//Disable interrupt (0x07 enable interrupt)
	
}

void Delay(uint32_t ms){
		SysTick->LOAD = 12000*ms-1;//24bit 16 777 215 ~1.3 sec max
		SysTick->VAL = 0;
		while( !(SysTick->CTRL&(1<<16)) );//wait interrupt flag

}

//	SYS->PB_H_MFP |= 0x04<<SYS_PB_H_MFP_PB12_MFP_Pos;
//	CLK->APBCLK |= (1<<6);//Frequency Divider Output Clock Enable Control FDIV_N
//	//CLK->CLKSEL2 &= ~0x06;
//	CLK->FRQDIV |= (1<<4);//Frequency Divider Enable Bit FDIV_EN