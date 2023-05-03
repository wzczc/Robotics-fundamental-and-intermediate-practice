#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "msp.h"
#include "oled.h"
#include "Clock.h"
#include "dS1302.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "usart.h"
#include "key.h"

#define TIMER_PERIOD    25000

const Timer_A_UpModeConfig upConfig = {
TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_64, // SMCLK/48 = 250KHz 1/250000 = 0.000004s = 0.004ms = 4us
        TIMER_PERIOD * 10,                        // 25000 tick period = 100ms
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
        };

int num = 0;
// ----------锟斤拷锟斤拷----------
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

char *city[] = { "Beijing:   ", "London:    ", "Washington:" };

char *location;

int count = 0;
int key = 10, secondkey = 5;
int a = 0, b = 0, c = 0, d = 0, e = 0;
int aw = 0, ah = 0, am = 0;
int pause = -1;
int start = 0;
int PIN2 = 0;

// ----------锟斤拷锟斤拷----------

// 锟叫讹拷
void PORT1_IRQHandler(void);
void PORT4_IRQHandler(void);
// 锟斤拷锟杰斤拷锟斤拷
void Home(struct ds1302time *time, struct AlarmTime alarm);
void Menu();
void StopWatch();
void Counter();
void AlarmClock();
void WorldTime();
void Port2_Init();
void Port2_Output(uint8_t data);
void ANO_DT_Init(void);
void EUSCIA2_IRQHandler(void);
void TimInit(void);

//int main(void)
//{
//    Clock_Init48MHz();
//    OLED_Init();
//    OLED_Clear();
//    Clock_Delay1ms(5);
//    TimeInit();
//
//    Usart0tInit();
//    Key_Init();
//    TimInit();
//
//    OLED_ShowString(0,0,"Beijing:",12);
//    OLED_ShowNum(68,0,(u32)(time->hour),2,12);
//    OLED_ShowString(82,0,":",12);
//    OLED_ShowNum(90,0,(u32)(time->min),2,12);
//    OLED_ShowString(104,0,":",12);
//    OLED_ShowNum(112,0,(u32)(time->sec),2,12);
//
//    OLED_ShowNum(0,3,(u32)(time->year),4,12);
//    OLED_ShowChinese(26,3,0,12);
//    OLED_ShowNum(40,3,(u32)(time->month),2,12);
//    OLED_ShowChinese(54,3,1,12);
//    OLED_ShowNum(68,3,(u32)(time->date),2,12);
//    OLED_ShowChinese(82,3,2,12);
//    OLED_ShowChinese(104,3,6,12);
//    OLED_ShowChinese(116,3,time->week+6,12);
////    while(1)
////    {
////        OLED_ShowString(0,0,"Beijing:",12);
////        OLED_ShowNum(68,0,(u32)(time->hour),2,12);
////        OLED_ShowString(82,0,":",12);
////        OLED_ShowNum(90,0,(u32)(time->min),2,12);
////        OLED_ShowString(104,0,":",12);
////        OLED_ShowNum(112,0,(u32)(time->sec),2,12);
////
////        OLED_ShowNum(0,3,(u32)(time->year),4,12);
////        OLED_ShowChinese(26,3,0,12);
////        OLED_ShowNum(40,3,(u32)(time->month),2,12);
////        OLED_ShowChinese(54,3,1,12);
////        OLED_ShowNum(68,3,(u32)(time->date),2,12);
////        OLED_ShowChinese(82,3,2,12);
////        OLED_ShowChinese(104,3,6,12);
////        OLED_ShowChinese(116,3,time->week+6,12);
////    }
//}

void TimInit(void)
{
    a = 0;
    //配置定时器TIMER_A0
    Timer_A_configureUpMode(TIMER_A3_BASE, &upConfig);
    //开�?�? 使能定时�?
//    Interrupt_enableSleepOnIsrExit();//退出中�?进入低功耗模�? 没必�?
    Interrupt_enableInterrupt(INT_TA3_0);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
    //开�?�?控制�?
    Interrupt_enableMaster();
}

