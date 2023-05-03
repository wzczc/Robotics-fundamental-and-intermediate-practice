#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "msp.h"
#include "oled.h"
#include "Clock.h"
#include "DS1302.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


// ----------变量----------
struct AlarmTime
{
    unsigned char week;
    unsigned char hour;
    unsigned char min;
} alarm;
struct ds1302time *time;
struct ds1302time BeijingTime;
char *city[] = { "Beijing:   ", "London:    ", "Washington:" };
char *location;
int count = 0;
int key = 10;                      //key为判断按键变量，key==0时表示home键被按下，显示home界面；key==4时表示设置键被按下，显示设置界面。
int secondkey = 5;                 //secondkey为选择键标志变量，sedcondkey==0时表示选择键未被按下，secondkey==1时表示选择键被按下。
int set = 0;                       //set为功能设置变量，每按下一次设置键，set加一，根据set值显示不同界面：set=1表示选中秒表功能，set=2表示选中闹铃功能，set=3表示选中世界时钟功能，set=4表示选中计时器功能。
int plus = 0;                      //plus为加号键变量，设置闹铃和计时器时间时按下加号键b加一。
int clear = 0;                     //clear为清屏变量，在程序中起清屏一次的作用。
int shift = 0;                     //shift为移位变量，在设置闹铃或计时器时间时switch8每按下一次d的值加一，在设置闹钟时间时：d%3=0设置周数位，d%3=1设置时针位，d%3=2设置分针位；在设置计时器时间时：d%2=0设置分针位，d%2=1设置秒针位。
int sw = 0;                        //sw为闹铃开关变量，在设置闹铃时每按下一次确定键sw加一，sw%2==0时闹铃关闭，sw%2==1时闹铃打开。
int aw = 0, ah = 0, am = 0;        //aw为闹铃的周变量，ah为闹铃的时变量，am为闹铃的分变量。
int pause = -1;                    //pause 为秒表功能的暂停计时变量，每按一次确定键使pause值变化，每按一次记一次时，按第四下时停止计时，按第五下时清零。
int start = 0;                     //start 为（倒）开始倒计时变量，设置完倒计时的时间后按下确定键使start加一就会开始倒计时。
int turnoff = 0;                   //turnoff 为闹钟停响变量，按下返回键时turnoff加一，闹铃响时按下返回键得到turnoff的值会让闹铃停响。
int back = 0;                      //back 为返回变量，进入具体功能后按下返回键让back值改变，根据back的值返回到设置界面。
int convert = 0;                   //convert 为24/12小时制切换变量，按一次switch7使convert值改变一次，并根据convert的值切换小时制。
int correct_t = 0;                 //校准时间按键。
int correct_d = 0;                 //校准日期按键。


// ----------函数----------
void PORT1_IRQHandler(void);       //中断函数
void PORT4_IRQHandler(void);       //中断函数
void Init();                       //初始化
void SetTime(int year,unsigned char month,unsigned char day,
             unsigned char hour,unsigned char minute,unsigned char second);    //初始时间设置
void GPIO_Interrupt();             //GPIO中断
void SerialCalibration();          //串口校时
void Home(struct ds1302time *time, struct AlarmTime alarm);    //主界面
void Menu();                       //菜单界面
void StopWatch();                  //秒表功能
void Counter();                    //计时器功能
void AlarmClock();                 //闹钟功能
void AlarmRing();                  //铃响
void WorldTime();                  //世界时钟
void Port2_Init();
void Port2_Output(uint8_t data);

