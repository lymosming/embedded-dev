#ifndef __MPU6050_H
#define __MPU6050_H
#include <stdint.h>

void Mpu6050_Init(void);
void Mpu6050_WriteReg(uint8_t addr, uint8_t data);
uint8_t Mpu6050_ReadReg(uint8_t addr);
uint8_t Mpu6050_GetID(void);
void Mpu6050_GetData(int16_t *accx, int16_t *accy, int16_t *accz, int16_t *gyrox, int16_t *gyroy, int16_t *gyroz);

#endif /* __MPU6050_H */
