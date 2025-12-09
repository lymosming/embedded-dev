#include "stm32f10x.h"
#include "Buzzer.h"
#include "LightSensor.h"

int main(void){
	
	Buzzer_Init();
	LightSensor_Init();
	while(1){
		
		// π‚œﬂ∞µ
		if(LightSensor_Get() == 1){
			Buzzer_Off();
		}else{
			
			Buzzer_On();
		}
	}
	
}
