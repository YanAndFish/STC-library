# A MCS-51 LIBRARIES OF KEIL C-51

Apply only to STC15xx and STC8xx.
Not recommended apply STC15Fxx.
**If you cannot see the Chinese part correctly, please use GB2321 in the Settings.**

​	这是一个兼容于STC15系列和STC8系列的库文件例程，为确保一些特殊的函数可以正确运行，请采用**24M系统时钟**。

​	库文件包含3部分，主函数代码，配置片内资源的`HAL`库以及配置外设的DRIVER库。`HAL`库可以根据定义直接使用，`DRIVER`库需要根据实际需求进行文件的添加以及头文件的包含、配置。

[TOC]

## core 

###  `core.c`
​	包含主程序`main()`,可不做修改。

### `interrupt.c`

​	包含中断服务以及中断任务栈。为确保多个用户应用程序调用相同的中断服务程序时不发生冲突，库文件添加了中断服务栈。用户程序在使用某中断时，只需在任务栈中添加任务而不需要处理其他用户程序的调用问题。中断任务栈最大数量为5。
​	注意！中断都是某个硬件特有的，因此当一个硬件出现多个中断事件时只会在同一个中断入口中产生，例如串口的中断，无论是接收中断还是发送中断都在同一个中断中产生。用户中断任务需要去判断相应的中断标志位做对应的操作。**但在任务结束后请勿清除中断标志位，这将会导致后续任务被忽略或产生阻塞！**中断标识位将会在所有中断任务结束后由中断任务程序清除。
```c
//函数用途：启动中断任务。
void enableInterrupt();
```

```c
//函数用途：关闭中断任务。
void disableInterrupt();
```

```c
//函数用途：添加中断任务。
//参数：中断结构体指针，任务ID，回调函数名
//返回 成功0 失败-1
int setInterruptTask(interruptTask *intTask,unsigned char taskID,void(*hander)());
```

```c
//函数用途：设置运行的任务ID(单一任务或所有任务)
//参数：中断结构体指针，任务ID
//返回 成功0 失败-1
int setInterruptRunTask(interruptTask *intTask,unsigned char taskID);
```

​	使用这类函数时`taskID`需要注意，**如果出现`taskID`指向空任务的情况**，系统将会跑飞，如果没有配置看门狗系统将因此死机。

### `bitctrl.c`

​	包含快速操作寄存器的函数，例如：

```c
P0 sh setHigh[3];//将P0寄存器的第3位置1
P0 sl setLow[3];//将P0寄存器的第三位置0
```

## HAL

### `HAL_DELAY.c`

​	包含软件延时函数，针对特定的时钟频率进行优化。**当单片机运行的低速模式时，延时程序将会出现较大误差。**

```c
//函数用途：进行毫秒级延时
//参数：延时毫秒
void delay(unsigned int ms);
```
```c
//函数用途：进行微秒级延时
//参数：延时微秒*10
void delay_10us(unsigned int us);
```

​	低于10微秒的延时在C语言中编译误差较大，因此以10微秒为单位进行微秒级延时，参数1即可延时10us。

### `HAL_GPIO.C`

​	包含单片机GPIO的模式配置和操作函数。

|  模式名称  |  参数名   |              备注              |
| :--------: | :-------: | :----------------------------: |
| 准双向模式 |   INOUT   | 可以输入或输出，但供电能力较弱 |
|  输入模式  |   INPUT   |       高阻状态，不能输出       |
| 强推挽输出 |  OUTPUT   |    可以输入和输出，灌电流强    |
|  开漏输出  | OUTPUT_OD |    需要上拉电阻才能正常输出    |

```c
//函数用途：设置引脚模式
//参数：引脚标号，引脚模式
//返回 成功0 失败-1
int pinMode(unsigned char pin,unsigned char mode);
```

```c
//函数用途：设置端口模式
//参数：端口标号，引脚模式
//返回 成功0 失败-1
int pinMode(unsigned char port,unsigned char mode);
```
```c
//函数用途：修改端口状态
//参数：端口标号，数据
void writePort(unsigned char port,unsigned char dat);
```

