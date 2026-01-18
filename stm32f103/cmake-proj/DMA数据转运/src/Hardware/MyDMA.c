#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include <stdint.h>

uint8_t Temp_size; // 用于存储传输数据的大小

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint8_t size){

    Temp_size = size; // 保存传输数据的大小
    // DMA是AHB总线外设，需要使能DMA的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // DMA结构体初始化
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA; // 外设站点的基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = AddrB; // 存储器站点地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 传输方向 外设站点作为数据源
    DMA_InitStructure.DMA_BufferSize = size; // 传输数据的大小 传输次数
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度  字节方式
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable; // 外设地址自增;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; // 存储器数据宽度 字节方式;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 存储器地址自增;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 传输模式  普通模式;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // 优先级  中等优先级;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; // 使用软件触发;
    // DMA1通道1
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  //  DMA_Cmd(DMA1_Channel1, ENABLE); // 使能DMA1通道1
    DMA_Cmd(DMA1_Channel1, DISABLE); // 先不使能DMA1通道1


}

// DMA数据转运函数 调用一次转运一次
void MyDMA_Transfer(void){
    
    DMA_Cmd(DMA1_Channel1, DISABLE); // 先关闭DMA通道
    DMA_SetCurrDataCounter(DMA1_Channel1, Temp_size);
    DMA_Cmd(DMA1_Channel1, ENABLE); // 重新使能DMA通道，开始传输数据
    // 等待传输完成
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1); // 清除传输完成标志
}
