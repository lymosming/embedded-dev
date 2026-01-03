#include "stm32f10x.h"

void Timer_Init(void){
	
	// 开启时钟 TIM2是APB1总线的外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 选择时机单元时钟
	TIM_InternalClockConfig(TIM2);
	
	// 配置时机单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式 向上计数
	/*
	定时频率 = 72M / (PSC+1) / (ARR+1)
	定时1秒也就是定时频率为 1HZ
	1HZ = 72M / (7200 - 1 + 1) / (10000 - 1 + 1)
	1M = 100000HZ
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // 周期 ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器，这里不需要，直接给0 （高级定时器才有）
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 先清除一次更新中断标志位，防止一开始就进入中断。导致一开始计数就是1
	// 避免刚初始化完就进入中断
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	// 使能更新中断 开启更新中断到NVIC的通路
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // 中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	// 启动定时器
	TIM_Cmd(TIM2, ENABLE);
}

/**
 *  中断函数，产生中断时，就会执行此函数
 * 可以放到使用的地方进行中断逻辑处理
 *//*
void TIM2_IRQHandler(void){
	
	// 判断中断标志位
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		// 清除中断标志位
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
