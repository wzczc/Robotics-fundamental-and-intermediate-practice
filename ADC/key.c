#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "key.h"

//�����궨��
#define rt BIT2
#define ensure BIT5
#define add BIT6
#define home BIT0
#define set BIT4
#define choose BIT7
//���ð���
#define spare BIT3


//���ڵ�
unsigned char Key_KeyNumber;
//�����а�����ʼ������������ģʽ
void key_init(void)
{
    //���ؼ�
    P4SEL0 &= ~rt;
    P4SEL1 &= ~rt;
    P4DIR &= ~rt;
    P4REN |= rt;
    P4OUT |= rt;
    //ȷ����
    P4SEL0 &= ~ensure;
    P4SEL1 &= ~ensure;
    P4DIR &= ~ensure;
    P4REN |= ensure;
    P4OUT |= ensure;
    //"+"�ż�
//    P4SEL0 &= ~add;
//    P4SEL1 &= ~add;
//    P4DIR &= ~add;
//    P4REN |= add;
//    P4OUT |= add;
    //��ҳ��
    P4SEL0 &= ~home;
    P4SEL1 &= ~home;
    P4DIR &= ~home;
    P4REN |= home;
    P4OUT |= home;
    //���ü�
    P4SEL0 &= ~set;
    P4SEL1 &= ~set;
    P4DIR &= ~set;
    P4REN |= set;
    P4OUT |= set;
    //ѡ���
//    P4SEL0 &= ~choose;
//    P4SEL1 &= ~choose;
//    P4DIR &= ~choose;
//    P4REN |= choose;
//    P4OUT |= choose;
    //���ü�
    P4SEL0 &= ~spare;
    P4SEL1 &= ~spare;
    P4DIR &= ~spare;
    P4REN |= spare;
    P4OUT |= spare;
}

//��ȡ�����ķ���ֵ
unsigned char Key_GetState(void)
{
    unsigned char keynum = 0;
    if((P4IN & rt) == 0)
    {
        keynum = 1;
    }
    if((P4IN & ensure) == 0)
    {
        keynum = 2;
    }
//    if((P4IN & add) == 0)
//    {
//        keynum = 3;
//    }
    if((P4IN & home) == 0)
    {
        keynum = 4;
    }
    if((P4IN & set) == 0)
    {
        keynum = 5;
    }
//    if((P4IN & choose) == 0)
//    {
//        keynum = 6;
//    }
    if((P4IN & spare) == 0)
    {
        keynum = 7;
    }
    return keynum;
}
//����ɨ�裬�жϰ����Ƿ���
void Key_Loop(void)
{
    static unsigned char NowState,LastState;
    LastState = NowState;
    NowState = Key_GetState();
    if(LastState == 1 && NowState == 0)
    {
        Key_KeyNumber = 1;
    }
    if(LastState == 2 && NowState == 0)
    {
        Key_KeyNumber = 2;
    }
    if(LastState == 3 && NowState == 0)
    {
        Key_KeyNumber = 3;
    }
    if(LastState == 4 && NowState == 0)
    {
        Key_KeyNumber = 4;
    }
    if(LastState == 5 && NowState == 0)
    {
        Key_KeyNumber = 5;
    }
    if(LastState == 6 && NowState == 0)
    {
        Key_KeyNumber = 6;
    }
    if(LastState == 7 && NowState == 0)
    {
        Key_KeyNumber = 7;
    }
}
//�������ķ���ֵ���ظ�������
unsigned char Key(void)
{
    unsigned char temp;
    temp = Key_KeyNumber;
    Key_KeyNumber = 0;
    return temp;
}

//ͨ��״̬��ʵ�ְ����ĵ�����˫������������
//1:����
//2��˫��
//3������
//������Ϊʲô���У�ֱ�Ӽ���Ƿ��������ְ�
//void key_loop(void)
//{
//    static unsigned char s2_NowState = 0,s2_LastState = 0,s1_NowState = 0,s1_LastState = 0;
//    s2_LastState = s2_NowState;
//    s1_LastState = s1_NowState;
//    s2_NowState = get_s2num();
//    s1_NowState = get_s1num();
//    if(s2_NowState == 1 && s2_LastState == 1)
//    {
//        s2_key_time++;
//    }
//    else if(s2_key_time > 50)
//    {
//        s2_state = 2;
//        s2_key_time = 0;
//    }
//    else if(0 < s2_key_time && s2_key_time < 25)
//    {
//        s2_state = 1;
//        s2_key_time = 0;
//    }
//    if(s1_NowState == 1 && s1_LastState == 1)
//    {
//        s1_key_time++;
//    }
//    else if(s1_key_time > 50)
//    {
//        s1_state = 2;
//        s1_key_time = 0;
//    }
//    else if(0 < s1_key_time && s1_key_time < 25)
//    {
//        s1_state = 1;
//        s1_key_time = 0;
//    }


