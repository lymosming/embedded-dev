#include "stm32f10x.h"
#include "OLED.h"
#include "IC.h"
#include "PWM.h"

int main (void){
	
	OLED_Init();
	IC_Init();
	PWM_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Duty:00%");
	
	// …Ë÷√
	PWM_SetPresclaer(720 - 1);
	PWM_SetCompare1(50);
	
	while(1){
		// ∂¡»°œ‘ æ
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(2, 6, IC_GetDuty(), 2);
	}
	
}
