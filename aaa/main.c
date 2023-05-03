#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "msp.h"
#include "oled.h"
#include "DS1302.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include "bmp.h"
//#include "usart.h"
//#include "key.h"
//#include "SysTick.h"


// ----------变量----------
struct AlarmTime
{
   unsigned char week;
   unsigned char hour;
   unsigned char min;
} alarm;

struct ds1302time *time;
struct ds1302time BeijingTime;
struct ds1302time LondonTime;
struct ds1302time WashingtonTime;

char *city[] = {"Beijing:   ","London:    ","Washington:"};
char *location;

int FirstKey = 0;
int SecondKey = 0;
int ThirdKey = 0;

int HomeClear = 0;
int ChooseClear = 0;
int EnterClear = 0;
int SettingClear = 0;

int enter = 0;
int plus = 0;


// ----------函数----------

// 中断
void GPIO_Interrupt();
void PORT1_IRQHandler();
void PORT4_IRQHandler();

// 功能界面
void Home(struct ds1302time *time,struct AlarmTime alarm);
void Menu();
void StopWatch();
void Counter();
void AlarmClock();
void WorldTime();


void main()
{
    Clock_Init48MHz();
    OLED_Init();

    location = city[0];

    //设置闹钟
    alarm.week = 1;
    alarm.hour = 8;
    alarm.min = 00;

    BeijingTime.year   = 2022;
    BeijingTime.month  = 5;
    BeijingTime.date    = 14;
    BeijingTime.hour   = 12;
    BeijingTime.min = 30;
    BeijingTime.sec = 00;
    BeijingTime.week   = getWeekdayByYearday( BeijingTime.year, BeijingTime.month, BeijingTime.date );

    LondonTime = BeijingTime;
    LondonTime.hour = BeijingTime.hour - 7;

    WashingtonTime = BeijingTime;
    WashingtonTime.hour = BeijingTime.hour - 12;


    GPIO_Interrupt();


    time = &(BeijingTime);

    DS1302_write_time(time);       /* 写入时间 */

    while(1)
    {
        //更新时间
        DS1302_read_time(time);
        time->week = getWeekdayByYearday(time->year, time->month, time->date);

        // 主界面
        if (FirstKey == 0)
        {
            ChooseClear = EnterClear = SettingClear = 0;
            enter = plus = 0;
            if (!HomeClear)
            {
                OLED_Clear();
                HomeClear = 1;
            }
            Home(time,alarm);
        }

        // 菜单界面
        while (FirstKey == 4)
        {
            HomeClear = ChooseClear = EnterClear = 0;
            enter = 0;
            if (!SettingClear)
            {
                OLED_Clear();
                SettingClear = 1;
            }
            Menu();
            OLED_ShowString(70,0,"*",12);

            int ClearFlag = 0;
            while (SecondKey == 6 && FirstKey == 4)
            {
                if (plus%4 == 0)
                {
                    if (ClearFlag == 3)
                    {
                        OLED_Clear();
                        ClearFlag = 0;
                    }
                    Menu();
                    OLED_ShowString(70,0,"*",12);
                    if (ThirdKey == 1)
                    {
                        // if (!ChooseClear)
                        // {
                            OLED_Clear();
                            // ChooseClear = 1;
                        // }
                        OLED_ShowNum(0,0,00,2,12);
                        OLED_ShowString(12,0,":",12);
                        OLED_ShowNum(18,0,00,2,12);
                        OLED_ShowString(30,0,":",12);
                        OLED_ShowNum(36,0,00,2,12);

                        int minute = 0;
                        int second = 0;
                        int tenms = 0;
                        int record = 0;

                        while (ThirdKey == 5)
                        {
                            if (enter == 2 && record == 0)
                            {
                                OLED_ShowNum(0,2,minute%60,2,12);
                                OLED_ShowString(12,2,":",12);
                                OLED_ShowNum(18,2,second,2,12);
                                OLED_ShowString(30,2,":",12);
                                OLED_ShowNum(36,2,tenms,2,12);
                                record = 1;
                            }
                            if (enter == 3 && record == 1)
                            {
                                OLED_ShowNum(0,4,minute%60,2,12);
                                OLED_ShowString(12,4,":",12);
                                OLED_ShowNum(18,4,second,2,12);
                                OLED_ShowString(30,4,":",12);
                                OLED_ShowNum(36,4,tenms,2,12);
                                record = 2;
                            }
                            if (enter == 4 && record == 2)
                            {
                                OLED_ShowNum(0,6,minute,2,12);
                                OLED_ShowString(12,6,":",12);
                                OLED_ShowNum(18,6,second,2,12);
                                OLED_ShowString(30,6,":",12);
                                OLED_ShowNum(36,6,tenms,2,12);
                                record = 3;
                            }
                            if (enter == 5)
                            {
                                break;
                            }
                            OLED_ShowNum(0,0,minute%60,2,12);
                            OLED_ShowString(12,0,":",12);
                            OLED_ShowNum(18,0,second,2,12);
                            OLED_ShowString(30,0,":",12);
                            OLED_ShowNum(36,0,tenms,2,12);

                            Clock_Delay1ms(7);
                            Clock_Delay1us(200);

                            tenms ++;
                            if (tenms == 100)
                            {
                                second ++;
                                tenms = 0;
                                if (second == 60)
                                {
                                    minute ++;
                                    second = 0;
                                }
                            }
                        }
                    }
                }
                if (plus%4 == 1)
                {
                    if (ClearFlag == 0)
                    {
                        OLED_Clear();
                        ClearFlag = 1;
                    }
                    Menu();
                    OLED_ShowString(70,2,"*",12);
                }
                if (plus%4 == 2)
                {
                    if (ClearFlag == 1)
                    {
                        OLED_Clear();
                        ClearFlag = 2;
                    }
                    Menu();
                    OLED_ShowString(70,4,"*",12);
                }
                if (plus%4 == 3)
                {
                    if (ClearFlag == 2)
                    {
                        OLED_Clear();
                        ClearFlag = 3;
                    }
                    Menu();
                    OLED_ShowString(70,6,"*",12);
                }
            }

       //    // 倒计时
       //    int minute = 0;
       //    int second = 0;
       //    int tenms = 0;
       //    OLED_ShowNum(0,0,minute%60,2,12);
       //    OLED_ShowString(12,0,":",12);
       //    OLED_ShowNum(18,0,second,2,12);
       //    OLED_ShowString(30,0,":",12);
       //    OLED_ShowNum(36,0,tenms,2,12);

       //    Clock_Delay1ms(7);
       //    Clock_Delay1us(200);

       //    if (tenms == 0)
       //    {
       //        tenms = 99;
       //        second --;
       //    }
       //    if (second == 0)
       //    {
       //        second = 59;
       //        minute --;
       //    }
       //    tenms --;
      }


          // if (ThirdKey == 1)
          // {
          //     if (!ChooseClear)
          //     {
          //         OLED_Clear();
          //         ChooseClear = 1;
          //     }
          //     OLED_ShowNum(0,0,00,2,12);
          //     OLED_ShowString(12,0,":",12);
          //     OLED_ShowNum(18,0,00,2,12);
          //     OLED_ShowString(30,0,":",12);
          //     OLED_ShowNum(36,0,00,2,12);

          //     int minute = 0;
          //     int second = 0;
          //     int tenms = 0;
          //     int record = 0;

          //     while (ThirdKey == 5)
          //     {
          //         if (enter == 2 && record == 0)
          //         {
          //             OLED_ShowNum(0,2,minute%60,2,12);
          //             OLED_ShowString(12,2,":",12);
          //             OLED_ShowNum(18,2,second,2,12);
          //             OLED_ShowString(30,2,":",12);
          //             OLED_ShowNum(36,2,tenms,2,12);
          //             record = 1;
          //         }
          //         if (enter == 3 && record == 1)
          //         {
          //             OLED_ShowNum(0,4,minute%60,2,12);
          //             OLED_ShowString(12,4,":",12);
          //             OLED_ShowNum(18,4,second,2,12);
          //             OLED_ShowString(30,4,":",12);
          //             OLED_ShowNum(36,4,tenms,2,12);
          //             record = 2;
          //         }
          //         if (enter == 4 && record == 2)
          //         {
          //             OLED_ShowNum(0,6,minute,2,12);
          //             OLED_ShowString(12,6,":",12);
          //             OLED_ShowNum(18,6,second,2,12);
          //             OLED_ShowString(30,6,":",12);
          //             OLED_ShowNum(36,6,tenms,2,12);
          //             record = 3;
          //         }
          //         if (enter == 5)
          //         {
          //             break;
          //         }
          //         OLED_ShowNum(0,0,minute%60,2,12);
          //         OLED_ShowString(12,0,":",12);
          //         OLED_ShowNum(18,0,second,2,12);
          //         OLED_ShowString(30,0,":",12);
          //         OLED_ShowNum(36,0,tenms,2,12);

          //         Clock_Delay1ms(7);
          //         Clock_Delay1us(200);

          //         tenms ++;
          //         if (tenms == 100)
          //         {
          //             second ++;
          //             tenms = 0;
          //             if (second == 60)
          //             {
          //                 minute ++;
          //                 second = 0;
          //             }
          //         }
          //     }
          // }
   }

}




