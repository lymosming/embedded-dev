#include "stm32f10x.h"

void IC_Init(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	// 选择内部时钟
	TIM_InternalClockConfig(TIM3);
	
	// 配置时机单元
	TIM_TimeBaseInitTypeDef timeinit;
	timeinit.TIM_ClockDivision = TIM_CKD_DIV1;
	timeinit.TIM_CounterMode = TIM_CounterMode_Up;
	timeinit.TIM_Period = 65536 - 1; // 设置大点防止溢出
	timeinit.TIM_Prescaler = 72 - 1;
	timeinit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timeinit);
	
	// 初始化输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// 配置成PWMI模式的标准结构
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
	
	// 选择触发源
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); 
	
	// 配置从模式为Reset
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3, ENABLE);
}

// 获取频率
uint32_t IC_GetFreq(void){
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}


// 获取占空比
uint32_t IC_GetDuty(void){
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}


