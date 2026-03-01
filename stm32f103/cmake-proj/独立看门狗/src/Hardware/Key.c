#include "stm32f10x.h"

void Key_Init(void){

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能GPIOA时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 选择PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

}

uint8_t GetKeyNum(void){

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0){ // 如果PA0被按下
        return 1; // 返回1
    }

    return 0; // 没有按键被按下返回0
}