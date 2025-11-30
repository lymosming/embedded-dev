#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void){
	
	// RCC GPIO 2个外设
	
	// 开启GPIO时钟 RCC外设使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // GPIO_Mode_Out_PP 推挽输出（高低电平都有驱动能力）
	// GPIO_Mode_Out_OD 开漏输出（高电平没有驱动能力）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // GPIOA => GPIO_Pin_0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 初始化GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 设置为低电平 LED灯亮
	GPIO_SetBits(GPIOA, GPIO_Pin_0); // 设置为低电平 LED灯灭
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET); // Bit_RESET 设置为低电平，Bit_SET为高电平
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0); // 使用0|1 BitAction 强制类型转换
	*/
	while(1){
		// 开 延时 关 延时
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		Delay_ms(1000);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		Delay_ms(1000);
		
	}
	
}
