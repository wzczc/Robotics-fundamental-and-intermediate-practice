#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include "Motor.h"
#include "PWM.h"
#include "UART0.h"
#include "Reflectance.h"

/*
 * 中心：0x3C 0x18
 * 偏右：
 *      4个：0xF0 0x78
 *      3个：0xE0 0x70 0x38
 *      2个：0xC0 0x60 0x30
 *      1个：0x80 0x40 0x20 0x10
 * 偏左：
 *      4个：0x0F 0x1E
 *      3个：0x07 0x0E 0x1C
 *      2个：0x03 0x06 0x0C
 *      1个：0x01 0x02 0x04 0x08
 * */

//由于我们的小车传感器U1无法显示，以下传感器的情况都是有一个坏了的情况

void main()
{
    uint8_t Data;

    //初始化
    Clock_Init48MHz();
    UART0_Initprintf();
    Reflectance_Init();
    Motor_Init();

    while(1)
    {
        uint16_t a=6000;  //较快的轮速
        uint16_t c=5800;  //使小车直行的偏置转速
        uint16_t b=4200;  //较慢的轮速

        //传感模块的读取
        Data = Reflectance_Read(500);
        printf("%d\n",Data);
        switch(Data)
        {
            //中心-->直行
            case 0x3C://0011 1100
            case 0x18://0001 1000
                Motor_Forward(c,a);
                break;

            //左转
            case 0xF0://1111 0000
            case 0xF8://1111 1000
            case 0xFC://1111 1100
            case 0xFE://1111 1110

                Motor_Left(a,a);

                break;

            //偏右-----------
            //四个传感器
            //case 0xF0: Motor_Forward(b,a);break;
            case 0x78: Motor_Forward(b,a);break;

            //3个传感器
            case 0xE0: Motor_Forward(b,a);break;
            case 0x70: Motor_Forward(b,a);break;
            case 0x38: Motor_Forward(b,a);break;

            //2个传感器
            case 0xC0: Motor_Forward(b,a);break;
            case 0x60: Motor_Forward(b,a);break;
            case 0x30: Motor_Forward(b,a);break;

            //1个传感器
            case 0x80: Motor_Forward(b,a);break;
            case 0x40: Motor_Forward(b,a);break;
            case 0x20: Motor_Forward(b,a);break;
            case 0x10: Motor_Forward(b,a);break;


            //右转
            case 0x3E://0011 1110
            case 0x1E://0001 1110
            case 0x0E://0000 1110
            case 0x00:

                Motor_Right(a,a);

                break;


            //偏左-----------
            //四个传感器
            /*case 0x0E://0F
                Motor_Forward(a,b);break;*/
            //case 0x1E: Motor_Forward(a,b);break;

            //3个传感器
            case 0x06://07
                Motor_Forward(a,b);break;
            //case 0x0E: Motor_Forward(a,b);break;
            case 0x1C: Motor_Forward(a,b);break;

            //2个传感器
            case 0x02://03
                Motor_Forward(a,b);break;
            /*case 0x06: Motor_Forward(a,b);break;*/
            case 0x0C: Motor_Forward(a,b);break;

            //1个传感器
            case 0x01: Motor_Forward(a,b);break;
            /*case 0x02: Motor_Forward(a,b);break;*/
            case 0x04: Motor_Forward(a,b);break;
            case 0x08: Motor_Forward(a,b);break;


            //其他情况-->直行
            default: Motor_Forward(a,a);break;
        }


    }

}


