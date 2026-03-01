#include "stm32f10x.h"

void Key_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置PA0为输入模式，带上拉电阻
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // 选择PA0和PA1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 输入模式，带上拉电阻
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t GetKeyNum(void){
    uint8_t key_num = 0;
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET){
        key_num = 1; // 按键1被按下
    }
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET){
        key_num = 2; // 按键2被按下
    }
    return key_num;
}   