void main()
{
    Init();
    SetTime(2022,5,31,12,00,00);
    GPIO_Interrupt();
    time = &(BeijingTime);
    DS1302_write_time(time); /* 写入时间 */
    int i = 0;

    while (1)
    {
        SerialCalibration();

        //更新时间
        DS1302_read_time(time);
        time->week = getWeekdayByYearday(time->year, time->month, time->day);

        if (key == 0)
        {
            if (i != key)
            {
                OLED_Clear();
            }
            Home(time, alarm);
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
            if (set % 4 == 1)
            {
                if (clear != set % 4)
                {
                    OLED_Clear();
                }
                clear = 1;
                Menu();
                OLED_ShowString(70, 0, "*", 12);
                StopWatch();
            }
            else if (set % 4 == 2)
            {
                if (clear != set % 4)
                {
                    OLED_Clear();
                }
                Menu();
                OLED_ShowString(70, 2, "*", 12);
                clear = 2;
                AlarmClock();
            }
            else if (set % 4 == 3)
            {
                if (clear != set % 4)
                {
                    OLED_Clear();
                }
                Menu();
                OLED_ShowString(70, 4, "*", 12);
                clear = 3;
                if (secondkey == 1)
                {
                    if (clear != key)
                    {
                        OLED_Clear();
                    }
                    clear = 1;
                    DS1302_write_time(time);
                    WorldTime();
                }
            }
            else if (set % 4 == 0 && set != 0)
            {
                if (clear != set % 4)
                {
                    OLED_Clear();
                }
                Menu();
                shift = 0;
                clear = 0;
                OLED_ShowString(70, 6, "*", 12);
                Counter();
                clear = 0;
            }
        }
        AlarmRing();
    }
}

void Init()
{
    UART0_Init();
    UART0_Initprintf();
    Port2_Init();
    Clock_Init48MHz();
    OLED_Init();
    location = city[0];
}

void SetTime(int year,unsigned char month,unsigned char day,
             unsigned char hour,unsigned char minute,unsigned char second)
{

    BeijingTime.year = year;
    BeijingTime.month = month;
    BeijingTime.day = day;
    BeijingTime.hour = hour;
    BeijingTime.minute = minute;
    BeijingTime.second =second;
    BeijingTime.week = getWeekdayByYearday(BeijingTime.year, BeijingTime.month,
                                           BeijingTime.day);
}

void GPIO_Interrupt()
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); // 设置引脚为上拉输入模式
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
                             GPIO_HIGH_TO_LOW_TRANSITION);  //中断跳变沿选择
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

    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);  // 清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN6);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN7);

    GPIO_registerInterrupt(GPIO_PORT_P1, PORT1_IRQHandler);  // 注册中断函数
    GPIO_registerInterrupt(GPIO_PORT_P4, PORT4_IRQHandler);  // 注册中断函数

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

void SerialCalibration()
{
    while (correct_t)
    {
        uint32_t correct_time = UART0_InUDec();
        time->hour = (u32)(correct_time/100);
        time->minute = (u32)(correct_time%100);
        DS1302_write_time(time);
        correct_t = 0;
    }
    while (correct_d)
    {
        uint32_t correct_date = UART0_InUDec();
        if (correct_date > 10000000)
        {
            time->year = correct_date/10000;
            correct_date = correct_date%10000;
            time->month = correct_date/100;
            time->day = correct_date%100;
            DS1302_write_time(time);
            correct_d = 0;
        } else
        {
            time->month = correct_date/100;
            time->day = correct_date%100;
            DS1302_write_time(time);
            correct_d = 0;
        }
    }
}

