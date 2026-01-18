#ifndef __AD_H
#define __AD_H
#include <stdint.h>

void AD_Init(void);
uint16_t AD_GetValue(uint8_t channel);

#endif /* __AD_H */
