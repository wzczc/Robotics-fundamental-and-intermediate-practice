/*#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include "Motor.h"
#include "PWM.h"

void main()
{
    Clock_Init48MHz();
    Motor_Init();

    Motor_Stop();
    Clock_Delay1ms(2000);

    int i=0;
    for(i=0;i<=1;i++)
    {

        Motor_Backward(9999,9999);
        Clock_Delay1ms(6250);


        Motor_Left(4000,4000);
        Clock_Delay1ms(1500);


        Motor_Forward(9999,9999);
        Clock_Delay1ms(1250);


        Motor_Right(8000,8000);
        Clock_Delay1ms(7750);


        Motor_Forward(9999,9999);
        Clock_Delay1ms(6250);


        Motor_Forward(4000,8000);
        Clock_Delay1ms(3000);

    }


    Motor_Stop();

}*/


#include "msp.h"

#include "Clock.h"
#include "Motor.h"
#include "PWM.h"
#include "UART0.h"
#include "Reflectance.h"
#include <stdio.h>

/**
 * main.c
 */


#define RED       0x01  //R
#define GREEN     0x02  //G
#define BLUE      0x04  //B
#define yellow    0x03  //RG-
#define sky blue  0x06  //-GB
#define white     0x07  //RGB
#define pink      0x05  //R-B

void Port1_Init(void){
  P1->SEL0 = 0x00;
  P1->SEL1 = 0x00;
                   // GPIO
  P1->DIR = 0x01;
  P1->REN = 0x12;
  P1->OUT = 0x13;
}

uint8_t Port1_Input(void){
    return (P1->IN&0x12);

}

void Port1_Output(uint8_t data1){
    P1->OUT = (P1->OUT&0xFE)|data1;
}


void Port2_Init(void){
  P2->SEL0 = 0x00;
  P2->SEL1 = 0x00; //configure P2.2-P2.0 as GPIO
  P2->DS = 0x07;   // make P2.2-P2.0 high drive strength
  P2->DIR = 0x07;  // make P2.2-P2.0 out
  P2->OUT = 0x00;  // all LEDs off
}
int x=1;
int i=1;
int ii=1;
int y=0;
int L=5000;
int R=5000;

void Port2_Output(uint8_t data){
  P2->OUT = data; // write all of P2 outputs
}
void main(void)
{
    Reflectance_Init();
    Clock_Init48MHz();
    UART0_Init();
    uint8_t status;
    uint8_t Data;
    Motor_Init();
    UART0_Initprintf();
    Port2_Init();
    Port1_Init();

    while(1){
        Motor_Forward(5800,6000);
        /*status = Port1_Input();
        switch(status){
        case 0x12:
            if(i==2){
                break;
            }
            x++;

            Motor_Stop();
            Port2_Output(0x00);
            if(x>200000 && x<400000){
                Port1_Output(0x00);
            }
            else if(x>400000){
                Port1_Output(0x01);
                x=1;
            }
            break;
        case 0x10:
            if(ii==1){
                i=2;
                ii=2;
                y=1;
                Port1_Output(0x00);
                while(1){
                    status = Port1_Input();
                    if(status==0x12){
                        break;
                    }
                }
                break;
            }
            else if(ii==2){
                x=1;
                i=1;
                ii=1;
                y=0;
                while(1){
                    status = Port1_Input();
                    if(status==0x12){
                        break;
                    }
                }
                break;
            }
        }
        if(y==1){
            Data=Reflectance_Read(500);
            Motor_Stop();
            switch(Data){
            case 0x00:
                Port2_Output(white);
                break;
            case 0x18://Ö±ÐÐ
                Port2_Output(GREEN);
                Motor_Forward(L,R);
                break;
            case 0x10:
            case 0x30:
                Port2_Output(GREEN);
                Motor_Forward(L-4000,R-1000);
                break;
            case 0x8:
            case 0xC:
                Port2_Output(GREEN);
                Motor_Forward(L-1000,R-4000);
                break;
            case 0xF8:
            case 0xFC:
                Port2_Output(BLUE);
                Motor_Left(L,R);
                break;
            case 0x3E:
            case 0x1E:
                Port2_Output(RED);
                Motor_Right(L,R);
                break;
            case 0xDA:
            case 0xBA:
            case 0xB6:
            case 0xEC:
            case 0x6C:
                Port2_Output(0x00);
                Motor_Stop();
                break;
            case 0x7E:
            case 0xFE:
                Port2_Output(yellow);
                Motor_Left(L,R);
                break;

            }

            status = Port1_Input();
            if(status==0x10){
                i=1;
                y=0;


            }

        }*/

    }


}




