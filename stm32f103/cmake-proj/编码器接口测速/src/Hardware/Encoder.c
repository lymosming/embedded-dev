#include "stm32f10x.h"

void Encoder_Init(void){
    // Encoder initialization code goes here
    // Configure GPIO pins for encoder input
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Assuming encoder connected to PA6 and PA7
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // Input pull-up
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 配置时机单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    TIM_TimeBaseStruct.TIM_Period = 65536 - 1;
    TIM_TimeBaseStruct.TIM_Prescaler = 1 - 1;
    TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);

    // 配置数字比较单元
    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    // 通道1配置
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(TIM3, &TIM_ICInitStruct);
    // 通道2配置
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM3, &TIM_ICInitStruct);


    // 配置编码器接口
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 获取编码器计数值
 */
uint8_t Encoder_GetCount(void){
    return TIM_GetCounter(TIM3);
}
