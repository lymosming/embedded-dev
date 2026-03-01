#include "stm32f10x.h"
#include <stdint.h>

uint8_t Serial_RxData; // 用于存储接收到的数据
uint8_t Serial_RxFlag; // 接收标志

void Serial_Init(void){
    // 初始化GPIOA 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitTypeDef;
    GPIO_InitTypeDef.GPIO_Pin = GPIO_Pin_9; // PA9 作为 USART1_TX
    GPIO_InitTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitTypeDef.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitTypeDef);

    GPIO_InitTypeDef.GPIO_Pin = GPIO_Pin_10; // PA10 作为 USART1_RX
    GPIO_InitTypeDef.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
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

    // 中断配置
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接收中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置优先级分组
    // 配置NVIC
    NVIC_InitTypeDef NVIC_InitDef;
    NVIC_InitDef.NVIC_IRQChannel = USART1_IRQn; // USART1中
    NVIC_InitDef.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级0
    NVIC_InitDef.NVIC_IRQChannelSubPriority = 1; // 子优先级0
    NVIC_InitDef.NVIC_IRQChannelCmd = ENABLE; // 使能中断通道
    NVIC_Init(&NVIC_InitDef);


    USART_Cmd(USART1, ENABLE); // 使能USART1

}

void Serial_SendChar(char c){
    USART_SendData(USART1, c); // 发送数据
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送缓冲区为空
}

// 响应中断
void USART1_IRQHandler(void){
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        Serial_RxData = USART_ReceiveData(USART1); // 读取接收数据
        Serial_RxFlag = 1; // 设置接收标志
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志位
    }
}

uint8_t Serial_GetRxData(void){
    Serial_RxFlag = 0; // 清除接收标志
    return Serial_RxData; // 返回接收到的数据
}

uint8_t Serial_GetRxFlag(void){
    return Serial_RxFlag; // 返回接收标志
}