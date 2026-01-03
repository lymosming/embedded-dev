#include "stm32f10x.h"

/**
* PA0引脚就是TIM2的ETR引脚 在这个引脚输入一个外部时钟
*
*/
void Timer_Init(void){
	
	// 开启TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// 开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	
	// 配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 通过ETR引脚的外部时钟模式2配置
	// TIM_ExtTRGPolarity_NonInverted 高电平或上升沿有效（不反相）
	// 0x00 表示不用滤波器 =》 红外传感器乱跳。 改用0x0F 修改采样频率就会稳定
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1; // 周期 ARR 从0计数到9 自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // PSC预分频器的值 这里不需要分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器，这里用不上，直接给0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 先清空一次中断状态标志 
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 开启更新中断到NVIC的通路
	
	// 配置NVIC 中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}

/**
*返回CNT计数器的值
*/
uint16_t Timer_GetCounter(void){
	return TIM_GetCounter(TIM2);
}
