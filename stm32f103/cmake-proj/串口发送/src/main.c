#include "stm32f10x.h"
#include "Serial.h"
#include "OLED.h"
#include "Delay.h"

uint8_t myArr[2] = {0x66, 0x55};

int main( void ){
    Serial_Init();
    OLED_Init();

    Serial_SendByte('A');
    Serial_SendString("Hello");
    Serial_SendNumber(123, 3);
    Serial_SendArray(myArr, 2);

    printf("hhh%d\r\n", 456);   
    Serial_Printf("OLDE %d", 888);



    while(1){

    }
}
