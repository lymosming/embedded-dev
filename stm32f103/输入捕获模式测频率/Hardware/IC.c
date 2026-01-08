// 输入捕获
#include "stm32f10x.h"

void IC_Init(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // PWM.c的TIM2是输出， 这里用TIM3作为输入
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 选择内部时钟
	TIM_InternalClockConfig(TIM3);
	
	
	// 配置时机单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // 设置大点，防止溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	
	// 初始化输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 极性 上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 直连通道
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// 选择触发源
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	// 配置从模式为Reset
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	// 开启定时器
	TIM_Cmd(TIM3, ENABLE);
	
}

// 获取频率
uint32_t IC_GetFreq(void){
	
	return 1000000 / (TIM_GetCapture1(TIM3) + 1); 
	// Fx = Fc / N    
	// Fc = 72M / (PSC + 1) = 1M 
	// N 就是读取CCR的值
	
	
}

