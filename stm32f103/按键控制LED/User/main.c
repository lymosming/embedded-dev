#include "stm32f10x.h"
#include "LED.h"
#include "Key.h"

uint8_t global_keynum;

int main(void){
	// LED 初始化
	LED_Init();
	// Key 初始化
	Key_Init();
	
	while(1){
		
		// 不断读取key的值
		global_keynum = Key_GetNum();
		// 按键1
		if(global_keynum == 1){
			// LED1_ON();
			LED1_Turn();
		}else if(global_keynum == 2){
			LED2_Turn();
		}
	}
	
}
