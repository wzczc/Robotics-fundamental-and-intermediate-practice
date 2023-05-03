#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include "Motor.h"
#include "PWM.h"
#include "UART0.h"
#include "Reflectance.h"

#define red                0x01
#define green              0x02
#define blue               0x04
#define yellow             0x03
#define sky blue           0x06
#define white              0x07
#define pink               0x05

void Port2_Init(void){
P2->SEL0 = 0x00;
P2->SEL1 = 0x00; //configure P2.2-P2.0 as GPIO
P2->DS = 0x07; // make P2.2-P2.0 high drive strength
P2->DIR = 0x07; // make P2.2-P2.0 out
P2->OUT = 0x00; // all LEDs off
}

void Port2_Output(uint8_t data){
P2->OUT = data; // write all of P2 outputs
}

//由于我们的小车传感器U1无法显示，以下传感器的情况都是有一个坏了的情况

void main()
{
    uint8_t Data;

    uint16_t f=5000;  //fast speed
    uint16_t l=1000;  //low speed
    uint16_t d1=325;  //time_delay1
    uint16_t d2=50;  //time_delay2
    uint16_t d4=350;  //time_delay4

    uint16_t add1=1000;  //add_speed

    //初始化
    Clock_Init48MHz();
    UART0_Init();
    UART0_Initprintf();
    Reflectance_Init();
    Motor_Init();
    Port2_Init();


    while(1)
    {
        //光电传感模块读取
        Data = Reflectance_Read(500);
        printf("%x\n",Data);
        switch(Data)

        {
            //无路径-->掉头-->右转
            case 0x00:
                Motor_Right(f,f);
                Clock_Delay1ms(d4);
                break;

            //直行
            case 0x18://0001 1000
                Port2_Output(red);
                Motor_Forward(f,f);
                break;

            //T和十字入口-->左转
            case 0xFF://1111 1111
            case 0xFE://1111 1110
            case 0x7F://0111 1111
            case 0x7E://0111 1110
                Port2_Output(green);
                Motor_Left(f,f+add1);
                Clock_Delay1ms(d1);
                break;

            //左转和直行左转-->左转
            case 0xF0://1111 0000
            case 0xF8://1111 1000
            case 0x78://0111 1000
                Motor_Forward(f,f);
                Clock_Delay1ms(d2);
                Port2_Output(blue);
                Motor_Left(f,f+add1);
                Clock_Delay1ms(d1);
                break;

            //右转-->右转，直行右转-->直行
            case 0x0F://0000 1111
            case 0x1F://0001 1111
            case 0x1E://0001 1110
                Port2_Output(yellow);
                Motor_Forward(f,f);
                Clock_Delay1ms(d2);
                Data=Reflectance_Read(500);
                if(Data==0x18 || Data==0x08 || Data==0x04 || Data==0x0C || Data==0x10 || Data==0x20 || Data==0x30)
                  //0001 1000     0000 1000     0000 0100     0000 1100     0001 0000     0010 0000     0011 0000
                {
                    Motor_Forward(f,f);
                } else {
                    Motor_Right(f+add1,f);
                    Clock_Delay1ms(d1);
                }
                break;

            //偏左-->右微调
            case 0x08://0000 1000
            case 0x0C://0000 1100
                Port2_Output(white);
                Motor_Forward(f,l);
                break;

            //偏右-->左微调
            case 0x10://0001 0000
            case 0x30://0011 0000
                Port2_Output(pink);
                Motor_Forward(l,f);
                break;

            //终点-->停止
            case 0xDA://1101 1010
            case 0x98://1001 1000
            case 0xB8://1011 1000
            case 0xBA://1011 1010
            case 0xB6://1011 0110
            case 0xEC://1110 1100
            case 0x6C://0110 1100
                Motor_Stop();

            //其他-->慢速直行
            default:
                Motor_Forward(f-1000,f-1000);
                break;
        }

    }

}


