#ifndef __MYFLASH_H
#define __MYFLASH_H
#include <stdint.h>

uint32_t MyFlash_ReadWord(uint32_t addr);
uint16_t MyFlash_ReadHalfWord(uint32_t addr);
uint8_t MyFlash_ReadByte(uint32_t addr);
void MyFlash_EraseAllPage(void);
void MyFlash_ErasePage(uint32_t page_addr);
void MyFlash_ProgramWord(uint32_t addr, uint32_t data);
void MyFlash_ProgramHalfWord(uint32_t addr, uint16_t data);

#endif
