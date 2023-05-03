# Robotics-fundamental-and-intermediate-practice
code for "机器人基础和中级实践"，基于MSP432和CCS IDE
## 机器人基础实践
使用红外传感器的巡线小车
* 硬件：TI小车套件
* 代码：**test** 文件夹里的 **findline.c**
* 效果：


https://user-images.githubusercontent.com/94534877/235879394-53ab9de8-3820-4cbd-999d-eadaf7bc0232.mp4



https://user-images.githubusercontent.com/94534877/235879480-4bb66ae7-b71a-4a77-817b-5175026026f6.mp4



https://user-images.githubusercontent.com/94534877/235879492-3d3ced76-a89e-4f37-8aa6-1b0c503a6cc5.mp4

## 机器人中级实践
电子时钟系统
* 硬件：OLED、DS1302、MSP432等
* 代码：**Serial_Test** 里的 **main.c**
* 功能：  
![image](https://user-images.githubusercontent.com/94534877/235887016-daedf0f9-31d2-4904-82ed-e2670ff4e157.png)
  * **时钟功能**：在OLED显示屏上显示出当前时间，格式为----XX : XX : XX；12/24小时可通过按键切换。
  * **日历功能**：在OLED显示屏上显示出当前日期，格式为----XXXX年 XX月XX日 周X；闰年闰月自动校准。
  ![image](https://user-images.githubusercontent.com/94534877/235886176-25fad2b2-6d5c-49b8-9fea-410cac864221.png)

  * **世界时钟功能**：在OLED屏显示出当前时区地点，有北京时间、华盛顿时间和伦敦时间。  
  ![image](https://user-images.githubusercontent.com/94534877/235886243-03a54cd2-e502-4756-bb19-af7957d20b79.png)

  * **闹钟功能**：在OLED显示屏显示出闹铃时间，格式为----闹铃：周X  XX时XX分；时间到达设定的闹铃时间后，闹钟响铃一分钟，期间可以通过按键来取消铃响。  
  ![image](https://user-images.githubusercontent.com/94534877/235886310-0b667ca0-617d-4307-a6ef-55dba69a6796.png)

  * **秒表功能**：在OLED显示屏上显示出时间计次，格式为----XX : XX : XX，分辨率为0.01s；通过按键控制时间计次，共可计次4次。  
  ![image](https://user-images.githubusercontent.com/94534877/235886356-fc3b112c-9f89-405e-bf83-04292b7aeebc.png)

  * **计时器功能**：在OLED显示屏上显示出倒计时的时间，格式为----XX : XX : XX，分辨率为0.01s；通过按键设置倒计时时间和控制倒计时的开始，并在倒计时结束时响铃5s。  
  ![image](https://user-images.githubusercontent.com/94534877/235886403-4f684eab-0f95-4fe8-af24-b34089250842.png)

  * **串口时间校准功能**：通过电脑终端发送时间自动校准显示屏上的时间；通过电脑终端发送日期自动校准显示屏上的日期。  
  ![image](https://user-images.githubusercontent.com/94534877/235886502-c20f5d7b-7277-41e7-9371-8b12b02e8d9e.png)![image](https://user-images.githubusercontent.com/94534877/235886518-13ef20d8-9db6-46ec-b573-e017978f8a90.png)

