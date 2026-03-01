#include "stm32f10x.h"
#include "OLED.h"
#include "MyRTC.h"

int main (void){

    OLED_Init();
    MyRTC_Init();

    OLED_ShowString(1, 1, "Date:xxxx-xx-xx");
    OLED_ShowString(2, 1, "Time:xx:xx:xx");
    OLED_ShowString(3, 1, "CNT:");
    OLED_ShowString(4, 1, "DIV:");
    
    while(1){
        MyRTC_ReadTime();
        OLED_ShowNum(1, 6, t_time[0], 4); // 显示年
        OLED_ShowNum(1, 11, t_time[1], 2); // 显示月
        OLED_ShowNum(1, 14, t_time[2], 2); // 显示日
        OLED_ShowNum(2, 6, t_time[3], 2); // 显示时
        OLED_ShowNum(2, 9, t_time[4], 2); // 显示分
        OLED_ShowNum(2, 12, t_time[5], 2); // 显示秒
        OLED_ShowNum(3, 6, RTC_GetCounter(), 10); // 显示RTC计数器的值
        OLED_ShowNum(4, 6, RTC_GetDivider(), 10); // 显示RTC预分频器的值
    }
}
