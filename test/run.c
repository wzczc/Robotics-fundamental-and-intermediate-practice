/*#include "msp.h"
#include "../inc/Clock.h"
#include <stdio.h>
#include "Motor.h"
#include "PWM.h"

void main()
{
    Motor_Init();

    Motor_Stop();
    Clock_Delay1ms(2000);

    Motor_Forward(9999,9999);
    Clock_Delay1ms(3000);

    Motor_Forward(9999,9999);
    Clock_Delay1ms(3000);
    int i=0;
    for(i=0;i<=1;i++)
    {
        //ǰ��2m
        Motor_Forward(9999,9999);
        Clock_Delay1ms(6250);

        //��ת90��
        Motor_Left(4000,4000);
        Clock_Delay1ms(1500);

        //ǰ��40cm
        Motor_Forward(9999,9999);
        Clock_Delay1ms(1250);

        //��ת1350��
        Motor_Right(8000,8000);
        Clock_Delay1ms(7750);

        //ǰ��2m
        Motor_Forward(9999,9999);
        Clock_Delay1ms(6250);

        //����ǰ��180��
        Motor_Forward(4000,8000);
        Clock_Delay1ms(3000);

    }//

    /*Motor_Left(4000,8000);
    Clock_Delay1ms(3000);

    //����160cm
    Motor_Backward(9999,9999);
    Clock_Delay1ms(5000);

    //���Һ���
    Motor_Backward(8000,5000);
    Clock_Delay1ms(3000);*/

    /*Motor_Stop();

}*/






