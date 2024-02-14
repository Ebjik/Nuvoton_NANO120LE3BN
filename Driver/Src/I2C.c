#include "I2C.h"

void I2C_Init(I2C_T * i2c,uint8_t mux){
	
		if(i2c==I2C0){
			if(mux==I2C0_0){
				SYS->PA_L_MFP = ((SYS->PA_L_MFP&~SYS_PA_L_MFP_PA4_MFP_Msk)|(0x05<<SYS_PA_L_MFP_PA4_MFP_Pos));//i2c SDA MF PA4
				SYS->PA_L_MFP = ((SYS->PA_L_MFP&~SYS_PA_L_MFP_PA5_MFP_Msk)|(0x05<<SYS_PA_L_MFP_PA5_MFP_Pos));//i2c SCL MF PA5
				CLK->APBCLK |= CLK_APBCLK_I2C0_EN_Msk;//i2c0 clock enable
				i2c->DIV = 0x28;//I2C0 Clock Divider Conrolltrol Register
				i2c->CON |= I2C_CON_IPEN_Msk;//i2c0 Function Enable
			}
			else if(mux==I2C0_1){
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA8_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA8_MFP_Pos));//i2c SDA MF PA8
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA9_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA9_MFP_Pos));;//i2c SCL MF PA9
				CLK->APBCLK |= CLK_APBCLK_I2C0_EN_Msk;//i2c0 clock enable
				i2c->DIV = 0x28;//I2C0 Clock Divider Conrolltrol Register
				i2c->CON |= I2C_CON_IPEN_Msk;//i2c0 Function Enable
			}
			else if(mux==I2C0_2){
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA12_MFP_Msk)|(0x05<<SYS_PA_H_MFP_PA12_MFP_Pos));//i2c SDA MF PA12
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA13_MFP_Msk)|(0x05<<SYS_PA_H_MFP_PA13_MFP_Pos));;//i2c SCL MF PA13
				CLK->APBCLK |= CLK_APBCLK_I2C0_EN_Msk;//i2c0 clock enable
				i2c->DIV = 0x28;//I2C0 Clock Divider Conrolltrol Register//0x28
				i2c->CON |= I2C_CON_IPEN_Msk;//i2c0 Function Enable
			}
		

		}
		//I2C1
		else{
			SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA10_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA10_MFP_Pos));//I2C1 SDA MF PA10
			SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA11_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA11_MFP_Pos));;//I2C1 SCL MF PA11
			CLK->APBCLK |= CLK_APBCLK_I2C1_EN_Msk;//I2C1 clock enable
			i2c->DIV = 0x28;//I2C1 Clock Divider Conrolltrol Register//0x28
			i2c->CON |= I2C_CON_IPEN_Msk;//I2C1 Function Enable
		}

}



void Control_Reg(I2C_T * i2c,uint8_t state){
	i2c->CON = (i2c->CON&~0x1E|state);
}


uint8_t I2C_Master_Write(I2C_T * i2c,uint8_t reg, uint8_t * data,uint8_t addr,uint8_t size){
	

	uint8_t error_code = 10;
	uint8_t end_transmission = 1;
	I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
	Control_Reg(i2c,STA|SI);//Start	impulse

		while(end_transmission){
		
		while( !(i2c->INTSTS&(1<<0)) );//wait update condition 
		
		switch (i2c->STATUS)
		{
			case 0x08://Start
				i2c->DATA = addr&~0x01;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
				Control_Reg(i2c,SI);	
				break;
			case 0x18://Master Transmit Address ACK
				i2c->DATA = reg;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
				Control_Reg(i2c,SI);
				break;
			case 0x28://Master Transmit Data ACK			
				if(size--){
				i2c->DATA = *data++;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag	
				Control_Reg(i2c,SI);
				}
				else{			
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag	
				Control_Reg(i2c,STO|SI);				
				end_transmission = 0;
				error_code = 0;//successful
				}
				break;	
			default:
				end_transmission = 0;
				error_code = i2c->STATUS;
				Control_Reg(i2c,STO|SI);//Stop impulse
			
				break;
		}
	}
	return error_code;
}

