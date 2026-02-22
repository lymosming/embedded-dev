#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>

uint8_t tx_packet[4];
uint8_t rx_packet[4];
uint8_t tx_flag;

void Serial_Init(void){
    // 初始化GPIOA 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // USART1 TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化USART1 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 发送和接收
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE); // 使能USART1

    // 配置中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接收中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void USART1_IRQHandler(void){
    static uint8_t rx_state;
    static uint8_t rx_num;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        uint8_t received = USART_ReceiveData(USART1);
        if(rx_state == 0){ // 等待包头
            if(received == 0xff){
                rx_state = 1; // 切换到接收数据状态
            }
        }else if(rx_state == 1){ // 等待接收数据
            rx_packet[rx_num] = received;
            rx_num++;
            if(rx_num >= 4){
                rx_state = 2; // 切换到等待包尾状态
                rx_num = 0; // 重置数据计数器
            }

        }else if(rx_state == 2){ // 等待包尾
            if(received == 0xfe){
                rx_state = 0; // 重置状态，准备接收下一个包
            }else{
                rx_state = 0; // 重置状态，准备接收下一个包
            }
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志
    }
}

uint8_t Serial_GetRxFlag(void){
    return tx_flag;
}  

void Serial_SendByte(uint8_t byte){
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送缓冲区为空
    USART_SendData(USART1, byte);
}

void Serial_SendArray(uint8_t* array, uint16_t length){
    for(uint16_t i = 0; i < length; i++){
        Serial_SendByte(array[i]);
    }
}

void Serial_SendPacket(void){
    Serial_SendByte(0xff); // 发送包头
    Serial_SendArray(tx_packet, 4); // 发送数据部分
    Serial_SendByte(0xfe); // 发送包尾
}