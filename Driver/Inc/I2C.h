#include "Nano100Series.h"

#define STA 	0x08//Start impulse
#define STO		0x04//Stop impulse
#define SI		0x10//Status active
#define AA		0x02//ACK

/*
PA4(36)I2C0_SDA(0x05)
PA5(37)I2C0_SCL(0x05)
*/
#define I2C0_0 	0x00
/*
PA8(7)I2C0_SDA(0x01)
PA9(6)I2C0_SCL(0x01)
*/
#define I2C0_1 	0x01
/*
PA12(28)I2C0_SDA(0x05)
PA13(27)I2C0_SCL(0x05)
*/
#define I2C0_2 	0x02
/*
PA10(5)I2C1_SDA(0x01)
PA11(4)I2C1_SCL(0x01)
*/
#define I2C1_0 	0x03

#define I2C_CLEAR_INTERRUPT_FLAG(I2C_T)				(I2C_T->INTSTS |= I2C_INTSTS_INTSTS_Msk)


void I2C0_init(void);
void I2C_Init(I2C_T * i2c,uint8_t mux);
uint8_t I2C_Master_Write(I2C_T * i2c,uint8_t reg, uint8_t * data,uint8_t addr,uint8_t size);
uint8_t I2C_Master_Read(I2C_T * i2c,uint8_t * data,uint8_t addr,uint8_t size);
uint8_t I2C_TxRx(I2C_T * i2c,uint8_t reg, uint8_t * data,uint8_t addr,uint8_t size);
void Control_Reg(I2C_T * i2c,uint8_t state);