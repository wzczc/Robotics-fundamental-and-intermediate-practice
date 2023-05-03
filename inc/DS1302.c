#include "DS1302.h"

#define DS_SCL		BIT5    /* DS_SCL = P8.5 */
#define DS_SDA		BIT6    /* DS_SDA = P8.6 */
#define DS_RST		BIT7    /* DS_RST = P8.7 */
#define DS_RST_IN	P8DIR	&= ~DS_RST
#define DS_RST_OUT	P8DIR	|= DS_RST
#define DS_RST0		P8OUT	&= ~DS_RST
#define DS_RST1		P8OUT	|= DS_RST
#define DS_SCL_IN	P8DIR	&= ~DS_SCL
#define DS_SCL_OUT	P8DIR	|= DS_SCL
#define DS_SCL0		P8OUT	&= ~DS_SCL
#define DS_SCL1		P8OUT	|= DS_SCL
#define DS_SDA_IN	P8DIR	&= ~DS_SDA
#define DS_SDA_OUT	P8DIR	|= DS_SDA
#define DS_SDA0		P8OUT	&= ~DS_SDA
#define DS_SDA1		P8OUT	|= DS_SDA
#define DS_SDA_BIT	(P8IN & DS_SDA)

/* DS1302��ַ���� */
#define ds1302_sec_add		0x80    /* �����ݵ�ַ */
#define ds1302_min_add		0x82    /* �����ݵ�ַ */
#define ds1302_hr_add		0x84    /* ʱ���ݵ�ַ */
#define ds1302_date_add		0x86    /* �����ݵ�ַ */
#define ds1302_month_add	0x88    /* �����ݵ�ַ */
#define ds1302_day_add		0x8a    /* �������ݵ�ַ */
#define ds1302_year_add		0x8c    /* �����ݵ�ַ */
#define ds1302_control_add	0x8e    /* �������ݵ�ַ */
#define ds1302_charger_add	0x90
#define ds1302_clkburst_add	0xbe

#define Ds1302_sec_add      0x81    /* �����ݵ�ַ */
#define Ds1302_min_add      0x83    /* �����ݵ�ַ */
#define Ds1302_hr_add       0x85    /* ʱ���ݵ�ַ */
#define Ds1302_date_add     0x87    /* �����ݵ�ַ */
#define Ds1302_month_add    0x89    /* �����ݵ�ַ */
#define Ds1302_day_add      0x8b    /* �������ݵ�ַ */
#define Ds1302_year_add     0x8d    /* �����ݵ�ַ */
#define Ds1302_control_add  0x8f    /* �������ݵ�ַ */
#define Ds1302_charger_add  0x91
// #define ds1302_clkburst_add 0xbe

/* ��DS1302д��һ���ֽ����� */
void W_Data(unsigned char addr, unsigned char data)
{
    unsigned char i;
    DS_RST_OUT;
    DS_SCL_OUT;
    DS_SDA_OUT;

    DS_RST0; /* ֹͣDS1302���� */
    DS_SCL0; /* ����ʱ���� */
    Clock_Delay1us(10);
    DS_RST1; /* ����DS1302���� */
    addr = addr & 0xFE; /* ���λ���㣬д���� */
    DS_SDA_OUT;
    for (i = 0; i < 8; i++) /* д��ַ */
    {
        DS_SCL0;
        if (addr & 0x01)
            DS_SDA1;
        else
            DS_SDA0;

        addr = addr >> 1;
        DS_SCL1; /* ����ʱ����������������ݵ�DS1302 */
        Clock_Delay1us(9);
    }
    for (i = 0; i < 8; i++) /* д���� */
    {
        DS_SCL0;
        if (data & 0x01)
            DS_SDA1;
        else
            DS_SDA0;
        data = data >> 1;
        DS_SCL1; /* ����ʱ����������������ݵ�DS1302 */
        Clock_Delay1us(9);
    }
    DS_RST0; /* ֹͣDS1302���� */
}

/* ��DS1302����һ���ֽ����� */
unsigned char R_Data(unsigned char addr)
{
    unsigned char i, temp;
    DS_RST_OUT;
    DS_SCL_OUT;
    DS_SDA_OUT;

    DS_RST0; /* ֹͣDS1302���� */
    DS_SCL0; /* ����ʱ���� */
    Clock_Delay1us(10);
    DS_RST1; /* ����DS1302���� */
    addr = addr | 0x01; /* ���λ�øߣ������� */
    DS_SDA_OUT;
    for (i = 0; i < 8; i++) /* д��ַ */
    {
        DS_SCL0;
        if (addr & 0x01)
            DS_SDA1;
        else
            DS_SDA0;
        addr = addr >> 1;
        DS_SCL1; /* ����ʱ����������������ݵ�DS1302 */
        Clock_Delay1us(9);
    }
    DS_SDA_IN;
    for (i = 0; i < 8; i++) /* ������ */
    {
        if ( DS_SDA_BIT)
            temp |= 0x80;
        else
            temp &= 0x7F;
        DS_SCL1; /* ����ʱ����������������ݵ�DS1302 */
        temp = temp >> 1;
        DS_SCL0;
        Clock_Delay1us(9);
    }
    DS_RST0; /* ֹͣDS1302���� */
    return (temp);
}

