#include "stm32f10x.h"
#include "OLED.h"
#include "system_stm32f10x.h"
#include "Delay.h"

int main (void){

    OLED_Init();
    OLED_ShowString(1, 1,"SYSCLK:");
    OLED_ShowNum(1, 8, SystemCoreClock, 8);
    // 修改主频可以修改system_stm32f10x.h中的SYSCLK_FREQ_xxHZ的值
    // 修改之后下面的闪烁频率会改变，注意修改之后需要重新编译烧录才能生效
    while(1){
        OLED_ShowString(2, 1, "Running");
        Delay_ms(500);
        OLED_ShowString(2, 1, "         ");
        Delay_ms(500);
    }
}