void Home(struct ds1302time *time, struct AlarmTime alarm)
{

    //世界时间
    if (convert % 2 == 0)
    {

        OLED_ShowString(0, 0, location, 12);
        OLED_ShowString(90, 2, "  ", 8);
        OLED_ShowNum(68, 0, (u32) (time->hour), 2, 12);
        OLED_ShowString(82, 0, ":", 12);
        OLED_ShowNum(90, 0, (u32) (time->minute), 2, 12);
        OLED_ShowString(104, 0, ":", 12);
        OLED_ShowNum(112, 0, (u32) (time->second), 2, 12);

        //日历
        OLED_ShowNum(0, 3, (u32) (time->year), 4, 12);
        OLED_ShowChinese(26, 3, 0, 12);
        OLED_ShowNum(40, 3, (u32) (time->month), 2, 12);
        OLED_ShowChinese(54, 3, 1, 12);
        OLED_ShowNum(68, 3, (u32) (time->day), 2, 12);
        OLED_ShowChinese(82, 3, 2, 12);
        OLED_ShowChinese(104, 3, 6, 12);
        OLED_ShowChinese(116, 3, time->week + 6, 12);

        //闹铃
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
    else if (convert % 2 != 0)
    {

        OLED_ShowString(0, 0, location, 12);
        if (time->hour < 12)
        {
            OLED_ShowNum(68, 0, (u32) (time->hour), 2, 12);
            OLED_ShowString(90, 2, "AM", 8);
        }
        else if (time->hour >= 12)
        {
            OLED_ShowNum(68, 0, (u32) (time->hour % 12), 2, 12);
            OLED_ShowString(90, 2, "PM", 8);
        }
        OLED_ShowString(82, 0, ":", 12);
        OLED_ShowNum(90, 0, (u32) (time->minute), 2, 12);
        OLED_ShowString(104, 0, ":", 12);
        OLED_ShowNum(112, 0, (u32) (time->second), 2, 12);

        //日历
        OLED_ShowNum(0, 3, (u32) (time->year), 4, 12);
        OLED_ShowChinese(26, 3, 0, 12);
        OLED_ShowNum(40, 3, (u32) (time->month), 2, 12);
        OLED_ShowChinese(54, 3, 1, 12);
        OLED_ShowNum(68, 3, (u32) (time->day), 2, 12);
        OLED_ShowChinese(82, 3, 2, 12);
        OLED_ShowChinese(104, 3, 6, 12);
        OLED_ShowChinese(116, 3, time->week + 6, 12);

        //闹铃
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

}

void Menu()            //菜单界面
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

void StopWatch()        //秒表
{
    while (secondkey == 1)
    {
        if (clear == secondkey)
        {
            OLED_Clear();
        }
        clear = 5;

        OLED_ShowNum(0, 0, 00, 2, 12);
        OLED_ShowString(12, 0, ":", 12);
        OLED_ShowNum(18, 0, 00, 2, 12);
        OLED_ShowString(30, 0, ":", 12);
        OLED_ShowNum(36, 0, 00, 2, 12);

        int minute = 0;
        int second = 0;
        int tenms = 0;

        int record = 0;
        while (pause != -1 && !back)

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
                if (clear != pause)
                {
                    OLED_Clear();
                }
                clear = -1;
                OLED_ShowNum(0, 0, 00, 2, 12);
                OLED_ShowString(12, 0, ":", 12);
                OLED_ShowNum(18, 0, 00, 2, 12);
                OLED_ShowString(30, 0, ":", 12);
                OLED_ShowNum(36, 0, 00, 2, 12);
                break;
            }

        }
        if (back != 0)
        {
            OLED_Clear();
            key = 4;
            set = 1;
            pause = -1;
            back = 0;
            secondkey = 0;
            break;
        }
    }
}