void TimeInit(void)
{
    time->year = 2022;
    time->month = 4;
    time->date = 27;
    time->hour = 14;
    time->min = 1;
    time->sec = 55;
    time->week = getWeekdayByYearday(time->year, time->month, time->date);
    DS1302_write_time(time);
}

void main()
{
    Port2_Init();
    //Port2_Output(0x00);
    Clock_Init48MHz();
    OLED_Init();
    location = city[0];
    //锟斤拷锟斤拷锟斤拷锟斤拷
    alarm.week = 5;
    alarm.hour = 12;
    alarm.min = 00;

    BeijingTime.year = 2022;
    BeijingTime.month = 4;
    BeijingTime.date = 29;
    BeijingTime.hour = 11;
    BeijingTime.min = 11;
    BeijingTime.sec = 11;
    BeijingTime.week = getWeekdayByYearday(BeijingTime.year, BeijingTime.month,
                                           BeijingTime.date);

    LondonTime = BeijingTime;
    LondonTime.hour = BeijingTime.hour - 7;

    WashingtonTime = BeijingTime;
    WashingtonTime.hour = BeijingTime.hour - 12;

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); // 锟斤拷锟斤拷锟斤拷锟斤拷为锟斤拷锟斤拷锟斤拷锟斤拷模�?
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN7);

    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1,
    GPIO_HIGH_TO_LOW_TRANSITION);  //锟叫讹拷锟斤拷锟斤拷锟斤拷选锟斤拷
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN1,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN2,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN3,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN4,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN5,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN6,
    GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN7,
    GPIO_HIGH_TO_LOW_TRANSITION);

    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);  // 锟斤拷锟斤拷卸�?�锟街�
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN7);

    GPIO_registerInterrupt(GPIO_PORT_P1, PORT1_IRQHandler);  // 注锟斤拷锟叫�?猴拷锟斤�?
    GPIO_registerInterrupt(GPIO_PORT_P4, PORT4_IRQHandler);  // 注锟斤拷锟叫�?猴拷锟斤�?

    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);   // 锟叫讹拷使锟斤拷
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN7);

    time = &(BeijingTime);

    DS1302_write_time(time); /* 写锟斤拷时锟斤拷 */
    int i = 0;
    int c = 0;

    while (1)
    {
        //锟斤拷锟斤拷时锟斤拷
        DS1302_read_time(time);
        time->week = getWeekdayByYearday(time->year, time->month, time->date);

        if (key == 0)
        {
            if (i != key)
            {
                OLED_Clear();
            }
            Home(time, alarm);
            Clock_Delay1us(9);
            i = 0;
        }
        if (key == 4)
        {
            if (i != key)
            {
                OLED_Clear();
            }
            i = 4;
            Menu();
            if (a % 4 == 1)
            {
                if (c != a % 4)
                {
                    OLED_Clear();
                }
                c = 1;
                Menu();
                OLED_ShowString(70, 0, "*", 12);
                while (secondkey == 1)
                {
                    if (c == secondkey)
                    {
                        OLED_Clear();
                    }
                    c = 5;

                    OLED_ShowNum(0, 0, 00, 2, 12);
                    OLED_ShowString(12, 0, ":", 12);
                    OLED_ShowNum(18, 0, 00, 2, 12);
                    OLED_ShowString(30, 0, ":", 12);
                    OLED_ShowNum(36, 0, 00, 2, 12);

                    int minute = 0;
                    int second = 0;
                    int tenms = 0;

                    int record = 0;
                    while (pause != -1)
                    {
                        if (pause == 1)
                        {
                            if (record != pause)
                            {
                                OLED_ShowNum(0, 2, minute % 60, 2, 12);
                                OLED_ShowString(12, 2, ":", 12);
                                OLED_ShowNum(18, 2, second, 2, 12);
                                OLED_ShowString(30, 2, ":", 12);
                                OLED_ShowNum(36, 2, tenms, 2, 12);
                                record = 1;
                            }

                        }
                        if (pause == 2)
                        {
                            if (record != pause)
                            {
                                OLED_ShowNum(0, 4, minute % 60, 2, 12);
                                OLED_ShowString(12, 4, ":", 12);
                                OLED_ShowNum(18, 4, second, 2, 12);
                                OLED_ShowString(30, 4, ":", 12);
                                OLED_ShowNum(36, 4, tenms, 2, 12);
                                record = 2;
                            }
                        }
                        if (pause == 3)
                        {
                            if (record != pause)
                            {
                                OLED_ShowNum(0, 6, minute, 2, 12);
                                OLED_ShowString(12, 6, ":", 12);
                                OLED_ShowNum(18, 6, second, 2, 12);
                                OLED_ShowString(30, 6, ":", 12);
                                OLED_ShowNum(36, 6, tenms, 2, 12);
                                record = 3;
                            }
                        }
                        if (pause < 4)
                        {
                            OLED_ShowNum(0, 0, minute % 60, 2, 12);
                            OLED_ShowString(12, 0, ":", 12);
                            OLED_ShowNum(18, 0, second, 2, 12);
                            OLED_ShowString(30, 0, ":", 12);
                            OLED_ShowNum(36, 0, tenms, 2, 12);
                            Clock_Delay1ms(7);
                            Clock_Delay1us(200);

                            tenms++;
                            if (tenms == 100)
                            {
                                second++;
                                tenms = 0;
                                if (second == 60)
                                {
                                    minute++;
                                    second = 0;
                                }
                            }
                        }
                        if (pause == 5)
                        {
                            pause = -1;
                            if (c != pause)
                            {
                                OLED_Clear();
                            }
                            c = -1;
                            OLED_ShowNum(0, 0, 00, 2, 12);
                            OLED_ShowString(12, 0, ":", 12);
                            OLED_ShowNum(18, 0, 00, 2, 12);
                            OLED_ShowString(30, 0, ":", 12);
                            OLED_ShowNum(36, 0, 00, 2, 12);
                            break;
                        }
                    }
                    if (PIN2 != 0)
                    {
                        OLED_Clear();
                        key = 4;
                        a = 1;
                        pause = -1;
                        PIN2 = 0;
                        secondkey = 0;
                        break;
                    }
                }
            }
            else if (a % 4 == 2)
            {
                if (c != a % 4)
                {
                    OLED_Clear();
                }
                Menu();
                OLED_ShowString(70, 2, "*", 12);
                c = 2;
                while (secondkey == 1)
                {
                    if (c != key)
                    {
                        OLED_Clear();
                    }
                    c = 1;

                    while (d % 3 == 0 && !PIN2)
                    {
                        if (c != d % 3)
                        {
                            OLED_Clear();
                        }
                        c = 0;
                        if (e % 2 == 0)
                        {
                            if (b % 7 != 0)
                            {
                                aw = b % 7;
                            }
                            else if (b % 7 == 0)
                            {
                                aw = b % 7 + 7;
                            }

                            OLED_ShowChinese(0, 0, 14, 12);
                            OLED_ShowChinese(12, 0, 15, 12);
                            OLED_ShowString(26, 0, ":", 12);
                            OLED_ShowChinese(40, 0, 6, 12);
                            OLED_ShowChinese(52, 0, aw + 6, 12);
                            OLED_ShowNum(68, 0, ah, 2, 12);
                            OLED_ShowChinese(82, 0, 3, 12);
                            OLED_ShowNum(98, 0, am, 2, 12);
                            OLED_ShowChinese(112, 0, 4, 12);
                            OLED_ShowString(50, 3, "OFF", 12);

                        }
                        else if (e % 2 != 0)
                        {
                            OLED_ShowChinese(0, 0, 14, 12);
                            OLED_ShowChinese(12, 0, 15, 12);
                            OLED_ShowString(26, 0, ":", 12);
                            OLED_ShowChinese(40, 0, 6, 12);
                            OLED_ShowChinese(52, 0, aw + 6, 12);
                            OLED_ShowNum(68, 0, ah, 2, 12);
                            OLED_ShowChinese(82, 0, 3, 12);
                            OLED_ShowNum(98, 0, am, 2, 12);
                            OLED_ShowChinese(112, 0, 4, 12);
                            OLED_ShowString(50, 3, "ON", 12);

                        }
                        if (d % 3 != 0)
                        {
                            b = 0;
                            break;
                        }

                    }
                    while (d % 3 == 1 && !PIN2)
                    {

                        if (e % 2 == 0)
                        {
                            ah = b % 24;
                            OLED_ShowChinese(0, 0, 14, 12);
                            OLED_ShowChinese(12, 0, 15, 12);
                            OLED_ShowString(26, 0, ":", 12);
                            OLED_ShowChinese(40, 0, 6, 12);
                            OLED_ShowChinese(52, 0, aw + 6, 12);
                            OLED_ShowNum(68, 0, ah, 2, 12);
                            OLED_ShowChinese(82, 0, 3, 12);
                            OLED_ShowNum(98, 0, am, 2, 12);
                            OLED_ShowChinese(112, 0, 4, 12);
                            OLED_ShowString(50, 3, "OFF", 12);

                        }
                        else if (e % 2 != 0)
                        {

                            OLED_ShowChinese(0, 0, 14, 12);
                            OLED_ShowChinese(12, 0, 15, 12);
                            OLED_ShowString(26, 0, ":", 12);
                            OLED_ShowChinese(40, 0, 6, 12);
                            OLED_ShowChinese(52, 0, aw + 6, 12);
                            OLED_ShowNum(68, 0, ah, 2, 12);
                            OLED_ShowChinese(82, 0, 3, 12);
                            OLED_ShowNum(98, 0, am, 2, 12);
                            OLED_ShowChinese(112, 0, 4, 12);
                            OLED_ShowString(50, 3, "ON", 12);

                        }
                        if (d % 3 != 1)
                        {
                            b = 0;
                            break;
                        }
                    }

                    while (d % 3 == 2 && !PIN2)
                    {

                        if (e % 2 == 0)
                        {
                            am = b % 60;
                            if (c != d % 3)
                            {
                                OLED_Clear();
                            }
                            c = 2;
                            OLED_ShowChinese(0, 0, 14, 12);
                            OLED_ShowChinese(12, 0, 15, 12);
                            OLED_ShowString(26, 0, ":", 12);
                            OLED_ShowChinese(40, 0, 6, 12);
                            OLED_ShowChinese(52, 0, aw + 6, 12);
                            OLED_ShowNum(68, 0, ah, 2, 12);
                            OLED_ShowChinese(82, 0, 3, 12);
                            OLED_ShowNum(98, 0, am, 2, 12);
                            OLED_ShowChinese(112, 0, 4, 12);
                            OLED_ShowString(50, 3, "OFF", 12);

                        }
                        else if (e % 2 != 0)
                        {
                            if (c == d % 3)
                            {
                                OLED_Clear();
                            }
                            c = 1;
                            OLED_ShowChinese(0, 0, 14, 12);
                            OLED_ShowChinese(12, 0, 15, 12);
                            OLED_ShowString(26, 0, ":", 12);
                            OLED_ShowChinese(40, 0, 6, 12);
                            OLED_ShowChinese(52, 0, aw + 6, 12);
                            OLED_ShowNum(68, 0, ah, 2, 12);
                            OLED_ShowChinese(82, 0, 3, 12);
                            OLED_ShowNum(98, 0, am, 2, 12);
                            OLED_ShowChinese(112, 0, 4, 12);
                            OLED_ShowString(50, 3, "ON", 12);

//                   if(aw+6==time->week+6){
//                       if(ah==time->hour){
//                           if(am==time->minute){
//                               Port2_Output(0x00);
                            //printf("hello");
//                               //P1OUT=0x06;
//                           }
//                       }
//                   }
                        }
                        if (d % 3 != 2)
                        {
                            b = 0;
                            break;
                        }

                    }
                    if (PIN2 != 0)
                    {
                        OLED_Clear();
                        key = 4;
                        a = 2;
                        d = 0;
                        c = 0;
                        PIN2 = 0;
                        secondkey = 0;
                        break;
                    }
                }

            }
            else if (a % 4 == 3)
            {
                if (c != a % 4)
                {
                    OLED_Clear();
                }
                Menu();

                OLED_ShowString(70, 4, "*", 12);
                c = 3;
                if (secondkey == 1)
                {
                    if (c != key)
                    {
                        OLED_Clear();
                    }
                    c = 1;
                    DS1302_write_time(time);
                    while (1)
                    {
                        //锟斤拷锟斤拷时锟斤拷
                        DS1302_read_time(time);
                        time->week = getWeekdayByYearday(time->year,
                                                         time->month,
                                                         time->date);
                        OLED_ShowChinese(0, 0, 25, 12);
                        OLED_ShowChinese(12, 0, 26, 12);
                        OLED_ShowString(40, 0, ":", 12);
                        OLED_ShowNum(50, 0, (u32) (time->hour), 2, 12);
                        OLED_ShowString(66, 0, ":", 12);
                        OLED_ShowNum(76, 0, (u32) (time->min), 2, 12);
                        OLED_ShowString(92, 0, ":", 12);
                        OLED_ShowNum(102, 0, (u32) (time->sec), 2, 12); //锟斤拷锟斤拷时锟斤拷
                        OLED_ShowChinese(0, 3, 27, 12);
                        OLED_ShowChinese(12, 3, 28, 12);
                        OLED_ShowChinese(24, 3, 29, 12);
                        OLED_ShowString(40, 3, ":", 12);
                        OLED_ShowNum(50, 3, (u32) (time->hour + 12), 2, 12);
                        OLED_ShowString(66, 3, ":", 12);
                        OLED_ShowNum(76, 3, (u32) (time->min), 2, 12);
                        OLED_ShowString(92, 3, ":", 12);
                        OLED_ShowNum(102, 3, (u32) (time->sec), 2, 12); //锟斤拷盛锟斤拷时锟斤�?
                        OLED_ShowChinese(0, 6, 30, 12);
                        OLED_ShowChinese(12, 6, 31, 12);
                        OLED_ShowString(40, 6, ":", 12);
                        OLED_ShowNum(50, 6, (u32) (time->hour - 5), 2, 12);
                        OLED_ShowString(66, 6, ":", 12);
                        OLED_ShowNum(76, 6, (u32) (time->min), 2, 12);
                        OLED_ShowString(92, 6, ":", 12);
                        OLED_ShowNum(102, 6, (u32) (time->sec), 2, 12); //锟阶讹拷时锟斤拷
                        if (PIN2 != 0)
                        {
                            OLED_Clear();
                            key = 4;
                            a = 3;
                            PIN2 = 0;
                            secondkey = 0;
                            break;
                        }
                    }
                }

            }

            else if (a % 4 == 0 && a != 0)
            {
                if (c != a % 4)
                {
                    OLED_Clear();
                }

                Menu();
                c = 0;
                OLED_ShowString(70, 6, "*", 12);
                int minute = 0;
                int second = 0;
                int tenms = 0;
                while (secondkey == 1)
                {
                    if (c != key)
                    {
                        OLED_Clear();
                    }
                    c = 1;

                    while (start % 2 == 0 && !PIN2)
                    {
                        while (d % 2 == 0 && start % 2 == 0 && !PIN2)
                        {
                            if (c != d % 2)
                            {
                                OLED_Clear();
                            }
                            c = 0;

                            minute = b;
                            OLED_ShowNum(0, 0, minute % 60, 2, 12);
                            OLED_ShowString(12, 0, ":", 12);
                            OLED_ShowNum(18, 0, second % 60, 2, 12);
                            OLED_ShowString(30, 0, ":", 12);
                            OLED_ShowNum(36, 0, tenms, 2, 12);

                            if (d % 2 != 0)
                            {
                                b = 0;
                                break;
                            }

                        }
                        while (d % 2 == 1 && start % 2 == 0 && !PIN2)
                        {

                            second = b;
                            OLED_ShowNum(0, 0, minute % 60, 2, 12);
                            OLED_ShowString(12, 0, ":", 12);
                            OLED_ShowNum(18, 0, second % 60, 2, 12);
                            OLED_ShowString(30, 0, ":", 12);
                            OLED_ShowNum(36, 0, tenms, 2, 12);

                            if (d % 2 != 1)
                            {
                                b = 0;
                                break;
                            }

                        }
                        //if(start%2!=0) break;
                    }
                    while (start % 2 == 1 && !PIN2)
                    {

                        if (c != 1)
                        {
                            OLED_Clear();
                        }
                        c = 1;
                        if (!minute && !second && !tenms)
                        {
                            minute = second = tenms = 0;
                            OLED_ShowNum(0, 0, minute % 60, 2, 12);
                            OLED_ShowString(12, 0, ":", 12);
                            OLED_ShowNum(18, 0, second, 2, 12);
                            OLED_ShowString(30, 0, ":", 12);
                            OLED_ShowNum(36, 0, tenms, 2, 12);
                            start = 0;
                            b = 0;
                        }
                        else
                        {
                            OLED_ShowNum(0, 0, minute % 60, 2, 12);
                            OLED_ShowString(12, 0, ":", 12);
                            OLED_ShowNum(18, 0, second, 2, 12);
                            OLED_ShowString(30, 0, ":", 12);
                            OLED_ShowNum(36, 0, tenms, 2, 12);
                            Clock_Delay1ms(7);
                            Clock_Delay1us(300);

                            if (!second && !tenms)
                            {
                                second = 60;
                                minute--;
                            }
                            if (!tenms)
                            {
                                tenms = 99;
                                second--;
                            }

                            tenms--;
                        }
                    }
                    if (PIN2 != 0)
                    {
                        OLED_Clear();
                        key = 4;
                        a = 4;
                        d = 0;
                        c = 0;
                        b = 0;
                        PIN2 = 0;
                        start = 0;
                        secondkey = 0;
                        break;
                    }

                }
                c = 0;
            }

        }
        if (e % 2 != 0)
        {
            if (time->week == aw)
            {
                if (time->hour == ah)
                {
                    if (time->min == am)
                    {
                        Port2_Output(0x01);
                        Clock_Delay1ms(1000);
                        Port2_Output(0x80);

                    }
                }
            }
        }

    }
}

