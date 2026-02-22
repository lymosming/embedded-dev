#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

void LED_Init(void){
    // 初始化GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct); 

}

void LED_On(void){
    // 点亮
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED_Off(void){
    // 熄灭
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
}