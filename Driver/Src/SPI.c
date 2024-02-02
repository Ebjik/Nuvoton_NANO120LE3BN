#include "SPI.h"

void SPI0_init(void){
	SYS->PC_L_MFP |= ( 0x01 << SYS_PC_L_MFP_PC1_MFP_Pos);//SPI0 CLK MF
	//SYS->PC_L_MFP |= ( 0x01 << SYS_PC_L_MFP_PC2_MFP_Pos);//SPI0 MISO MF
	SYS->PC_L_MFP |= ( 0x01 << SYS_PC_L_MFP_PC3_MFP_Pos);//SPI0 MOSI MF
	

	
	CLK->APBCLK |= (1<<12);//SPI0 clock enable
	CLK->CLKSEL2 |= (1<<20);//SPI0 clock sourse HCLK
	SPI0->CTL |= (8<<SPI_CTL_TX_BIT_LEN_Pos);		//Length=8bit 
	//SPI0->CTL |= (1<<10);// LSB
	//SPI0->CTL |= (1<<2);//TX_NEG
	SPI0->CLKDIV |= 0x05;
}



void Write_spi(uint8_t data){
	SPI0->TX0 = data;//Data buffer
	SPI0->CTL |= SPI_CTL_GO_BUSY_Msk;//Transmit data
	while(SPI0->CTL & SPI_CTL_GO_BUSY_Msk);
}
