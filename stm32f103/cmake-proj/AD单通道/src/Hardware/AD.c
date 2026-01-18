#include "stm32f10x.h"

void AD_Init(void){
    // ADC1 初始化代码放在这里
    // 开启ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    // 配置GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PA0作为模拟输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ADCCK 配置
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 6分频 72MHz/6=12MHz <14MHz
    
    // 选择规则组的输入通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    // 用结构体初始化ADC 单次循环非扫描模式
    // 如果要改成连续转换非扫描模式，只需将ContinuousConvMode设为ENABLE & ADC_SoftwareStartConvCmd(ADC1, ENABLE);挪到Init中，删除
    //     while(! ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); 即可

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 扫描选择模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 数据对齐右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发选择：软件触发（内部触发）
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 连续转换模式关闭
    ADC_InitStructure.ADC_NbrOfChannel = 1; // 转换通道数目1
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE); // 使能ADC1

    // 对ADC进行校准
    ADC_ResetCalibration(ADC1); // 复位校准
    while(ADC_GetResetCalibrationStatus(ADC1)); // 等待校准复位完成
    ADC_StartCalibration(ADC1); // 开始校准
    while(ADC_GetCalibrationStatus(ADC1)); // 等待校准完成
}

uint16_t AD_GetValue(void){
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 软件触发转换开始
    while(! ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // 等待转换完成 规则组转换完成标志位
    return ADC_GetConversionValue(ADC1); // 获取转换值
}
