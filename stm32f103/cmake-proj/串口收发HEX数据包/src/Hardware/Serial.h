#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <stdint.h>

extern uint8_t tx_packet[];

void Serial_Init(void);
void Serial_SendByte(uint8_t byte);
void Serial_SendArray(uint8_t* array, uint16_t length);
void Serial_SendPacket(void);
uint8_t Serial_GetRxFlag(void);

#endif // __SERIAL_H__
