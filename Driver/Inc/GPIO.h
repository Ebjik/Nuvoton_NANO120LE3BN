#include "Nano100Series.h"

#define INPUT 			0x00
#define OUTPUT 			0x01
#define OPENDRAIN 	0x02


void GPIO_Mode(GPIO_T * port,uint8_t pin,uint8_t mode);
void GPIO_Output(GPIO_T * port,uint8_t pin,uint8_t value);
uint8_t GPIO_ReadPin(GPIO_T * port,uint8_t pin);	
	