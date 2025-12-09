#include "stm32f10x.h"
#include "Delay.h"

int main(void){
	
	// 使用B口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while(1){
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_12); // 设置为低电平
		Delay_ms(1000);
		GPIO_SetBits(GPIOB, GPIO_Pin_12); // 高电平
		Delay_ms(1000);
	}
	
}
