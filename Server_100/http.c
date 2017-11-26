#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "http.h"
#include "log.h"
#include "basic_define.h"

#define FILE_LEN_MAX 255
#define BUF_SIZE 1024*8

int http_parser_req();

char * http_parser_GetFileName(const char* buf,const int buf_len)
{
      int copy_len = 0, cnt = 0;
      static char file_name[FILE_LEN_MAX] = {0};
      memset(file_name, 0, FILE_LEN_MAX);

      if (buf == NULL)
      {
            print(LOG_ERROR,"[%s,%d] Parms Error\n", __FUNCTION__, __LINE__);
            return FAILED;
      }

      cnt = 5;
      while(copy_len < FILE_LEN_MAX && cnt < buf_len)
      {
            if(*(buf+cnt) == ' ' || *(buf+cnt) == '&' || *(buf+cnt) == '?')
            {
                  break;
            }
            copy_len ++;
            cnt ++;
      }
      if(copy_len > 0)
      {
            strncpy(file_name, buf+5, copy_len);
      }

      return file_name;
}

int tcp_send_Data(const int connfd,const char* buf,int buf_len)
{
      ssize_t check_len = 0, send_len = 0, cur = 0;
      while(buf_len > 0)
      {
            //send_len = 1366 > buf_len? buf_len
            check_len = send(connfd, buf+cur, buf_len, 0);
            cur += check_len;
            buf_len -= check_len;
            print(LOG_DEBUG, "check_len:%d, buf_len:%d, cur:%d\n", check_len, buf_len, cur);
      }
}

int tcp_send_File(const int connfd,const char* file_name)
{
      print(LOG_DEBUG,"[%s, %d] file_name : %s\n", __FUNCTION__, __LINE__, file_name);
      static FILE *fd = NULL;
      static char buf[BUF_SIZE] = {0}, use_once = 1;
      size_t read_len = 0;
if(use_once)
{use_once = 0;
      if (file_name == NULL || file_name[0] == 0)
      {
            print(LOG_ERROR,"[%s,%d] Parms Error\n", __FUNCTION__, __LINE__);
            return FAILED;
      }

      if((fd = fopen(file_name, "r")) == NULL)
      {
            print(LOG_ERROR, "[%s,%d] fopen:%s Failed\n", __FUNCTION__, __LINE__, file_name);
            perror(NULL);
            return FAILED;
      }
}
      do
      {
            read_len = fread(buf, 1, sizeof(buf), fd);
            if (read_len > 0)
            {
                  tcp_send_Data(connfd, buf, read_len);
            }
      }while(read_len > 0);
      fseek(fd, 0L, SEEK_SET);

      return SUCCESS;
}

