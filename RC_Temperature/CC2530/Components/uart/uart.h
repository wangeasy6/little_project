/*
******************************************************************************
* @file uart.h
* @author SM Application Team
* @version V1.0
* @date 2014-11-12
* @brief 串口头文件
******************************************************************************
* @attention
*
* <h2><center>Copyright (c) 2009 - 2012 , DaLian SanMU Electronic
Science and Technology Co., Ltd</center></h2>
*
******************************************************************************
*/
#ifndef __UART__
#define __UART__

#include <ioCC2530.h>
#include <string.h>

typedef unsigned char uchar;    //重定义 unsigned char类型为uchar
typedef unsigned int  uint;     //重定义 unsigned int类型为uint
#define TX_SIZE    20               //宏定义TX_SIZE
#define TX_STRING  "Hello Zigbee"   //宏定义TX_STRING

void InitCLK();
void DelayMS(uint msec) ;    //延时函数
void InitUart(void) ;      //串口初始化函数
void UartSendString(char *Data, int len);  //串口发送函数

#endif