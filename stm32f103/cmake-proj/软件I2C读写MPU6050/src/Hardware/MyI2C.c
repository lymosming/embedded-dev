#include "stm32f10x.h"
#include "Delay.h"

void MyI2C_Init(void){

    // 初始化GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出 虽然有out但是它也可以输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11); // 使SCL SDA 处于高电平

    

}

// 设置SCL电平
void MyI2C_W_SCL(uint8_t bit_value){
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)bit_value);
    Delay_us(10); // 延时10微秒，确保信号稳定
}

// 设置SDA电平
void MyI2C_W_SDA(uint8_t bit_value){
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)bit_value);
    Delay_us(10); // 延时10微秒，确保信号稳定
}

// 读取SDA电平
uint8_t MyI2C_R_SDA(void){
    uint8_t bit_value;
    bit_value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    Delay_us(10); // 延时10微秒，确保信号稳定
    return bit_value;
}

void MyI2C_Start(void){
    // I2C起始条件：SDA从高电平变为低电平
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);

}

void MyI2C_Stop(void){
    // I2C停止条件：SDA从低电平变为高电平
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t byte){

    /*
    // 取出最高位
    MyI2C_W_SDA(byte & 0x80); // 发送最高位
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);

    // 取出次高位
    MyI2C_W_SDA(byte & 0x40); // 发送次高位
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
    */
    uint8_t i;
    for(i = 0; i < 8; i++){
        MyI2C_W_SDA(byte & (0x80 >> i)); // 一位一位发送
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }

}

uint8_t MyI2C_ReceiveByte(void){
    uint8_t i, byte = 0x00;
    MyI2C_W_SDA(1); // 释放SDA线，准备接收数据
    for(i = 0; i < 8; i++){
        MyI2C_W_SCL(1);
        if(MyI2C_R_SDA() == 1){
            byte |= 0x80 >> i; // 将接收到的位放入byte中
        }
        MyI2C_W_SCL(0);

    }
    return byte;
}

// 发送应答位
void MyI2C_SendAck(uint8_t ack){
    MyI2C_W_SDA(ack); // 发送ACK信号
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void){
    uint8_t ack;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    ack = MyI2C_R_SDA(); // 读取ACK信号
    MyI2C_W_SCL(0);
    return ack;
}