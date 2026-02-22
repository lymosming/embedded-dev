#include "stm32f10x.h"
#include "MyI2C.h"
#include "OLED.h"
#include "Mpu6050.h"

int16_t ax, ay, az, gx, gy, gz;

int main (void){
    OLED_Init();

    uint8_t ID = Mpu6050_GetID();
    OLED_ShowHexNum(1, 1, ID, 2); // 显示MPU6050的ID，应该是0x68

    /*
    MyI2C_Init();
    // 测试应答位
    MyI2C_Start();
    MyI2C_SendByte(0xD0); // 1101 000 0 如果是0xa0就会显示001 因为没有这个设备地址，所以会收到NACK
    uint8_t ack = MyI2C_ReceiveAck();
    MyI2C_Stop();
    OLED_ShowNum(1, 1, ack, 3); // 显示ACK值，0表示ACK成功，1表示NACK
    */

    // MPU6050测试
    Mpu6050_Init();
    Mpu6050_WriteReg(0x6B, 0x00); // 解除睡眠模式
    Mpu6050_WriteReg(0x19, 0xAA); // 写入0XAA
    uint8_t data = Mpu6050_ReadReg(0x19); // 读取寄存器值
    OLED_ShowNum(1, 1, data, 2); // 显示读取到的值，应该是0xAA

    while(1){
        // 读取陀螺仪 和加速度计数据
        Mpu6050_GetData(&ax, &ay, &az, &gx, &gy, &gz);
        OLED_ShowNum(2, 1, ax, 5); // 显示加速度X轴数据
        OLED_ShowNum(3, 1, ay, 5); // 显示加速度Y轴数据
        OLED_ShowNum(4, 1, az, 5); // 显示加速度Z轴数据
        OLED_ShowNum(2, 8, gx, 5); // 显示陀螺仪X轴数据
        OLED_ShowNum(3, 8, gy, 5); // 显示陀螺仪Y轴数据
        OLED_ShowNum(4, 8, gz, 5); // 显示陀螺仪Z轴数据
    }
}
