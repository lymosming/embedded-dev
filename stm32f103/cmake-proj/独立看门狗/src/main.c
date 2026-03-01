#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"
#include "Key.h"

int main (void){

    OLED_Init();

    OLED_ShowString(1, 1, "IWDG TEST");

    if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET){
        OLED_ShowString(2, 1, "IWDGRST");
        Delay_ms(500);
        OLED_ShowString(2, 1, "          ");
        RCC_ClearFlag();
    }else{
        OLED_ShowString(3, 1, "RST OK");
        Delay_ms(500);
        OLED_ShowString(3, 1, "          ");
        Delay_ms(100);
    }

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // IWDG寄存器允许写入
    IWDG_SetPrescaler(IWDG_Prescaler_16); // 设置IWDG预分频值为16 计算方式：IWDG计数频率=40KHz/16=2.5KHz 1s计数2500次
    IWDG_SetReload(2499); // 设置IWDG重装载值为2499 计算公式：IWDG计数频率=40KHz/16=2.5KHz 1s计数2500次 2499为0.9996s
    IWDG_ReloadCounter(); // 喂一次
    IWDG_Enable(); // 使能IWDG

    while(1){
        GetKeyNum(); // 获取按键状态 当按键按住不放时，以下代码就不会执行，IWDG就会复位系统
        IWDG_ReloadCounter(); // 喂

        OLED_ShowString(4, 1, "FEED IWDG");
        Delay_ms(200); // 延时
        OLED_ShowString(4, 1, "          ");
        Delay_ms(600); // 延时

    }
}
