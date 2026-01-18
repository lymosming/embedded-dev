#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

void AD_Init(void){

    // 开启ADC1和GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 设置ADC时钟为PCLK2的6分频，即12MHz

    // 初始化GPIO时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; // 假设使用PA0-PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化ADC时钟
    ADC_InitTypeDef ADC_InitTypeDefStructure;
    ADC_InitTypeDefStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式
    ADC_InitTypeDefStructure.ADC_ScanConvMode = DISABLE; // 扫描模式选择
    ADC_InitTypeDefStructure.ADC_ContinuousConvMode = DISABLE; // 连续转换模式关闭
    ADC_InitTypeDefStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发转换关闭
    ADC_InitTypeDefStructure.ADC_DataAlign = ADC_DataAlign_Right; // 数据对齐右对齐
    ADC_InitTypeDefStructure.ADC_NbrOfChannel = 1; // 转换通道数
    ADC_Init(ADC1, & ADC_InitTypeDefStructure);

    ADC_Cmd(ADC1, ENABLE); // 使能ADC1

    // 对ADC进行校准
    ADC_ResetCalibration(ADC1); // 复位校准
    while(ADC_GetResetCalibrationStatus(ADC1)); // 等待复位校准完成
    ADC_StartCalibration(ADC1); // 开始校准
    while(ADC_GetCalibrationStatus(ADC1)); // 等待校准完成

}

// 获取ADC值
uint16_t AD_GetValue(uint8_t channel){
    // 选择规则组输入通道
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_55Cycles5); //
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 启动软件转换
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // 等待转换完成
    return ADC_GetConversionValue(ADC1); // 获取转换值
}
