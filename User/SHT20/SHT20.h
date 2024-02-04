#include "Nano100Series.h"


#define SHT20_ADDR 0x80 
#define TNOHOLD 0xF3
#define THOLD 0xE3
#define RHNOHOLD 0xF5
#define RHHOLD 0xE5


int8_t SHT20_ReadT(void);
uint16_t SHT20_ReadRH(void);

//6352
//12375