```c
//函数用途：修改引脚状态
//参数：引脚标号，数据
void writePin(unsigned char pin,unsigned char dat);
```

```c
//函数用途：读取端口状态
//参数：端口标号
//返回：端口状态
unsigned char readPort(unsigned char port);
```

```c
//函数用途：读取引脚状态
//参数：引脚标号
//返回：引脚状态
bit readPin(unsigned char pin);
```

​	以上函数使用时，引脚标号只需要著名数字部分，例如`P1^0`只需要写`10。`同理，端口标号亦是如此。

### `HAL_TIMER.c`

​	包含定时器的配置函数和控制函数。STC15/STC8有5个定时器可供设置。除定时器0和定时器1外，其他定时器不具有模式设置功能，只有模式0，16位自动重载模式。一些关于配置定时器的宏定义如下：

| 定时器  | 宏定义 |
| :-----: | :----: |
| 定时器0 | Timer0 |
| 定时器1 | Timer1 |
| 定时器2 | Timer2 |
| 定时器3 | Timer3 |
| 定时器4 | Timer4 |

|        模式        |        宏定义        |
| :----------------: | :------------------: |
|    16位自动重载    |    Time_16BitAuto    |
|        16位        |      Time_16Bit      |
|    8位自动重载     |    Time_8BitAuto     |
| 16位自动重载无中断 | Time_16BitAutoNoMask |

| 定时器时钟 |  宏定义   |
| :--------: | :-------: |
| 不分频模式 | CLOCK_1T  |
| 12分频模式 | CLOCK_12T |
|  外部输入  | CLOCK_Ext |

|  状态  | 宏定义  |
| :----: | :-----: |
|  置高  |  HIGH   |
|  置低  |   LOW   |
|  使能  | ENABLE  |
| 不使能 | DISABLE |

```c
//函数用途：配置定时器
//参数：定时器名称，模式，中断优先级，中断使能，时钟类型，是否输出，初值，是否运行
void TIMER_config(Time_config *time,unsigned char TimeMode,unsigned char TimePolity,unsigned char TimeInterrupt,unsigned char TimeClkSource,unsigned char TimeClkOut,unsigned int TimeValue,unsigned char TimeRun);
```

```c
//函数用途：上传定时器设置
//参数：定时器名称
//返回 成功0 失败-1
int TIMER_setup(unsigned char timer);
```

```c
//函数用途：添加定时器中断任务
//参数：定时器名称，中断任务函数
//返回 成功0 失败-1
int TIMER_addTask(unsigned char timer,void(*hander)());
```

```c
//函数用途：快速配置定时器
//参数：定时器名称，定时延时时间，中断任务函数
//返回 成功0 失败-1
int TIMER_set(unsigned char timer,unsigned int ms,void(*callback)());
```

​	使用`TIMER_set`快速配置定时器时，定时器将会以1T模式，16位自动重载1ms时间延时，达到设定值后产生运行中断任务。这将会占用一个中断任务栈空间。如果单片机设置为空闲模式，**这将会每1ms唤醒单片机**。设置完成后你需要使用`TIMER_start()`函数使定时器运行。

```c
//函数用途：启动或关闭定时器
//参数：定时器名称
//返回 成功0 失败-1
int TIMER_start(unsigned char timer);
int TIMER_end(unsigned char timer);
```

### `HAL_EXTI.c`

​	包含外部中断的配置函数和控制函数。STC15/STC8具有5个外部中断输入源，其中除了外部中断0和外部中断1外，其余中断均不可设置中断触发模式并固定为下降沿触发。

|    模式    |     宏定义     |
| :--------: | :------------: |
|  边缘触发  | EXTI_RISE_FALL |
| 下降沿触发 |   EXTI_FALL    |


|  状态  | 宏定义  |
| :----: | :-----: |
|  置高  |  HIGH   |
|  置低  |   LOW   |
|  使能  | ENABLE  |
| 不使能 | DISABLE |
```c
//函数用途：配置外部中断
//参数：外部中断名称，模式，中断优先级，中断使能
void EXTI_config(Exti_config *exti,	unsigned char	ExtiMode,unsigned char	ExtiPolity,unsigned char	ExtiInterrupt);
```
```c
//函数用途：上传外部中断设置
//参数：定时器名称
//返回 成功0 失败-1
int EXTI_setup(unsigned char exti);
```
```c
//函数用途：添加外部中断中断任务
//参数：外部中断名称，中断任务函数
//返回 成功0 失败-1
int EXTI_addTask(unsigned char exti,void(*hander)());
```