void GPIO_Interrupt()
{
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);  // 设置引脚为上拉输入模式
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);
   GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN7);

   GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);  //中断跳变沿选择
   GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN2,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN3,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN5,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6,GPIO_HIGH_TO_LOW_TRANSITION);
   GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN7,GPIO_HIGH_TO_LOW_TRANSITION);

   GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN1);  // 清除中断标志
   GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN4);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN0);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN1);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN2);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN3);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN4);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN5);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN6);
   GPIO_clearInterruptFlag(GPIO_PORT_P4,GPIO_PIN7);


   GPIO_registerInterrupt(GPIO_PORT_P1,PORT1_IRQHandler);  // 注册中断函数
   GPIO_registerInterrupt(GPIO_PORT_P4,PORT4_IRQHandler);  // 注册中断函数

   GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);   // 中断使能
   GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN2);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN3);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN4);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);
   GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN7);

}



void PORT4_IRQHandler()
{
   int i = 0;

   uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4); //获取中断状态
   GPIO_clearInterruptFlag(GPIO_PORT_P4,status);   //清除标志位

   if(status & GPIO_PIN0)    // 判断中断口
   {
       for(i=0;i<100;i++);
       FirstKey = 0;
   }

   else if(status & GPIO_PIN1)    // 判断中断口
   {
       for(i=0;i<100;i++);
       ThirdKey = 1;
   }

   else if(status & GPIO_PIN2)    // 判断中断口
   {
       for(i=0;i<100;i++);
   }

   else if(status & GPIO_PIN3)    // 判断中断口
   {
       for(i=0;i<100;i++);
   }

   else if(status & GPIO_PIN4)    // 判断中断口
   {
       for(i=0;i<100;i++);
       FirstKey = 4;
   }

   else if(status & GPIO_PIN5)    // 判断中断口
   {
       for(i=0;i<100;i++);
       enter ++;
       ThirdKey = 5;
   }

   else if(status & GPIO_PIN6)    // 判断中断口
   {
       for(i=0;i<100;i++);
       plus ++;
       SecondKey = 6;
       ThirdKey = 0;
   }

   else if(status & GPIO_PIN7)    // 判断中断口
   {
       for(i=0;i<100;i++);
   }



}


