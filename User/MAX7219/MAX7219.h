#include "Nano100Series.h"

void Write_7219(uint8_t add,uint8_t data);
void Init_7219(void);
void Clear_7219(void);
void Number_7219(uint32_t num,uint8_t position);
void RTC_7219(uint8_t * sec,uint8_t * min,uint8_t * hour);