void PORT4_IRQHandler(void)
{
    int i = 0;

    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4); //锟斤拷取锟叫讹拷状�?
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);   //锟斤拷锟斤拷锟�?�疚�?

    if (status & GPIO_PIN0)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;
        key = 0;
    }

    else if (status & GPIO_PIN1)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;
        if (key == 4)
        {
            secondkey = 1;
        }
    }

    else if (status & GPIO_PIN2)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;
        PIN2++;
    }

    else if (status & GPIO_PIN3)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;

    }

    else if (status & GPIO_PIN4)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;
        if (key == 4)
        {
            key = 4;
            a = a + 1;
        }
        else if (key == 0)
        {
            key = 4;
        }
    }

    else if (status & GPIO_PIN5)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;
        if (a % 4 == 2)
        {
            e = e + 1;
        }
        else if (a % 4 == 1)
        {
            if (secondkey == 1)
            {
                pause++;
            }
        }
        else if (a % 4 == 0)
        {
            if (secondkey == 1)
            {

                start++;
            }
        }

    }

    else if (status & GPIO_PIN6)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;
        if (secondkey == 1)
        {
            if (e % 2 == 0)
            {
                b = b + 1;
            }
            else if (start % 2 == 0)
            {
                b++;
            }
        }
    }

    else if (status & GPIO_PIN7)    // 锟叫讹拷锟叫�?匡拷
    {
        for (i = 0; i < 100; i++)
            ;
        if (a % 4 == 2)
        {
            if (secondkey == 1)
            {
                d = d + 1;
            }
        }
        else if (a % 4 == 0)
        {
            if (secondkey == 1)
            {
                d = d + 1;
            }

        }
    }

}

