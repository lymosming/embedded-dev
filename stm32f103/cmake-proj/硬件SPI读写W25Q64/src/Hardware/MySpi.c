#include "stm32f10x.h"
#include "stm32f10x_spi.h"

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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    gpio.GPIO_Pin = GPIO_Pin_4;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    gpio.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    gpio.GPIO_Pin = GPIO_Pin_6;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    // 初始化SPI
    SPI_InitTypeDef spi;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 分频率
    spi.SPI_CPHA = SPI_CPHA_1Edge; // 时钟相位 第1个开始采样
    spi.SPI_CPOL = SPI_CPOL_Low; // 模式0 低电平
    spi.SPI_CRCPolynomial = 7; // CRC检验的多项式，随便填个7
    spi.SPI_DataSize = SPI_DataSize_8b; // 8位数据帧
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 双线全双工
    spi.SPI_FirstBit = SPI_FirstBit_MSB; // 高位先行
    spi.SPI_Mode = SPI_Mode_Master; // 主机模式
    spi.SPI_NSS = SPI_NSS_Soft; // 软件NSS
    SPI_Init(SPI1, &spi);

    SPI_Cmd(SPI1, ENABLE);

    MySpi_W_SS(1); // 给高电平 不选中从机

}

void MySpi_Start(void){
    MySpi_W_SS(0);
}

void MySpi_Stop(void){
    MySpi_W_SS(1);
}

// 交换一个字节或者读写一个字节
uint8_t MySpi_SwapByte(uint8_t bytesend){
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET); // 等待TXE=1
    SPI_I2S_SendData(SPI1, bytesend); // 发送数据
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET); // 等待RXNE=1
    return SPI_I2S_ReceiveData(SPI1);
}