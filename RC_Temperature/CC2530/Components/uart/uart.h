/*
******************************************************************************
* @file uart.h
* @author SM Application Team
* @version V1.0
* @date 2014-11-12
* @brief ����ͷ�ļ�
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

typedef unsigned char uchar;    //�ض��� unsigned char����Ϊuchar
typedef unsigned int  uint;     //�ض��� unsigned int����Ϊuint
#define TX_SIZE    20               //�궨��TX_SIZE
#define TX_STRING  "Hello Zigbee"   //�궨��TX_STRING

void InitCLK();
void DelayMS(uint msec) ;    //��ʱ����
void InitUart(void) ;      //���ڳ�ʼ������
void UartSendString(char *Data, int len);  //���ڷ��ͺ���

#endif