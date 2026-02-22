#include "stm32f10x.h"
#include "Mpu6050_Reg.h"

#define MPU6050_ADDR 0xD0

void Mpu6050_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio_type;
    gpio_type.GPIO_Mode = GPIO_Mode_AF_OD; // 复用推挽输出
    gpio_type.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    gpio_type.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_type);

    // 初始化I2C
    I2C_InitTypeDef I2C_InitTypeStructure;
    I2C_InitTypeStructure.I2C_Mode = I2C_Mode_I2C; // 模式
    I2C_InitTypeStructure.I2C_Ack = I2C_Ack_Enable; // 应答位配置 给应答
    I2C_InitTypeStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 响应几位地址
    I2C_InitTypeStructure.I2C_OwnAddress1 = 0x00; // 地址 不需要从机，随便给个地址
    I2C_InitTypeStructure.I2C_DutyCycle = I2C_DutyCycle_2; // 时钟占空比
    I2C_InitTypeStructure.I2C_ClockSpeed = 50000; // 时钟频率
    I2C_Init(I2C2, &I2C_InitTypeStructure);

    I2C_Cmd(I2C2, ENABLE);
}

// 封装等待事件函数
void Mpu6050_WaitEvent(I2C_TypeDef *i2cx, uint32_t i2c_event){
    uint32_t timeout;
    timeout = 10000;
    while(I2C_CheckEvent(i2cx, i2c_event) != SUCCESS){
        timeout--;
        if(timeout <= 0){
            break;
        }
    }
}

void Mpu6050_WriteReg(uint8_t addr, uint8_t data){
    I2C_GenerateSTART(I2C2, ENABLE); // 生成起始条件
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 等待主机模式已选择事件 EV5事件

    // 发送从机地址
    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED); // 等待EV6事件

    I2C_SendData(I2C2, addr); // 发送DR
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 字节正在发送

    I2C_SendData(I2C2, data); // 发送数据
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 等待字节已经发送完毕事件 EV8_2

    I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t Mpu6050_ReadReg(uint8_t addr){
    uint8_t data;
    I2C_GenerateSTART(I2C2, ENABLE);
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Transmitter);
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_SendData(I2C2, addr);
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);


    I2C_GenerateSTART(I2C2, ENABLE); // 重复起始条件
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

    I2C_Send7bitAddress(I2C2, MPU6050_ADDR, I2C_Direction_Receiver); // 发送从机地址
    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

    I2C_AcknowledgeConfig(I2C2, DISABLE); // 设置ACK=0 不应答
    I2C_GenerateSTOP(I2C2, ENABLE); // 申请终止条件

    Mpu6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // 等待EV7事件后，一个字节数据就到DR了
    data = I2C_ReceiveData(I2C2); // 读取DR数据

    I2C_AcknowledgeConfig(I2C2, ENABLE); // ACK置1

    return data;

}

uint8_t Mpu6050_GetID(void){
    return Mpu6050_ReadReg(MPU6050_REG_WHO_AM_I);
}

void Mpu6050_GetData(uint8_t *ax, uint8_t *ay, uint8_t *az, uint8_t *gx, uint8_t *gy, uint8_t *gz){
    uint8_t datah, datal;
    datah = Mpu6050_ReadReg(MPU6050_REG_ACCEL_XOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_ACCEL_XOUT_L);
    *ax = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_ACCEL_YOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_ACCEL_YOUT_L);
    *ay = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_ACCEL_YOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_ACCEL_YOUT_L);
    *ay = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_GYRO_XOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_GYRO_XOUT_L);
    *gx = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_GYRO_YOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_GYRO_YOUT_L);
    *gy = (datah << 8) | datal;

    datah = Mpu6050_ReadReg(MPU6050_REG_GYRO_ZOUT_H);
    datal = Mpu6050_ReadReg(MPU6050_REG_GYRO_ZOUT_L);
    *gz = (datah << 8) | datal;
}