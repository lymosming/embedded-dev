#include "stm32f10x.h"
#include "CountSensor.h"
#include "OLED.h"

int main(void){
	
	OLED_Init();
	CountSensor_Init();
	OLED_ShowString(1, 1, "count:");
	
	while(1){
		
		OLED_ShowNum(1, 7, Count6Sensor_Get(), 5);
		
	}
}
