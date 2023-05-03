//#include "msp.h"
//#include "stdio.h"
//#include "CortexM.h"
//#include "UART0.h"
//#include "ADC14.h"
//#include "LaunchPad.h"
//#include "SysTickInts.h"
//#include "Clock.h"
//#include "oled.h"
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//
//
//int i = 0;
//int j = 0;
//float max1 = 0;
//float max2 = 0;
//int min1 = 100;
//int min2 = 100;
//int MAX_num1 = 0;
//int MAX_num2 = 0;
//uint32_t RawADC1 = 0;
//uint32_t RawADC2 = 0;
//int ADCflag = 0;
//float MAX_mean1 = 0.0;
//float MAX_mean2 = 0.0;
//int EndMAX_num1 = 0;
//int EndMAX_num2 = 0;
//int Over_flag = 0;
//int length = 2000;
//float ADC1[2000] = { 0 };
//float ADC2[2000] = { 0 };
//int ADCMAX1[2000] = { 0 };
//int ADCEndMAX1[2000] = {0};
//int ADCMAX2[2000] = { 0 };
//int ADCEndMAX2[2000] = {0};
//int CollectFlag = 0;
//float Sum1 = 0.0;
//float Sum2 = 0.0;
//
//void SysTick_Handler(void)
//{
//    if (CollectFlag < length)
//    {
//        ADCflag = 1;
//    }
//}
//
//void main(void)
//{
//    OLED_Init();
//    OLED_Clear();
//    Clock_Init48MHz();
//    ADC0_InitSWTriggerCh67();
//    UART0_Initprintf();
//    LaunchPad_Init();
//    SysTick_Init(4800, 2);  // 采样频率为48MHz/period
//    EnableInterrupts();
//
//    uint32_t* ch6 = &RawADC1;
//    uint32_t* ch7 = &RawADC2;
//
//    while (1)
//    {
//        if (ADCflag == 1)
//        {
//            ADC_In67(ch6,ch7);
//            ADC1[CollectFlag] = (RawADC1*1.0/16383)*3.3;
//            ADC2[CollectFlag] = (RawADC2*1.0/16383)*3.3;
//            Sum1 += ADC1[CollectFlag];
//            Sum2 += ADC2[CollectFlag];
//            CollectFlag++;
//            ADCflag = 0;
//        }
//        if (CollectFlag >= length && Over_flag == 0)
//        {
//            for (j = 0; j < length; j++)
//            {
//                //ADC[j] = (ADCRaw[j] * 1.0 / 16383) * 3.3;
//                //printf("%.4f\t", ADC1[j]);
//            }
//            for (j = 0; j < length; j++)
//            {
//                //ADC[j] = (ADCRaw[j] * 1.0 / 16383) * 3.3;
//                //printf("%.4f\t", ADC2[j]);
//            }
//            Over_flag = 1;
//        }
//
//
////        //----------------------------
////        if (Over_flag == 1)
////        {
////            // 直流
////            float sum1 = 0;
////            float sum2 = 0;
////            for (i = 0; i <length; i++)
////            {
////                sum1 += ADC1[i];
////                sum2 += ADC2[i];
////            }
////
////            float v1 = sum1/3000.0;
////            float v2 = sum2/3000.0;
////
////            printf("%.4f\n",v1);
////            printf("%.4f\n",v2);
////
////            Over_flag = 2;
////        }
////        //----------------------------
//
//        //交流
//        if (Over_flag == 1)
//        {
//            for (i=0;i<length;i++)
//            {
//                if (max1 < ADC1[i])
//                {
//                    max1 = ADC1[i];
//                }
//                if (max2 < ADC2[i])
//                {
//                    max2 = ADC2[i];
//                }
//            }
//
//
//            for(i = 0;i<length;i++)
//            {
//                if((ADC1[i] > (max1-0.1)))
//                {
//                    ADCMAX1[MAX_num1] = i;
//                    if (MAX_num1>0)
//                    {
//                        if(ADCMAX1[MAX_num1]-ADCMAX1[MAX_num1-1] != 1)
//                        {
//                            ADCEndMAX1[EndMAX_num1] = ADCMAX1[MAX_num1];
//                            EndMAX_num1++;
//                        }
//                    }
//                    MAX_num1++;
//                }
//             }
//
//            for(i = 0;i<length;i++)
//            {
//                if((ADC2[i] > (max2-0.1)))
//                {
//                    ADCMAX2[MAX_num2] = i;
//                    if (MAX_num2>0)
//                    {
//                        if(ADCMAX2[MAX_num2]-ADCMAX2[MAX_num2-1] != 1)
//                        {
//                            ADCEndMAX2[EndMAX_num2] = ADCMAX2[MAX_num2];
//                            EndMAX_num2++;
//                        }
//                    }
//                    MAX_num2++;
//                }
//            }
//
//            float average1 = Sum1/(length);
//            float average2 = Sum2/(length);
//
//            printf("%.4f\n",average1);
//            printf("%.4f\n",average2);
//
//            printf("%.4f\n",max1);
//            printf("%.4f\n",max2);
//
//            float v1 = (max1 - average1)/sqrt(2);
//            float v2 = (max2 - average2)/sqrt(2);
//
//            printf("%.4f\n",v1);
//            printf("%.4f\n",v2);
//
//            MAX_mean1 = (ADCEndMAX1[EndMAX_num1-1]-ADCEndMAX1[0])*1.0/(EndMAX_num1-1);
//            printf("\n%f",MAX_mean1);
//            printf("\n%dhz",(int)(10000/MAX_mean1));
//
//
//            MAX_mean2 = (ADCEndMAX2[EndMAX_num2-1]-ADCEndMAX2[0])*1.0/(EndMAX_num2-1);
//            printf("\n%f",MAX_mean2);
//            printf("\n%dhz",(int)(10000/MAX_mean2));
//            Over_flag = 2;
//        }
//    }
//}





