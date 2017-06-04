/*
*	���ܣ�ʹ�ú��⴫��������Ƿ����˴��룬�����˴������������������Ԥ�����
*		��Ӧ���ţ����⴫����P2^0,������P2^1,����ģ���UART(P3^0+P3^1),ָʾ������P0^0 - P0^3(�͵�ƽ����)
*	���ʱ�䣺2017/05/24
*/
#include <reg51.h>
#include <string.h>

sbit red = P2^0;
sbit bee = P2^1;
#define PASS 0					//����������ͨ��ʱ�ĵ�ƽ
#define CYCLE 20
#define BEE_T 9
#define SEND_DLY 2
#define BACK_DATA_LEN 15

unsigned char code msg_CMGF[]={"AT+CMGF=1\r\n"};
unsigned char code msg_CSCS[]={"AT+CSCS=\"GSM\"\r\n"};
unsigned char code msg_number[]={"AT+CMGS=\"18523591056\"\r\n"};   //���ź���
unsigned char code msg_content[]={"someone is break in! --From Home\r\n"};	//��������
static unsigned char recv_ok = 0;			//��1ʱ��ʾһ��ָ��������
static unsigned char i = 0;					//back_data ��������
unsigned char back_data[BACK_DATA_LEN];		//����ģ��ش����ݵĻ���

void delay(int x)
{  
	int i,j;
    for (i=0;i<x;i++)                  
       for(j=0;j<600;j++);
}

void init_seri()	//Uart��ʼ����������Ϊ9600
{
	TMOD = 0x20;			
	TH1 = 0xFd;
	TR1 = 1;
	SCON = 0x50;	   
	EA = 1;		//�����ж�
	ES = 1;		//�������ж�	
}

void bee_run(unsigned char n)	//����������Դ����������������ʱ��Ϊ��ֵBEE_T
{
   while(n--)
   {
		bee = 1;
		delay(CYCLE);
		bee = 0;
		delay(CYCLE);
   }
}

void seri_recv(void) interrupt 4	//���ն���ģ��ط�����
{	
	if(RI == 1)   					//��Ӳ�����յ�һ������ʱ��RI����λ
	{	 
		if(i==BACK_DATA_LEN) 
		{
			i = 0;
		} 
		if(SBUF == '\n' || SBUF == 0x20)	//�յ����л��߿ո��ʾһ��ָ���˵
		{
			recv_ok = 1;
		}
		back_data[i] = SBUF;
		RI = 0;   
		i++;		
	}
}

//�������back_data
void data_clear()
{
	 memset(back_data, 0, sizeof(back_data));
	 i = 0;
	 recv_ok = 0;
}
   	
//UART����ģ��
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

//��������Ƿ�ɹ�
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

//�����͵�����ָ��󣬶���ģ��ط���> ������ʾ����������ݡ��˺������ڼ�� ��>��
unsigned char check_send()
{
	if(recv_ok)
	{
		recv_ok = 0;
		if( back_data[0] == 0x3e )	//0x3e ASCII��ӦΪ ��>��
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

//����ģ���ڽ��յ�ָ��֮��Ὣָ��ط��������˺�����������ǰ�����еĻط�ָ��
void jump()
{  
	while( !recv_ok );
	recv_ok = 0;
	data_clear();
	while( !recv_ok );
	recv_ok = 0;
	data_clear();
}

//���ö��ŷ���ģʽ
void Set_Text_Mode(void)
{ 	
	send_str(msg_CMGF); 				  	//���ö���ΪTEXTģʽ
	jump();
	while( !check_ok() );
	P0 &= ~0x01;							//TEXTģʽ���óɹ�����һ��ָʾ������

	send_str(msg_CSCS); 					//���������ַ���Ϊ"GSM"
	jump();
	while( !check_ok() );
	P0 &= ~0x02;							//�ַ������óɹ����ڶ���ָʾ������
 
}

//���Ͷ���
void Send_Message(void)
{	  
	data_clear();
	send_str(msg_number); 
	jump();
	while( !check_send() );   
	P0 &= ~0x04;							//���ź������óɹ���������ָʾ������
	 
	send_str(msg_content);   
	send_char(0x1a);
	P0 &= ~0x08;							//���Ͷ��ųɹ������ĸ�ָʾ������
}

int main()
{
	init_seri();
	P0 = 0xff;								//ָʾ�Ƴ�ʼ��
	Set_Text_Mode();
	while(1)
	{
		if(red == PASS)						//�����⵽����ͨ��
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