void Home(struct ds1302time *time, struct AlarmTime alarm)
{

    //锟斤拷锟斤拷时锟斤拷
    OLED_ShowString(0, 0, location, 12);
    OLED_ShowNum(68, 0, (u32) (time->hour), 2, 12);
    OLED_ShowString(82, 0, ":", 12);
    OLED_ShowNum(90, 0, (u32) (time->min), 2, 12);
    OLED_ShowString(104, 0, ":", 12);
    OLED_ShowNum(112, 0, (u32) (time->sec), 2, 12);

    //锟斤拷锟斤拷
    OLED_ShowNum(0, 3, (u32) (time->year), 4, 12);
    OLED_ShowChinese(26, 3, 0, 12);
    OLED_ShowNum(40, 3, (u32) (time->month), 2, 12);
    OLED_ShowChinese(54, 3, 1, 12);
    OLED_ShowNum(68, 3, (u32) (time->date), 2, 12);
    OLED_ShowChinese(82, 3, 2, 12);
    OLED_ShowChinese(104, 3, 6, 12);
    OLED_ShowChinese(116, 3, time->week + 6, 12);

    //锟斤拷锟斤拷
    OLED_ShowChinese(0, 6, 14, 12);
    OLED_ShowChinese(12, 6, 15, 12);
    OLED_ShowString(26, 6, ":", 12);
    OLED_ShowChinese(40, 6, 6, 12);
    if (aw == 0)
    {
        OLED_ShowChinese(52, 6, aw + 13, 12);
    }
    else if (aw != 0)
    {
        OLED_ShowChinese(52, 6, aw + 6, 12);
    }
    OLED_ShowNum(68, 6, ah, 2, 12);
    OLED_ShowChinese(82, 6, 3, 12);
    OLED_ShowNum(98, 6, am, 2, 12);
    OLED_ShowChinese(112, 6, 4, 12);

}

