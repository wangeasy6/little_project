#include "log.h"
#include <stdarg.h>
static unsigned char g_level = 0;

void set_level(const unsigned char level)
{
      g_level = level;
}

void print(const unsigned char level,const char *va_alist, ...)
{
      if(!(level & g_level))
            return ;

      va_list args;

      switch (level & g_level)
      {
            case LOG_INFO:
                  printf("[INFO] ");
                  break;

            case LOG_DEBUG:
                  printf("[DEBUG] ");
                  break;

            case LOG_ERROR:
                  printf("[ERROR] ");
                  break;

            default:
                  return ;
      }

      va_start(args, va_alist);
      vfprintf(stdout, va_alist, args);
      va_end(args);
}
//perror -> printe