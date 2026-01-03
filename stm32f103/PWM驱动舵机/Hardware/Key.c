#include "stm32f10x.h"
#include "Delay.h"

void Key_Init(void){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

uint16_t Key_GetNum(void){
	uint16_t num = 0;
	// 按键按下
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
		
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0); // 等待按键松开
		
		num = 1;
	}
	
	return num;
}
