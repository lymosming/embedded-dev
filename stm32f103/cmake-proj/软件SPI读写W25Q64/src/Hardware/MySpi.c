#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

// 写SS引脚
void MySpi_W_SS(uint8_t bit){
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)bit);
}

void MySpi_W_SCK(uint8_t bit){
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)bit);
}

void MySpi_W_MOSI(uint8_t bit){
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)bit);
}

uint8_t MySpi_R_MISO(void){
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySpi_Init(void){
    // 初始化GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_4 | GPIO_Pin_7;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    gpio.GPIO_Pin = GPIO_Pin_6;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    // 初始化默认电平设置
    MySpi_W_SS(1); // 置为高电平，不选中从机
    MySpi_W_SCK(0); // 使用SPI模式0

}

void MySpi_Start(void){
    MySpi_W_SS(0);
}

void MySpi_Stop(void){
    MySpi_W_SS(1);
}

// 交换一个字节或者读写一个字节
uint8_t MySpi_SwapByte(uint8_t bytesend){
    uint8_t i, receive = 0x00;
    for(i = 0; i < 8; i++){
        MySpi_W_MOSI(bytesend & (0x80 >> i));
        MySpi_W_SCK(1); // 产生上升沿，从机自动会把MISO数据读走
        if (MySpi_R_MISO() == 1) {
            receive |= (0x80 >> i);
        }
        MySpi_W_SCK(0); // 产生下降沿
    }
    return receive;
}