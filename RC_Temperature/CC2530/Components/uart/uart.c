/*
******************************************************************************
* @file uart.c
* @author SM Application Team
* @version V1.0
* @date 2014-11-17
* @brief ���ڿ����ļ�
******************************************************************************
* @attention
*
* <h2><center>Copyright (c) 2009 - 2012 , DaLian SanMU Electronic
Science and Technology Co., Ltd</center></h2>
*
******************************************************************************
*/
#include <uart.h>         //��������ͷ�ļ�

/**
* @brief ����ϵͳʱ��Դ
*
* @param ��
* @return ��
* @note 
*/
void InitCLK()
{
    CLKCONCMD &= ~0x40;             //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);        //�ȴ������ȶ�Ϊ32M
    CLKCONCMD &= ~0x47;             //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ   
}

/**
* @brief �Ժ���Ϊ��λ��ʱ 16MʱԼΪ535,32MʱҪ����,ϵͳʱ�Ӳ��޸�Ĭ��Ϊ16M
*
* @param   msec ��ʱ������ֵԽ����ʱԽ��
* @return ��
* @note 
*/
void DelayMS(uint msec)          //��ʱ����
{  
    uint i,j;                    //�����޷������ͱ���i,j
    
    for (i=0; i<msec; i++)       //ѭ��msec�Σ�ÿ��i��1
        for (j=0; j<1070; j++);  //ѭ��1070�Σ�ÿ��j��1
}

/**
* @brief ���ڳ�ʼ������
*
* @param ��
* @return ��
* @note 
*/
void InitUart(void)         //���ڳ�ʼ������
{ 
    PERCFG = 0x00;           //������ƼĴ��� USART 0��IOλ��:0ΪP0��λ��1 
    P0SEL = 0x0c;            //P0_2,P0_3�������ڣ����蹦�ܣ�
    P2DIR &= ~0XC0;          //P0������ΪUART0
    
    U0CSR |= 0x80;           //����ΪUART��ʽ
    U0GCR |= 11;				       
    U0BAUD |= 216;           //��������Ϊ115200
    UTX0IF = 0;              //UART0 TX�жϱ�־��ʼ��λ0
}

/**
* @brief ���ڷ��ͺ���
*
* @param Data:���ͻ�����   len:���ͳ���
* @return ��
* @note 
*/
void UartSendString(char *Data, int len)  //���ڷ��ͺ���
{
    uint i;                     //�����޷������ͱ���i
    
    for(i=0; i<len; i++)        //ѭ��len�Σ�ÿ��i��1
    {
        U0DBUF = *Data++;       //������һ���ֽ�
        while(UTX0IF == 0);     //ѭ��
        UTX0IF = 0;             //����жϱ�־λ
    }
}
