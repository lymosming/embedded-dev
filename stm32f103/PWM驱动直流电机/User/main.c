#include "stm32f10x.h"
#include "Key.h"
#include "OLED.h"
#include "Motor.h"

uint8_t key_num;
int8_t speed; // 有符号的速度变量


int main (void) {
	
	OLED_Init();
	Key_Init();
	Motor_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	
	while(1){
		key_num = Key_GetNum();
		if(key_num == 1){
			speed += 20;
			if(speed > 100){
				speed = -100;
			}
		}
		Motor_SetSpeed(speed);
		OLED_ShowSignedNum(1, 7, speed, 3);
	}
	
}
