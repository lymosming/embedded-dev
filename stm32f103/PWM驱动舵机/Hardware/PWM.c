#include "stm32f10x.h"

void PWM_Init(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIO时钟
	
	// GPIO 配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2); // 配置内部时钟 选择时机单元时钟
	
	// 配置时机单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式 向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1; // ARR 20K
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC 预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器 这里不需要，给0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 初始化数字比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); // 初始化结构体
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出比较极性 高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; // CCR 由函数控制，给0  根据上述TIM_Period等值，CCR取值范围500-2500对应0.5ms-2.5ms
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); // 使用通道2 通道1是：TIM_OC1Init()
	
	TIM_Cmd(TIM2, ENABLE); // 启动定时器
	
}

/**
* 设置CCR
*/
void PWM_SetCompare2(uint16_t compare){
	TIM_SetCompare2(TIM2, compare);
}
