#include "stm32f10x.h"
#include "Serial.h"
#include "Delay.h"
#include "OLED.h"

int main (void){
    OLED_Init();
    Serial_Init();
    OLED_ShowString(1, 1, "RxData:");

    while(1){
        // 查询的方式接收数据
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET); // 等待接收数据
        char recevied = USART_ReceiveData(USART1); // 读取接收数据
        OLED_ShowChar(2, 8, recevied); // 在OLED上显示接

        // 中断的方式接收数据
        if(Serial_GetRxFlag()){
            char intr_received = Serial_GetRxData();
            OLED_ShowChar(2, 8, intr_received); // 在OLED上显示接收的数据
        }

        OLED_ShowString(3, 1, "Running"); // 在OLED上显示字符串
        Delay_ms(100);
        OLED_ShowString(3, 1, "        "); // 清除显示
        Delay_ms(100);
        __WFI(); // 睡眠等待中断唤醒 进入睡眠后上面的Running字符串就不会显示了，只有当接收到数据时才会显示接收的数据
        
    }

    
}
