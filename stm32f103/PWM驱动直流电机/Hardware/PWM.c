#include "stm32f10x.h"

void PWM_Init(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 选择时机单元时钟
	TIM_InternalClockConfig(TIM2);
	
	// 时机单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1; // PSC 可以72改成36，频率超2Khz，人耳听不见
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器 这里不需要 给0即可
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	
	// 数字比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出比较极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; // CCR
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); // 使用通道3
	
	
	// 开启定时器
	TIM_Cmd(TIM2, ENABLE);
	
}

void PWM_SetCompare3(uint16_t compare){
	TIM_SetCompare3(TIM2, compare);
}
