#include "stm32f10x.h"
#include "OLED.h"
#include "AD.h"
#include "Delay.h"

uint16_t ad0, ad1, ad2, ad3;

int main(void){

    OLED_Init();
    AD_Init();

    OLED_ShowString(1, 1, "AD0:");
    OLED_ShowString(2,1, "AD1:");
    OLED_ShowString(3,1, "AD2:");
    OLED_ShowString(4,1, "AD3:");

    while(1){
        ad0 = AD_GetValue(ADC_Channel_0);
        ad1 = AD_GetValue(ADC_Channel_1);
        ad2 = AD_GetValue(ADC_Channel_2);
        ad3 = AD_GetValue(ADC_Channel_3);
        OLED_ShowNum(1, 5, ad0, 4);
        OLED_ShowNum(2, 5, ad1, 4);
        OLED_ShowNum(3, 5, ad2, 4); 
        OLED_ShowNum(4, 5, ad3, 4);
        Delay_ms(500);
    }
}