```c
//函数用途：快速配置外部中断
//参数：外部中断名称，模式，中断任务函数
//返回 成功0 失败-1
int EXTI_set(unsigned char exti,unsigned char mode,void (*hander)());
```

```c
//函数用途：打开、关闭外部中断
//参数：外部中断名称
//返回 成功0 失败-1
int EXTI_ON(unsigned char exti);
int EXTI_OFF(unsigned char exti);
```

### `HAL_UART.c`

​	包含硬件串口的配置和操作函数。这个库函数包含发送缓存器和接收缓存器。每个串口都有一个独立运作的缓存器，因此，对RAM空间有较大占用。当RAM空间不足时，可按照实际情况改变其大小，缓冲区大小可在<HAL_UART.h>配置。**串口可以更换引脚位置，但本库不包含相应配置函数，用户仍可配置寄存器修改或在下载器中调整选项。**如果串口发送时出现乱码，可能是以下几点原因产生：

1.波特率过低，发送太慢

2.发送缓冲区太小

3.串口中断阻塞，可能由更高优先中断频繁发起中断服务请求所致。

| 串口  | 宏定义 |
| :---: | :----: |
| 串口1 | Uart1  |
| 串口2 | Uart2  |
| 串口3 | Uart3  |
| 串口4 | Uart4  |

|              模式              |     宏定义     |
| :----------------------------: | :------------: |
|    同步移位输出（兼容较差）    | UartShiftRight |
|       8位数据,可变波特率       |  Uart8bitBRT   |
| 9位数据,固定波特率（兼容较差） |    Uart9bit    |
| 9位数据,可变波特率（兼容较差） |  Uart9bitBRT   |

|  状态  | 宏定义  |
| :----: | :-----: |
|  置高  |  HIGH   |
|  置低  |   LOW   |
|  使能  | ENABLE  |
| 不使能 | DISABLE |

```c
//函数用途：配置串口
//参数：串口名称，模式，波特率发生器，波特率，是否多机通信，是否允许接收，波特率加倍，中断，中断优先级，是否短路RX和TX
void UART_config(UartConfig *uart,unsigned char	UartMode,unsigned char UartBrt,unsigned long	UartBaudRate,unsigned char UartMoreCommunicate,unsigned char UartRxEnable,unsigned char	BaudRateDouble,unsigned char UartInterrupt,unsigned char UartPolity,unsigned char UartShort);
```

```c
//函数用途：上传串口配置
//参数：串口名称
//返回 成功0 失败-1
int UART_setup(unsigned char uart);
```

```c
//函数用途：添加串口中断函数
//参数：串口名称，中断任务函数
//返回 成功0 失败-1
int UART_addTask(unsigned char uart,void(*hander)());
```

```c
//函数用途：快速配置串口
//参数：串口名称，波特率
//返回 成功0 失败-1
int UART_begin(unsigned char uart,unsigned long baud);
```

​	使用快速配置串口函数时，串口使用8位模式，打开中断，波特率发生器为与串口标号对应的定时器并允许接收。即便你没有在这个函数配置中断任务函数，**中断任务函数仍会占用一个** 用于接收信息并载入接收缓存器。

```c
//函数用途：关闭串口
//参数：串口名称
//返回 成功0 失败-1
int UART_end(unsigned char uart);
```

```c
//函数用途：读取串口缓存器内容
//参数：串口名称
//返回 缓存器中的一个字节
char UART_read(unsigned char uart);
```

```c
//函数用途：读取串口缓存器未读字节数量
//参数：串口名称
//返回 缓存器中未读字节数量
unsigned int UART_available(unsigned char uart);
```

```c
//函数用途：清空串口缓存器
//参数：串口名称
//返回 成功0 失败-1
int UART_flush(unsigned char uart);
```

