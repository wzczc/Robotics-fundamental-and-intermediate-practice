//#include "msp.h"
//#include "ADC14.h"
//#include "SysTickInts.h"
//#include "UART0.h"
//#include "CLock.h"
//#include "stdio.h"
//#include "CortexM.h"
//#include "Key.h"
//#include "oled.h"
//#include "data_process.h"
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include "stdio.h"
//#include "math.h"
//#define times 3000   //��������
//
//
////���ڲ���
//unsigned char test_flag = 0;
//
//int num = 0;
//unsigned char collect_flag = 0;  //�ɼ���־λ��Ϊ1ʱ��ȡ��ѹ
//int Key_Count = 0;
//unsigned char Key_Num = 0;
//
////���ݴ������
//uint32_t ADCRaw_Channel1_Flag[2000] = {0};  //ÿ�����2000���㣬��ʱ����0.5msȡһ����
//uint32_t ADCRaw_Channel2_Flag[2000] = {0};  //ͨ��2
//float ADCDeal_Channel1_Flag[2000] = {0};  //����������ݴ�Ŵ�
//float ADCDeal_Channel2_Flag[2000] = {0};  //ͨ��2
//int ADCFreq_Channel1[2000] = {0};
//int ADCFreq_Channel2[2000] = {0};
//int ADCEndFreq_Channel1[100] = {0};
//int ADCEndFreq_Channel2[100] = {0};
//
////���鳤��
//int f_length1 = 0;
//int f_length2 = 0;
//int s_length1 = 0;
//int s_length2 = 0;
//
////������ʱ��
//int ch6 = 0;
//int ch7 = 0;
//int *p1; //p4.6
//int *p2; //p4.7
//int temp[5] = {0};
//
////�������
//int frecq1 = 0;
//int frecq2 = 0;
//float phase = 0.0;  //��λ��
//float voltage1 = 0.0;  //���ֵ
//float voltage2 = 0.0;  //���ֵ
//
//unsigned over_flag = 0;
////���ڼ���
//int j = 0;
//int k = 0;
//
////ģʽѡ��
//unsigned char mode = 0;
//
////�����ж�
//void SysTick_Handler(void)
//{
//    if(num<=times)
//    {
//        collect_flag++;
//        collect_flag%=2;
//    }
//}
//
//void T32_INT1_IRQHandler(void)
//{
//    Timer32_clearInterruptFlag(TIMER32_0_BASE);
//    Key_Count++;
//    if(Key_Count>=20)
//    {
//        Key_Count = 0;
//        Key_Loop();
//    }
//    Timer32_setCount(TIMER32_0_BASE,3000);
//    Timer32_startTimer(TIMER32_0_BASE,true);
//}
//
//void main(void)
//{
//    p1 = &ch6;
//    p2 = &ch7;
////    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
//    init();                         //��ʼ��
//    OLED_Init();
//    OLED_Clear();
//    key_init();
//    Clock_Init48MHz();   //��ʼ��ʱ��Ϊ48MHZ
//    UART0_Initprintf();
//    SysTick_Init(480,0);   //ϵͳ�δ�ʱ����ȡ��Ƶ��Ϊ50000HZ�����ȼ�Ϊ2
//    UART0_Init();
//    ADC0_InitSWTriggerCh67();  //p4.6 6ͨ�� p4.7 7ͨ��
//
//    //�򿪶�ʱ��1��Ԥ��Ƶ1,cpuƵ��Ϊ48MHZ��32λ������������ģʽ �¼��������ڵ���0ʱ�����ж�
//    Timer32_initModule(TIMER32_0_BASE,TIMER32_PRESCALER_16,TIMER32_32BIT,TIMER32_PERIODIC_MODE);
////    //�����ж�
//    Interrupt_enableInterrupt(INT_T32_INT1);
//    //�������ж�
//    EnableInterrupts();  //ʹ���жϣ�5.31��ʱ��TI����
//    Timer32_setCount(TIMER32_0_BASE,3000); //1ms�ж�
//    Timer32_enableInterrupt(TIMER32_0_BASE);
//    Timer32_startTimer(TIMER32_0_BASE,true);
//
//    //�������ж�
//    EnableInterrupts();  //ʹ���жϣ�5.31��ʱ��TI����
//    Interrupt_enableMaster();
//
//    //���ڲ���
////    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0);
//
//    while(1)
//    {
//        if(collect_flag == 1)
//        {
//            num++;
//            ADC_In67(p1,p2);
//            ADCRaw_Channel1_Flag[num] = *p1;
//            ADCRaw_Channel2_Flag[num] = *p2;
//        }
//        if(num >= times && over_flag == 0)
//        {
//            SysTick->CTRL = 0;
//            OLED_ShowCHinese(32,3,73);
//            OLED_ShowCHinese(48,3,74);
//            OLED_ShowCHinese(64,3,75);
//            OLED_ShowCHinese(80,3,76);
//            for(j = 0; j<times; j++)
//            {
//                ADCDeal_Channel1_Flag[j] = (ADCRaw_Channel1_Flag[j]*1.0/16384)*3.3;
//                ADCDeal_Channel2_Flag[j] = (ADCRaw_Channel2_Flag[j]*1.0/16384)*3.3;
//                printf("%.4fV \t",ADCDeal_Channel1_Flag[j]);
//                //printf("%.4fV \n",ADCDeal_Channel2_Flag[j]);
//            }
//            over_flag = 1;
//        }
//        //�����ֵ
//        if(over_flag == 1)
//        {
//
//            // ------------------------------------------------------------------------
////            // 直流电压测量
////                        float sum1 = 0;
////                        float sum2 = 0;
////                        int i = 0;
////                        for (i = 0; i <times; i++)
////                        {
////                            sum1 += ADCDeal_Channel1_Flag[i];
////                            sum2 += ADCDeal_Channel2_Flag[i];
////                        }
////
////                        float v1 = sum1/times;
////                        float v2 = sum2/times;
////
////
////                        if (v1 < 3.30){
////                            v1 = (v1 - 0.160)/0.942;
////                        }
////                        if (v2 < 3.30){
////                            v2 = (v2 - 0.160)/0.942;
////                        }
////
////                        printf("%.4f\n",v1);
////                        printf("%.4f\n",v2);
////
////                        over_flag = 2;
////
////            // ------------------------------------------------------------------------
//
//            // 交流
//
//                                    int i = 0;
//                                    float sum1 = 0;
//                                    float sum2 = 0;
//                                    for (i = 0; i <times; i++)
//                                    {
//                                        sum1 += ADCDeal_Channel1_Flag[i];
//                                        sum2 += ADCDeal_Channel2_Flag[i];
//                                    }
//
//                                    float average1 = sum1/times;
//                                    float average2 = sum2/times;
//
//
//            float max1 = 0.0;
//            float max2 = 0.0;
//            max1 = get_max(ADCDeal_Channel1_Flag, times);
//            max2 = get_max(ADCDeal_Channel2_Flag, times);
//
//            float v1 = (max1 - average1)/(sqrt(2)*0.952);
//            float v2 = (max2 - average2)/(sqrt(2)*0.952);
////            printf("%f\n",v1);
////            printf("%f\n",v2);
//
//
//            voltage1 =  max1;
//            voltage2 =  max2;
////            printf("\n%.4f\n",max1);
////            printf("\n%.4f\n",max2);
//            f_length1 =  get_index(ADCDeal_Channel1_Flag,ADCFreq_Channel1,times,max1);
//            f_length2 =  get_index(ADCDeal_Channel2_Flag,ADCFreq_Channel2,times,max2);
////            printf("\n%d",f_length1);
////            printf("\n%d",f_length2);
//            s_length1 =  delete_data(ADCFreq_Channel1,ADCEndFreq_Channel1,f_length1);
////            printf("\n%d",s_length1);
//            s_length2 =  delete_data(ADCFreq_Channel2,ADCEndFreq_Channel2,f_length2);
////            printf("\n%d",s_length1);
////            printf("\n%d",s_length2);
////            printf("\nchanel1");
//            for(j =0 ; j<s_length2+1; j++)
//            {
////                printf("\n%d",ADCEndFreq_Channel2[j]);
//            }
////            printf("\nchanel2");
//            for(j =0 ; j<s_length1+1; j++)
//            {
////                printf("\n%d",ADCEndFreq_Channel1[j]);
//            }
//            frecq1 = (50000*(s_length2-2))/(ADCEndFreq_Channel2[s_length2-1]-ADCEndFreq_Channel2[1]);
//            frecq2 = (50000*(s_length1-2))/(ADCEndFreq_Channel1[s_length1-1]-ADCEndFreq_Channel1[1]);
//            printf("\n%dhz",frecq1);
////            printf("\n%dhz",frecq2);
//
//            //������λ��
//            if(((frecq1>=frecq2) && (frecq1 - frecq2)<2) || ((frecq1<=frecq2) && (frecq2 - frecq1)<2))  //Ƶ�ʽ������
//            {
//                phase = 0.0;
//                for(j = 0; j < s_length1; j++)
//                {
//                    int min = 2000;
//                    for(k = 0; k < s_length2; k++)
//                    {
//                        if(min > abs(ADCEndFreq_Channel1[j]-ADCEndFreq_Channel2[k]))
//                        {
//                            min = abs(ADCEndFreq_Channel1[j]-ADCEndFreq_Channel2[k]);
//                        }
//                    }
//                    phase += min;
////                    printf("\n%.3f",phase);
//                }
//                phase = phase/((s_length1)*1.0);
////                printf("\n%.3f",phase);
//                phase = phase*0.00002*frecq1*360;
//                //printf("\n%.3f",phase);
//            }
//            over_flag =2;
//            OLED_ShowCHinese(32,5,77);
//            OLED_ShowCHinese(48,5,78);
//            OLED_ShowCHinese(64,5,75);
//            OLED_ShowCHinese(80,5,76);
//        }
//        if(over_flag == 2)
//        {
//                    if(mode == 0)
//                    {
//                        OLED_ShowString(32,0,"ADC");
//                        OLED_ShowCHinese(64,0,73);
//                        OLED_ShowCHinese(80,0,74);
//                    }
//
//                    Key_Num = Key();
//
//                    if(Key_Num == 1)
//                    {
//                        OLED_Clear();
//                        mode = 0;
//                        num = 0;
//                        SysTick->CTRL = 0x00000007;   //�򿪲ɼ��ж�
//                        over_flag = 0;
//                    }
//                    if(Key_Num == 2)
//                    {
//                        OLED_Clear();
//                        mode = 1;
//                    }
//                    if(Key_Num == 5)
//                    {
//                        OLED_Clear();
//                        mode = 2;
//                    }
//                    if(mode == 1)
//                    {
//                        OLED_ShowString(0,3,"CH1");
//                        OLED_ShowChar(32,3,':');
//                        float_int(temp,voltage1);
//            //            printf("%.4f",voltage1);
//                        OLED_ShowNum(40,3,temp[0],1,16);
//                        OLED_ShowChar(48,3,'.');
//                        OLED_ShowNum(56,3,temp[1],1,16);
//                        OLED_ShowNum(64,3,temp[2],1,16);
//                        OLED_ShowNum(72,3,temp[3],1,16);
//                        OLED_ShowNum(80,3,temp[4],1,16);
//                        OLED_ShowChar(88,3,'V');
//
//                        OLED_ShowString(0,6,"CH2");
//                        OLED_ShowChar(32,6,':');
//                        float_int(temp,voltage2);
//            //            printf("%.4f",voltage2);
//                        OLED_ShowNum(40,6,temp[0],1,16);
//                        OLED_ShowChar(48,6,'.');
//                        OLED_ShowNum(56,6,temp[1],1,16);
//                        OLED_ShowNum(64,6,temp[2],1,16);
//                        OLED_ShowNum(72,6,temp[3],1,16);
//                        OLED_ShowNum(80,6,temp[4],1,16);
//                        OLED_ShowChar(88,6,'V');
//                    }
//                    if(mode == 2)
//                    {
//
//                        OLED_ShowString(0,0,"CH1");
//                        OLED_ShowChar(24,0,':');
//                        OLED_ShowNum(32,0,frecq1,4,16);
//            //            printf("%d",frecq1);
//                        OLED_ShowString(72,0,"hz");
//
//                        OLED_ShowString(0,3,"CH2");
//                        OLED_ShowChar(24,3,':');
//                        OLED_ShowNum(32,3,frecq2,4,16);
//            //            printf("%d",frecq2);
//                        OLED_ShowString(72,3,"hz");
//
//                        out_phase(temp,phase);
//                        OLED_ShowNum(32,6,temp[0],1,16);
//                        OLED_ShowNum(40,6,temp[1],1,16);
//                        OLED_ShowNum(48,6,temp[2],1,16);
//                        OLED_ShowChar(56,6,'.');
//                        OLED_ShowNum(64,6,temp[3],1,16);
//                        OLED_ShowNum(72,6,temp[4],1,16);
//                        OLED_ShowCHinese(80,6,83);
//
//                    }
//        }
//
////            //���ط�ֵ���
////            for(i = 0 ; i<times; i++)
////            {
////                if( (ADCDeal_Flag[i]>max-error) && (ADCDeal_Flag[i] < (max + error)))
////                {
////                    ADCFreq[freq_num] = i;
////                    freq_num++;
//////                    printf("%d\n",i);
////                }
////            }
////            for(i = 0; i<freq_num; i++)
////            {
////                if(ADCFreq[i+1]-ADCFreq[i] == 1)
////                {
////
////                }
////                else
////                {
////                    ADCEndFreq[freqend_num] = ADCFreq[i];
//////                    printf("%d\n",ADCEndFreq[freqend_num]);
//////                    printf("\n%d\n",freqend_num);
////                    freqend_num++;
////                }
////
////            }
////            //���Ƶ��
//////            printf("\n%d",freqend_num);
////            int freq_mean = (ADCEndFreq[freqend_num-1]-ADCEndFreq[0])/(freqend_num-1);
//////            printf("\n%d",freq_mean);
////            printf("\n%dhz",50000/freq_mean);
////            over_flag = 2;
////        }
////
////
////
////
//////        //����ֱ����ѹƽ��ֵ
//////        for(i = 0; i <2000; i++)
//////        {
//////
//////        }
//    }
//}
//
//
//













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
#define times 2000   //采样次数

