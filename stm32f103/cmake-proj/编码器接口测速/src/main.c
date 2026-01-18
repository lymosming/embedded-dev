#include "stm32f10x.h"
#include "OLED.h"
#include "Encoder.h"
#include "Delay.h"

int16_t speed;

int main(void){
    OLED_Init();
    Encoder_Init();
    OLED_ShowString(1, 1, "CNT:");
    while(1){
        
        OLED_ShowSignedNum(1, 5, Encoder_GetCount(), 5);
        Delay_ms(1000); // 人手转给1秒  可以改成使用中断方式定义speed 在中断中进行写入，就不需要这个延时
        
        // OLED_ShowSignedNum(1, 5, speed, 5); // 需要依赖中断配置，可引人其他项目的Timer.h
    }
}

// 需引人其他项目的Timer.h Timer_Init();
void TIM2_IRQHandler(void){
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
        speed = Encoder_GetCount();
        // 在这里处理定时器中断
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
