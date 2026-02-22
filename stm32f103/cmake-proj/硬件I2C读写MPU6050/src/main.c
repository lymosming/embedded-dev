#include "stm32f10x.h"
#include "OLED.h"
#include "Mpu6050.h"

uint8_t ID;
uint8_t ax, ay, az, gx, gy, gz;

int main (void){

    OLED_Init();
    Mpu6050_Init();
    ID = Mpu6050_GetID();
    OLED_ShowHexNum(1, 1, ID, 2);
    
    while(1){

        Mpu6050_GetData(&ax, &ay, &az, &gx, &gy, &gz);
        OLED_ShowSignedNum(2, 1, ax, 5);
        OLED_ShowSignedNum(3, 1, ay, 5);
        OLED_ShowSignedNum(4, 1, az, 5);
        OLED_ShowSignedNum(2, 8, gx, 5);
        OLED_ShowSignedNum(3, 8, gy, 5);
        OLED_ShowSignedNum(4, 8, gz, 5);
    }
}
