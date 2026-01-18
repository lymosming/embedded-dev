#include "stm32f10x.h"
#include "OLED.h"
#include "AD.h"
#include "Delay.h"

uint16_t adc_value;
float voltage; // 电压

int main(void){
    OLED_Init();
    AD_Init();
    OLED_ShowString(1, 1, "ADCValue:");
    OLED_ShowString(2, 1, "Voltage:0.00V");

    while(1){
        adc_value = AD_GetValue();
        voltage = ((float)adc_value / 4095.0) * 3.3; // 0-3.3V
        OLED_ShowNum(1, 10, adc_value, 4);
        OLED_ShowNum(2, 9, voltage, 1); // 整数部分
        // 因为shownum只能显示整数，所以乘以100取整后显示小数部分
        OLED_ShowNum(2, 11, (uint16_t)(voltage * 100) % 100, 2); // 小数部分
        Delay_ms(500);
    }
}
