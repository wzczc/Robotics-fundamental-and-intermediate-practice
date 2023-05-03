#ifndef DSS1302_H_
#define DSS1302_H_
 
#include "msp.h"
#include "oled.h"
#include "UART0.h"
#include "Clock.h"

//鏃堕棿缁撴瀯浣�
typedef struct
{   
    unsigned char year;    //00-99,鍓嶉潰鑷繁鍔犲叆20锛屾瘮濡傝鍑�13涓�2013
    unsigned char month;
    unsigned char date;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned char week;
}Time;

extern Time time;    //鏃ュ巻缁撴瀯浣�

#define delay_time              5
//DS1302鍦板潃瀹氫箟
#define ds1302_sec_add          0x80        //绉掓暟鎹湴鍧�
#define ds1302_min_add          0x82        //鍒嗘暟鎹湴鍧�
#define ds1302_hr_add           0x84        //鏃舵暟鎹湴鍧�
#define ds1302_date_add         0x86        //鏃ユ暟鎹湴鍧�
#define ds1302_month_add        0x88        //鏈堟暟鎹湴鍧�
#define ds1302_day_add          0x8a        //鏄熸湡鏁版嵁鍦板潃
#define ds1302_year_add          0x8c        //骞存暟鎹湴鍧�
#define ds1302_control_add      0x8e        //鎺у埗鏁版嵁鍦板潃
#define ds1302_charger_add      0x90
#define ds1302_clkburst_add     0xbe
 
//SCLK:P8.5
#define        SCLK_DIR        (P8DIR)
#define        SCLK_OUT        (P8OUT)
#define        SCLK_REN        (P8REN)
#define        SCLK_PIN        (BIT5)
 
#define        SCLK_UP           (Set_Bit(SCLK_REN,SCLK_PIN))
#define        SCLK_DirOut       (Set_Bit(SCLK_DIR,SCLK_PIN))
#define        SCLK_H            (Set_Bit(SCLK_OUT,SCLK_PIN))
#define        SCLK_L            (Clr_Bit(SCLK_OUT,SCLK_PIN))
 
 
//DS_SDA:P8.6
#define        IO_DIR            (P8DIR)
#define        IO_OUT            (P8OUT)
#define        IO_IN             (P8IN)
#define        IO_REN            (P8REN)
#define        IO_PIN            (BIT6)
 
#define        IO_UP            (Set_Bit(IO_REN,IO_PIN))
#define        IO_DirOut        (Set_Bit(IO_DIR,IO_PIN))
#define        IO_H             (Set_Bit(IO_OUT,IO_PIN))
#define        IO_L             (Clr_Bit(IO_OUT,IO_PIN))
 
 
#define        IO_DirIn        (Clr_Bit(IO_DIR,IO_PIN))
#define        IO_Data         (Get_Bit(IO_IN,IO_PIN))
 
//RST:P8.7
#define        RST_DIR            (P8DIR)
#define        RST_OUT            (P8OUT)
#define        RST_IN             (P8IN)
#define        RST_REN            (P8REN)
#define        RST_PIN            (BIT7)
 
#define        RST_UP            (Set_Bit(RST_REN,RST_PIN))
#define        RST_DirOut        (Set_Bit(RST_DIR,RST_PIN))
#define        RST_H              (Set_Bit(RST_OUT,RST_PIN))
#define        RST_L            (Clr_Bit(RST_OUT,RST_PIN))
 
 
void DS1302_Init(void);
void ds1302_write_byte(unsigned char addr, unsigned char data);
unsigned char ds1302_read_byte(unsigned char addr);
void ds1302_write_time(unsigned char year,unsigned char month,unsigned char day,unsigned char hour,unsigned char min,unsigned char sec,unsigned char week);
void ds1302_read_time(void);
void ds1302_sendtime_uart(void);
 
#endif
