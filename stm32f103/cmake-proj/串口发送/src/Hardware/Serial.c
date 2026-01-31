#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>

void Serial_Init(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出 供USART1的TX使用
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化USART 
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600; // 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制 不使用流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx; // 串口模式 只使用发送;
    USART_InitStructure.USART_Parity = USART_Parity_No; // 检验位 不使用校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 停止位 1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字长 8位
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE); // 使能串口

}

// 发送数据函数
void Serial_SendByte(uint8_t byte){
    USART_SendData(USART1, byte);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送完成 不需要清零 标志位会自动清零
}

void Serial_SendArry(uint8_t* array, uint16_t length){
    uint16_t i;
    for(i = 0; i < length; i++){
        Serial_SendByte(array[i]);
    }
}

void Serial_SendString(char* str){
    uint8_t i;
    for(i = 0; str[i] != '\0'; i++){
        Serial_SendByte(str[i]);
    }
}

// 计算x的y次方
uint32_t Serial_Pow(uint32_t x, uint32_t y){
    uint32_t result = 1;
    while(y--){
        result *= x;
    }
    return result;
}

void Serial_SendNumber(uint32_t num, uint8_t length){
    uint8_t i;
    for(i = 0; i < length; i++){
        uint32_t divisor = Serial_Pow(10, length - i - 1); // 计算10的(length - i - 1)次方
        uint8_t digit = (num / divisor) % 10; // 提取对应位数字
        Serial_SendByte(digit + '0'); // 转换为字符并发送 + 0x30(ASCII码偏移)
    }

}

// 重定义printf函数
int fputc(int ch, FILE *f){
    Serial_SendByte((uint8_t)ch);
    return ch;
}

// 封装sprintf函数
void Serial_Printf(char* format, ...){
    char string[100];
    va_list args;
    va_start(args, format);
    vsprintf(string, format, args);
    va_end(args);
    Serial_SendString(string);
}