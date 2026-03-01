#include "stm32f10x.h"
#include "OLED.h"   
#include "Key.h"
#include "MyFlash.h"
#include <stdint.h>
#include "Store.h"

uint16_t store_arr[4];

#define addr_start 0x08000010 // FLASH扇区1的起始地址
uint8_t key_num;

int main (void){

    Key_Init(); // 初始化按键
    OLED_Init(); // 初始化OLED显示屏

    OLED_ShowHexNum(1, 1, MyFlash_ReadWord(addr_start), 8);
    OLED_ShowHexNum(2, 1, MyFlash_ReadHalfWord(addr_start), 4);
    OLED_ShowHexNum(3, 1, MyFlash_ReadByte(addr_start), 2);

    // 写入数据
    MyFlash_ErasePage(0x0800FC00); // 擦除FLASH扇区7
    MyFlash_ProgramWord(0x0800FC00, 0x12345678); // 编程
    MyFlash_ProgramHalfWord(0x0800FC10, 0xABCD); // 编程一个半字
    
    while(1){
        key_num = GetKeyNum(); // 获取按键状态
        if(key_num == 1){ // 按键1被按下
            MyFlash_ErasePage(0x08000000); // 擦除FLASH扇区1
            OLED_ShowString(4, 1, "Page Erased");

            store_arr[0]++;
            store_arr[1] += 2;
            store_arr[2] += 3;
            store_arr[3] += 4;
            Store_Save(); // 保存数据到FLASH
        }
        if(key_num == 2){ // 按键2被按下
            MyFlash_EraseAllPage(); // 擦除所有FLASH扇区
            OLED_ShowString(4, 1, "All Pages Erased");

            Store_Clear(); // 清空存储数据
        }

        // OLED显示store_arr存储数据
        // OLED_ShowHexNum(5, 1, store_arr[0], 4);
        // OLED_ShowHexNum(6, 1, store_arr[1], 4);
        // OLED_ShowHexNum(7, 1, store_arr[2], 4);
        // OLED_ShowHexNum(8, 1, store_arr[3], 4);
    }
}
