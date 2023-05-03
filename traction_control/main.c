#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include "Motor.h"
#include "PWM.h"
#include "UART0.h"
#include "Reflectance.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

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

static int count = 0;
static int type = 1;

void PORT1_IRQHandler(void);

void main()
{
    uint8_t Data;
    volatile uint32_t i;

    MAP_WDT_A_holdTimer();

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // ��������Ϊ���ģʽ

    /*GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);//����Ϊ�͵�ƽ
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);*/

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);  // ��������Ϊ��������ģʽ
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);

    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);  //�ж�������ѡ��
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4,GPIO_LOW_TO_HIGH_TRANSITION);

    GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN1);  // ����жϱ�־
    GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN4);

    GPIO_registerInterrupt(GPIO_PORT_P1,PORT1_IRQHandler);  // ע���жϺ���

    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);   // �ж�ʹ��
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    Clock_Init48MHz();
    UART0_Initprintf();

    while (count==0)
    {
        for(i=0;i<2200000;i++);
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

    Reflectance_Init();
    Motor_Init();

    while(type==1)
    {

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

        uint16_t a=7000;
        uint16_t b=7000;



        Data = Reflectance_Read(500);
        printf("%d\n",Data);
        switch(Data)
        {
            //��·��
            case 0x08:
                Motor_Stop();
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
                break;

            //ֱ��
            case 0x18:
                Motor_Forward(a,a);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                break;

            //��ת
            case 0xF8:
                Motor_Forward(a,a);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);

            //��ת
            case 0x1E://0x1F
                Motor_Forward(a,a);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

            //ʮ��·��
            case 0xFE://0xFF
                Motor_Forward(a,a);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

            //�յ�
            case 0xDA://0xDB
                Motor_Stop();
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

            default:
                Motor_Stop();
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                break;
        }


    }

    while(type!=1)
    {
        uint16_t a=7000;
        uint16_t b=7000;

        Data = Reflectance_Read(500);
        printf("%d\n",Data);
        switch(Data)
        {
            //��·��
            case 0xFE://FF
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
                break;

            //ֱ��
            case 0xE6://E7
                Motor_Forward(a,a);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                break;

            //��ת
            case 0x06://0x07
                Motor_Left(a,a);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);

            //��ת
            case 0xE0:
                Motor_Right(a,a);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

            //ʮ��·��
            case 0x00:
                Motor_Forward(a,a);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

            //�յ�
            case 0x24:
                Motor_Stop();
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

            default:
                Motor_Stop();
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                break;
        }


    }
}

void PORT1_IRQHandler(void)    // �жϺ���
{
    int i = 0;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN1)==GPIO_PIN1)    // �ж��жϿ�
    {
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
        for(i=0;i<100;i++);//����
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0){
            count++;
            if(count%2==1){
                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                /*GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);*/
            } else {
                while (1)
                {
                    for(i=0;i<2200000;i++);
                    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
                }
            }
        }
    }

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN1)==GPIO_PIN4)    // �ж��жϿ�
    {
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
        for(i=0;i<100;i++);//����
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==1){
            type++;
        }
    }
}
