#include "Nano100Series.h"
#include "Font.h"

//212x104 resolution 
#define Epaper_WIDTH   104
#define Epaper_HEIGHT  212

#define ROTATE_0            0
#define ROTATE_90           1
#define ROTATE_180          2
#define ROTATE_270          3


#define IF_INVERT_COLOR     0

#define CS 					0x00		//PC0
#define DC 					0x04 	//PC2
#define BUSY 				0x16 	//PA8
#define RESET 			0x18 	//PA9



#define CS_HIGH				(PC->DOUT |= (0x01 << 0)) 
#define CS_LOW				(PC->DOUT &= ~(0x01 << 0))

#define RESET_HIGH		(PB->DOUT |= (1<<0))
#define RESET_LOW			(PB->DOUT &= ~(1<<0))

#define DATA					(PC->DOUT |= (0x01 << 2)) 
#define COMMAND				(PC->DOUT &= ~(0x01 << 2))


void Epaper_init(void);
void Epaper_send(uint8_t data);
void Epaper_cmd(uint8_t command);
void Epaper_data(uint8_t data);
void Epaper_reset(void);
void Epaper_busy(void);
void SetFullReg(void);
void TurnOnDisplay(void);
void Displayimage();
void Epaper_sleep(void);
void DrawAbsolutePixel(int x, int y, int colored);
void DrawPixel(int x, int y, int colored);
void Clear(int colored);
void DrawCharAt(uint8_t x, uint8_t y, char ascii_char,uint8_t colored);
void DrawStringAt(uint8_t x, uint8_t y, const char* text,uint8_t colored);