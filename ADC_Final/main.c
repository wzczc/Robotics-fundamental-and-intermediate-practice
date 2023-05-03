#include <stdio.h>
#include <math.h>
#include "msp.h"
#include "UART0.h"
#include "CLock.h"
#include "ADC14.h"
#include "Key.h"
#include "oled.h"
#include "SysTickInts.h"
#include "CortexM.h"
#include "data_process.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>




int length = 2000;    //采样数组长度
int count = 0;        //计数
int CollectFlag = 0;  //采集标志位
int KeyCount = 0;     //中断计数
int ShowFlag = 0;     //显示标志
uint32_t ADC1_Raw[2000] = { 0 };  //未处理过的采样点数组
uint32_t ADC2_Raw[2000] = { 0 };  
float ADC1[2000] = { 0 };         //最终的采样点数组
float ADC2[2000] = { 0 };  
int ADC1_MaxRaw[2000] = { 0 };    //未处理过的最大值采样点数组
int ADC2_MaxRaw[2000] = { 0 };
int ADC1_Max[100] = { 0 };        //未处理过的最大值采样点数组
int ADC2_Max[100] = { 0 };
int MaxRaw1 = 0;   //未处理过的最大值采样点长度
int MaxRaw2 = 0;
int Max1Length = 0;   //处理过的最大值采样点长度
int Max2Length = 0;
int waveforms = -1;    //波形
uint32_t RawADC1 = 0;  //双通道
uint32_t RawADC2 = 0;
int temp[5] = { 0 };   //暂存
int key = 100;         //按键
int CollectFrequency = 0;   //采样频率
int frequency1 = 0;         //频率
int frequency2 = 0;
float phase = 0.0;          //相位差
float DCvoltage1 = 0.0;  //直流电压
float DCvoltage2 = 0.0;  
float ACvoltage1 = 0.0;  //交流电压
float ACvoltage2 = 0.0;
int OverFlag = 0;  
float max1 = 0.0;
float max2 = 0.0;
int j = 0;
int k = 0;


void GPIO_Interrupt();
void PORT4_IRQHandler();
void PORT1_IRQHandler();
void SysTick_Handler();
void T32_INT1_IRQHandler();
void ShowNums(int *A,float a);
void ShowPhase(int *A,float a);


