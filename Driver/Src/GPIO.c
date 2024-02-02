#include "GPIO.h"


void GPIO_Mode(GPIO_T * port,uint8_t pin,uint8_t mode){
	
	if(pin >=0&&pin<16)
		port->PMD = ( (port->PMD&~(0x03<<pin*2)) | (mode << pin*2) );
	
}

void GPIO_Output(GPIO_T * port,uint8_t pin,uint8_t value){
	
	if(pin >=0&&pin<16&&value>=0&&value<2)	
		port->DOUT = ( (port->DOUT&~(0x01<<pin)) | ((uint32_t)value << pin) );
		

}

uint8_t GPIO_ReadPin(GPIO_T * port,uint8_t pin){
	
	if(port->PIN&(1<<pin))return 1;
	else return 0;


}