```c
//函数用途：发送一个字符串
//参数：串口名称，字符内容
//返回 成功0 失败-1
int UART_print(unsigned char uart,unsigned char *string);
//函数用途：发送一个字符串并换行
//参数：串口名称，字符内容
//返回 成功0 失败-1
int UART_println(unsigned char uart,unsigned char *string);
//函数用途：发送一个字节
//参数：串口名称，字节内容
//返回 成功0 失败-1
int UART_write(unsigned char uart,unsigned char dat);
```

### `HAL_PWM.c`

​	包含硬件PWM的配置函数和操作函数。**STC15只有6路PWM，标号为2-7**，STC8有8路PWM，标号为0-7。PWM引脚可以切换，但是本库文件不包含相应的函数，用户可通过配置寄存器修改，但很有可能**出现预料之外的错误**。硬件PWM使用独立的15位定时器产生时钟，通过调整初值可修改每个PWM周期中包含的时钟数。**值得注意的是，这类时钟从0开始计数，到达设定值溢出并返回重新计数，与常规定时器有区别。**

|  名称   |     宏定义     |
| :-----: | :------------: |
| PWM周期 | PWM_VALUE=1000 |

|     时钟类型      |       宏定义       |
| :---------------: | :----------------: |
| 定时器2的溢出中断 |     pwmTimer2      |
|  系统时钟的分频   | sysTimer(x) x:1-16 |

|   中断类型    |  宏定义   |
| :-----------: | :-------: |
| 翻转为高电平  | INT_HIGH  |
| 翻转为低电平  |  INT_LOW  |
|     翻转      |  INT_ALL  |
| PWM定时器溢出 | INT_TIMER |

|  状态  | 宏定义  |
| :----: | :-----: |
|  置高  |  HIGH   |
|  置低  |   LOW   |
|  使能  | ENABLE  |
| 不使能 | DISABLE |

```c
//函数用途：配置PWM
//参数：PWM时钟是否运行，时钟类型，中断类型，中断优先级
void PWM_config(unsigned char PwmRun,unsigned char PwmClkSource,unsigned char PwmInterrupt,unsigned char PwmPolity);
```

​	这里输入的中断类型只能为`INT_TIMER`、`ENABLE`、`DISABLE`

```c
//函数用途：上传PWM配置
//返回 成功0 失败-1
int PWM_setup();
```

```c
//函数用途：添加PWM中断函数
//参数：串口名称，中断任务函数
//返回 成功0 失败-1
int PWM_addTask(void(*hander)());
```

```c
//函数用途：快速配置PWM
//参数：PWM端口号，占空值，中断类型
//返回 成功0 失败-1
int PWM_begin(unsigned char port,unsigned int duty,unsigned char pwmint);
```

​	使用此函数快速配置PWM时，端口号仅需输入数字。参数中的**占空值并非占空比**，占空值为每个PWM周期中输出高电平的时钟数量。

```c
//函数用途：关闭PWM
//参数：PWM端口号
//返回 成功0 失败-1
int PWM_end(unsigned char port);
```

​	当所有PWM端口都被关闭时，将会关闭PWM时钟。

```c
//函数用途：PWM占空值设置
//参数：PWM端口号，占空值
//返回 成功0 失败-1
int PWM_set(unsigned char port,unsigned int duty);
```

```c
//函数用途：将占空值转换为占空比
//参数：占空值
//返回 占空比
unsigned int PWM_persent(unsigned long persent);
```

### `HAL_ADC.c`

​	包含ADC的配置函数和操作函数。ADC结果采用12位精度。**ADC转换速度等级STC15只有4个等级0-3，而STC8有16个等级0-15。**其范围不变，但STC8的速度等级更多，分辨率更高。

|  状态  | 宏定义  |
| :----: | :-----: |
|  置高  |  HIGH   |
|  置低  |   LOW   |
|  使能  | ENABLE  |
| 不使能 | DISABLE |

```c
//函数用途：配置ADC
//参数：ADC是否运行，中断类型，中断优先级,ADC速度
void ADC_config(unsigned char AdcRun,unsigned char AdcInterrupt,unsigned char AdcPolity,unsigned char AdcSpeed);
```

