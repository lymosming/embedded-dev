#include "stm32f10x.h"
#include "OLED.h"
#include "w25q64.h"

uint8_t MID;
uint16_t DID;
uint8_t arr_write[] = {0x01, 0x02, 0x03, 0x04};
uint8_t arr_read[4];

int main (void){
    OLED_Init();
    w25q64_Init();

    OLED_ShowString(1, 1, "MID:    DID:    ");
    OLED_ShowString(2, 1, "W:");
    OLED_ShowString(3, 1, "R:");

    w25q64_ReadID(&MID, &DID);
    OLED_ShowHexNum(1, 5, MID, 2);
    OLED_ShowHexNum(1, 12, DID, 2);

    w25q64_SectorErase(0x000000);
    w25q64_PageProgram(0x000000, arr_write, 4);
    w25q64_ReadData(0x000000, arr_read, 4);

    OLED_ShowHexNum(2, 3, arr_write[0], 2);
    OLED_ShowHexNum(2, 6, arr_write[0], 2);
    OLED_ShowHexNum(2, 9, arr_write[0], 2);
    OLED_ShowHexNum(2, 12, arr_write[0], 2);

    OLED_ShowHexNum(3, 3, arr_read[0], 2);
    OLED_ShowHexNum(3, 6, arr_read[0], 2);
    OLED_ShowHexNum(3, 9, arr_read[0], 2);
    OLED_ShowHexNum(3, 12, arr_read[0], 2);
    
    while(1){

    }
}
