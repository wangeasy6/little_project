#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define PRINTF_SIGN 1
#define SEND_INFO 0
#define DEBUG 0

#define func_in() print(INFO,"[%s] [%d] In\n",__FUNCTION__,__LINE__)
#define func_at() print(INFO,"[%s] [%d] At\n",__FUNCTION__,__LINE__)
#define func_out() print(INFO,"[%s] [%d] Out\n",__FUNCTION__,__LINE__)

typedef enum
{
      LOG_INFO  = 0x01,
      LOG_DEBUG = 0x02,
      LOG_ERROR = 0x04,

      LOG_ALL   = 0xff
}LOG_TAG;

void set_level(const unsigned char level);                         //设置打印级别
void print(const unsigned char level,const char *, ...);   //带级别的打印


#endif
