#include "stm32f10x.h"

void PWM_Init(void){
	
	// 开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 选择时机单元时钟
	TIM_InternalClockConfig(TIM2);
	
	
	// 配置时机单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频、
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器，给0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	
	// 配置数字比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0; // CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	
	// 开启定时器
	TIM_Cmd(TIM2, ENABLE);
	
}

// 设置CCR
void PWM_SetCompare1(uint16_t compare){
	TIM_SetCompare1(TIM2, compare);
}

// 设置prescaler
void PWM_SetPrescaler(uint16_t prescaler){
	TIM_PrescalerConfig(TIM2, prescaler, TIM_PSCReloadMode_Immediate);
}

