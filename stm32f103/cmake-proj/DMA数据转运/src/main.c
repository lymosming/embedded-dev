#include "stm32f10x.h"
#include "OLED.h"
#include "MyDMA.h"
#include "Delay.h"

uint8_t data_a[] = {0x01, 0x02, 0x03, 0x04};
uint8_t data_b[] = {0, 0, 0, 0};

int main(void){

    OLED_Init();

    /*
    自动转运
    // 转运前
    OLED_ShowHexNum(1, 1, data_a[0], 2);
    OLED_ShowHexNum(1, 4, data_a[1], 2);
    OLED_ShowHexNum(1, 7, data_a[2], 2);
    OLED_ShowHexNum(1, 10, data_a[3], 2);
    OLED_ShowHexNum(2, 1, data_b[0], 2);
    OLED_ShowHexNum(2, 4, data_b[1], 2);
    OLED_ShowHexNum(2, 7, data_b[2], 2);
    OLED_ShowHexNum(2, 10, data_b[3], 2);
    MyDMA_Init((uint32_t)data_a, (uint32_t)data_b, 4);
    // 转运后
    OLED_ShowHexNum(3, 1, data_a[0], 2);
    OLED_ShowHexNum(3, 4, data_a[1], 2);
    OLED_ShowHexNum(3, 7, data_a[2], 2);
    OLED_ShowHexNum(3, 10, data_a[3], 2);
    OLED_ShowHexNum(4, 1, data_b[0], 2);
    OLED_ShowHexNum(4, 4, data_b[1], 2);
    OLED_ShowHexNum(4, 7, data_b[2], 2);
    OLED_ShowHexNum(4, 10, data_b[3], 2);

    */

    // 动态转运
    OLED_ShowString(1, 1, "dataa:");
    OLED_ShowHexNum(1, 8, (uint32_t)data_a, 8);
    OLED_ShowString(3, 1, "datab:");
    OLED_ShowHexNum(3, 8, (uint32_t)data_b, 8);
    MyDMA_Init((uint32_t)data_a, (uint32_t)data_b, 4);

    while(1){

        data_a[0]++; data_a[1]++; data_a[2]++; data_a[3]++;
        // 转运前
        OLED_ShowHexNum(2, 1, data_a[0], 2);
        OLED_ShowHexNum(2, 4, data_a[1], 2);
        OLED_ShowHexNum(2, 7, data_a[2], 2);
        OLED_ShowHexNum(2, 10, data_a[3], 2);
        OLED_ShowHexNum(4, 1, data_b[0], 2);
        OLED_ShowHexNum(4, 4, data_b[1], 2);
        OLED_ShowHexNum(4, 7, data_b[2], 2); 
        OLED_ShowHexNum(4, 10, data_b[3], 2);  
        Delay_ms(1000);

        MyDMA_Transfer(); // 调用转运函数进行数据转运

        // 转运后
        OLED_ShowHexNum(2, 1, data_a[0], 2);
        OLED_ShowHexNum(2, 4, data_a[1], 2);
        OLED_ShowHexNum(2, 7, data_a[2], 2);
        OLED_ShowHexNum(2, 10, data_a[3], 2);
        OLED_ShowHexNum(4, 1, data_b[0], 2);
        OLED_ShowHexNum(4, 4, data_b[1], 2);
        OLED_ShowHexNum(4, 7, data_b[2], 2); 
        OLED_ShowHexNum(4, 10, data_b[3], 2);  
        
        Delay_ms(1000);
    }
}

