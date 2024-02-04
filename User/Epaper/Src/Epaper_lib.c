#include "Epaper_lib.h"
#include "Epaper_lut.h"

uint8_t image[2756];
uint8_t rotate = ROTATE_270;

//CS PB3
//D/C PA12
//Reset PF0
//Busy PF1
void Epaper_init(void){
	
	PC->PMD |= (0x01<<GP_PMD_PMD0_Pos);//CS Epaper OUTPUT //PC0
	CS_HIGH;//CS HIGH-deactive,LOW-active //PC0	
	PC->PMD |= (0x01<< GP_PMD_PMD2_Pos); // D/C Epaper OUTPUT,LOW-command,HIGH-data //PC2
	PB->PMD |= (0x01 << GP_PMD_PMD0_Pos);//Reset OUTPUT //PB0
	RESET_HIGH;//HIGH-normal state,LOW-reset //PA9	
	PB->PMD &= ~(0x01<<GP_PMD_PMD1_Pos);//BUSY INPUT ,HIGH-IDLE state,LOW-busy // PB1
	Epaper_reset();
	
	
  Epaper_cmd(0x01);//POWER SETTING
  Epaper_data(0x03);
  Epaper_data(0x00);
  Epaper_data(0x2B);
  Epaper_data(0x2B);
  Epaper_data(0x03);

	Epaper_cmd(0x06);	//boost soft start
	Epaper_data(0x17);     //A
	Epaper_data(0x17);     //B
	Epaper_data(0x17);     //C

	Epaper_cmd(0x04);//Power ON
	Epaper_busy();

	Epaper_cmd(0x00);	//panel setting
	Epaper_data(0xbf);     //LUT from OTP,128x296
	Epaper_data(0x0e);     //VCOM to 0V fast

	Epaper_cmd(0x30);	//PLL setting
	Epaper_data(0x3a);     // 3a 100HZ   29 150Hz 39 200HZ	31 171HZ

	Epaper_cmd(0x61);	//resolution setting
	Epaper_data(Epaper_WIDTH);
	Epaper_data((Epaper_HEIGHT >> 8) & 0xff);
	Epaper_data(Epaper_HEIGHT& 0xff);

	Epaper_cmd(0x82);	//vcom_DC setting
	Epaper_data(0x28);
	
		Clear(0);
}

void Epaper_send(uint8_t data){
	
	CS_LOW;
	Write_spi(data);
	CS_HIGH;
}

void Epaper_cmd(uint8_t command){

	COMMAND;
  Epaper_send(command);
}

void Epaper_data(uint8_t data){

	DATA;
	Epaper_send(data);
}

void Epaper_reset(void){

	RESET_LOW;
	Delay(10);
	RESET_HIGH;
	Delay(200);
}

void Epaper_busy(void){
	
	while(!(PB->PIN&(1<<1)));//LOW-busy

}

void SetFullReg(void){
	
	Epaper_cmd(0x82);
	Epaper_data(0x00);
	Epaper_cmd(0X50);
	Epaper_data(0x97);

	unsigned int count;
	Epaper_cmd(0x20);
	for(count=0; count<44; count++) {
			Epaper_data(EPD_2IN13D_lut_vcomDC[count]);
	}

	Epaper_cmd(0x21);
	for(count=0; count<42; count++) {
			Epaper_data(EPD_2IN13D_lut_ww[count]);
	}

	Epaper_cmd(0x22);
	for(count=0; count<42; count++) {
			Epaper_data(EPD_2IN13D_lut_bw[count]);
	}

	Epaper_cmd(0x23);
	for(count=0; count<42; count++) {
			Epaper_data(EPD_2IN13D_lut_wb[count]);
	}

	Epaper_cmd(0x24);
	for(count=0; count<42; count++) {
			Epaper_data(EPD_2IN13D_lut_bb[count]);
	}
}

void TurnOnDisplay(void){
	
	Epaper_cmd(0x12);		 //DISPLAY REFRESH
	Delay(50);
	Epaper_busy();
}

void Displayimage()
{
    int Width, Height;
    Width = (Epaper_WIDTH % 8 == 0)? (Epaper_WIDTH / 8 ): (Epaper_WIDTH / 8 + 1);
    Height = Epaper_HEIGHT;

    Epaper_cmd(0x10);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            Epaper_data(0x00);
        }
    }

    Epaper_cmd(0x13);
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
        	Epaper_data( image[i + j * Width] );

        }
    }

	SetFullReg();
	TurnOnDisplay();
}

void Epaper_sleep(void){
	Epaper_cmd(0x07);
	Epaper_data(0xA5);
}

void DrawAbsolutePixel(int x, int y, int colored) {
    if (x < 0 || x >= Epaper_WIDTH || y < 0 || y >= Epaper_HEIGHT) {
        return;
    }
    if (IF_INVERT_COLOR) {
        if (colored) {
            image[(x + y * Epaper_WIDTH) / 8] |= 0x80 >> (x % 8);
        } else {
            image[(x + y * Epaper_WIDTH) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (colored) {
            image[(x + y * Epaper_WIDTH) / 8] &= ~(0x80 >> (x % 8));
        } else {
            image[(x + y * Epaper_WIDTH) / 8] |= 0x80 >> (x % 8);
        }
    }
}

void DrawPixel(int x, int y, int colored) {
    int point_temp;
    if (rotate == ROTATE_0) {
        if(x < 0 || x >= Epaper_WIDTH || y < 0 || y >= Epaper_HEIGHT) {
            return;
        }
        DrawAbsolutePixel(x, y, colored);
    } else if (rotate == ROTATE_90) {
        if(x < 0 || x >= Epaper_HEIGHT || y < 0 || y >= Epaper_WIDTH) {
          return;
        }
        point_temp = x;
        x = Epaper_WIDTH - y;
        y = point_temp;
        DrawAbsolutePixel(x, y, colored);
    } else if (rotate == ROTATE_180) {
        if(x < 0 || x >= Epaper_WIDTH || y < 0 || y >= Epaper_HEIGHT) {
          return;
        }
        x = Epaper_WIDTH - x;
        y = Epaper_HEIGHT - y;
        DrawAbsolutePixel(x, y, colored);
    } else if (rotate == ROTATE_270) {
        if(x < 0 || x >= Epaper_HEIGHT || y < 0 || y >= Epaper_WIDTH) {
          return;
        }
        point_temp = x;
        x = y;
        y = Epaper_HEIGHT - point_temp;
        DrawAbsolutePixel(x, y, colored);
    }
}

void Clear(int colored) {
    for (int x = 0; x < Epaper_WIDTH; x++) {
        for (int y = 0; y < Epaper_HEIGHT; y++) {
            DrawAbsolutePixel(x, y, colored);
        }
    }
}

//  17, /* Width */
//  24, /* Height */
void DrawCharAt(uint8_t x, uint8_t y, char ascii_char,uint8_t colored) {
    uint8_t i, j;
    unsigned int char_offset = (ascii_char - ' ') * 24 * (17 / 8 + (17 % 8 ? 1 : 0));
    const unsigned char* ptr = &Font24[char_offset];

    for (j = 0; j < 24; j++) {
        for (i = 0; i < 17; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                DrawPixel(x + i, y + j, colored);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (17 % 8 != 0) {
            ptr++;
        }
    }
}

void DrawStringAt(uint8_t x, uint8_t y, const char* text,uint8_t colored) {
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;
    
    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        DrawCharAt(refcolumn, y, *p_text,colored);
        /* Decrement the column position by 16 */
        refcolumn += 17;
        /* Point on the next character */
        p_text++;
        counter++;
    }
}

