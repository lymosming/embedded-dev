#include "stm32f10x.h"

int16_t count;

void Encoder_Init(void){
	
	// 使能 GPIOB AFIO 外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 配置中断引脚 AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); // 引脚0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); // 引脚1
	
	// 配置EXTI
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断触发模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	// 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; // 中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
}

int16_t Encoder_GetCount(void){
	int16_t temp;
	temp = count;
	count = 0; // 重置清零
	return temp;
}

void EXTI0_IRQHandler(void){ // 编码器A接口
	
	if(EXTI_GetITStatus(EXTI_Line0) == SET){
		// 反转
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0){ // 防止抖动
			
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
				count--;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0); // 清除终端标志位
	}
}

void EXTI1_IRQHandler(void){ // 编码器B接口
	
	if(EXTI_GetITStatus(EXTI_Line1) == SET){
		// 正转
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0){
				count++;
			}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

