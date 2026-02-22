#include "stm32f10x.h"  
#include "OLED.h"
#include "Serial.h"
#include "Key.h"

int main (void){

    OLED_Init();
    Serial_Init();
    Key_Init();
    // Serial_SendPacket();

    OLED_ShowString(1, 1, "RxPacket:");
    OLED_ShowString(3, 1, "TxPacket:");


    while(1){
        // 检测按键按下，发送数据包
        if(Key_GetState()){
            tx_packet[0]++;
            tx_packet[1]++;
            tx_packet[2]++;
            tx_packet[3]++;
            // 发送数据包
            Serial_SendPacket();
            OLED_ShowHexNum(4, 1, tx_packet[0], 2);
            OLED_ShowHexNum(4, 4, tx_packet[1], 2);
            OLED_ShowHexNum(4, 7, tx_packet[2], 2);
            OLED_ShowHexNum(4, 10, tx_packet[3], 2);
            while(Key_GetState()); // 等待按键释放
        }

        // 显示接收到的数据包
        if(Serial_GetRxFlag()){
            OLED_ShowHexNum(2, 1, tx_packet[0], 2);
            OLED_ShowHexNum(2, 4, tx_packet[1], 2);
            OLED_ShowHexNum(2, 7, tx_packet[2], 2);
            OLED_ShowHexNum(2, 10, tx_packet[3], 2);
        }
    }
}
