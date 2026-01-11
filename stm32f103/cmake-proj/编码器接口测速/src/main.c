#include "stm32f10x.h"
#include "OLED.h"
#include "Encoder.h"

int main(void){
    OLED_Init();
    Encoder_Init();
    OLED_ShowString(1, 1, "Num:");
    while(1){
        OLED_ShowNum(1, 5, Encoder_GetCount(), 5);
    }
}
