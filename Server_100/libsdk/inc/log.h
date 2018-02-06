#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

//#define USE_LOG_MODULE

typedef unsigned int uint;
typedef unsigned char uchar;

#define log_in() log_p(INFO,"[%s] [%s] [%d] In\n", __FILE__, __FUNCTION__, __LINE__)
#define log_at() log_p(INFO,"[%s] [%s] [%d] At\n", __FILE__, __FUNCTION__, __LINE__)
#define log_out() log_p(INFO,"[%s] [%s] [%d] Out\n", __FILE__, __FUNCTION__, __LINE__)

#ifdef USE_LOG_MODULE
      #define log_fl(MOD, LEVEL, FORMAT, ARGS...) \
            log_p(MOD,LEVEL,"[%s][%s][%d] "FORMAT, __FILE__, __FUNCTION__, __LINE__, ##ARGS )
#else
      #define log_fl(LEVEL, FORMAT, ARGS...) \
            log_p(LEVEL,"[%s][%s][%d] "FORMAT, __FILE__, __FUNCTION__, __LINE__, ##ARGS )
#endif

typedef enum
{
      LOG_M_MAIN = 0,
      LOG_M_INIT,
      LOG_M_SEVER,
      LOG_M_MEDIA,
      LOG_M_LED,

      LOG_M_MAX
}LOG_MODULE;

typedef enum
{
      LOG_T_INFO  = 0x01,
      LOG_T_DEBUG = 0x02,
      LOG_T_WARNING = 0x04,
      LOG_T_ERROR = 0x08,

      LOG_T_ALL   = 0xff
}LOG_TAG;

#ifdef USE_LOG_MODULE
void log_set_level(const uint mod, const uchar level);             //设置打印级别
void log_p(const uint mod, const uchar level,const char *, ...);   //带级别的打印
#else
void log_set_level(const uchar level);             //设置打印级别
void log_p(const uchar level,const char *, ...);   //带级别的打印
#endif
int log_init(void);

#endif
