#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include "Motor.h"
#include "PWM.h"
#include "UART0.h"
#include "Reflectance.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/*
 * 十字或T：1直行；2左转；3右转；4右转；5左转；6右转
 * 直左或左转：1左转；2左转；3左转；4左转；5左转；6左转；7左转；8直行
 * 直右或右转：1右转；2右转；3右转；4直行；5右转
 * */

void main() {

    uint8_t data;

    Clock_Init48MHz();
    UART0_Initprintf();
    Reflectance_Init();
    Motor_Init();

    while(1)
    {
        uint16_t f=4800;          //fast 4500
        uint16_t l=1500;          //low  1200

        uint16_t s=4500;

        uint16_t d0=350;

        uint16_t d1=125;           //delay1
        uint16_t d2=400;          //delay2
        uint16_t d3=125;          //delay3

        uint16_t d4=50;          //delay3
        uint16_t d5=125;
        uint16_t d6=400;

        data = Reflectance_Read(500);
        printf("%d\n",data);
        switch(data)
        {
            //无路径
            case 0x00:
                Motor_Left(f,f);
                Clock_Delay1ms(d0);
                break;

            //直行
            case 0x3C:
                Motor_Forward(s,s);
                break;
            case 0x18:
                Motor_Forward(s,s);
                break;

            //左转
            case 0xF8:
            case 0xF0:
            case 0x78:
                    Motor_Forward(f,f);
                    Clock_Delay1ms(d1);
                    Motor_Left(f,f);
                    Clock_Delay1ms(d2);
                    Motor_Forward(f,f);
                    Clock_Delay1ms(d3);
                    break;

            //右转
           // case 0x3F:
            case 0x0F://0000 1110
            case 0x1E://0x1F 0000
            case 0x0E://0x0F 0000
                    /*Motor_Forward(f,f);
                    Clock_Delay1ms(d4);
                    data=Reflectance_Read(1000);
                    if(data==0x04 | 0x08 | 0x18 | 0x0C | 0x30 | 0x10 | 0x20) {
                        Motor_Forward(f,f);
                        //Clock_Delay1ms(d5);
                    } else {*/
                        Motor_Right(f,f);
                        Clock_Delay1ms(d6);
                        Motor_Forward(f,f);
                        Clock_Delay1ms(d5);
                    //}
                break;

            //T或十字入口
            case 0xFE://0xFF
            case 0x7E:
            case 0xFC:
            case 0x3E:
            case 0x7C:
                Motor_Forward(f,f);
                Clock_Delay1ms(d1);
                Motor_Left(f,f);
                Clock_Delay1ms(d2);
                Motor_Forward(f,f);
                Clock_Delay1ms(d4);
                break;

            //偏右-----------

            //2个传感器
            case 0x60:
                Motor_Forward(l,f);
                break;
            case 0x30:
                Motor_Forward(l,f);
                break;

            //1个传感器
            case 0x40:
                Motor_Forward(l,f);
                break;
            case 0x20:
                Motor_Forward(l,f);
                break;
            case 0x10:
                Motor_Forward(l,f);
                break;


            //偏左-----------

            //2个传感器
            case 0x06://07
                Motor_Forward(f,l);
                break;
            case 0x0C:
                Motor_Forward(f,l);
                break;

            //1个传感器
            case 0x02:
                Motor_Forward(f,l);
                break;
            case 0x04:
                Motor_Forward(f,l);
                break;
            case 0x08:
                Motor_Forward(f,l);
                break;

           //终点
            case 0xDA:
                Motor_Stop();


            default: Motor_Forward(f-1000,f-1000);break;
        }

    }

}
