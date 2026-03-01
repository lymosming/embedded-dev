#include "stm32f10x.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"
#include "OLED.h"
#include "Key.h"

uint8_t key_num;
uint16_t arr_write[] = {0x1234, 0x5678};
uint16_t arr_read[2];

int main (void){
    
    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

    PWR_BackupAccessCmd(ENABLE); // 访问控制
    OLED_ShowString(1, 1, "W:");
    OLED_ShowString(2, 1, "R:");

    while(1){

        key_num = getKeyNum();
        if(key_num == 1){
            arr_write[0]++; 
            arr_write[1]++;
            BKP_WriteBackupRegister(BKP_DR1, arr_write[0]);
            BKP_WriteBackupRegister(BKP_DR2, arr_write[1]);

            OLED_ShowHexNum(1, 3, arr_write[0], 4);
            OLED_ShowHexNum(1, 8, arr_write[1], 4);
        }
        arr_read[0] = BKP_ReadBackupRegister(BKP_DR1);
        arr_read[1] = BKP_ReadBackupRegister(BKP_DR2);

        OLED_ShowHexNum(2, 3, arr_read[0], 4);
        OLED_ShowHexNum(2, 8, arr_read[1], 4);

    }
}