uint8_t I2C_Master_Read(I2C_T * i2c,uint8_t * data,uint8_t addr,uint8_t size){
	
	uint8_t error_code = 10;//successful
	uint8_t end_transmission = 1;
	I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
	Control_Reg(i2c,STA|SI);//Start	impulse

	
		while(end_transmission){

		while( !(i2c->INTSTS&(1<<0)) );//wait update condition 			
		switch (i2c->STATUS)
		{
			case 0x08://Start 
				i2c->DATA = addr|0x01;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
				Control_Reg(i2c,SI);				
				break;
			case 0x40://Master Receive Address ACK
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
				Control_Reg(i2c,AA|SI);
				break;
			case 0x50://Master Receive Data ACK
				if(size--){
				*data++ = i2c->DATA;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag	
				size == 1 ? Control_Reg(i2c,SI) : Control_Reg(i2c,AA|SI);	// if last byte in package NACK otherwise ACK
				}
				break;
			case 0x58://Master Receive Data NACK	
				*data++ = i2c->DATA;
				Control_Reg(i2c,STO|SI);				
				end_transmission = 0;
				error_code = 0;//successful
				break;		
			default:
				end_transmission = 0;
				error_code = i2c->STATUS;
				Control_Reg(i2c,STO|SI);//Stop impulse
				break;
		}
	}

	return error_code;
}

uint8_t I2C_TxRx(I2C_T * i2c,uint8_t reg, uint8_t * data,uint8_t addr,uint8_t size){
	

	uint8_t error_code = 10;
	uint8_t end_transmission = 1;
	I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
	Control_Reg(i2c,STA|SI);//Start	impulse

		while(end_transmission){
		
		while( !(i2c->INTSTS&(1<<0)) );//wait update condition 
		
		switch (i2c->STATUS)
		{
			case 0x08://Start
				i2c->DATA = addr&~0x01;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
				Control_Reg(i2c,SI);	
				break;
			case 0x18://Master Transmit Address ACK
				i2c->DATA = reg;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);
				Control_Reg(i2c,SI);
				break;
			case 0x28://Master Transmit Data ACK	
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag				
				Control_Reg(i2c,STA|SI);//Repeat Start
				break;
			case 0x10://Master Repeat Start
				i2c->DATA = addr|0x01;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
				Control_Reg(i2c,SI);	
				break;			
			case 0x40://Master Receive Address ACK
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag
				Control_Reg(i2c,AA|SI);
				break;
			case 0x50://Master Receive Data ACK
				if(size--){
				*data++ = i2c->DATA;
				I2C_CLEAR_INTERRUPT_FLAG(i2c);//clear flag	
				size == 1 ? Control_Reg(i2c,SI) : Control_Reg(i2c,AA|SI);	// if last byte in package NACK otherwise ACK
				}
				break;
				case 0x58://Master Receive Data NACK	
					*data++ = i2c->DATA;
					Control_Reg(i2c,STO|SI);				
					end_transmission = 0;
					error_code = 0;//successful
				break;	
			default:
				end_transmission = 0;
				error_code = i2c->STATUS;
				Control_Reg(i2c,STO|SI);//Stop impulse
			
				break;
		}
	}
	return error_code;
}


//			case 0x08://Start

//			case 0x10://Master Repeat Start

//			case 0x18://Master Transmit Address ACK

//			case 0x20://Master Transmit Address NACK

//			case 0x28://Master Transmit Data ACK

//			case 0x30://Master Transmit Data NACK

//			case 0x38://Master Arbitration Lost

//			case 0x40://Master Receive Address ACK

//			case 0x48://Master Receive Address NACK

//			case 0x50://Master Receive Data ACK

//			case 0x58://Master Receive Data NACK

//			case 0xF8://Bus Released
