#include "stm32f10x.h"

void Key_Init(void){
    // 初始化GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PB0作为按键输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetState(void){
    // 读取按键状态
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_RESET){
        return 1; // 按键按下
    }else{
        return 0; // 按键未按下
    }
}