//用于测试
unsigned char test_flag = 0;

int num = 0;
unsigned char collect_flag = 0;  //采集标志位，为1时采取电压
int Key_Count = 0;
unsigned char Key_Num = 0;

//数据存放数组
uint32_t ADCRaw_Channel1_Flag[2000] = { 0 };  //每秒采样2000个点，定时器隔0.5ms取一次样
uint32_t ADCRaw_Channel2_Flag[2000] = { 0 };  //通道2
float ADCDeal_Channel1_Flag[2000] = { 0 };  //被处理的数据存放处
float ADCDeal_Channel2_Flag[2000] = { 0 };  //通道2
int ADCFreq_Channel1[2000] = { 0 };
int ADCFreq_Channel2[2000] = { 0 };
int ADCEndFreq_Channel1[100] = { 0 };
int ADCEndFreq_Channel2[100] = { 0 };

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
int temp[5] = { 0 };

//按键值
int key = 10;
int key_num = 10;

//输出数据
int frecq1 = 0;
int frecq2 = 0;
float phase = 0.0;  //相位差
float DCvoltage1 = 0.0;  //平均值
float DCvoltage2 = 0.0;  //平均值
float ACvoltage1 = 0.0;  //有效值
float ACvoltage2 = 0.0;  //有效值

unsigned over_flag = 0;
//用于计数
int j = 0;
int k = 0;

