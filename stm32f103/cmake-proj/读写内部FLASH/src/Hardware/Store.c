#include "stm32f10x.h"
#include "MyFlash.h"
#include <stdint.h>

#define STORE_START_ADDR 0x0800FC00 // 存储起始地址
#define STORE_COUNT 512 // 存储单元数量
uint16_t store_data[STORE_COUNT]; // 存储数据数组

void Store_Init(void){
    if(MyFlash_ReadHalfWord(STORE_START_ADDR) != 0xA5A5){
        MyFlash_ErasePage(STORE_START_ADDR); // 擦除存储页
        MyFlash_ProgramHalfWord(STORE_START_ADDR, 0xA5A5); // 写入标志位
        uint16_t i;
        for(i = 0; i < STORE_COUNT; i++){
            MyFlash_ProgramHalfWord(STORE_START_ADDR + i * 2, 0x0000); // 初始化存储单元为0
        }
    }

    // 从FLASH读取数据到数组
    for(uint16_t i = 0; i < STORE_COUNT; i++){
        store_data[i] = MyFlash_ReadHalfWord(STORE_START_ADDR + i * 2); // 从FLASH读取数据到数组
    }
}

void Store_Save(void){
    MyFlash_ErasePage(STORE_START_ADDR); // 擦除存储页

    // 将数组数据写入FLASH
    for(uint16_t i = 0; i < STORE_COUNT; i++){
        MyFlash_ProgramHalfWord(STORE_START_ADDR + i * 2, store_data[i]); // 将数组数据写入FLASH
    }
}

void Store_Clear(void){
    for(uint16_t i = 0; i < STORE_COUNT; i++){
        store_data[i] = 0x0000; // 清空数组数据
    }
    Store_Save(); // 保存清空后的数据到FLASH
}
