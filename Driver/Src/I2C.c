#include "I2C.h"


/*

PA4(36)i2c_SDA(0x05)
PA5(37)i2c_SCL(0x05)

PA9(6)i2c_SCL(0x01)
PA8(7)i2c_SDA(0x01)

PA13(27)i2c_SCL(0x05)
PA12(28)i2c_SDA(0x05)

PA11(4)I2C1_SCL(0x01)
PA10(5)I2C1_SDA(0x01)

 
*/
void I2C_Init(I2C_T * i2c,uint8_t dir){
	
		if(i2c==I2C0){
			if(dir==0){
				SYS->PA_L_MFP = ((SYS->PA_L_MFP&~SYS_PA_L_MFP_PA4_MFP_Msk)|(0x05<<SYS_PA_L_MFP_PA4_MFP_Pos));//i2c SDA MF PA4
				SYS->PA_L_MFP = ((SYS->PA_L_MFP&~SYS_PA_L_MFP_PA5_MFP_Msk)|(0x05<<SYS_PA_L_MFP_PA5_MFP_Pos));//i2c SCL MF PA5
				CLK->APBCLK |= CLK_APBCLK_I2C0_EN_Msk;//i2c clock enable
				i2c->DIV = 0x28;//I2C Clock Divider Conrolltrol Register//0x28
				i2c->CON |= I2C_CON_IPEN_Msk;//i2c Function Enable
			}
			else if(dir==1){
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA8_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA8_MFP_Pos));//i2c SDA MF PA8
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA9_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA9_MFP_Pos));;//i2c SCL MF PA9
				CLK->APBCLK |= CLK_APBCLK_I2C0_EN_Msk;//i2c clock enable
				i2c->DIV = 0x28;//I2C Clock Divider Conrolltrol Register//0x28
				i2c->CON |= I2C_CON_IPEN_Msk;//i2c Function Enable
			}
			else{
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA12_MFP_Msk)|(0x05<<SYS_PA_H_MFP_PA12_MFP_Pos));//i2c SDA MF PA12
				SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA13_MFP_Msk)|(0x05<<SYS_PA_H_MFP_PA13_MFP_Pos));;//i2c SCL MF PA13
				CLK->APBCLK |= CLK_APBCLK_I2C0_EN_Msk;//i2c clock enable
				i2c->DIV = 0x28;//I2C Clock Divider Conrolltrol Register//0x28
				i2c->CON |= I2C_CON_IPEN_Msk;//i2c Function Enable
			}

			

		}
		else if(i2c==I2C1){
			SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA10_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA10_MFP_Pos));//I2C1 SDA MF PA10
			SYS->PA_H_MFP = ((SYS->PA_H_MFP&~SYS_PA_H_MFP_PA11_MFP_Msk)|(0x01<<SYS_PA_H_MFP_PA11_MFP_Pos));;//I2C1 SCL MF PA11
			CLK->APBCLK |= CLK_APBCLK_I2C1_EN_Msk;//I2C1 clock enable
			I2C1->DIV = 0x28;//I2C1 Clock Divider Conrolltrol Register//0x28
			I2C1->CON |= I2C_CON_IPEN_Msk;//I2C1 Function Enable
		}

}

//void i2c_init(void){
//	
//	SYS->PA_H_MFP |= (0x01 << SYS_PA_H_MFP_PA8_MFP_Pos);//i2c SDA MF PA8
//	SYS->PA_H_MFP |= (0x01 << SYS_PA_H_MFP_PA9_MFP_Pos);//i2c SCL MF PA9
//	
//	CLK->APBCLK |= CLK_APBCLK_i2c_EN_Msk;//i2c clock enable
//	
//	i2c->DIV = 0x28;//I2C Clock Divider Conrolltrol Register//0x28
//	i2c->CON |= I2C_CON_IPEN_Msk;//i2c Function Enable


//}

void Control_Reg(I2C_T * i2c,uint8_t state){
		i2c->CON = (i2c->CON&~0x1E|state);
}

