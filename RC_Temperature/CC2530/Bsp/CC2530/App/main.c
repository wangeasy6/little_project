/*
* 功能：使用CC2530，采集和利用Zigbee传输温度值
*     本程序包含了温度采集子节点和温度收集总结点，通过主程序中的 appMode 选择编译子节点还是总结点
*     子节点：通过DS18B20采集温度数值，然后通过Zigbee点对点传给总结点
*     总结点：收集子节点的温度信息，再通过串口发送给网关
* 完成时间：2017.05.16
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

//温度采集总节点：收集子节点的温度信息，并通过串口发送给网关
static void appCollect()
{
    char strTemp[4];
    strTemp[2] = '\r';
    strTemp[3] = '\n';
    InitCLK(); 
    InitUart();                       //调置串口相关寄存器
    
    // Initialize 
    basicRfConfig.myAddr = COLLECT_ADDR;
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
    basicRfReceiveOn();
    while (TRUE) {
        while(!basicRfPacketIsReady()); //检查模块是否已经可以接收下一个数据，如果准备好刚返回 TRUE
        //把收到的数据复制到buffer中
        if( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL) > 0 ) {
            UartSendString("temperature : ", sizeof("temperature : ")); //提示
            strTemp[0] =  *pRxData/10+48;        //取出十位数
            strTemp[1] = *pRxData%10+48;        //取出个位数
            UartSendString( strTemp, sizeof(strTemp) ); //串口发送数据
        }
    }
}

//温度采集子节点：通过DS18B20采集温度数值，然后通过Zigbee点对点传给总结点
static void appNode()
{
    P0SEL &= 0x7f;                  //DS18B20的io口初始化
    uchar temp_t;
    
    // Initialize BasicRF
    basicRfConfig.myAddr = NODE_ADDR;
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }

    // 由于模块只需要发射，所以把接收屏蔽掉以降低功耗。
    basicRfReceiveOff();
    
    // Main loop
    while (TRUE) {
        temp_t = ReadDs18B20();           //温度读取函数
        pTxData[0] = temp_t;
        basicRfSendPacket(COLLECT_ADDR, pTxData, APP_PAYLOAD_LENGTH);

        Delay_ms(1000);
    }
}

void main(void)
{
    uint8 appMode = NODE; // appMode取值:NONE、NODE、COLLECT
    // Config basi
    basicRfConfig.panId = PAN_ID;      //设置节点PAN IDcRF
    basicRfConfig.channel = RF_CHANNEL;//设置节点RF通道
    basicRfConfig.ackRequest = TRUE;   //目标确认
#ifdef SECURITY_CCM                    //是否加密，预定义里取消了加密
    basicRfConfig.securityKey = key;
#endif
    // Initalise board peripherals 始化外围设备
    halBoardInit();
    
    // Initalise hal_rf 硬件抽象层的rf进行初始化初
    if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }

    switch( appMode )
    {
      case NODE:
        appNode();      //发射模块,采集DS18B20并通过RF发送到采集节点
        break;
      case COLLECT:
        appCollect();   //接收模块 接收节点信息并通过串口发送到服务器
        break;
      default :
        break;
    }
    return ;
}