//模式选择
unsigned char mode = 0;

void PORT4_IRQHandler();

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

    //GPIO_registerInterrupt(GPIO_PORT_P1, PORT1_IRQHandler);  // 注册中断函数
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

//    else if (status & GPIO_PIN1)    // 判断中断口
//    {
//        for (i = 0; i < 100; i++)
//            ;
//        if (key == 4)
//        {
//            secondkey = 1;
//        }
//    }
//
    else if (status & GPIO_PIN2)    // 判断中断口
    {
        for (i = 0; i < 100; i++)
            ;
        key = 2;
    }
//
//    else if (status & GPIO_PIN3)    // 判断中断口
//    {
//        for (i = 0; i < 100; i++)
//            ;
//        convert++;
//    }

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
//
//    else if (status & GPIO_PIN6)    // 判断中断口
//    {
//        for (i = 0; i < 100; i++)
//            ;
//        if (secondkey == 1)
//        {
//            if (sw % 2 == 0)
//            {
//                plus = plus + 1;
//            }
//            else if (start % 2 == 0)
//            {
//                plus = plus + 1;
//            }
//        }
//    }

//    else if (status & GPIO_PIN7)    // 判断中断口
//    {
//        for (i = 0; i < 100; i++)
//            ;
//        if (set % 4 == 2)
//        {
//            if (secondkey == 1)
//            {
//                shift = shift + 1;
//            }
//        }
//        else if (set % 4 == 0)
//        {
//            if (secondkey == 1)
//            {
//                shift = shift + 1;
//            }
//
//        }
//    }

}
//采样中断
void SysTick_Handler(void)
{
    if (num <= times)
    {
        collect_flag++;
        collect_flag %= 2;
    }
}

