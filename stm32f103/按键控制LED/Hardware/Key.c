#include "stm32f10x.h"
#include "Delay.h"

void Key_Init(void){
	// 使用GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入 读取按键
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void){
	uint8_t keynum = 0;
	// 如果为0低电平 按键按下
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
		// 防抖动
		Delay_ms(20);
		// 等待按键松开
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		
		// 防抖动
		Delay_ms(20);
		keynum = 1;
	}
	
	
	// 按键2
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		keynum = 2;
	}
	
	return keynum;
}
