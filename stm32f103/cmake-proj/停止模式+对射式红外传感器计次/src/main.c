#include "stm32f10x.h"
#include "CountSensor.h"
#include "OLED.h"
#include "Delay.h"

int main(void){
	
	OLED_Init();
	CountSensor_Init();
	OLED_ShowString(1, 1, "count:");
	
	// 使能PWR时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	while(1){
		
		OLED_ShowNum(1, 7, Count6Sensor_Get(), 5);
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);

		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); // 进入STOP模式，等待中断唤醒
		SystemInit(); // 重新配置系统时钟 防止唤醒后时钟异常
	}
}
