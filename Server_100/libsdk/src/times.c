// 关于时间的
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int time_GetUTCTime_s(char* buf, size_t buf_len)
{
      if (buf == NULL || buf_len <= 0)
      {
            return 0;
      }

      int copy_len = 0;
      struct timeval tmval;
      struct tm tm_now = {0};
      time_t t_now = 0;

      gettimeofday(&tmval, NULL);
      t_now = tmval.tv_sec;
      localtime_r( &t_now, &tm_now);

      copy_len = snprintf( buf, buf_len-1, "[%02d:%02d:%02d.%03ld]",
                  tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, tmval.tv_usec/1000 );

      return copy_len;
}

int time_GetUTCTime(char* buf, size_t buf_len)
{
      if (buf == NULL || buf[0] == 0 || buf_len < 0)
      {
            return 0;
      }

      struct timeval tmval;
      struct tm tm_now = {0};
      time_t t_now = 0;

      gettimeofday(&tmval, NULL);
      t_now = tmval.tv_sec;
      localtime_r( &t_now, &tm_now);

      snprintf( buf, buf_len-1, "%02d-%02d-%02d",
          tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);

      return 1;
}