```c
//函数用途：上传ADC配置
void ADC_setup();
```

```c
//函数用途：添加ADC中断函数
//参数：中断任务函数
//返回 成功0 失败-1
int ADC_addTask(void (*hander)());
```

```c
//函数用途：快速配置ADC
//参数：是否使能中断
//返回 成功0 失败-1
void ADC_begin(unsigned char inte);
```

​	使能中断时，函数将会配置中断任务。发生中断时，**中断任务将ADC结果放入ADC结构体**。不使能中断时，ADC函数将会产生阻塞，一直等待ADC**转换结束并将结果放入ADC结构体**。因此，无论如何获取ADC结果，用户读取ADC值都需要读取ADC结构体中的结果。读取方法：读取`ADC_return`的值。

```c
//函数用途：关闭ADC
void ADC_end();
```

```c
//函数用途：开始ADC转换
//参数：ADC通道
int ADC_start(unsigned char port);
```

​	ADC通道仅需要输入通道数字。

### `HAL_IAP.c`

​	包含IAP操作函数。STC15/STC8部分型号不带有EEPROM，同时，这类型芯片支持使用IAP技术直接操作程序flash空间。即使在带有EEPROM的芯片中仍可使用，因此库中采用IAP技术。然而这有缺陷。真正的EEPROM可以对数据进行按位擦除操作，而flash则不能如此操作。**因此，IAP库带有能装下整个扇区的缓存器，这会消耗大量RAM空间。**但是这项改变将带来优势，不同的数据不需要同时读出写入，借助缓存器，写入读出可以随意操作。

​	本库中，由于缓存器的使用，**数据的真实地址不作描述，只讨论首地址以及相对地址。**首地址可在<HAL_IAP.h>中修改。

```c
//函数用途：IAP读取一个字节
//参数：数据相对地址
//返回：IAP字节
unsigned char IAP_read(unsigned int EE_address);
```

```c
void IAP_reads(unsigned int EE_address,unsigned char *DataAddress,unsigned int number);
```

```c
//函数用途：IAP擦除扇区
//参数：数据相对地址
void IAP_erase(unsigned int EE_address);
```

​	这个函数将会擦除整个扇区，擦除扇区取决于地址所在的扇区，这个地址不需要为扇区首地址。

```c
//函数用途：IAP写入数据
//参数：数据相对地址，数据
void IAP_write(unsigned int EE_address,unsigned char Data);
```

​	这个函数并没有真正将数据写入flash，而是将改动写入缓存器。在第一次运行时，地址所在的整个扇区数据将会被加载到缓存器，并在之后的操作中修改缓存器。

​**注意：由于缓存器一次只能缓存一个扇区，因此不可跨扇区操作，如果需要跨扇区，请先使用`IAP_update()`上传数据到flash。**

```c
//函数用途：将缓存器数据写入相对应扇区
void IAP_update();
```

​	使用`IAP_write()`后必须使用本函数将数据写入flash，否则掉电会丢失。

### `HAL_POWER.c`

​	包含单片机电源和复位的操作。

|  分频   | 宏定义  |
| :-----: | :-----: |
| 不分频  |  DIV_1  |
|  2分频  |  DIV_2  |
|  4分频  |  DIV_4  |
|  8分频  |  DIV_8  |
| 16分频  | DIV_16  |
| 32分频  | DIV_32  |
| 64分频  | DIV_64  |
| 128分频 | DIV_128 |

```c
//函数用途：使单片机运行在低速模式
//参数：分频数
int POWER_div(unsigned char div);
```

```c
//函数用途：使单片机运行在空闲模式
void POWER_idle();
```

​	单片机运行在空闲模式时，只有CPU被关闭，其他外设正常供电，任何中断可唤醒CPU。

```c
//函数用途：使单片机运行在掉电模式
//参数：是否使能掉电定时器，延时时间
void POWER_down(unsigned char clock,unsigned int ms);
```

​	单片机运行在掉电模式时，大部分设备关闭，其中包括需要时钟驱动的设备，例如定时器，时钟，PWM。

