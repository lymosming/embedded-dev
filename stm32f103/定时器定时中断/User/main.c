#include "stm32f10x.h"
#include "OLED.h"
#include "Timer.h"

uint16_t t_num;

int main(void){
	
	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "Num:");
	
	while(1){
		OLED_ShowNum(1, 5, t_num, 5);
		// CNT计数器值的变化
		OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5);
	}
	
}

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		t_num++;
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