void Counter()          //计时器
{
    int minute = 0;
    int second = 0;
    int tenms = 0;
    while (secondkey == 1)
    {
        if (clear != key)
        {
            OLED_Clear();
        }
        clear = 1;
        plus = 0;
        while (start % 2 == 0 && !back)
        {
            while (shift % 2 == 0 && start % 2 == 0 && !back)
            {
                if (clear != shift % 2)
                {
                    OLED_Clear();
                }
                clear = 0;

                minute = plus;
                OLED_ShowNum(0, 0, minute % 60, 2, 12);
                OLED_ShowString(0, 2, "__", 12);
                OLED_ShowString(12, 0, ":", 12);
                OLED_ShowNum(18, 0, second, 2, 12);
                OLED_ShowString(30, 0, ":", 12);
                OLED_ShowNum(36, 0, tenms, 2, 12);

                if (shift % 2 != 0)
                {
                    plus = 0;
                    break;
                }

            }
            while (shift % 2 == 1 && start % 2 == 0 && !back)
            {
                if (clear != shift % 2)
                {
                    OLED_Clear();
                }
                clear = 1;

                second = plus;
                OLED_ShowNum(0, 0, minute % 60, 2, 12);
                OLED_ShowString(12, 0, ":", 12);
                OLED_ShowNum(18, 0, second, 2, 12);
                OLED_ShowString(18, 2, "__", 12);
                OLED_ShowString(30, 0, ":", 12);
                OLED_ShowNum(36, 0, tenms, 2, 12);

                if (shift % 2 != 1)
                {
                    plus = 0;
                    break;
                }

            }

        }
        while (start % 2 == 1 && !back)
        {

            if (clear == 1)
            {
                OLED_Clear();
            }
            clear = 2;
            if (!minute && !second && !tenms && !back)
            {
                minute = second = tenms = 0;
                OLED_ShowNum(0, 0, minute % 60, 2, 12);
                OLED_ShowString(12, 0, ":", 12);
                OLED_ShowNum(18, 0, second, 2, 12);
                OLED_ShowString(30, 0, ":", 12);
                OLED_ShowNum(36, 0, tenms, 2, 12);
                Port2_Output(0x01);
                Clock_Delay1ms(5000);
                Port2_Output(0x81);
                start = 0;
                plus = 0;
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
        if (back != 0)
        {
            OLED_Clear();
            key = 4;
            set = 4;
            shift = 0;
            plus = 0;
            back = 0;
            start = 0;
            secondkey = 0;

            break;
        }
    }
}

void AlarmClock()       //闹铃
{
    while (secondkey == 1)
    {
        if (clear != secondkey)
        {
            OLED_Clear();
        }
        clear = 1;
        while (shift % 3 == 0 && !back)
        {

            if (sw % 2 == 0)
            {
                if (plus % 7 != 0)
                {
                    aw = plus % 7;
                }
                else if (plus % 7 == 0)
                {
                    aw = plus % 7 + 7;
                }
                if (clear != shift % 3)
                {
                    OLED_Clear();
                }
                clear = 0;
                OLED_ShowChinese(0, 0, 14, 12);
                OLED_ShowChinese(12, 0, 15, 12);
                OLED_ShowString(26, 0, ":", 12);
                OLED_ShowChinese(40, 0, 6, 12);
                OLED_ShowChinese(52, 0, aw + 6, 12);
                OLED_ShowString(52, 2, "-", 12);
                OLED_ShowNum(68, 0, ah, 2, 12);
                OLED_ShowChinese(82, 0, 3, 12);
                OLED_ShowNum(98, 0, am, 2, 12);
                OLED_ShowChinese(112, 0, 4, 12);
                OLED_ShowString(50, 4, "OFF", 12);

            }
            else if (sw % 2 != 0)
            {
                if (clear == shift % 3)
                {
                    OLED_Clear();
                }
                clear = 1;
                OLED_ShowChinese(0, 0, 14, 12);
                OLED_ShowChinese(12, 0, 15, 12);
                OLED_ShowString(26, 0, ":", 12);
                OLED_ShowChinese(40, 0, 6, 12);
                OLED_ShowChinese(52, 0, aw + 6, 12);
                OLED_ShowNum(68, 0, ah, 2, 12);
                OLED_ShowChinese(82, 0, 3, 12);
                OLED_ShowNum(98, 0, am, 2, 12);
                OLED_ShowChinese(112, 0, 4, 12);
                OLED_ShowString(50, 4, "ON", 12);
                turnoff = 0;

            }
            if (shift % 3 != 0)
            {
                plus = 0;
                break;
            }

        }
        while (shift % 3 == 1 && !back)
        {

            if (sw % 2 == 0)
            {
                ah = plus % 24;
                if (clear != shift % 3)
                {
                    OLED_Clear();
                }
                clear = 1;
                OLED_ShowChinese(0, 0, 14, 12);
                OLED_ShowChinese(12, 0, 15, 12);
                OLED_ShowString(26, 0, ":", 12);
                OLED_ShowChinese(40, 0, 6, 12);
                OLED_ShowChinese(52, 0, aw + 6, 12);
                OLED_ShowNum(68, 0, ah, 2, 12);
                OLED_ShowString(68, 2, "-", 12);
                OLED_ShowChinese(82, 0, 3, 12);
                OLED_ShowNum(98, 0, am, 2, 12);
                OLED_ShowChinese(112, 0, 4, 12);
                OLED_ShowString(50, 4, "OFF", 12);

            }
            else if (sw % 2 != 0)
            {
                if (clear == shift % 3)
                {
                    OLED_Clear();
                }
                clear = 0;

                OLED_ShowChinese(0, 0, 14, 12);
                OLED_ShowChinese(12, 0, 15, 12);
                OLED_ShowString(26, 0, ":", 12);
                OLED_ShowChinese(40, 0, 6, 12);
                OLED_ShowChinese(52, 0, aw + 6, 12);
                OLED_ShowNum(68, 0, ah, 2, 12);
                OLED_ShowChinese(82, 0, 3, 12);
                OLED_ShowNum(98, 0, am, 2, 12);
                OLED_ShowChinese(112, 0, 4, 12);
                OLED_ShowString(50, 4, "ON", 12);
                turnoff = 0;

            }
            if (shift % 3 != 1)
            {
                plus = 0;
                break;
            }
        }

        while (shift % 3 == 2 && !back)
        {

            if (sw % 2 == 0)
            {
                am = plus % 60;
                if (clear != shift % 3)
                {
                    OLED_Clear();
                }
                clear = 2;
                OLED_ShowChinese(0, 0, 14, 12);
                OLED_ShowChinese(12, 0, 15, 12);
                OLED_ShowString(26, 0, ":", 12);
                OLED_ShowChinese(40, 0, 6, 12);
                OLED_ShowChinese(52, 0, aw + 6, 12);
                OLED_ShowNum(68, 0, ah, 2, 12);
                OLED_ShowChinese(82, 0, 3, 12);
                OLED_ShowNum(98, 0, am, 2, 12);
                OLED_ShowString(98, 2, "-", 12);
                OLED_ShowChinese(112, 0, 4, 12);
                OLED_ShowString(50, 4, "OFF", 12);

            }
            else if (sw % 2 != 0)
            {
                if (clear == shift % 3)
                {
                    OLED_Clear();
                }
                clear = 1;
                OLED_ShowChinese(0, 0, 14, 12);
                OLED_ShowChinese(12, 0, 15, 12);
                OLED_ShowString(26, 0, ":", 12);
                OLED_ShowChinese(40, 0, 6, 12);
                OLED_ShowChinese(52, 0, aw + 6, 12);
                OLED_ShowNum(68, 0, ah, 2, 12);
                OLED_ShowChinese(82, 0, 3, 12);
                OLED_ShowNum(98, 0, am, 2, 12);
                OLED_ShowChinese(112, 0, 4, 12);
                OLED_ShowString(50, 4, "ON", 12);
                turnoff = 0;

            }
            if (shift % 3 != 2)
            {
                plus = 0;
                break;
            }

        }
        if (back != 0)
        {
            OLED_Clear();
            key = 4;
            set = 2;
            shift = 0;
            back = 0;
            secondkey = 0;
            break;
        }
    }

}

void AlarmRing()
{
    if (sw % 2 != 0)
    {
        if (time->week == aw)
        {
            if (time->hour == ah)
            {
                if (time->minute == am)
                {
                    if (!turnoff)
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

void WorldTime()        //世界时钟
{
    while (1)
    {
        //更新时间
        DS1302_read_time(time);
        time->week = getWeekdayByYearday(time->year,
                                         time->month,
                                         time->day);
        OLED_ShowChinese(0, 0, 25, 12);
        OLED_ShowChinese(12, 0, 26, 12);
        OLED_ShowString(40, 0, ":", 12);
        OLED_ShowNum(50, 0, (u32) (time->hour), 2, 12);
        OLED_ShowString(66, 0, ":", 12);
        OLED_ShowNum(76, 0, (u32) (time->minute), 2, 12);
        OLED_ShowString(92, 0, ":", 12);
        OLED_ShowNum(102, 0, (u32) (time->second), 2, 12); //北京时间
        OLED_ShowChinese(0, 3, 27, 12);
        OLED_ShowChinese(12, 3, 28, 12);
        OLED_ShowChinese(24, 3, 29, 12);
        OLED_ShowString(40, 3, ":", 12);
        OLED_ShowNum(50, 3, (u32) ((time->hour + 12) % 24), 2,
                     12);
        OLED_ShowString(66, 3, ":", 12);
        OLED_ShowNum(76, 3, (u32) (time->minute), 2, 12);
        OLED_ShowString(92, 3, ":", 12);
        OLED_ShowNum(102, 3, (u32) (time->second), 2, 12); //华盛顿时间
        OLED_ShowChinese(0, 6, 30, 12);
        OLED_ShowChinese(12, 6, 31, 12);
        OLED_ShowString(40, 6, ":", 12);
        OLED_ShowNum(50, 6, (u32) ((time->hour + 19) % 24), 2,
                     12);
        OLED_ShowString(66, 6, ":", 12);
        OLED_ShowNum(76, 6, (u32) (time->minute), 2, 12);
        OLED_ShowString(92, 6, ":", 12);
        OLED_ShowNum(102, 6, (u32) (time->second), 2, 12); //伦敦时间
        if (back != 0)
        {
            OLED_Clear();
            key = 4;
            set = 3;
            back = 0;
            secondkey = 0;
            break;
        }
    }
}

void PORT1_IRQHandler(void)
{
    int i = 0;

    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1); //获取中断状态
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);   //清除标志位

    if (status & GPIO_PIN1)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        correct_t = 1;
    }
    if (status & GPIO_PIN4)
    {
        for (i = 0; i < 100; i++)
            ;
        correct_d = 1;
    }
}

void PORT4_IRQHandler(void)
{
    int i = 0;

    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4); //获取中断状态
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);   //清除标志位

    if (status & GPIO_PIN0)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        key = 0;
    }

    else if (status & GPIO_PIN1)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        if (key == 4)
        {
            secondkey = 1;
        }
    }

    else if (status & GPIO_PIN2)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        if (key == 4)
        {
            back++;
        }
        else if (key == 0)
        {
            turnoff++;

        }
    }

    else if (status & GPIO_PIN3)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        convert++;
    }

    else if (status & GPIO_PIN4)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        if (key == 4)
        {
            key = 4;
            set = set + 1;
        }
        else if (key == 0)
        {
            key = 4;
        }
    }

    else if (status & GPIO_PIN5)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        if (set % 4 == 2)
        {
            if (secondkey == 1)
            {
                sw = sw + 1;
            }
        }
        else if (set % 4 == 1)
        {
            if (secondkey == 1)
            {
                pause++;
            }
        }
        else if (set % 4 == 0 && set != 0)
        {
            if (secondkey == 1)
            {
                start++;
            }
        }

    }

    else if (status & GPIO_PIN6)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        if (secondkey == 1)
        {
            if (sw % 2 == 0)
            {
                plus = plus + 1;
            }
            else if (start % 2 == 0)
            {
                plus = plus + 1;
            }
        }
    }

    else if (status & GPIO_PIN7)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        if (set % 4 == 2)
        {
            if (secondkey == 1)
            {
                shift = shift + 1;
            }
        }
        else if (set % 4 == 0)
        {
            if (secondkey == 1)
            {
                shift = shift + 1;
            }

        }
    }

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
