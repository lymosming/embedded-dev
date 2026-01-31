#include "stm32f10x.h"

void Serial_Init(void){
    // 初始化GPIOA 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitTypeDef;
    GPIO_InitTypeDef.GPIO_Pin = GPIO_Pin_9; // PA9 作为 USART1_TX
    GPIO_InitTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitTypeDef.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitTypeDef);

    // 初始化USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitDef;
    USART_InitDef.USART_BaudRate = 9600; // 波特率
    USART_InitDef.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitDef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 收发模式
    USART_InitDef.USART_Parity = USART_Parity_No; // 无奇偶校
    USART_InitDef.USART_StopBits = USART_StopBits_1; // 1位停止位
    USART_InitDef.USART_WordLength = USART_WordLength_8b; // 8位数据位
    USART_Init(USART1, &USART_InitDef);

    USART_Cmd(USART1, ENABLE); // 使能USART1

}

void Serial_SendChar(char c){
    USART_SendData(USART1, c); // 发送数据
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送缓冲区为空
}
