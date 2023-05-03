#include "msp.h"
#include "Clock.h"
#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//记录次数
static int count=0;
static int num=0;


//中断函数声明
void PORT1_IRQHandler(void);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // 关闭看门狗

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // 设置引脚为输出模式
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);//设置为低电平
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);  // 设置引脚为上拉输入模式
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);

    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);  //中断跳变沿选择
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);

    GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN1);  // 清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN4);

    GPIO_registerInterrupt(GPIO_PORT_P1,PORT1_IRQHandler);  // 注册中断函数

    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);   // 中断使能
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);

    //循环
    while(1)
    {
        ;
    }

}

void PORT1_IRQHandler(void)    // 中断函数
{
    int i = 0;
    int k=0;

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN1)==GPIO_PIN1)    // 判断中断口
    {

        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1); //清除中断标志

        //实现按下S1，LED2以六种不同的颜色交替闪烁，循环3次的效果
        for(k=0;k<3;k++){
        for(i=0;i<100;i++);//防抖
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        for(i=0;i<100000;i++);//防抖
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        for(i=0;i<100000;i++);//防抖
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        for(i=0;i<100000;i++);//防抖
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        for(i=0;i<100000;i++);//防抖
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
        for(i=0;i<100000;i++);//防抖
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
        for(i=0;i<100000;i++);//防抖
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
        }
    }
        /*if(num%2==1){
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)==0){
            count++;
            if(count%6==1){
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
            } else if(count%6==2){
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
            } else if(count%6==3){
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
            } else if(count%6==4){
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
            } else if(count%6==5){
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
            } else {
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
            }
        }
        } else {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }*/
    //}


    /*if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN4)==GPIO_PIN4)    // 判断中断口
    {
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
        for(i=0;i<100;i++);    //消抖
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 1)
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
    }*/

    if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN4)==GPIO_PIN4)    // 判断中断口
    {
        num++;//计数加1
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4); //清除中断标志
        for(i=0;i<100;i++);//防抖

        //实现按下S2可以轮流控制LED1和LED2的效果
        if(num%2==1){
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }else {
            if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0){
                count++;
                if(count%6==1){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                } else if(count%6==2){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                } else if(count%6==3){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
                } else if(count%6==4){
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                } else if(count%6==5){
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
                } else {
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
                    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
                    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
                }
            }
            //长按
        /*int j=0;
         * while(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4)==0){
                j++;
        }
        if(j>200000) {
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
            count=0;
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
            GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
        }*/
        }
    }
}