​	在掉电模式时，外部中断可唤醒CPU，另外定时器中断在打开的状态下定时器引脚发生电平变化亦可唤醒CPU。但此刻的定时器中断与定时器溢出与否无关，因为时钟已关闭。

```c
//函数用途：使单片机复位并运行在用户代码区
void POWER_reset();
```

```c
//函数用途：使单片机在接收到下载指令时能自动复位并下载
//返回 成功0 失败-1
int POWER_autoReboot();
```

​	此函数可使得单片机在收到下载信号后自复位进入ISP区。**但需要先配置串口1，否则将不会工作。同时，配置此函数会占用一个串口1中断任务。**

```c
//函数用途：使单片机复位并运行在ISP区
void POWER_Reboot();
```

​	使用此函数复位时，单片机可检测下载信号。

### `HAL_WDT.c`

​	包含看门狗定时器的配置函数和操作函数。

|  名称   | 延时时间 | 宏定义  |
| :-----: | :------: | :-----: |
|  2分频  |  32.7ms  |  WDT_2  |
|  4分频  |  65.5ms  |  WDT_4  |
|  8分频  |  131ms   |  WDT_8  |
| 16分频  | 262.1ms  | WDT_16  |
| 32分频  | 624.3ms  | WDT_32  |
| 64分频  | 1048.6ms | WDT_64  |
| 128分频 | 2097.2ms | WDT_128 |
| 256分频 | 4194.3ms | WDT_256 |

```c
//函数用途：开启看门狗
//参数：分频，是否在空闲模式继续运行看门狗
int WDT_begin(unsigned char prescale,unsigned char idle);
```

```c
//函数用途：关闭看门狗
void WDT_end();
```

```c
//函数用途：清除看门狗计数器
void WDT_clean();
```

### `HAL_PCA.c`

​	包含PCA模块的配置函数和操作函数。

|时钟源	   |宏定义  |
|:-----:|:-----:|
|系统时钟不分频|PCA_1	|
|系统时钟2分频 |PCA_2	 |
|系统时钟4分频 |PCA_4	 |
|系统时钟6分频 |PCA_6	 |
|系统时钟8分频 |PCA_8	 |
|系统时钟12分频|PCA_12 |
|定时器0溢出   |PCA_Time0|
|外部输入	   |PCA_Eci|

| 模式 | 宏定义 |
| :--: | :----: |
| 上升沿捕获 |PCA_RISE|
| 下降沿捕获 |PCA_FALL|
| 边缘捕获 |PCA_RISE_FALL|
| 软件定时器 |PCA_TIME|
| 高速输出 |PCA_TOG|

```c
//函数用途：配置PCA
//参数：时钟类型，是否中断，中断优先级，是否运行
void PCA_config(unsigned char PcaClock,unsigned char PcaInterrupt,unsigned char PcaPolity,unsigned char PcaRun);
```

```c
//函数用途：上传PCA配置
void PCA_setup();
```

```c
//函数用途：添加PCA中断函数
//参数：中断任务函数
//返回 成功0 失败-1
int PCA_addTask(void (*hander)());
```

```c
//函数用途：快速配置PCA
//参数：端口，模式，比较值，中断类型
//返回 成功0 失败-1
int PCA_begin(unsigned char port,unsigned char mode,unsigned int value,unsigned char inte);
```

​	快速配置时，默认输入时钟类型为系统时钟不分频。

```c
//函数用途：关闭PCA
void PCA_end();
```

```c
//函数用途：PCA输出开关
//参数：端口，开关
//返回 成功0 失败-1
int PCA_tog(unsigned char port,unsigned char onoff);
```

​	在配置中模式设置为高速输出后，tog默认打开。

### `HAL_SPI.c`

​	包含SPI通讯接口的配置函数和操作函数。

|     时钟源     |   宏定义   |
| :------------: | :--------: |
| 系统时钟4分频  | SPI_CLK_4  |
| 系统时钟8分频  | SPI_CLK_8  |
| 系统时钟16分频 | SPI_CLK_16 |
| 系统时钟32分频 | SPI_CLK_32 |

