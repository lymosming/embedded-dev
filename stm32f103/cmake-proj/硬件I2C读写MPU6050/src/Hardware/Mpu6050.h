#ifndef __MPU6050_H
#define __MPU6050_H
#include <stdint.h>

void Mpu6050_Init(void);
void Mpu6050_WriteReg(uint8_t addr, uint8_t data);
uint8_t Mpu6050_GetID(void);
uint8_t Mpu6050_ReadReg(uint8_t addr);
void Mpu6050_GetData(uint8_t *ax, uint8_t *ay, uint8_t *az, uint8_t *gx, uint8_t *gy, uint8_t *gz);

#endif