void main(void)
{
    OLED_Init();
    GPIO_Interrupt();
    OLED_Clear();
    key_init();
    Clock_Init48MHz();   
    UART0_Initprintf();
    UART0_Init();
    ADC0_InitSWTriggerRawADC17();  


    uint32_t* ch6 = &RawADC1;
    uint32_t* ch7 = &RawADC2;


    //打开定时器，不分频,48MHZ，32位计数器，周期模式下计数器，到达0时产生中断
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_16, TIMER32_32BIT,
    TIMER32_PERIODIC_MODE);
    Interrupt_enableInterrupt(INT_T32_INT1);


    //开启总中断
    EnableInterrupts();  
    Timer32_setKeyCount(TIMER32_0_BASE, 3000); 
    Timer32_enableInterrupt(TIMER32_0_BASE);
    Timer32_startTimer(TIMER32_0_BASE, true);
    Interrupt_enableMaster();


    while (1)
    {
        if (CollectFlag == 1)
        {
            count++;
            ADC_In67(ch6, ch7);
            ADC1_Raw[count] = *ch6;
            ADC2_Raw[count] = *ch7;
            CollectFlag = 0;
        }
        if (count >= length && OverFlag == 0)
        {
            SysTick->CTRL = 0;
            for (j = 0; j < length; j++)
            {
                ADC1[j] = (ADC1_Raw[j] * 1.0
                        / 16384) * 3.3;
                ADC2[j] = (ADC2_Raw[j] * 1.0
                        / 16384) * 3.3;
            }
            OverFlag = 1;
        }


        if (OverFlag == 1)
        {
            //测直流电压----平均值
            int i = 0;
            for (i=0;i<length;i++)
            {
                if (max1 < ADC1[i])
                {
                    max1 = ADC1[i];
                }
                if (max2 < ADC2[i])
                {
                    max2 = ADC2[i];
                }
            }
            float sum1 = 0.0;
            float sum2 = 0.0;
            for (i = 0; i < length; i++)
            {
                sum1 += ADC1[i];
                sum2 += ADC2[i];
            }

            float average1 = sum1 / length;
            float average2 = sum2 / length;

            if (average1 < 3.30)
            {
                average1 = (average1 - 0.160) / 0.942;
            }
            if (average2 < 3.30)
            {
                average2 = (average2 - 0.160) / 0.942;
            }
            DCvoltage1 = average1;
            DCvoltage2 = average2;


            //测交流电压----有效值
            ACvoltage1 = (max1 - average1) / (sqrt(2) * 0.952);
            ACvoltage2 = (max2 - average2) / (sqrt(2) * 0.952);


            //测频率
            for(i = 0;i<length;i++)
            {
                if((ADC1[i] > (max1-0.1)))
                {
                    ADC1_MaxRaw[MaxRaw1] = i;
                    if (MaxRaw1>0)
                    {
                        if(ADC1_MaxRaw[MaxRaw1]-ADC1_MaxRaw[MaxRaw1-1] != 1)
                        {
                            ADC1_Max[Max1Length] = ADC1_Max[Max2Length];
                            Max2Length++;
                        }
                    }
                    MaxRaw1++;
                }
            }
            for(i = 0;i<length;i++)
            {
                if((ADC2[i] > (max2-0.1)))
                {
                    ADC2_MaxRaw[MaxRaw2] = i;
                    if (MaxRaw2>0)
                    {
                        if(ADC2_MaxRaw[MaxRaw2]-ADC2_MaxRaw[MaxRaw2-1] != 1)
                        {
                            ADC2_Max[Max2Length] = ADC2_Max[Max2Length];
                            Max2Length++;
                        }
                    }
                    MaxRaw2++;
                }
            }
            
            frequency1 = (CollectFrequency * (Max1Length - 2))
                    / (ADC1_Max[Max1Length - 1]
                            - ADC1_Max[1]);
            frequency2 = (CollectFrequency * (Max2Length - 2))
                    / (ADC2_Max[Max2Length - 1]
                            - ADC2_Max[1]);


            //测量相位差
            phase = 0.0;
            for (j = 0; j < Max1Length; j++)
            {
                int min = 2000;
                for (k = 0; k < Max2Length; k++)
                {
                    if (min
                            > abs(ADC1_Max[j]
                                    - ADC2_Max[k]))
                    {
                        min = abs(
                                ADC1_Max[j]
                                        - ADC2_Max[k]);
                    }
                }
                phase += min;
            }
            phase = phase * 0.00002 * frequency1 * 360 / (Max1Length * 1.0);


            //测波形
            if (MaxRaw1 < 250)
            {
                waveforms = 1;
            } else if (MaxRaw1 < 500)
            {
                waveforms = 0;
            } else
            {
                waveforms = 2;
            }


            OverFlag = 2;
        }

        
        if (OverFlag == 2)
        {
            if (key == 0)    //电参量测量
            {
                OLED_Clear();
                ShowFlag = 0;
                key = 10;
            }
            if (key == 2)  //重新采样
            {
                OLED_Clear();
                ShowFlag = 10;
                key = 10;
                count = 0;
                SysTick->CTRL = 0x00000007;   //打开采集中断，重新采样
                OverFlag = 0;
            }
            if (key == 4)   //直流
            {
                OLED_Clear();
                ShowFlag = 1;
                key = 10;
            }
            if (key == 5)   //交流
            {
                OLED_Clear();
                ShowFlag = 2;
                key = 10;
            }
            if (key == 1)   //交流
            {
                OLED_Clear();
                ShowFlag = 3;
                key = 10;
            }
            if (ShowFlag == 0)     //显示“电参量测量”
            {
                OLED_ShowChinese(0, 0, 14, 16);
                OLED_ShowChinese(16, 0, 15, 16);
                OLED_ShowChinese(32, 0, 16, 16);
                OLED_ShowChinese(48, 0, 17, 16);
                OLED_ShowChinese(64, 0, 18, 16);
            }

            if (ShowFlag == 1)     //直流测量显示
            {
                OLED_ShowString(0, 0, "DC", 16);
                OLED_ShowString(32, 0, ":", 16);
                OLED_ShowString(50, 0, "C", 16);
                OLED_ShowString(58, 0, "H", 16);
                OLED_ShowString(66, 0, "1", 16);
                OLED_ShowString(84, 0, "C", 16);
                OLED_ShowString(92, 0, "H", 16);
                OLED_ShowString(100, 0, "2", 16);

                OLED_ShowString(0, 3, "U/V", 16);
                ShowNums(temp, DCvoltage1);

                OLED_ShowNum(32, 3, temp[0], 1, 16);
                OLED_ShowString(40, 3, ".", 16);
                OLED_ShowNum(48, 3, temp[1], 1, 16);
                OLED_ShowNum(56, 3, temp[2], 1, 16);
                OLED_ShowNum(64, 3, temp[3], 1, 16);


                ShowNums(temp, DCvoltage2);

                OLED_ShowNum(80, 3, temp[0], 1, 16);
                OLED_ShowChar(88, 3, '.', 16);
                OLED_ShowNum(96, 3, temp[1], 1, 16);
                OLED_ShowNum(104, 3, temp[2], 1, 16);
                OLED_ShowNum(112, 3, temp[3], 1, 16);

            }
            if (ShowFlag == 2)             
            {

                OLED_ShowString(0, 0, "AC", 16);
                OLED_ShowString(32, 0, ":", 16);
                OLED_ShowString(50, 0, "C", 16);
                OLED_ShowString(58, 0, "H", 16);
                OLED_ShowString(66, 0, "1", 16);
                OLED_ShowString(84, 0, "C", 16);
                OLED_ShowString(92, 0, "H", 16);
                OLED_ShowString(100, 0, "2", 16);
                OLED_ShowString(0, 2, "U/V", 16);
                OLED_ShowString(0, 4, "f/HZ", 16);
                OLED_ShowChinese(0, 6, 30, 16);
                OLED_ShowString(16, 6, "/", 16);
                OLED_ShowChinese(24, 6, 31, 16);
                ShowNums(temp, ACvoltage1);

                OLED_ShowNum(32, 2, temp[0], 1, 16);
                OLED_ShowString(40, 2, ".", 16);
                OLED_ShowNum(48, 2, temp[1], 1, 16);
                OLED_ShowNum(56, 2, temp[2], 1, 16);
                OLED_ShowNum(64, 2, temp[3], 1, 16);


                ShowNums(temp, ACvoltage2);

                OLED_ShowNum(80, 2, temp[0], 1, 16);
                OLED_ShowString(88, 2, ".", 16);
                OLED_ShowNum(96, 2, temp[1], 1, 16);
                OLED_ShowNum(104, 2, temp[2], 1, 16);
                OLED_ShowNum(112, 2, temp[3], 1, 16);
                OLED_ShowNum(40, 4, frequency1, 4, 16);
                OLED_ShowNum(80, 4, frequency2, 4, 16);

                ShowPhase(temp, phase);
                OLED_ShowNum(48, 6, temp[0], 1, 16);
                OLED_ShowNum(56, 6, temp[1], 1, 16);
                OLED_ShowNum(64, 6, temp[2], 1, 16);
                OLED_ShowString(72, 6, ".", 16);
                OLED_ShowNum(80, 6, temp[3], 1, 16);
                OLED_ShowNum(88, 6, temp[4], 1, 16);


            }
            if (ShowFlag == 3)
            {
                if(waveforms == 0){
                    OLED_ShowChinese(0, 0, 32, 16);
                    OLED_ShowChinese(16, 0, 33, 16);
                    OLED_ShowChinese(32, 0, 37, 16);
                    OLED_ShowString(0, 2, "U/V", 16);
                    OLED_ShowString(0, 4, "f/HZ", 16);
                    ShowNums(temp, (max1-0.16)/0.952);
                    OLED_ShowNum(32, 2, temp[0], 1, 16);
                    OLED_ShowString(40, 2, ".", 16);
                    OLED_ShowNum(48, 2, temp[1], 1, 16);
                    OLED_ShowNum(56, 2, temp[2], 1, 16);
                    OLED_ShowNum(64, 2, temp[3], 1, 16);
                    OLED_ShowNum(40, 4, frequency1, 4, 16);

                }
                else if(waveforms == 1){
                    OLED_ShowChinese(0, 0, 34, 16);
                    OLED_ShowChinese(16, 0, 35, 16);
                    OLED_ShowChinese(32, 0, 37, 16);
                    OLED_ShowString(0, 2, "U/V", 16);
                    OLED_ShowString(0, 4, "f/HZ", 16);
                    ShowNums(temp, (max1-0.16)/0.952);
                    OLED_ShowNum(32, 2, temp[0], 1, 16);
                    OLED_ShowString(40, 2, ".", 16);
                    OLED_ShowNum(48, 2, temp[1], 1, 16);
                    OLED_ShowNum(56, 2, temp[2], 1, 16);
                    OLED_ShowNum(64, 2, temp[3], 1, 16);
                    OLED_ShowNum(40, 4, frequency1, 4, 16);

                }
                else if(waveforms == 2){
                    OLED_ShowChinese(0, 0, 36, 16);
                    OLED_ShowChinese(16, 0, 37, 16);
                    OLED_ShowString(0, 2, "U/V", 16);
                    OLED_ShowString(0, 4, "f/HZ", 16);
                    ShowNums(temp, (max1-0.16)/0.952);
                    OLED_ShowNum(32, 2, temp[0], 1, 16);
                    OLED_ShowString(40, 2, ".", 16);
                    OLED_ShowNum(48, 2, temp[1], 1, 16);
                    OLED_ShowNum(56, 2, temp[2], 1, 16);
                    OLED_ShowNum(64, 2, temp[3], 1, 16);
                    OLED_ShowNum(40, 4, frequency1, 4, 16);

                }
            }
        }
}