/*******************************************
 * �������ƣ�Set_DS1302
 * ��    �ܣ�����DS1302�ڲ���ʱ��
 * ��    ����ptr--ָ�������ݵ�ַ��ָ��
 * ����ֵ  ����
 * ˵��    ��д�����ݵĸ�ʽ��
 *          �� �� ʱ �� �� ���� ��  ����7���ֽڡ�
 ********************************************/
void Set_DS1302(unsigned char *ptr)
{
    unsigned char i;
    unsigned char addr = 0x80;

    W_Data(0x8e, 0x00); /* ����д�� */

    for (i = 7; i > 0; i--)
    {
        W_Data(addr, *ptr++);
        addr += 2;
    }
    W_Data(0x8e, 0x80); /* ��ֹ */
}

/* ��DS302����ʱ������ */
void DS1302_Read_Time(unsigned char *ptr)
{
    ptr[1] = R_Data( ds1302_year_add); /* �� */
    ptr[2] = R_Data( ds1302_month_add); /* �� */
    ptr[3] = R_Data( ds1302_date_add); /* �� */
    ptr[4] = R_Data( ds1302_hr_add); /* ʱ */
    ptr[5] = R_Data( ds1302_min_add); /* �� */
    ptr[6] = (R_Data( ds1302_sec_add)) & 0x7f; /* �룬������ĵ�7λ�����ⳬ��59 */
    ptr[7] = R_Data( ds1302_day_add); /* �� */
}

/* ��ȡʱ�� */
void DS1302_read_time(struct ds1302time *p)
{
    /* ������ ʱ���� ���� */
    unsigned char time_buf[8] =
            { 0x20, 0x20, 0x04, 0x28, 0x21, 0x16, 0x50, 0x04 };

    DS1302_Read_Time(time_buf); /* ��ȡ */

    p->year = 2000 + (time_buf[1] >> 4) * 10 + (time_buf[1] & 0x0f);
    p->month = (time_buf[2] >> 4) * 10 + (time_buf[2] & 0x0f);
    p->day = (time_buf[3] >> 4) * 10 + (time_buf[3] & 0x0f);
    p->hour = (time_buf[4] >> 4) * 10 + (time_buf[4] & 0x0f);
    p->minute = (time_buf[5] >> 4) * 10 + (time_buf[5] & 0x0f);
    p->second = (time_buf[6] >> 4) * 10 + (time_buf[6] & 0x0f);
    p->week = (time_buf[7] >> 4) * 10 + (time_buf[7] & 0x0f);
}

/* д��ʱ�� */
void DS1302_write_time(struct ds1302time *p)
{
    /* ������ ʱ���� ���� */
    unsigned char time_buf[8] =
            { 0x20, 0x20, 0x03, 0x03, 0x23, 0x59, 0x55, 0x02 };

    time_buf[1] = p->year % 100 / 10 * 16 + p->year % 100 % 10;
    time_buf[2] = p->month % 100 / 10 * 16 + p->month % 100 % 10;
    time_buf[3] = p->day % 100 / 10 * 16 + p->day % 100 % 10;
    time_buf[4] = p->hour % 100 / 10 * 16 + p->hour % 100 % 10;
    time_buf[5] = p->minute % 100 / 10 * 16 + p->minute % 100 % 10;
    time_buf[6] = p->second % 100 / 10 * 16 + p->second % 100 % 10;
    time_buf[7] = p->week % 100 / 10 * 16 + p->week % 100 % 10;

    W_Data( ds1302_control_add, 0x00); /* �ر�д���� */
    W_Data( ds1302_sec_add, 0x80); /* ��ͣʱ��  */
    /* DS1302_Write_Byte(ds1302_charger_add, 0xA9);      //������ */
    W_Data( ds1302_year_add, time_buf[1]); /* �� */
    W_Data( ds1302_month_add, time_buf[2]); /* �� */
    W_Data( ds1302_date_add, time_buf[3]); /* �� */
    W_Data( ds1302_hr_add, time_buf[4]); /* ʱ */
    W_Data( ds1302_min_add, time_buf[5]); /* �� */
    W_Data( ds1302_sec_add, time_buf[6]); /* �� */
    W_Data( ds1302_day_add, time_buf[7]); /* �� */
    W_Data( ds1302_control_add, 0x80); /* ��д���� */
}

/*
 * �������ջ�ȡ���� https://www.cnblogs.com/fengbohello/p/3264300.html
 * ����1��7 ��������һ��������
 */
int getWeekdayByYearday(int iY, int iM, int iD)
{
    int iWeekDay = -1;
    if (1 == iM || 2 == iM)
    {
        iM += 12;
        iY--;
    }
    iWeekDay = (iD + 1 + 2 * iM + 3 * (iM + 1) / 5 + iY + iY / 4 - iY / 100
            + iY / 400) % 7;
    if (iWeekDay == 0)
        iWeekDay = 7;
    return (iWeekDay);
}

