#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void){
	// RCC GPIO 外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while(1){
			GPIO_Write(GPIOA, ~0x0001); // 0x0000 0000 0000 0001
			Delay_ms(500);
			GPIO_Write(GPIOA, ~0x0002); 
			Delay_ms(500);
			GPIO_Write(GPIOA, ~0x0004); 
			Delay_ms(500);
			GPIO_Write(GPIOA, ~0x0008); // 0x0000 0000 0000 1000
			Delay_ms(500);
			GPIO_Write(GPIOA, ~0x0010); 
			Delay_ms(500);
			GPIO_Write(GPIOA, ~0x0020); // 0x0000 0000 0001 0100
			Delay_ms(500);
			GPIO_Write(GPIOA, ~0x0040); 
			Delay_ms(500);
			GPIO_Write(GPIOA, ~0x0080); 
			Delay_ms(500);
		
	}
}
