#include "stm32f10x.h"
#include "OLED.h"

int main(void){
	
	OLED_Init();
	
	OLED_ShowChar(1, 1, 'B');
	
	while(1){
		
	}
	
}
