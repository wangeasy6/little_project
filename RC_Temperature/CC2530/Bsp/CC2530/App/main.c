/*
* ���ܣ�ʹ��CC2530���ɼ�������Zigbee�����¶�ֵ
*     ������������¶Ȳɼ��ӽڵ���¶��ռ��ܽ�㣬ͨ���������е� appMode ѡ������ӽڵ㻹���ܽ��
*     �ӽڵ㣺ͨ��DS18B20�ɼ��¶���ֵ��Ȼ��ͨ��Zigbee��Ե㴫���ܽ��
*     �ܽ�㣺�ռ��ӽڵ���¶���Ϣ����ͨ�����ڷ��͸�����
* ���ʱ�䣺2017.05.16
*/
#include <stdio.h>
#include <string.h>
#include <hal_lcd.h>
#include <hal_led.h>
#include <hal_joystick.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <hal_int.h>
#include "hal_mcu.h"
#include "hal_button.h"
#include "hal_rf.h"
#include "util_lcd.h"
#include "basic_rf.h"
#include "ds18b20.h" 
#include "uart.h"


/***********************************************************************************
* CONSTANTS
*/
// Application parameters
#define RF_CHANNEL                25      // 2.4 GHz RF channel

// BasicRF address definitions
#define PAN_ID                0x2007
#define NODE_ADDR             0x2520
#define COLLECT_ADDR          0xBEEF
#define APP_PAYLOAD_LENGTH        1

// Application states
#define IDLE                      0
#define SEND_CMD                  1

// Application role
#define NONE                      0
#define NODE                      1
#define COLLECT                   2
#define APP_MODES                 2

/***********************************************************************************
* LOCAL VARIABLES
*/
static uint8 pTxData[APP_PAYLOAD_LENGTH];
static uint8 pRxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;

#ifdef SECURITY_CCM
// Security key
static uint8 key[]= {
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
};
#endif

//�¶Ȳɼ��ܽڵ㣺�ռ��ӽڵ���¶���Ϣ����ͨ�����ڷ��͸�����
static void appCollect()
{
    char strTemp[4];
    strTemp[2] = '\r';
    strTemp[3] = '\n';
    InitCLK(); 
    InitUart();                       //���ô�����ؼĴ���
    
    // Initialize 
    basicRfConfig.myAddr = COLLECT_ADDR;
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
    basicRfReceiveOn();
    while (TRUE) {
        while(!basicRfPacketIsReady()); //���ģ���Ƿ��Ѿ����Խ�����һ�����ݣ����׼���øշ��� TRUE
        //���յ������ݸ��Ƶ�buffer��
        if( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL) > 0 ) {
            UartSendString("temperature : ", sizeof("temperature : ")); //��ʾ
            strTemp[0] =  *pRxData/10+48;        //ȡ��ʮλ��
            strTemp[1] = *pRxData%10+48;        //ȡ����λ��
            UartSendString( strTemp, sizeof(strTemp) ); //���ڷ�������
        }
    }
}

//�¶Ȳɼ��ӽڵ㣺ͨ��DS18B20�ɼ��¶���ֵ��Ȼ��ͨ��Zigbee��Ե㴫���ܽ��
static void appNode()
{
    P0SEL &= 0x7f;                  //DS18B20��io�ڳ�ʼ��
    uchar temp_t;
    
    // Initialize BasicRF
    basicRfConfig.myAddr = NODE_ADDR;
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }

    // ����ģ��ֻ��Ҫ���䣬���԰ѽ������ε��Խ��͹��ġ�
    basicRfReceiveOff();
    
    // Main loop
    while (TRUE) {
        temp_t = ReadDs18B20();           //�¶ȶ�ȡ����
        pTxData[0] = temp_t;
        basicRfSendPacket(COLLECT_ADDR, pTxData, APP_PAYLOAD_LENGTH);

        Delay_ms(1000);
    }
}

void main(void)
{
    uint8 appMode = NODE; // appModeȡֵ:NONE��NODE��COLLECT
    // Config basi
    basicRfConfig.panId = PAN_ID;      //���ýڵ�PAN IDcRF
    basicRfConfig.channel = RF_CHANNEL;//���ýڵ�RFͨ��
    basicRfConfig.ackRequest = TRUE;   //Ŀ��ȷ��
#ifdef SECURITY_CCM                    //�Ƿ���ܣ�Ԥ������ȡ���˼���
    basicRfConfig.securityKey = key;
#endif
    // Initalise board peripherals ʼ����Χ�豸
    halBoardInit();
    
    // Initalise hal_rf Ӳ��������rf���г�ʼ����
    if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }

    switch( appMode )
    {
      case NODE:
        appNode();      //����ģ��,�ɼ�DS18B20��ͨ��RF���͵��ɼ��ڵ�
        break;
      case COLLECT:
        appCollect();   //����ģ�� ���սڵ���Ϣ��ͨ�����ڷ��͵�������
        break;
      default :
        break;
    }
    return ;
}
