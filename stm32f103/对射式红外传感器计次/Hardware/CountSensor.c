#include "stm32f10x.h"
#include "Delay.h"

uint16_t s_count;

void CountSensor_Init(void){
	
	// APB2 的外设GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 开启AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// EXIT NVIC（内核外设） 都不需要开启时钟
	
	// 设置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	// AFIO中断引脚选择配置 14引脚
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// 配置 EXIT中断
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // 配置中断线 对应PB14引脚
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 指定中断线的新状态
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 指定为中断模式
	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 指定下降沿触发
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 指定上升沿

	EXTI_Init(&EXTI_InitStructure);
	
	
	// 配置 NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // 指定中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
	// &为取地址符号
	NVIC_Init(&NVIC_InitStructure);
	
}

uint16_t Count6Sensor_Get(void){
	return s_count;
}

/**
 * 中断函数
 */
void EXTI15_10_IRQHandler(void){
	
	// 判断是14进来的
	if(EXTI_GetITStatus(EXTI_Line14) == SET){
		// 为低电平 防抖
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0){
			Delay_ms(1);
			s_count ++;
			Delay_ms(1);
		}
		EXTI_ClearITPendingBit(EXTI_Line14); // 清除中断标志位 防止一直申请中断
		Delay_ms(100);
	}
}
