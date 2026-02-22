#include "stm32f10x.h"
#include "MyI2C.h"
#include "Mpu6050_Reg.h"

#define MPU6050_ADDR 0xD0

// 指定地址写
void Mpu6050_WriteReg(uint8_t addr, uint8_t data){
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDR); // 发送设备地址
    MyI2C_ReceiveAck(); // 接收应答
    MyI2C_SendByte(addr); // 发送寄存器地址
    MyI2C_ReceiveAck(); // 接收应答
    MyI2C_SendByte(data); // 发送数据
    MyI2C_ReceiveAck(); // 接收应答
    MyI2C_Stop();
}

// 指定地址读
uint8_t Mpu6050_ReadReg(uint8_t addr){
    uint8_t data;
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDR); // 发送设备地址
    MyI2C_ReceiveAck(); // 接收应答
    MyI2C_SendByte(addr); // 发送寄存器地址
    MyI2C_ReceiveAck(); // 接收应答

    MyI2C_Start(); // 重新发送起始条件
    MyI2C_SendByte(MPU6050_ADDR | 0x01); // 发送设备地址，读模式
    MyI2C_ReceiveAck(); 
    data = MyI2C_ReceiveByte(); // 接收数据
    MyI2C_SendAck(1); // 发送NACK，表示读取结束
    MyI2C_Stop();
    return data;
}

void Mpu6050_Init(void){
    MyI2C_Init();
    // 初始化MPU6050
    Mpu6050_WriteReg(MPU6050_REG_PWR_MGMT_1, 0x01); // 解除睡眠，选择陀螺仪时钟
    Mpu6050_WriteReg(MPU6050_REG_PWR_MGMT_2, 0x00); // 6个轴均不待机
    Mpu6050_WriteReg(MPU6050_REG_SMPLRT_DIV, 0x09); // 采样分频为10
    Mpu6050_WriteReg(MPU6050_REG_CONFIG, 0x06); // 滤波参数给最大
    Mpu6050_WriteReg(MPU6050_REG_GYRO_CONFIG, 0x18); // 陀螺仪量程最大
    Mpu6050_WriteReg(MPU6050_REG_ACCEL_CONFIG, 0x18); // 加速计最大量程
}   

uint8_t Mpu6050_GetID(void){
    return Mpu6050_ReadReg(MPU6050_REG_WHO_AM_I);
}

// 获取加速度和陀螺仪数据
void Mpu6050_GetData(int16_t *accx, int16_t *accy, int16_t *accz, int16_t *gyrox, int16_t *gyroy, int16_t *gyroz){
    uint8_t datah, datal;
    datah = Mpu6050_ReadReg(MPU6050_REG_ACCEL_XOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_ACCEL_XOUT_L);
    *accx = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_ACCEL_YOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_ACCEL_YOUT_L);
    *accy = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_ACCEL_ZOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_ACCEL_ZOUT_L);
    *accz = (datah << 8) | datal;

    // 陀螺仪数据
    datah = Mpu6050_ReadReg(MPU6050_REG_GYRO_XOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_GYRO_XOUT_L);
    *gyrox = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_GYRO_YOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_GYRO_YOUT_L);
    *gyroy = (datah << 8) | datal;  

    datah = Mpu6050_ReadReg(MPU6050_REG_GYRO_ZOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_GYRO_ZOUT_L);
    *gyroz = (datah << 8) | datal;
}