#include "msp.h"
#include "ADC14.h"
#include "SysTickInts.h"
#include "UART0.h"
#include "CLock.h"
#include "stdio.h"
#include "CortexM.h"
#include "Key.h"
#include "oled.h"
#include "data_process.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "stdio.h"
#include "math.h"


int length = 2000;
int num = 0;
unsigned char collect_flag = 0;  //采集标志位，为1时采取电压
int Key_Count = 0;
unsigned char Key_Num = 0;

//数据存放数组
float ADC1[1000] = { 0 };
float ADC2[1000] = { 0 };
int ADCMAX1[1000] = { 0 };
int ADCEndMAX1[1000] = {0};
int ADCMAX2[1000] = { 0 };
int ADCEndMAX2[1000] = {0};

uint32_t RawADC1 = 0.0;
uint32_t RawADC2 = 0.0;

int Max_num1 = 0;
int Max_num2 = 0;
int EndMax_num1 = 0;
int EndMax_num2 = 0;
int Max_mean1 = 0;
int Max_mean2 = 0;



//数组长度
int f_length1 = 0;
int f_length2 = 0;
int s_length1 = 0;
int s_length2 = 0;

//数据临时点
int ch6 = 0;
int ch7 = 0;
int *p1; //p4.6
int *p2; //p4.7
int temp[5] = {0};

//输出数据
int frecq1 = 0;
int frecq2 = 0;
float phase = 0.0;  //相位差
float voltage1 = 0.0;  //峰峰值
float voltage2 = 0.0;  //峰峰值


//用于计数
int j = 0;
int k = 0;

//模式选择
unsigned char mode = 0;

//采样中断
void SysTick_Handler(void)
{
    if(num<=length)
    {
        collect_flag++;
        collect_flag%=2;
    }
}

void T32_INT1_IRQHandler(void)
{
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
    Key_Count++;
    if(Key_Count>=20)
    {
        Key_Count = 0;
        Key_Loop();
    }
    Timer32_setCount(TIMER32_0_BASE,3000);
    Timer32_startTimer(TIMER32_0_BASE,true);
}

