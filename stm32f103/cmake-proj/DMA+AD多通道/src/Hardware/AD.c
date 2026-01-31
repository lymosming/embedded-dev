#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

uint16_t AD_Value[4]; // 用于存储ADC转换结果的数组

// ADC扫描模式+DMA单次数据转运
/*

如果要改成 连续转换模式+循环DMA传输模式
1. 将ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // 连续转换模式 打开
2. 将DMA_Structure.DMA_Mode = DMA_Mode_Circular; // 循环传输模式 打开
3. 去掉AD_GetValue函数，直接调用ADC_SoftwareStartConvCmd 放在AD_Init()最后启动转换即可

*/
void AD_Init(void){

    // 开启GPIOA ADC时钟 DMA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC时钟配置为PCLK2的6分频，12MHz

    // 初始化APIOA
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化ADC
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; // 扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 单次扫描模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 4; // 转换通道数
    ADC_Init(ADC1, &ADC_InitStructure); 

    // 扫描4个通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);


    // 初始化DMA
    DMA_InitTypeDef DMA_Structure;
    DMA_Structure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; // 外设地址
    DMA_Structure.DMA_MemoryBaseAddr = (uint32_t)AD_Value; // 存到变量中
    DMA_Structure.DMA_DIR = DMA_DIR_PeripheralSRC; // 外设到内存
    DMA_Structure.DMA_BufferSize = 4; // 缓冲区大小 4个ADC通道所以给4次
    DMA_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据大小
    DMA_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 内存数据大小
    DMA_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 内存地址递增 每转运一次挪一个坑
    DMA_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不递增 始终转运同一位置的数据
    DMA_Structure.DMA_Mode = DMA_Mode_Normal; // 单次传输模式
    DMA_Structure.DMA_Priority = DMA_Priority_High; // 高优先
    DMA_Structure.DMA_M2M = DMA_M2M_Disable; // 不使用软件触发，使用ADC1硬件触发
    DMA_Init(DMA1_Channel1, &DMA_Structure);


    DMA_Cmd(DMA1_Channel1, ENABLE); // 使能DMA1通道1

    // 开启ADC到DMA的输出
    ADC_DMACmd(ADC1, ENABLE);

    ADC_Cmd(ADC1, ENABLE); // 使能ADC1

    // 校准ADC
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1)); // 等待校准复位完成
    ADC_StartCalibration(ADC1); // 开始校准
    while(ADC_GetCalibrationStatus(ADC1)); // 等待校准完成

}

void AD_GetValue(void){
    // 重新写入传输计数器
    DMA_Cmd(DMA1_Channel1, DISABLE); // 关闭DMA1通道1
    DMA_SetCurrDataCounter(DMA1_Channel1, 4); // 设置DMA传输数次数
    DMA_Cmd(DMA1_Channel1, ENABLE); // 使能DMA1通道1
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 软件触发ADC转换

    // 等待DMA传输完成 转运总是在转换之后的
    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1); // 清除DMA传输完成标志
}
