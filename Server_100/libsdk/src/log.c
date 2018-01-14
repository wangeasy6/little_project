#include "log.h"
#include "times.h"
#include <stdarg.h>
#include <string.h>

#define LOG_FILE_NAME "log.txt"
#define LOG_INI_FILE "log.ini"

static uchar g_log_tag_str[][20] =
{
      "T_INFO",
      "T_DEBUG",
      "T_WARNING",
      "T_ERROR",
      "T_ALL",
};

#ifdef USE_LOG_MODULE
static uchar g_log_modu_str[][20] =
{
      "M_MAIN",
      "M_INIT",
      "M_SEVER",
      "M_MEDIA",
      "M_LED",
      "M_MAX",
};
#endif

#ifdef USE_LOG_MODULE
static uchar g_level[LOG_M_MAX] = {0};
#else
static uchar g_level = 0;
#endif

#ifdef USE_LOG_MODULE
void log_set_level(const uint mod, const uchar level)
{
      printf("log_set_level: mod=%d, level=%d\n", mod, level);
      if (mod >= LOG_M_MAX)
      {
            return ;
      }

      g_level[mod] = level;
      return ;
}

void log_init(void)
{
      FILE *fd = fopen(LOG_INI_FILE, "r");
      if (fd == NULL)
      {
            perror("fopen LOG_INI_FILE");
            return ;
      }

      int level = 0;
      char buf[128] = {0};
      size_t read_len = 0, i, j;

      while (!feof(fd))
      {
            fgets(buf, sizeof(buf), fd);
            read_len = strlen(buf);

            for(i = 0; i <= LOG_M_MAX; i++)
            {
                  if(strstr(buf, g_log_modu_str[i]))
                  {
                        for(j = 0; j < read_len; j++)
                        {
                              if(buf[j] == '=')
                              {
                                    j++;
                                    if(j < read_len)
                                    {
                                          level = atoi(buf+j);
                                    }
                                    else
                                    {
                                          break;
                                    }

                                    switch(level)
                                    {
                                          case LOG_T_INFO:
                                          case LOG_T_DEBUG:
                                          case LOG_T_WARNING:
                                          case LOG_T_ERROR:
                                          case LOG_T_ALL:
                                                if(i == LOG_M_MAX)
                                                {
                                                      for(i = 0; i < LOG_M_MAX; i++)
                                                      {
                                                            log_set_level(i, level);
                                                      }

                                                      return ;
                                                }
                                                log_set_level(i, level);
                                                break;
                                          default:
                                                break;
                                    }
                                    break;
                              }
                        }
                        break;
                  }
            }
            memset(buf, 0, sizeof(buf));
      }

      return ;
}

void log_p(const uint mod, const uchar level,const char *va_alist, ...)
{
      if (mod >= LOG_M_MAX || !(g_level[mod] & level))
      {
            return ;
      }

      va_list args;
      int pos = 0;
      char buf[128] = {0};

#ifdef USE_LOG_TIME
      if ( time_GetUTCTime_s(buf, sizeof(buf)) <= 0)
      {
            printf("[%s] [%s] [%d] time_GetUTCTime_s error\n", __FILE__, __FUNCTION__, __LINE__);
      }
      pos = strlen(buf);
#endif

      snprintf(buf+pos, sizeof(buf+pos), "[%s]", g_log_modu_str[mod]);
      switch (level & g_level[mod])
      {
            case LOG_T_INFO:
                  strcat(buf, "[T_INFO] ");
                  break;

            case LOG_T_DEBUG:
                  strcat(buf, "[T_DEBUG] ");
                  break;

            case LOG_T_WARNING:
                  strcat(buf, "[T_WARNING] ");
                  break;

            case LOG_T_ERROR:
                  strcat(buf, "[T_ERROR] ");
                  break;

            default:
                  return ;
      }

      fprintf(stderr, buf, 0);

      va_start(args, va_alist);
      vfprintf(stderr, va_alist, args);
      va_end(args);

      fflush(stderr);

      return ;
}

int log_write_file(char* buf, size_t buf_len, char* file_name)
{
      return file_write(buf, buf_len, file_name, "a+");
}

void log_to_file(const uint mod, const uchar level,const char *va_alist, ...)
{
      if (mod >= LOG_M_MAX || !(g_level[mod] & level))
      {
            return ;
      }

      va_list args;
      int pos = 0;
      char buf[128] = {0};

#ifdef USE_LOG_TIME
      if ( time_GetUTCTime_s(buf, sizeof(buf)) <= 0)
      {
            printf("[%s] [%s] [%d] time_GetUTCTime_s error\n", __FILE__, __FUNCTION__, __LINE__);
      }
      pos = strlen(buf);
#endif

      snprintf(buf+pos, sizeof(buf+pos), "[%s]", g_log_modu_str[mod]);
      switch (level & g_level[mod])
      {
            case LOG_T_INFO:
                  strcat(buf, "[T_INFO] ");
                  break;

            case LOG_T_DEBUG:
                  strcat(buf, "[T_DEBUG] ");
                  break;

            case LOG_T_WARNING:
                  strcat(buf, "[T_WARNING] ");
                  break;

            case LOG_T_ERROR:
                  strcat(buf, "[T_ERROR] ");
                  break;

            default:
                  return ;
      }

      log_write_file(buf, strlen(buf), LOG_FILE_NAME);

      va_start(args, va_alist);
      memset(buf, 0, sizeof(buf));
      vsnprintf(buf, sizeof(buf), va_alist, args);
      log_write_file(buf, strlen(buf), LOG_FILE_NAME);
      va_end(args);

      return ;
}


#else
void log_set_level(const unsigned char level)
{
      g_level = level;
}

void log_p(const uchar level,const char *va_alist, ...)
{
      if(!(level & g_level))
      {
            return ;
      }

      va_list args;

      switch (level & g_level)
      {
            case LOG_T_INFO:
                  printf("[INFO] ");
                  break;

            case LOG_T_DEBUG:
                  printf("[DEBUG] ");
                  break;

            case LOG_T_WARNING:
                  printf("[WARNING] ");
                  break;

            case LOG_T_ERROR:
                  printf("[ERROR] ");
                  break;

            default:
                  return ;
      }

      va_start(args, va_alist);
      vfprintf(stderr, va_alist, args);
      va_end(args);
      fflush(stderr);

      return ;
}
#endif