uint8_t I2C_Master_Write(I2C_T * i2c,uint8_t reg, uint8_t * data,uint8_t addr,uint8_t size){
	

	uint8_t error_code = 10;
	uint8_t end_transmission = 1;
	i2c->INTSTS |= (1<<0);//clear flag
	Control_Reg(i2c,STA|SI);//Start	impulse

		while(end_transmission){
		
		while( !(i2c->INTSTS&(1<<0)) );//wait update condition 
		
		switch (i2c->STATUS)
		{
			case 0x08://Start
				i2c->DATA = addr&~0x01;
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,SI);	
				break;
			case 0x18://Master Transmit Address ACK
				i2c->DATA = reg;
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,SI);
				break;
			case 0x28://Master Transmit Data ACK			
				if(size--){
				i2c->DATA = *data++;
				i2c->INTSTS |= (1<<0);//clear flag	
				Control_Reg(i2c,SI);
				}
				else{			
				i2c->INTSTS |= (1<<0);//clear flag	
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
	i2c->INTSTS |= (1<<0);//clear flag
	Control_Reg(i2c,STA|SI);//Start	impulse

	
		while(end_transmission){

		while( !(i2c->INTSTS&(1<<0)) );//wait update condition 			
		switch (i2c->STATUS)
		{
			case 0x08://Start 
				i2c->DATA = addr|0x01;
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,SI);				
				break;
			case 0x40://Master Receive Address ACK
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,AA|SI);
				break;
			case 0x50://Master Receive Data ACK
				if(size--){
				*data++ = i2c->DATA;
				i2c->INTSTS |= (1<<0);//clear flag	
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
	i2c->INTSTS |= (1<<0);//clear flag
	Control_Reg(i2c,STA|SI);//Start	impulse

		while(end_transmission){
		
		while( !(i2c->INTSTS&(1<<0)) );//wait update condition 
		
		switch (i2c->STATUS)
		{
			case 0x08://Start
				i2c->DATA = addr&~0x01;
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,SI);	
				break;
			case 0x18://Master Transmit Address ACK
				i2c->DATA = reg;
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,SI);
				break;
			case 0x28://Master Transmit Data ACK	
				i2c->INTSTS |= (1<<0);//clear flag				
				Control_Reg(i2c,STA|SI);//Repeat Start
				break;
			case 0x10://Master Repeat Start
				i2c->DATA = addr|0x01;
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,SI);	
				break;			
			case 0x40://Master Receive Address ACK
				i2c->INTSTS |= (1<<0);//clear flag
				Control_Reg(i2c,AA|SI);
				break;
			case 0x50://Master Receive Data ACK
				if(size--){
				*data++ = i2c->DATA;
				i2c->INTSTS |= (1<<0);//clear flag	
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

//switch (i2c->STATUS)
//		{
//			case 0x08://Start
//				i2c->DATA = addr;
//				i2c->CON |= I2C_CON_I2C_STS_Msk;//Status active
//				PA->DOUT |= (1<<13);
//				i2c->INTSTS |= 0x01;//update flag interrupt
//				break;
//			case 0x10://Master Repeat Start
//				break;
//			case 0x18://Master Transmit Address ACK
//				i2c->DATA = data;
//				i2c->CON |= I2C_CON_I2C_STS_Msk;//Status active
//				PA->DOUT |= (1<<14);
//				i2c->INTSTS=0x01;//update flag interrupt
//				break;
//			case 0x20://Master Transmit Address NACK
//				end_transmission = 0;
//				error_code = 1;
//				break;
//			case 0x28://Master Transmit Data ACK
//				end_transmission = 0;
//				PA->DOUT |= (1<<15);
//				error_code = 0;//successful
//				break;
//			case 0x30://Master Transmit Data NACK
//				end_transmission = 0;
//				error_code = 2;
//				break;
//			case 0x38://Master Arbitration Lost
//				end_transmission = 0;
//				error_code = 3;
//				break;
//			case 0x40://Master Receive Address ACK
//				break;
//			case 0x48://Master Receive Address NACK
//				end_transmission = 0;
//				error_code = 4;
//				break;
//			case 0x50://Master Receive Data ACK
//				break;
//			case 0x58://Master Receive Data NACK
//				end_transmission = 0;
//				error_code = 5;
//				break;
//			case 0x00://Bus error
//				end_transmission = 0;
//				error_code = 6;
//				break;
//			case 0xF8://Bus Released
//				end_transmission = 0;
//				error_code = 7;
//				break;		
//			default:
//				break;
//		}

//	do{
//		
//		i2c->CON |= I2C_CON_START_Msk;//Start impulse
//		while(i2c->STATUS!=0x08);

//		i2c->DATA = count1;//ADDR
//		i2c->CON |= I2C_CON_I2C_STS_Msk;//Status active
//		while(i2c->STATUS!=0x18&&i2c->STATUS!=0x40&&i2c->STATUS!=0x20&&i2c->STATUS!=0x48);//Master Transmit Address ACK
//		if(i2c->STATUS==0x18||i2c->STATUS==0x40)break;
//		STO;//end transmit 
//		SI;
//		while(i2c->STATUS!=0xF8);

//	}while(count1-- >= 0);