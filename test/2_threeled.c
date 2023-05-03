/*#include "msp.h"
#include "../inc/Clock.h"
#include <stdio.h>

#define RED       0x01  //R
#define GREEN     0x02  //G
#define BLUE      0x04  //B
#define yellow    0x03  //RG-
#define sky blue  0x06  //-GB
#define white     0x07  //RGB
#define pink      0x05  //R-B

void Port2_Init(void){
  P2->SEL0 = 0x00;
  P2->SEL1 = 0x00; //configure P2.2-P2.0 as GPIO
  P2->DS = 0x07;   // make P2.2-P2.0 high drive strength
  P2->DIR = 0x07;  // make P2.2-P2.0 out
  P2->OUT = 0x00;  // all LEDs off
}

void Port2_Output(uint8_t data){
  P2->OUT = data; // write all of P2 outputs
}

/*void main(void)
{
    Clock_Init48MHz(); //Initialize clock to 48 MHz
    Port2_Init(); // initialize P2.2-P2.0 and make them outputs (P2.2-P2.0 built-in LEDs)

    while(1){
        Port2_Output(RED);
        Clock_Delay1ms(1000);  //Delay about 1000 ms

        Port2_Output(yellow);
        Clock_Delay1ms(1000);  //Delay about 1000 ms

        Port2_Output(white);
        Clock_Delay1ms(1000);  //Delay about 1000 ms

        Port2_Output(GREEN);
        Clock_Delay1ms(1000);  //Delay about 1000 ms
        Port2_Output(BLUE);
        Clock_Delay1ms(1000);  //Delay about 1000 ms
    }
}*/
