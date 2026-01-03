#include "stm32f10x.h"
#include "PWM.h"

void Servo_Init(void){
	PWM_Init();
	
}

/*
设置舵机角度
0度 =》 500
180度 =》 2500

*/
void Servo_SetAngle(float angle){
	
	PWM_SetCompare2(angle / 180 * 2000 + 500);
}
