#include "stm32f10x.h"

uint8_t tx_packet[4];
uint8_t rx_packet[4];
uint8_t tx_flag;

void Serial_Init(void){
    // 初始化代码
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    // 初始化GPIOA
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; // RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 初始化USART1
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStruct);
    USART_Cmd(USART1, ENABLE);

    // 使能接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // 中断组优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

}

// 中断处理
void USART1_IRQHandler(void){
    static uint8_t tx_state = 0;
    static uint8_t rx_index = 0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        char data = USART_ReceiveData(USART1);

        if(tx_state == 0){ // 等待包头
            if(data == 0xff){
                tx_state = 1;
                rx_index = 0;
            }
            
        }else if(tx_state == 1){ // 接收数据
            rx_packet[rx_index++] = data;
            if(rx_index >= 4){
                tx_state = 2;
            }

        }else if(tx_state == 2){ // 等待包尾
            if(data == 0xfe){
                // 完整包接收完毕，处理数据
                tx_flag = 1;
            }
            tx_state = 0;
        }
        
    }
}

void Serial_SendChar(char c){
    // 发送一个字符
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);
}

// 发送字符串
void Serial_SendString(const char* str){
    while(*str){
        Serial_SendChar(*str++);
    }
}