void T32_INT1_IRQHandler(void)
{
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
    Key_Count++;
    if (Key_Count >= 20)
    {
        Key_Count = 0;
        Key_Loop();
    }
    Timer32_setCount(TIMER32_0_BASE, 3000);
    Timer32_startTimer(TIMER32_0_BASE, true);
}

void main(void)
{
    p1 = &ch6;
    p2 = &ch7;
//    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    //init();                         //初始化
    OLED_Init();
    GPIO_Interrupt();
    OLED_Clear();
    key_init();
    Clock_Init48MHz();   //初始化时钟为48MHZ
    UART0_Initprintf();
    SysTick_Init(480, 0);   //系统滴答定时器，取样频率为50000HZ，优先级为2
    UART0_Init();
    ADC0_InitSWTriggerCh67();  //p4.6 6通道 p4.7 7通道

    //打开定时器1，预分频1,cpu频率为48MHZ，32位计数器，周期模式 下计数器，在到达0时产生中断
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_16, TIMER32_32BIT,
    TIMER32_PERIODIC_MODE);
//    //开启中断
    Interrupt_enableInterrupt(INT_T32_INT1);
    //开启总中断
    EnableInterrupts();  //使能中断，5.31暂时用TI给的
    Timer32_setCount(TIMER32_0_BASE, 3000); //1ms中断
    Timer32_enableInterrupt(TIMER32_0_BASE);
    Timer32_startTimer(TIMER32_0_BASE, true);

    //开启总中断
    EnableInterrupts();  //使能中断，5.31暂时用TI给的
    Interrupt_enableMaster();

    //用于测试
