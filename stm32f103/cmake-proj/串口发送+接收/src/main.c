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
    }

    
}
