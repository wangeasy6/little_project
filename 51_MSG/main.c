/*
*	功能：使用红外传感器监测是否有人闯入，当有人闯入蜂鸣器报警并发送预设短信
*		对应引脚：红外传感器P2^0,蜂鸣器P2^1,短信模块接UART(P3^0+P3^1),指示灯连接P0^0 - P0^3(低电平点亮)
*	完成时间：2017/05/24
*/
#include <reg51.h>
#include <string.h>

sbit red = P2^0;
sbit bee = P2^1;
#define PASS 0					//红外检测有人通过时的电平
#define CYCLE 20
#define BEE_T 9
#define SEND_DLY 2
#define BACK_DATA_LEN 15

unsigned char code msg_CMGF[]={"AT+CMGF=1\r\n"};
unsigned char code msg_CSCS[]={"AT+CSCS=\"GSM\"\r\n"};
unsigned char code msg_number[]={"AT+CMGS=\"18523591056\"\r\n"};   //短信号码
unsigned char code msg_content[]={"someone is break in! --From Home\r\n"};	//短信内容
static unsigned char recv_ok = 0;			//置1时表示一条指令接收完成
static unsigned char i = 0;					//back_data 数组索引
unsigned char back_data[BACK_DATA_LEN];		//短信模块回传内容的缓存

void delay(int x)
{  
	int i,j;
    for (i=0;i<x;i++)                  
       for(j=0;j<600;j++);
}

void init_seri()	//Uart初始化，波特率为9600
{
	TMOD = 0x20;			
	TH1 = 0xFd;
	TR1 = 1;
	SCON = 0x50;	   
	EA = 1;		//开总中断
	ES = 1;		//开串口中断	
}

void bee_run(unsigned char n)	//用于驱动无源蜂鸣器报警，传入时间为宏值BEE_T
{
   while(n--)
   {
		bee = 1;
		delay(CYCLE);
		bee = 0;
		delay(CYCLE);
   }
}

void seri_recv(void) interrupt 4	//接收短信模块回发内容
{	
	if(RI == 1)   					//当硬件接收到一个数据时，RI会置位
	{	 
		if(i==BACK_DATA_LEN) 
		{
			i = 0;
		} 
		if(SBUF == '\n' || SBUF == 0x20)	//收到换行或者空格表示一条指令解说
		{
			recv_ok = 1;
		}
		back_data[i] = SBUF;
		RI = 0;   
		i++;		
	}
}

//用于清空back_data
void data_clear()
{
	 memset(back_data, 0, sizeof(back_data));
	 i = 0;
	 recv_ok = 0;
}
   	
//UART发送模块
void send_str(char * string)
{
	while(*string)
	{
		SBUF = *string;
		while(!TI);
		TI = 0;
		string++;
	}
}

void send_char(unsigned char b)    
{
    SBUF=b;
	while(!TI);   
	TI=0;   
}

//检查设置是否成功
unsigned char check_ok()
{
	if(recv_ok)
	{
		recv_ok = 0;
		if( back_data[0] == 0x4f && back_data[1] == 0x4b )
		{
			return 1;
		}else
		{ 
			bee_run(BEE_T);
			bee = 1;
			P0 = P0>>1;
		}
	}
	return 0;
}

//当发送第三条指令后，短信模块回发“> ”，提示输入短信内容。此函数用于检测 “>”
unsigned char check_send()
{
	if(recv_ok)
	{
		recv_ok = 0;
		if( back_data[0] == 0x3e )	//0x3e ASCII对应为 “>”
		{
			return 1;
		}else
		{ 
			bee_run(BEE_T);
			bee = 1; 
			P0 = P0>>1;
		}
		data_clear();
	}
	return 0;
}

//短信模块在接收到指令之后会将指令回发回来，此函数用于跳过前面两行的回发指令
void jump()
{  
	while( !recv_ok );
	recv_ok = 0;
	data_clear();
	while( !recv_ok );
	recv_ok = 0;
	data_clear();
}

//设置短信发送模式
void Set_Text_Mode(void)
{ 	
	send_str(msg_CMGF); 				  	//配置短信为TEXT模式
	jump();
	while( !check_ok() );
	P0 &= ~0x01;							//TEXT模式设置成功，第一个指示灯亮起

	send_str(msg_CSCS); 					//设置输入字符集为"GSM"
	jump();
	while( !check_ok() );
	P0 &= ~0x02;							//字符集设置成功，第二个指示灯亮起
 
}

//发送短信
void Send_Message(void)
{	  
	data_clear();
	send_str(msg_number); 
	jump();
	while( !check_send() );   
	P0 &= ~0x04;							//短信号码设置成功，第三个指示灯亮起
	 
	send_str(msg_content);   
	send_char(0x1a);
	P0 &= ~0x08;							//发送短信成功，第四个指示灯亮起
}

int main()
{
	init_seri();
	P0 = 0xff;								//指示灯初始化
	Set_Text_Mode();
	while(1)
	{
		if(red == PASS)						//红外检测到有人通过
		{
			P0 |= ~0x03;
			Send_Message();
			while(1)
			{
				bee_run(BEE_T);
				if(red != PASS)
					break;
			}
			bee = 1;
		}
	}
	return 0;
}