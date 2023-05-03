#include "dss1302.h"

Time time; 

void DS1302_Init(void)
{
    SCLK_DirOut;
    RST_DirOut;
    IO_DirOut;
    SCLK_L;
    RST_L;
    Clock_Delay1us(10);
    SCLK_H;
}


//鍚慏S1302鍐欏叆涓�瀛楄妭鏁版嵁
void ds1302_write_byte(unsigned char addr, unsigned char data)
{
    unsigned char i;
    IO_DirOut;
    RST_H;                    //鍚姩DS1302鎬荤嚎
    //鍐欏叆鐩爣鍦板潃锛歛ddr
    addr = addr & 0xFE;   //鏈�浣庝綅缃浂锛屽瘎瀛樺櫒0浣嶄负0鏃跺啓锛屼负1鏃惰
    for (i = 0; i < 8; i ++) {
        if (addr & 0x01) {
            IO_H;
            }
        else {
            IO_L;
            }
        SCLK_H;      //浜х敓鏃堕挓
        Clock_Delay1us(delay_time);
        SCLK_L;
        Clock_Delay1us(delay_time);
        addr = addr >> 1;
        }
    //鍐欏叆鏁版嵁锛歞
    for (i = 0; i < 8; i ++) {
        if (data & 0x01) {
            IO_H;
            }
        else {
            IO_L;
            }
        SCLK_H;      //浜х敓鏃堕挓
        Clock_Delay1us(delay_time);
        SCLK_L;
        Clock_Delay1us(delay_time);
        data = data >> 1;
        }
    RST_L;        //鍋滄DS1302鎬荤嚎
}

//浠嶥S1302璇诲嚭涓�瀛楄妭鏁版嵁
unsigned char ds1302_read_byte(unsigned char addr)
{

    unsigned char i,temp;
    RST_H;                    //鍚姩DS1302鎬荤嚎
    //鍐欏叆鐩爣鍦板潃锛歛ddr
    addr = addr | 0x01;    //鏈�浣庝綅缃珮锛屽瘎瀛樺櫒0浣嶄负0鏃跺啓锛屼负1鏃惰
    IO_DirOut;
    for (i = 0; i < 8; i ++)
    {
        if (addr & 0x01)
        {
            IO_H;
        }
        else
        {
            IO_L;
        }
        SCLK_H;      //浜х敓鏃堕挓
        Clock_Delay1us(delay_time);
        SCLK_L;
        Clock_Delay1us(delay_time);
        addr = addr >> 1;
    }
    //杈撳嚭鏁版嵁锛歵emp
    IO_DirIn;
    for (i = 0; i < 8; i ++)
    {
        temp = temp >> 1;
        if (IO_Data)
//        if( P2IN & BIT4)
        {
            temp |= 0x80;
        }
        else
        {
            temp &= 0x7F;
        }
        SCLK_H;      //浜х敓鏃堕挓
        Clock_Delay1us(delay_time);
        SCLK_L;
        Clock_Delay1us(delay_time);
        }
    RST_L;                    //鍋滄DS1302鎬荤嚎
    return temp;
}

//鍚慏S302鍐欏叆鏃堕挓鏁版嵁
void ds1302_write_time(unsigned char year,unsigned char month,unsigned char day,unsigned char hour,unsigned char min,unsigned char sec,unsigned char week)
{
    unsigned char temp=0;
    ds1302_write_byte(ds1302_control_add,0x00);            //鍏抽棴鍐欎繚鎶�
    ds1302_write_byte(ds1302_sec_add,0x80);                //鏆傚仠鏃堕挓
    //ds1302_write_byte(ds1302_charger_add,0xa9);        //娑撴祦鍏呯數

    temp=(year/10<<4)|(year%10&0x0F);
    ds1302_write_byte(ds1302_year_add,119);        //骞�

    temp=(month/10<<4)|(month%10&0x0F);
    ds1302_write_byte(ds1302_month_add,temp);    //鏈�

    temp=(day/10<<4)|(day%10&0x0F);
    ds1302_write_byte(ds1302_date_add,temp);        //鏃�

    temp=((hour/10<<4)|(hour%10&0x0F))&0x3F;
    ds1302_write_byte(ds1302_hr_add,temp);        //鏃�

    temp=(min/10<<4)|(min%10&0x0F);
    ds1302_write_byte(ds1302_min_add,temp);        //鍒�

    temp=(sec/10<<4)|(sec%10&0x0F);
    temp=temp&0x7f;
    ds1302_write_byte(ds1302_sec_add,temp);        //绉�
    
    temp=week;
    ds1302_write_byte(ds1302_day_add,temp);        //鍛�

    ds1302_write_byte(ds1302_control_add,0x80);            //鎵撳紑鍐欎繚鎶�
}

//浠嶥S302璇诲嚭鏃堕挓鏁版嵁
void ds1302_read_time(void)
{
    unsigned char temp=0;

    temp=ds1302_read_byte(ds1302_year_add);//骞�
    time.year=(temp>>4)*10+temp&0x0F; //骞�

    temp=ds1302_read_byte(ds1302_month_add);//鏈�
    time.month=(temp>>4)*10+temp&0x0F;    //鏈�

    temp=ds1302_read_byte(ds1302_date_add);//鏃�
    time.date=((temp&0x70)>>4)*10 + (temp&0x0F);  //鏃�

    temp=ds1302_read_byte(ds1302_hr_add);//鏃�
    time.hour=((temp&0x70)>>4)*10 + (temp&0x0F);

    temp=ds1302_read_byte(ds1302_min_add);    //鍒�
    time.min=((temp&0x70)>>4)*10 + (temp&0x0F);    //鍒�

    temp=(ds1302_read_byte(ds1302_sec_add))&0x7f;//绉掞紝灞忚斀绉掔殑绗�7浣嶏紝閬垮厤瓒呭嚭59,姝や綅鎺у埗鏃堕挓鏄惁寮�鍚�
    time.sec=((temp&0x70)>>4)*10 + (temp&0x0F);//绉掞紝灞忚斀绉掔殑绗�7浣嶏紝閬垮厤瓒呭嚭59
    
    temp=ds1302_read_byte(ds1302_day_add); //鍛�
    time.week=(temp>>4)*10+temp&0x0F; //鍛�
}
 
void ds1302_sendtime_uart(void)
{

    UART0_OutChar(time.year/10+'0');
    UART0_OutChar(time.year%10+'0');
    UART0_OutChar('-');
    UART0_OutChar(time.month/10+'0');
    UART0_OutChar(time.month%10+'0');
    UART0_OutChar('-');
    UART0_OutChar(time.date/10+'0');
    UART0_OutChar(time.date%10+'0');
    UART0_OutChar(' ');

    UART0_OutChar(time.hour/10+'0');
    UART0_OutChar(time.hour%10+'0');
    UART0_OutChar(':');
    UART0_OutChar(time.min/10+'0');
    UART0_OutChar(time.min%10+'0');
    UART0_OutChar(':');
    UART0_OutChar(time.sec/10+'0');
    UART0_OutChar(time.sec%10+'0');

    UART0_OutChar(' ');
    UART0_OutChar(time.week/10+'0');
    UART0_OutChar(time.week%10+'0');

    UART0_OutChar('\n');
}
