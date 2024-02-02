#include "Nano100Series.h"

#define STA 	0x08//Start impulse
#define STO		0x04//Stop impulse
#define SI		0x10//Status active
#define AA		0x02//ACK



void I2C0_init(void);
void I2C_Init(I2C_T * i2c,uint8_t dir);
uint8_t I2C_Master_Write(I2C_T * i2c,uint8_t reg, uint8_t * data,uint8_t addr,uint8_t size);
uint8_t I2C_Master_Read(I2C_T * i2c,uint8_t * data,uint8_t addr,uint8_t size);
uint8_t I2C_TxRx(I2C_T * i2c,uint8_t reg, uint8_t * data,uint8_t addr,uint8_t size);
void Control_Reg(I2C_T * i2c,uint8_t state);