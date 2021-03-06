#include "ds18b20.h" 

#define Ds18b20IO P0_7       //温度传感器引脚

/**
* @brief us延时函数
*
* @param k 延时参数
* @return 无
* @note 
*/
void Delay_us(unsigned int k)//us延时函数
{
    T1CC0L = 0x06; 
    T1CC0H = 0x00; 
    T1CTL = 0x02; 
    while(k)
    { 
        while(!(T1CNTL >= 0x04));
        k--;
    }
    T1CTL = 0x00;  //关闭定时器
}

/**
* @brief ms延时函数
*
* @param k 延时参数
* @return 无
* @note 
*/
void Delay_ms(unsigned int k)
{
    T1CC0L = 0xe8;
    T1CC0H = 0x03;
    T1CTL = 0x0a; //模模式 32分频
    while(k)
    {
        while(!((T1CNTL >= 0xe8)&&(T1CNTH >= 0x03)));
        k--;
    }
    T1CTL = 0x00; //关闭定时器
}

/**
* @brief s延时函数
*
* @param k 延时参数
* @return 无
* @note 
*/
void Delay_s(unsigned int k)
{
    while(k)
    {
        Delay_ms(1000);
        k--;
    }
}

//时钟频率为32M
/**
* @brief Ds18b20延时函数
*
* @param k 延时参数
* @return 无
* @note 
*/
void Ds18b20Delay(unsigned int k)
{
    unsigned int i,j;
    for(i=0;i<k;i++)
        for(j=0;j<2;j++);
}

/**
* @brief 设置端口为输入
*
* @param 无
* @return 无
* @note 
*/
void Ds18b20InputInitial(void)//设置端口为输入
{
    P0DIR &= 0x7f;
}

/**
* @brief 设置端口为输出
*
* @param 无
* @return 无
* @note 
*/
void Ds18b20OutputInitial(void)//设置端口为输出
{
    P0DIR |= 0x80;
}

/**
* @brief ds18b20初始化  初始化成功返回0x00，失败返回0x01
*
* @param 无
* @return 无
* @note 
*/
unsigned char Ds18b20Initial(void)
{
    unsigned char Status = 0x00;
    unsigned int CONT_1 = 0;
    unsigned char Flag_1 = 1;
    Ds18b20OutputInitial(); //DS18B20输出初始化
    Ds18b20IO = 1;      //DQ复位
    Delay_us(260);  //稍做延时
    Ds18b20IO = 0;      //单片机将DQ拉低
    Delay_us(750);  //精确延时 大于 480us 小于960us
    Ds18b20IO = 1;      //拉高总线
    Ds18b20InputInitial();//设置IO输入
    while((Ds18b20IO != 0)&&(Flag_1 == 1))//等待ds18b20响应，具有防止超时功能
    {                                      //等待约60ms左右
        CONT_1++;
        Delay_us(10);                //延时
        if(CONT_1 > 8000)Flag_1 = 0;     //标志位置0
        Status = Ds18b20IO;
    }
    Ds18b20OutputInitial();  //设置IO输出 
    Ds18b20IO = 1;      //拉高总线
    Delay_us(100);       //延时
    return Status;       //返回初始化状态
}

/**
* @brief Ds18b20写函数
*
* @param infor
* @return 无
* @note 
*/
void Ds18b20Write(unsigned char infor)
{
    unsigned int i;
    Ds18b20OutputInitial();
    for(i=0;i<8;i++)
    {
        if((infor & 0x01))      
        {
            Ds18b20IO = 0;
            Delay_us(6);
            Ds18b20IO = 1;
            Delay_us(50);
        }
        else
        {
            Ds18b20IO = 0;
            Delay_us(50);
            Ds18b20IO = 1;
            Delay_us(6);
        }
        infor >>= 1;
    }
}

/**
* @brief Ds18b20读函数
*
* @param 无
* @return 无
* @note 
*/
unsigned char Ds18b20Read(void)
{
    unsigned char Value = 0x00;
    unsigned int i;
    Ds18b20OutputInitial();
    Ds18b20IO = 1;
    Ds18b20Delay(10);
    for(i=0;i<8;i++)
    {
        Value >>= 1; 
        Ds18b20OutputInitial();
        Ds18b20IO = 0;// 给脉冲信号
        Delay_us(3);
        Ds18b20IO = 1;// 给脉冲信号
        Delay_us(3);
        Ds18b20InputInitial();
        if(Ds18b20IO == 1) Value |= 0x80;
        Delay_us(15);
    } 
    return Value;
}

/**
* @brief 温度读取函数
*
* @param 无
* @return 无
* @note 
*/
unsigned char ReadDs18B20(void) 
{
    unsigned char V1,V2;   //定义高低8位 缓冲
    unsigned char temp;    //定义温度缓冲寄存器
    
    Ds18b20Initial();
    Ds18b20Write(0xcc);    // 跳过读序号列号的操作
    Ds18b20Write(0x44);    // 启动温度转换
    
    Ds18b20Initial();
    Ds18b20Write(0xcc);    //跳过读序号列号的操作 
    Ds18b20Write(0xbe);    //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
    
    V1 = Ds18b20Read();    //低位
    V2 = Ds18b20Read();    //高位
    temp = ((V1 >> 4)+((V2 & 0x07)*16)); //转换数据 

    return temp;
}

/**
* @brief Ds18b20延时函数
*
* @param k 延时参数
* @return 无
* @note 
*/
//温度读取函数 带1位小数位
float floatReadDs18B20(void) 
{
    unsigned char V1,V2;   //定义高低8位 缓冲
    unsigned int temp;     //定义温度缓冲寄存器
    float fValue;
    Ds18b20Initial();
    Ds18b20Write(0xcc);    // 跳过读序号列号的操作
    Ds18b20Write(0x44);    // 启动温度转换
    
    Ds18b20Initial();
    Ds18b20Write(0xcc);    //跳过读序号列号的操作 
    Ds18b20Write(0xbe);    //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
    
    V1 = Ds18b20Read();    //低位
    V2 = Ds18b20Read();    //高位
    //temp = ((V1 >> 4)+((V2 & 0x07)*16)); //转换数据 
    temp=V2*0xFF+V1;
    fValue = temp*0.0625;
    
    return fValue;
}