/*
******************************************************************************
* @file uart.c
* @author SM Application Team
* @version V1.0
* @date 2014-11-17
* @brief 串口控制文件
******************************************************************************
* @attention
*
* <h2><center>Copyright (c) 2009 - 2012 , DaLian SanMU Electronic
Science and Technology Co., Ltd</center></h2>
*
******************************************************************************
*/
#include <uart.h>         //包含串口头文件

/**
* @brief 设置系统时钟源
*
* @param 无
* @return 无
* @note 
*/
void InitCLK()
{
    CLKCONCMD &= ~0x40;             //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);        //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;             //设置系统主时钟频率为32MHZ   
}

/**
* @brief 以毫秒为单位延时 16M时约为535,32M时要调整,系统时钟不修改默认为16M
*
* @param   msec 延时参数，值越大延时越久
* @return 无
* @note 
*/
void DelayMS(uint msec)          //延时函数
{  
    uint i,j;                    //定义无符号整型变量i,j
    
    for (i=0; i<msec; i++)       //循环msec次，每次i加1
        for (j=0; j<1070; j++);  //循环1070次，每次j加1
}

/**
* @brief 串口初始化函数
*
* @param 无
* @return 无
* @note 
*/
void InitUart(void)         //串口初始化函数
{ 
    PERCFG = 0x00;           //外设控制寄存器 USART 0的IO位置:0为P0口位置1 
    P0SEL = 0x0c;            //P0_2,P0_3用作串口（外设功能）
    P2DIR &= ~0XC0;          //P0优先作为UART0
    
    U0CSR |= 0x80;           //设置为UART方式
    U0GCR |= 11;				       
    U0BAUD |= 216;           //波特率设为115200
    UTX0IF = 0;              //UART0 TX中断标志初始置位0
}

/**
* @brief 串口发送函数
*
* @param Data:发送缓冲区   len:发送长度
* @return 无
* @note 
*/
void UartSendString(char *Data, int len)  //串口发送函数
{
    uint i;                     //定义无符号整型变量i
    
    for(i=0; i<len; i++)        //循环len次，每次i加1
    {
        U0DBUF = *Data++;       //传送下一个字节
        while(UTX0IF == 0);     //循环
        UTX0IF = 0;             //清除中断标志位
    }
}
