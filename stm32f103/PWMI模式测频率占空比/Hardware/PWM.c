#include "stm32f10x.h"

void PWM_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = GPIO_Pin_0;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	// 选择内部时钟
	TIM_InternalClockConfig(TIM2);
	
	// 配置时机单元
	TIM_TimeBaseInitTypeDef timeinit;
	timeinit.TIM_ClockDivision = TIM_CKD_DIV1;
	timeinit.TIM_CounterMode = TIM_CounterMode_Up;
	timeinit.TIM_Period = 100 - 1;
	timeinit.TIM_Prescaler = 720 - 1;
	timeinit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timeinit);
	
	// 配置数字比较单元
	TIM_OCInitTypeDef ocinit;
	TIM_OCStructInit(&ocinit);
	ocinit.TIM_OCMode = TIM_OCMode_PWM1;
	ocinit.TIM_OCPolarity = TIM_OCPolarity_High;
	ocinit.TIM_OutputState = TIM_OutputState_Enable;
	ocinit.TIM_Pulse = 0; // CCR
	TIM_OC1Init(TIM2, &ocinit);
	
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t compare){
	TIM_SetCompare1(TIM2, compare);
}

void PWM_SetPresclaer(uint16_t prescaler){
	TIM_PrescalerConfig(TIM2, prescaler, TIM_PSCReloadMode_Immediate);
}

