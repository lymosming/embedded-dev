#ifndef __MYSPI_H
#define __MYSPI_H
#include <stdint.h>

void MySpi_Init(void);
void MySpi_Start(void);
void MySpi_Stop(void);
uint8_t MySpi_SwapByte(uint8_t bytesend);

#endif 