//    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0);

    while (1)
    {
        if (collect_flag == 1)
        {
            num++;
            ADC_In67(p1, p2);
            ADCRaw_Channel1_Flag[num] = *p1;
            ADCRaw_Channel2_Flag[num] = *p2;
        }
        if (num >= times && over_flag == 0)
        {
            SysTick->CTRL = 0;
//            OLED_ShowChinese(32,3,73);
//            OLED_ShowChinese(48,3,74);
//            OLED_ShowChinese(64,3,75);
//            OLED_ShowChinese(80,3,76);
            for (j = 0; j < times; j++)
            {
                ADCDeal_Channel1_Flag[j] = (ADCRaw_Channel1_Flag[j] * 1.0
                        / 16384) * 3.3;
                ADCDeal_Channel2_Flag[j] = (ADCRaw_Channel2_Flag[j] * 1.0
                        / 16384) * 3.3;
                printf("%.4fV \t", ADCDeal_Channel1_Flag[j]);
                printf("%.4fV \n", ADCDeal_Channel2_Flag[j]);
            }
            over_flag = 1;
        }
        //计算峰值
        if (over_flag == 1)
        {
            //测试
            float max1 = 0.0;
            float max2 = 0.0;
            max1 = get_max(ADCDeal_Channel1_Flag, times);
            max2 = get_max(ADCDeal_Channel2_Flag, times);
            float sum1 = 0;
            float sum2 = 0;
            int i = 0;
            for (i = 0; i < times; i++)
            {
                sum1 += ADCDeal_Channel1_Flag[i];
                sum2 += ADCDeal_Channel2_Flag[i];
            }

            float v1 = sum1 / times;
            float v2 = sum2 / times;

            if (v1 < 3.30)
            {
                v1 = (v1 - 0.160) / 0.942;
            }
            if (v2 < 3.30)
            {
                v2 = (v2 - 0.160) / 0.942;
            }
            DCvoltage1 = v1;
            DCvoltage2 = v2;
            //printf("\n%.4f\n", v1);
            //printf("\n%.4f\n", v2);
            i = 0;
            sum1 = 0;
            sum2 = 0;
            for (i = 0; i < times; i++)
            {
                sum1 += ADCDeal_Channel1_Flag[i];
                sum2 += ADCDeal_Channel2_Flag[i];
            }

            float average1 = sum1 / times;
            float average2 = sum2 / times;

            max1 = 0.0;
            max2 = 0.0;
            max1 = get_max(ADCDeal_Channel1_Flag, times);
            max2 = get_max(ADCDeal_Channel2_Flag, times);

            v1 = (max1 - average1) / (sqrt(2) * 0.952);
            v2 = (max2 - average2) / (sqrt(2) * 0.952);
            ACvoltage1 = v1;
            ACvoltage2 = v2;

            f_length1 = get_index(ADCDeal_Channel1_Flag, ADCFreq_Channel1,
            times,
                                  max1);
            f_length2 = get_index(ADCDeal_Channel2_Flag, ADCFreq_Channel2,
            times,
                                  max2);
//            printf("\n%d", f_length1);
//            printf("\n%d", f_length2);
            s_length1 = delete_data(ADCFreq_Channel1, ADCEndFreq_Channel1,
                                    f_length1);
//            printf("\n%d", s_length1);
            s_length2 = delete_data(ADCFreq_Channel2, ADCEndFreq_Channel2,
                                    f_length2);
//            printf("\n%d", s_length1);
//            printf("\n%d", s_length2);
//            printf("\nchanel1");
            for (j = 0; j < s_length2 + 1; j++)
            {
//                printf("\n%d", ADCEndFreq_Channel2[j]);
            }
//            printf("\nchanel2");
            for (j = 0; j < s_length1 + 1; j++)
            {
//                printf("\n%d", ADCEndFreq_Channel1[j]);
            }
            frecq1 = (50000 * (s_length2 - 2))
                    / (ADCEndFreq_Channel2[s_length2 - 1]
                            - ADCEndFreq_Channel2[1]);
            frecq2 = (50000 * (s_length1 - 2))
                    / (ADCEndFreq_Channel1[s_length1 - 1]
                            - ADCEndFreq_Channel1[1]);
            printf("\n%dhz", frecq1);
            printf("\n%dhz", frecq2);

            //测量相位差
            if (((frecq1 >= frecq2) && (frecq1 - frecq2) < 2)
                    || ((frecq1 <= frecq2) && (frecq2 - frecq1) < 2))  //频率近似相等
            {
                phase = 0.0;
                for (j = 0; j < s_length1; j++)
                {
                    int min = 2000;
                    for (k = 0; k < s_length2; k++)
                    {
                        if (min
                                > abs(ADCEndFreq_Channel1[j]
                                        - ADCEndFreq_Channel2[k]))
                        {
                            min = abs(
                                    ADCEndFreq_Channel1[j]
                                            - ADCEndFreq_Channel2[k]);
                        }
                    }
                    phase += min;
//                    printf("\n%.3f",phase);
                }
                phase = phase / ((s_length1) * 1.0);
//                printf("\n%.3f",phase);
                phase = phase * 0.00002 * frecq1 * 360;
//                printf("\n%.3fD",phase);
            }
            over_flag = 2;
//            OLED_ShowCHinese(32,5,77);//电参量测量
//            OLED_ShowCHinese(48,5,78);
//            OLED_ShowCHinese(64,5,75);
//            OLED_ShowCHinese(80,5,76);
//            printf("\nhahaha\n");
        }
        if (over_flag == 2)
        {
            if (key == 0)    //电参量测量
            {
                OLED_Clear();
                key_num = 0;
                key = 10;
            }

            if (key == 2)  //重新采样
            {
                OLED_Clear();
                key_num = 10;
                key = 10;
                num = 0;
                SysTick->CTRL = 0x00000007;   //打开采集中断，重新采样
                over_flag = 0;
            }
            if (key == 4)   //直流
            {
                OLED_Clear();
                key_num = 1;
                key = 10;
            }
            if (key == 5)   //交流
            {
                OLED_Clear();
                key_num = 2;
                key = 10;
            }

            if (key_num == 0)     //显示“电参量测量”
            {
                OLED_ShowChinese(0, 0, 14, 16);
                OLED_ShowChinese(16, 0, 15, 16);
                OLED_ShowChinese(32, 0, 16, 16);
                OLED_ShowChinese(48, 0, 17, 16);
                OLED_ShowChinese(64, 0, 18, 16);
            }

            if (key_num == 1)     //直流测量显示
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
                float_int(temp, DCvoltage1);
                //            printf("%.4f",voltage1);
                OLED_ShowNum(32, 3, temp[0], 1, 16);
                OLED_ShowString(40, 3, ".", 16);
                OLED_ShowNum(48, 3, temp[1], 1, 16);
                OLED_ShowNum(56, 3, temp[2], 1, 16);
                OLED_ShowNum(64, 3, temp[3], 1, 16);
//                        OLED_ShowChar(80,3,'V');

                float_int(temp, DCvoltage2);
                //            printf("%.4f",voltage2);
                OLED_ShowNum(80, 3, temp[0], 1, 16);
                OLED_ShowChar(88, 3, '.', 16);
                OLED_ShowNum(96, 3, temp[1], 1, 16);
                OLED_ShowNum(104, 3, temp[2], 1, 16);
                OLED_ShowNum(112, 3, temp[3], 1, 16);

            }
            if (key_num == 2)             //交流测量显示
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
                float_int(temp, ACvoltage1);
                //            printf("%.4f",voltage1);
                OLED_ShowNum(32, 2, temp[0], 1, 16);
                OLED_ShowString(40, 2, ".", 16);
                OLED_ShowNum(48, 2, temp[1], 1, 16);
                OLED_ShowNum(56, 2, temp[2], 1, 16);
                OLED_ShowNum(64, 2, temp[3], 1, 16);
                //                        OLED_ShowChar(80,3,'V');

                float_int(temp, ACvoltage2);
                //            printf("%.4f",voltage2);
                OLED_ShowNum(80, 2, temp[0], 1, 16);
                OLED_ShowString(88, 2, ".", 16);
                OLED_ShowNum(96, 2, temp[1], 1, 16);
                OLED_ShowNum(104, 2, temp[2], 1, 16);
                OLED_ShowNum(112, 2, temp[3], 1, 16);
                OLED_ShowNum(40, 4, frecq1, 4, 16);
                OLED_ShowNum(80, 4, frecq2, 4, 16);
                //OLED_ShowString(106, 4, "HZ", 16);
                out_phase(temp, phase);
                OLED_ShowNum(48, 6, temp[0], 1, 16);
                OLED_ShowNum(56, 6, temp[1], 1, 16);
                OLED_ShowNum(64, 6, temp[2], 1, 16);
                OLED_ShowString(72, 6, ".", 16);
                OLED_ShowNum(80, 6, temp[3], 1, 16);
                OLED_ShowNum(88, 6, temp[4], 1, 16);
                //OLED_ShowString(88, 2, "°", 16);

            }
        }

//            //返回峰值序号
//            for(i = 0 ; i<times; i++)
//            {
//                if( (ADCDeal_Flag[i]>max-error) && (ADCDeal_Flag[i] < (max + error)))
//                {
//                    ADCFreq[freq_num] = i;
//                    freq_num++;
////                    printf("%d\n",i);
//                }
//            }
//            for(i = 0; i<freq_num; i++)
//            {
//                if(ADCFreq[i+1]-ADCFreq[i] == 1)
//                {
//
//                }
//                else
//                {
//                    ADCEndFreq[freqend_num] = ADCFreq[i];
////                    printf("%d\n",ADCEndFreq[freqend_num]);
////                    printf("\n%d\n",freqend_num);
//                    freqend_num++;
//                }
//
//            }
//            //输出频率
////            printf("\n%d",freqend_num);
//            int freq_mean = (ADCEndFreq[freqend_num-1]-ADCEndFreq[0])/(freqend_num-1);
////            printf("\n%d",freq_mean);
//            printf("\n%dhz",50000/freq_mean);
//            over_flag = 2;
//        }
//
//
//
//
////        //计算直流电压平均值
////        for(i = 0; i <2000; i++)
////        {
////
////        }
    }
}