void Home(struct ds1302time *time,struct AlarmTime alarm)
{
   //世界时间
   OLED_ShowString(0,0,location,12);
   OLED_ShowNum(68,0,(u32)(time->hour),2,12);
   OLED_ShowString(82,0,":",12);
   OLED_ShowNum(90,0,(u32)(time->min),2,12);
   OLED_ShowString(104,0,":",12);
   OLED_ShowNum(112,0,(u32)(time->sec),2,12);

   //日历
   OLED_ShowNum(0,3,(u32)(time->year),4,12);
   OLED_ShowChinese(26,3,0,12);
   OLED_ShowNum(40,3,(u32)(time->month),2,12);
   OLED_ShowChinese(54,3,1,12);
   OLED_ShowNum(68,3,(u32)(time->date),2,12);
   OLED_ShowChinese(82,3,2,12);
   OLED_ShowChinese(104,3,6,12);
   OLED_ShowChinese(116,3,time->week+6,12);

   //闹铃
   OLED_ShowChinese(0,6,14,12);
   OLED_ShowChinese(12,6,15,12);
   OLED_ShowString(26,6,":",12);
   OLED_ShowChinese(40,6,6,12);
   OLED_ShowChinese(52,6,alarm.week+6,12);
   OLED_ShowNum(68,6,alarm.hour,2,12);
   OLED_ShowChinese(82,6,3,12);
   OLED_ShowNum(98,6,alarm.min,2,12);
   OLED_ShowChinese(112,6,4,12);
}

void Menu()
{
   OLED_ShowNum(0,0,1,1,12);
   OLED_ShowString(6,0,".",12);
   OLED_ShowChinese(18,0,16,12);
   OLED_ShowChinese(30,0,17,12);
   OLED_ShowNum(0,2,2,1,12);
   OLED_ShowString(6,2,".",12);
   OLED_ShowChinese(18,2,14,12);
   OLED_ShowChinese(30,2,15,12);
   OLED_ShowNum(0,4,3,1,12);
   OLED_ShowString(6,4,".",12);
   OLED_ShowChinese(18,4,18,12);
   OLED_ShowChinese(30,4,19,12);
   OLED_ShowChinese(42,4,20,12);
   OLED_ShowChinese(54,4,21,12);
   OLED_ShowNum(0,6,4,1,12);
   OLED_ShowString(6,6,".",12);
   OLED_ShowChinese(18,6,22,12);
   OLED_ShowChinese(30,6,23,12);
   OLED_ShowChinese(42,6,24,12);
}

void StopWatch()
{

}

void Counter()
{

}

void AlarmClock()
{

}

void WorldTime()
{

}