| 时钟边缘 |  宏定义  |
| :------: | :------: |
|  上升沿  | SPI_RISE |
|  下降沿  | SPI_FALL |

| 数据发送顺序 |  宏定义  |
| :----------: | :------: |
|   高位先发   | SPI_MSBF |
|   低位先发   | SPI_LSBF |

```c
//函数用途：初始化SPI接口
void SPI_begin();
```

​	在初始化SPI接口前请先**初始化SPI对象**。默认16分频，上升沿，高位先发，查询方式。

```c
//函数用途：关闭SPI接口
void SPI_end()；
```

​	关闭SPI接口后，相关接口可以作为普通IO使用，但**请先设置端口模式**。

```c
//函数用途：PCA输出开关
//参数：对象，发送的数据
void SPI_send(SPIslave *sla,unsigned char dat);
```

​	向SPI对象发送一个字节数据，如果设置为中断模式，中断产生后将会将返回的数据写入对象的`SPIslave.result`，如果采用查询方式，将会**产生阻塞，直到数据返回**。

```c
//函数用途：设置SPI
void SPI_setup();
```

​	配置SPI的参数，如果需要修改而不是使用默认值，请先调用`SPI_config()`。

```c
//函数用途：配置SPI的参数
//参数：中断优先级，是否使能中断，时钟源，时钟边缘，数据发送顺序
void SPI_config(unsigned char SpiPolity,unsigned char SpiInterrupt,unsigned char SpiClock,unsigned char SpiClockMode,unsigned char SpiSendMode);
```

​	配置SPI的参数，执行修改需要调用`SPI_setup()`或者`SPI_begin()`。

```c
//函数用途：初始化SPI对象
//参数：SPI对象，片选引脚
void SPI_slaveBegin(SPIslave *sla,unsigned char pin);
```

​	初始化SPI对象。


## DRIVER

​	此处放置外设驱动库，其中包含2个样例外设驱动库。

### `KeyBoard.c`

​	矩阵键盘读取键值库。 一般情况下，一个单片机可搭载至多4个4*4矩阵键盘。因此，**使用库函数前需要创建对象**。例如：`Keyboard keyboard1`

|                         模式                         |     宏定义      |
| :--------------------------------------------------: | :-------------: |
|       阻塞模式，按键按下并弹起时才会返回按键值       |  KEYBOARD_ONCE  |
| 非阻塞模式，按键在保持按下时仍会每隔一段时间产生返回 | KEYBOARD_REPEAT |

```c
//函数用途：配置键盘
//参数：键盘对象，端口，模式，延时时间
//返回 成功0 失败-1
int KEYBOARD_begin(KeyBoard *kb,unsigned char port,unsigned char mode,unsigned int time);
```

​	当设置为非阻塞模式时，延时时间生效，而非阻塞模式延时时间没有影响。

```c
//函数用途：扫描键盘获取键值
//参数：键盘对象
void KEYBOARD_getkey(KeyBoard *kb);
```

​	扫描完成后，键值保存在对象的`.keynum`中，用户可直接读取。如果扫描结束后无按键按下，则键值为0xff。

### `RgbLed.c`

​	通过PWM驱动全彩LED库。 一般情况下，一个单片机可搭载至多2个全彩LED。因此，**使用库函数前需要创建对象**。例如：`RgbLed led1`

​	库中带有gamma电压矫正，默认采用gamma=2.60校正电压的非线性误差。如需修改gamma值或取消gamma值校正，请修改<RgbLed.h>。

```c
//函数用途：配置全彩led
//参数：led对象，红色端口，绿色端口，蓝色端口
//返回 成功0 失败-1
int RGBLED_begin(RgbLed *led,unsigned char rport,unsigned char gport,unsigned char bport);
```

​	端口号请参考<HAL_PWM.c>。

```c
//函数用途：调整led颜色亮度
//参数：键盘对象，红色值，绿色值，蓝色值
void RGBLED_set(RgbLed *led,unsigned int red,unsigned int green,unsigned int blue);
```

​	占空值输入范围 0-999。

```c
//函数用途：关闭led
//参数：键盘对象，端口，模式，延时时间
void RGBLED_end(RgbLed *led);
```