void GPIO_Interrupt()            //按键中断
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); // 设置引脚为上拉输入模式
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);

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


    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);  // 清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN0);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN5);


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
        key = 1;

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

            SysTick_Init(480, 2);
            CollectFrequency = 50000;

    }

    else if (status & GPIO_PIN2)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        key = 2;
    }

    else if (status & GPIO_PIN3)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        SysTick_Init(240000, 2);
        CollectFrequency = 200;
    }

    else if (status & GPIO_PIN4)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;

        key = 4;

    }

    else if (status & GPIO_PIN5)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;

        key = 5;
    }
}

//采样中断
void SysTick_Handler(void)
{
    if (count <= length)
    {
        CollectFlag++;
        CollectFlag %= 2;
    }
}

void T32_INT1_IRQHandler(void)
{
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
    KeyCount++;
    if (KeyCount >= 20)
    {
        KeyCount = 0;
        Key_Loop();
    }
    Timer32_setKeyCount(TIMER32_0_BASE, 3000);
    Timer32_startTimer(TIMER32_0_BASE, true);
}

void ShowNums(int *A,float a)
{
   int b = a*10000;
   int c = 0;
   for( i = 0; i<5 ; i++)
   {
       c = b%10;
       b = b/10;
       A[4-i] = c;
   }
}

void ShowPhase(int *A,float a)
{
    int B = a/1;   
    int c = (a-b)*100;
    int d = 0;
    for(i = 0; i < 3 ; i++) 
    {
        d = z%10;
        b = b/10;
        array[2-i] = d;
    }
    d = 0;
    for(i = 3; i<5; i++)
    {
        d = c%10;
        c = c/10;
        A[7-i] = d;
    }
}