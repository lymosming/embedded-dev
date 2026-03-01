#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

int main (void){

    OLED_Init();

    OLED_ShowString(1, 1, "WWDG TEST");

    if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET){
        OLED_ShowString(2, 1, "WWDGRST");
        Delay_ms(500);
        OLED_ShowString(2, 1, "          ");
        RCC_ClearFlag();
    }else{
        OLED_ShowString(3, 1, "RST OK");
        Delay_ms(500);
        OLED_ShowString(3, 1, "          ");
        Delay_ms(100);
    }

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE); // 使能WWDG时钟
    WWDG_SetPrescaler(WWDG_Prescaler_8); // 设置分频系数为8
    WWDG_SetWindowValue(0x40 | 21); // 设置窗口值 30ms 计算公式：T = (4096 * 8) / (72MHz / 8) * (0x40 - 21) = 30ms
    WWDG_Enable(0x40 | 54); // 会喂一次 使能WWDG并设置计数值  50ms 计算公式：T = (4096 * 8) / (72MHz / 8) * (0x40 - 54) = 50ms

    while(1){
        GetKeyNum(); // 获取按键状态 当按键按住不放时，以下代码就不会执行，WWDG就会复位系统

        OLED_ShowString(4, 1, "FEED WWDG");
        Delay_ms(20); // 延时
        OLED_ShowString(4, 1, "          ");
        Delay_ms(20); // 延时

        WWDG_SetCounter(0x40 | 54); // 刷新WWDG计数值  50ms 计算公式：T = (4096 * 8) / (72MHz / 8) * (0x40 - 54) = 50ms

    }
}