void Menu()            //锟剿碉拷锟斤拷锟斤拷
{
    OLED_ShowNum(0, 0, 1, 1, 12);
    OLED_ShowString(6, 0, ".", 12);
    OLED_ShowChinese(18, 0, 16, 12);
    OLED_ShowChinese(30, 0, 17, 12);
    OLED_ShowNum(0, 2, 2, 1, 12);
    OLED_ShowString(6, 2, ".", 12);
    OLED_ShowChinese(18, 2, 14, 12);
    OLED_ShowChinese(30, 2, 15, 12);
    OLED_ShowNum(0, 4, 3, 1, 12);
    OLED_ShowString(6, 4, ".", 12);
    OLED_ShowChinese(18, 4, 18, 12);
    OLED_ShowChinese(30, 4, 19, 12);
    OLED_ShowChinese(42, 4, 20, 12);
    OLED_ShowChinese(54, 4, 21, 12);
    OLED_ShowNum(0, 6, 4, 1, 12);
    OLED_ShowString(6, 6, ".", 12);
    OLED_ShowChinese(18, 6, 22, 12);
    OLED_ShowChinese(30, 6, 23, 12);
    OLED_ShowChinese(42, 6, 24, 12);

}

void StopWatch()        //锟斤拷锟�?
{

}

void Counter()          //锟斤拷时锟斤�?
{

}

void AlarmClock()       //锟斤拷锟斤拷
{
    OLED_ShowChinese(0, 6, 14, 12);
    OLED_ShowChinese(12, 6, 15, 12);
    OLED_ShowString(26, 6, ":", 12);
    OLED_ShowChinese(40, 6, 6, 12);
    OLED_ShowChinese(52, 6, alarm.week + 6, 12);
    OLED_ShowNum(68, 6, alarm.hour, 2, 12);
    OLED_ShowChinese(82, 6, 3, 12);
    OLED_ShowNum(98, 6, alarm.min, 2, 12);
    OLED_ShowChinese(112, 6, 4, 12);

}

void WorldTime()        //锟斤拷锟斤拷时锟斤拷
{

}

void Port2_Init(void)
{
    P2->SEL0 = 0x00;
    P2->SEL1 = 0x00; //configure P2.7-P2.0 as GPIO
    P2->DIR = 0x80; // make P2.7-P2.0 high drive strength
    P2->REN = 0x01; // make P2.7-P2.0 out
    P2->OUT = 0x81; // all LEDs off
}

void Port2_Output(uint8_t data)
{
    P2->OUT = data; // write all of P2 outputs
}

