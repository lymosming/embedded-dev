#include "stm32f10x.h"

void PWM_Init(void){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启时钟
	
	// 配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 重映射引脚： PA0 => PA15
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); // GPIO_PartialRemap1_TIM2 部分重映射
	// GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 将JTAG复用解除，就可以把 PA15当作GPIO用
	// 如果重映射端口正好是调式端口，以上3句代码都需要。
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 采用复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 如果使用了上面的重映射，就改成15引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2); // 选择时机单元时钟
	
	// 配置时机单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式，向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // 周期ARR  
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器 这里不需要，置0
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 初始化数字比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); // 初始化一次，让所有属性有默认值，下面再单独修改
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出比较极性 高极性 极性不翻转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
	/*
	PWM频率：Freq = CK_PSC / (PSC + 1) / (ARR + 1)
	PWM占空比：Duty = CCR / (ARR + 1)
	PWM分辨率： Reso = 1 / (ARR + 1)
	
	如果要产生一个频率为1MHz 占空比为50% 分辨率为1%的PWM波形：
	系统时钟72Mhz
	1M = 72M / (PSC + 1) / (ARR + 1)
	50% = CCR / (ARR + 1)
	1& = 1 / (ARR + 1)
	得出：PSC / ARR / CCR 
	
	*/
	// TIM_OCInitStructure.TIM_Pulse = 50; // 设置CCR 寄存器值
	TIM_OCInitStructure.TIM_Pulse = 0; // 由下面 PWM_SetCompare1控制
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE); // 启动定时器
}

/**
* 设置CCR 的值，可以达到控制PWM波形。进而控制LED亮度
*/
void PWM_SetCompare1(uint16_t compare){
	TIM_SetCompare1(TIM2, compare);
}