void main(void)
{


    OLED_Init();
    OLED_Clear();
    key_init();
    Clock_Init48MHz();   //初始化时钟为48MHZ
    UART0_Initprintf();
    SysTick_Init(480,0);   //系统滴答定时器，取样频率为50000HZ，优先级为2
    UART0_Init();
    ADC0_InitSWTriggerCh67();  //p4.6 6通道 p4.7 7通道

    //打开定时器1，预分频1,cpu频率为48MHZ，32位计数器，周期模式 下计数器，在到达0时产生中断
    Timer32_initModule(TIMER32_0_BASE,TIMER32_PRESCALER_16,TIMER32_32BIT,TIMER32_PERIODIC_MODE);
//    //开启中断
    Interrupt_enableInterrupt(INT_T32_INT1);
    //开启总中断
    EnableInterrupts();  //使能中断，5.31暂时用TI给的
    Timer32_setCount(TIMER32_0_BASE,3000); //1ms中断
    Timer32_enableInterrupt(TIMER32_0_BASE);
    Timer32_startTimer(TIMER32_0_BASE,true);

    //开启总中断
    EnableInterrupts();  //使能中断，5.31暂时用TI给的
    Interrupt_enableMaster();

    uint32_t *ch6 = &RawADC1;
    uint32_t *ch7 = &RawADC2;

    int Over_flag = 0;

    //用于测试
//    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0);

    while(1)
    {
        if(collect_flag == 1)
        {
            num++;
            ADC_In67(ch6,ch7);
            ADC1[num] = (RawADC1*1.0/16383)*3.3;
            ADC2[num] = (RawADC2*1.0/16383)*3.3;
            //printf("%f\t",ADC1[num]);
            //printf("%d\t",num);
        }
        if(num >= length)
        {
            SysTick->CTRL = 0;
            // OLED_ShowCHinese(32,3,73);
            // OLED_ShowCHinese(48,3,74);
            // OLED_ShowCHinese(64,3,75);
            // OLED_ShowCHinese(80,3,76);
            Over_flag = 1;
        }

//        if (Over_flag == 1)
//        {
//           // 直流
//           float sum1 = 0;
//           float sum2 = 0;
//           int i = 0;
//           for (i = 0; i <length; i++)
//           {
//               sum1 += ADC1[i];
//               sum2 += ADC2[i];
//           }
//
//           float v1 = sum1/3000.0;
//           float v2 = sum2/3000.0;
//
//           printf("%.4f\n",v1);
//           printf("%.4f\n",v2);
//
//           Over_flag = 2;
//        }

        // 交流
        if(Over_flag == 1)
        {
            float max1 = 0.0;
            float max2 = 0.0;
            int i = 0 ;

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
//            printf("\n%.4f\n",max1);
//            printf("\n%.4f\n",max2);

           for(i = 0;i<length;i++)
           {
               if((ADC1[i] > (max1-0.1)))
               {
                   ADCMAX1[Max_num1] = i;
                   if (Max_num1>0)
                   {
                       if(ADCMAX1[Max_num1]-ADCMAX1[Max_num1-1] != 1)
                       {
                           ADCEndMAX1[EndMax_num1] = ADCMAX1[Max_num1];
                           EndMax_num1++;
                       }
                   }
                   Max_num1++;
               }
                if((ADC2[i] > (max2-0.1)))
               {
                   ADCMAX2[Max_num2] = i;
                   if (Max_num2>0)
                   {
                       if(ADCMAX2[Max_num2]-ADCMAX2[Max_num2-1] != 1)
                       {
                           ADCEndMAX2[EndMax_num2] = ADCMAX2[Max_num2];
                           EndMax_num2++;
                       }
                   }
                   Max_num2++;
               }
            }

           Max_mean1 = (ADCEndMAX1[EndMax_num1-1]-ADCEndMAX1[0])*1.0/(EndMax_num1-1);
           //printf("\n%f",Max_mean1);
           int f1 = 50000/Max_mean1;
           printf("\n%dHz",f1);

           Max_mean2 = (ADCEndMAX2[EndMax_num2-1]-ADCEndMAX2[0])*1.0/(EndMax_num2-1);
           //printf("\n%f",Max_mean2);
           int f2 = 50000/Max_mean2;
           printf("\n%dHz",50000/Max_mean2);

            //测量相位差
            float phase = 0.0;
            for(i = 0; i < EndMax_num1; i++)
                {
                    int min = 1000;
                    for(j = 0; j < EndMax_num2; j++)
                    {
                        if(min > abs(ADCEndMAX1[i]-ADCEndMAX2[j]))
                        {
                            min = abs(ADCEndMAX1[i]-ADCEndMAX2[j]);
                        }
                    }
                    phase += min;
                }
                phase = phase/((EndMax_num1)*1.0);
                phase = phase*0.00002*f1*360;
                printf("\n%.3f°",phase);
            }
            Over_flag =2;

            // OLED_ShowCHinese(32,5,77);
            // OLED_ShowCHinese(48,5,78);
            // OLED_ShowCHinese(64,5,75);
            // OLED_ShowCHinese(80,5,76);
//            printf("\nhahaha\n");
        }

}




