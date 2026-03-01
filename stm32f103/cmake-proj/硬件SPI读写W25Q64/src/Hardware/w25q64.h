#ifndef __W25Q64_H
#define __W25Q64_H
#include <stdint.h>

void w25q64_Init(void);
void w25q64_ReadID(uint8_t *mid, uint16_t *did);
void w25q64_PageProgram(uint32_t addr, uint8_t *data_arr, uint16_t count);
void w25q64_SectorErase(uint32_t addr);
void w25q64_ReadData(uint32_t addr, uint8_t *data_arr, uint32_t count);
#endif
