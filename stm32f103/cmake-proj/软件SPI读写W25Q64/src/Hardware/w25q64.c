#include "MySpi.h"
#include "w25q64_ins.h"

void w25q64_init(void){
    MySpi_Init();
}

// 获取厂商ID 和 设备ID
void w25q64_ReadID(uint8_t *mid, uint16_t *did){
    MySpi_Start();
    MySpi_SwapByte(W25Q64_JEDEC_ID); // 读ID号指令
    *mid = MySpi_SwapByte(W25Q64_DUMMY_BYTE); // 随便抛送数据比如0xFF ，重要的是收数据
    *did = MySpi_SwapByte(W25Q64_DUMMY_BYTE); // 再交换一次，返回设备ID的高8位
    *did <<= 8; // 左移8位
    *did |= MySpi_SwapByte(W25Q64_DUMMY_BYTE); // 获取低8位，并与前面高8位组合
    MySpi_Stop();
}

// 写使能
void w25q64_WriteEnable(void){
    MySpi_Start();
    MySpi_SwapByte(W25Q64_WRITE_ENABLE);
    MySpi_Stop();
}

// 等待Busy为0
void w25q64_WaitBusy(void){
    MySpi_Start();
    uint32_t timeout;
    timeout = 100000;
    MySpi_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
    while((MySpi_SwapByte(W25Q64_DUMMY_BYTE) & 0x001) == 0x01){
        timeout--;
        if(timeout == 0){
            break;
        }
    }
    MySpi_Stop();
}

// 发送页编程
void w25q64_PageProgram(uint32_t addr, uint8_t *data_arr, uint16_t count){
    w25q64_WriteEnable();
    MySpi_Start();
    MySpi_SwapByte(W25Q64_PAGE_PROGRAM);
    // 发送24位地址
    MySpi_SwapByte(addr >> 16);
    MySpi_SwapByte(addr >> 8);
    MySpi_SwapByte(addr);
    uint8_t i;
    for(i = 0; i < 8; i++){
        MySpi_SwapByte(data_arr[i]);
    }
    MySpi_Stop();
    w25q64_WaitBusy(); // 等待
}

// 擦除扇区
void w25q64_SectorErase(uint32_t addr){
    w25q64_WriteEnable(); // 写使能

    MySpi_Start();
    MySpi_SwapByte(W25Q64_SECTOR_ERASE_4KB); // 发送指令
    MySpi_SwapByte(addr >> 16);
    MySpi_SwapByte(addr >> 8);
    MySpi_SwapByte(addr);
    MySpi_Stop();
    w25q64_WaitBusy();
}

// 读取数据
void w25q64_ReadData(uint32_t addr, uint8_t *data_arr, uint32_t count){
    MySpi_Start();
    MySpi_SwapByte(W25Q64_READ_DATA);
    // 发送24位地址
    MySpi_SwapByte(addr >> 16);
    MySpi_SwapByte(addr >> 8);
    MySpi_SwapByte(addr);
    uint32_t i;
    for(i = 0; i < count; i++){
        data_arr[i] = MySpi_SwapByte(W25Q64_DUMMY_BYTE);
    }
    MySpi_Stop();
}