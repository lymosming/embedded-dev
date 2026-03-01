#include "stm32f10x.h"
#include "OLED.h"
#include "MyRTC.h"
#include "Delay.h"

int main (void){

    OLED_Init();
    MyRTC_Init();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 使能PWR时钟

    OLED_ShowString(1, 1, "CNT:");
    OLED_ShowString(2, 1, "ALR:"); // 闹钟值
    OLED_ShowString(3, 1, "ALRF:"); // 闹钟标志位

    uint32_t alarm = RTC_GetCounter() + 10; // 设置闹钟为当前计数器值加10秒
    RTC_SetAlarm(alarm); // 设置闹钟
    OLED_ShowNum(2, 6, alarm, 10); // 显示设置的闹钟值

    // 启用Wakeup pin功能，使能外部中断唤醒
    PWR_WakeUpPinCmd(ENABLE);
    
    while(1){
        
        OLED_ShowNum(1, 6, RTC_GetCounter(), 10); // 显示RTC计数器的值
        OLED_ShowNum(3, 7, RTC_GetFlagStatus(RTC_FLAG_ALR), 1); // 显示闹钟标志位

        OLED_ShowString(4, 9, "STANDBY");
        Delay_ms(100);
        OLED_ShowString(4, 9, "        ");
        Delay_ms(100);
        OLED_Clear(); // 清屏
        
        // 进入待机模式
        PWR_EnterSTANDBYMode();

    }
}
