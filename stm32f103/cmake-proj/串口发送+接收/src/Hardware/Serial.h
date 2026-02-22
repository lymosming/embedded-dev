#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdint.h>

void Serial_Init(void);
void Serial_SendChar(char c);
uint8_t Serial_GetRxData(void);
uint8_t Serial_GetRxFlag(void);

#endif
