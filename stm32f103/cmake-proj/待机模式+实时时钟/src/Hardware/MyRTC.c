#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include <time.h>

uint16_t t_time[] = {2024, 1, 1, 23, 59, 59}; // 年、月、日、时、分、秒
void MyRTC_SetTime(void);

void MyRTC_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE); // RTC寄存器允许访问

    // 判断断电初始化
    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5){
        // 开启LSE时钟并等待LSE启动完成
        RCC_LSEConfig(RCC_LSE_ON);
        while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);

        // 选择RTC CLK时钟源
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        // 使能RTC时钟
        RCC_RTCCLKCmd(ENABLE);

        // 等待同步
        RTC_WaitForSynchro();
        RTC_WaitForLastTask(); // 等待上一个RTC任务完成

        // 配置预分频器
        RTC_SetPrescaler(32768 - 1); // LSE频率为32.768 kHz，预分频器设置为32767
        RTC_WaitForLastTask(); // 写入后调用一次等待

        // 设置初始时间（例如：2024-01-01 00:00:00）
        /*
        RTC_SetCounter(0); // 从0开始计数
        RTC_WaitForLastTask(); // 写入后调用一次等待
        */

        MyRTC_SetTime(); // 设置RTC时间为t_time数组中的值

        // 写入标志，表示RTC已初始化
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }else{
        RTC_WaitForSynchro();
        RTC_WaitForLastTask(); // 等待上一个RTC任务完成
    }
    
}

void MyRTC_SetTime(void){
    time_t time_cnt;
    struct tm time_date;
    time_date.tm_year = t_time[0] - 1900; // 年份从1900开始
    time_date.tm_mon = t_time[1] - 1; // 月份从
    time_date.tm_mday = t_time[2];
    time_date.tm_hour = t_time[3];
    time_date.tm_min = t_time[4];
    time_date.tm_sec = t_time[5];
    time_cnt = mktime(&time_date); // 将时间结构转换为时间戳
    RTC_SetCounter(time_cnt); // 设置RTC计数器
    RTC_WaitForLastTask(); // 写入后调用一次等待
}

void MyRTC_ReadTime(void){
    time_t time_cnt;
    struct tm time_date;
    time_cnt = RTC_GetCounter(); // 获取RTC计数器的值
    time_date = *localtime(&time_cnt); // 将时间戳转换为时间结构
    t_time[0] = time_date.tm_year + 1900; // 年份
    t_time[1] = time_date.tm_mon + 1; // 月份
    t_time[2] = time_date.tm_mday; // 日
    t_time[3] = time_date.tm_hour; // 时
    t_time[4] = time_date.tm_min; // 分
    t_time[5] = time_date.tm_sec; // 秒
}
