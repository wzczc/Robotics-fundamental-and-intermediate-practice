#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include "Motor.h"
#include "PWM.h"
#include "UART0.h"
#include "Reflectance.h"

/*
 * ���ģ�0x3C 0x18
 * ƫ�ң�
 *      4����0xF0 0x78
 *      3����0xE0 0x70 0x38
 *      2����0xC0 0x60 0x30
 *      1����0x80 0x40 0x20 0x10
 * ƫ��
 *      4����0x0F 0x1E
 *      3����0x07 0x0E 0x1C
 *      2����0x03 0x06 0x0C
 *      1����0x01 0x02 0x04 0x08
 * */

//�������ǵ�С��������U1�޷���ʾ�����´����������������һ�����˵����

void main()
{
    uint8_t Data;

    //��ʼ��
    Clock_Init48MHz();
    UART0_Initprintf();
    Reflectance_Init();
    Motor_Init();

    while(1)
    {
        uint16_t a=6000;  //�Ͽ������
        uint16_t c=5800;  //ʹС��ֱ�е�ƫ��ת��
        uint16_t b=4200;  //����������

        //����ģ��Ķ�ȡ
        Data = Reflectance_Read(500);
        printf("%d\n",Data);
        switch(Data)
        {
            //����-->ֱ��
            case 0x3C://0011 1100
            case 0x18://0001 1000
                Motor_Forward(c,a);
                break;

            //��ת
            case 0xF0://1111 0000
            case 0xF8://1111 1000
            case 0xFC://1111 1100
            case 0xFE://1111 1110

                Motor_Left(a,a);

                break;

            //ƫ��-----------
            //�ĸ�������
            //case 0xF0: Motor_Forward(b,a);break;
            case 0x78: Motor_Forward(b,a);break;

            //3��������
            case 0xE0: Motor_Forward(b,a);break;
            case 0x70: Motor_Forward(b,a);break;
            case 0x38: Motor_Forward(b,a);break;

            //2��������
            case 0xC0: Motor_Forward(b,a);break;
            case 0x60: Motor_Forward(b,a);break;
            case 0x30: Motor_Forward(b,a);break;

            //1��������
            case 0x80: Motor_Forward(b,a);break;
            case 0x40: Motor_Forward(b,a);break;
            case 0x20: Motor_Forward(b,a);break;
            case 0x10: Motor_Forward(b,a);break;


            //��ת
            case 0x3E://0011 1110
            case 0x1E://0001 1110
            case 0x0E://0000 1110
            case 0x00:

                Motor_Right(a,a);

                break;


            //ƫ��-----------
            //�ĸ�������
            /*case 0x0E://0F
                Motor_Forward(a,b);break;*/
            //case 0x1E: Motor_Forward(a,b);break;

            //3��������
            case 0x06://07
                Motor_Forward(a,b);break;
            //case 0x0E: Motor_Forward(a,b);break;
            case 0x1C: Motor_Forward(a,b);break;

            //2��������
            case 0x02://03
                Motor_Forward(a,b);break;
            /*case 0x06: Motor_Forward(a,b);break;*/
            case 0x0C: Motor_Forward(a,b);break;

            //1��������
            case 0x01: Motor_Forward(a,b);break;
            /*case 0x02: Motor_Forward(a,b);break;*/
            case 0x04: Motor_Forward(a,b);break;
            case 0x08: Motor_Forward(a,b);break;


            //�������-->ֱ��
            default: Motor_Forward(a,a);break;
        }


    }

}


