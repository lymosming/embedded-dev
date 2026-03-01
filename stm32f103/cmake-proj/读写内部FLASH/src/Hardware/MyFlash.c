#include "stm32f10x.h"

// 读取FLASH内存
uint32_t MyFlash_ReadWord(uint32_t addr){
    return *((__IO uint32_t *)addr);
}

// 读取FLASH内存的一半字
uint16_t MyFlash_ReadHalfWord(uint32_t addr){
    return *((__IO uint16_t *)addr);
}

// 读取FLASH内存的一个字节
uint8_t MyFlash_ReadByte(uint32_t addr){
    return *((__IO uint8_t *)addr);
}

// 擦除FLASH所有扇区
void MyFlash_EraseAllPage(void){
    FLASH_Unlock(); // 解锁FLASH
    FLASH_EraseAllPages(); // 擦除所有扇区
    FLASH_Lock(); // 锁定FLASH
}

// 擦除FLASH指定扇区
void MyFlash_ErasePage(uint32_t page_addr){
    FLASH_Unlock(); // 解锁FLASH
    FLASH_ErasePage(page_addr); // 擦除指定扇区
    FLASH_Lock(); // 锁定FLASH
}

// 页编程
void MyFlash_ProgramWord(uint32_t addr, uint32_t data){
    FLASH_Unlock(); // 解锁FLASH
    FLASH_ProgramWord(addr, data); // 编程一个字
    FLASH_Lock(); // 锁定FLASH
}

// 半字编程
void MyFlash_ProgramHalfWord(uint32_t addr, uint16_t data){
    FLASH_Unlock(); // 解锁FLASH
    FLASH_ProgramHalfWord(addr, data); // 编程一个半字
    FLASH_Lock(); // 锁定FLASH
}
