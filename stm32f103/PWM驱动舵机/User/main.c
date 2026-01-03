#include "stm32f10x.h"
#include "Key.h"
#include "Servo.h"
#include "OLED.h"

uint16_t key_num;
float angle;

int main (void){
	
	Key_Init();
	Servo_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "Angle:");
	
	while(1){
		key_num = Key_GetNum();
		
		if(key_num == 1){
			angle += 60;
			if(angle > 180){
				angle = 0;
			}
		}
		Servo_SetAngle(angle);
		OLED_ShowNum(1, 7, angle, 3);
	}
	
}
