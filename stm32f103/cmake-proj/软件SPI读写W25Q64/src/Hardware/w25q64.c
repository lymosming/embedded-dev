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

