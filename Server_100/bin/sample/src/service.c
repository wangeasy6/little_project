#include "service.h"
typedef struct __SHARED
{
      volatile unsigned char status[MAX_THREAD];
      volatile int connfd[MAX_THREAD];
      pthread_mutex_t mutex;
} SHARED;

#define Max_Send_Length 8*1024
static int g_socket_fd;
SHARED g_data;

int service_init(void)
{
      int s_ret;
      int opt = 1;
      struct sockaddr_in servaddr;
      socklen_t addrlen;
      int port = 9999;

      g_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
      if (g_socket_fd == -1)
      {
            perror("server->socket error:");
            return FAILED;
      }

      log_p(LOG_T_INFO, "Socket created\n");

      int yes = 1;
      setsockopt(g_socket_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof(yes));
      int nSendBufLen = 32*1024;
      setsockopt( g_socket_fd, SOL_SOCKET, SO_SNDBUF, ( const char* )&nSendBufLen, sizeof( int ) );

      memset(&servaddr, 0, sizeof(struct sockaddr_in));
      servaddr.sin_family = AF_INET;
      servaddr.sin_port = htons(port);
      servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

      s_ret = bind(g_socket_fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));
      while (s_ret == -1)
      {
            perror("server->bind:");
            port--;
            servaddr.sin_port = htons(port);
            s_ret = bind(g_socket_fd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));
            if (port < 9000)
            {
                  close(g_socket_fd);
                  return FAILED;
            }
      }
      log_p(LOG_T_INFO,"[bind] IP:Port  %s:%d\n", get_ip(), port);

      s_ret = listen(g_socket_fd, MAX_THREAD);
      if (s_ret == -1)
      {
            perror("server->listen:");
            close(g_socket_fd);
            return FAILED;
      }
      log_p(LOG_T_INFO,"server listend\n");

      pthread_mutex_init(&g_data.mutex,NULL);
      return SUCCESS;
}

int send_data(const char *data,const unsigned int length)
{
      static int LEN,len,pos,i,test;
      static ssize_t ret;
      static char send_buf[Max_Send_Length] = {0};
      LEN = length;

      for(i = 0; i < MAX_THREAD ; i++)
      {
            if(g_data.status[i] != UN_USE)
            {
                  if ( LEN < 1024 )
                  {
                        pthread_mutex_lock(&g_data.mutex);
                        ret = write(g_data.connfd[i], data, strlen(data));
                        pthread_mutex_unlock(&g_data.mutex);

                        if (-1 == ret )
                        {
                              perror("server->write");
                              return FAILED;
                        }
                  }
                  else
                  {
#if DEBUG
                        if(test == 1)
                              continue;
                        test = 1;
#endif
                        char header[10] = {0};
                        log_p(LOG_T_DEBUG,"send pic,size:%d\n",LEN);
                        len = LEN;
                        snprintf(header, sizeof(header), "H%d\r\n", LEN);
                        pthread_mutex_lock(&g_data.mutex);
                        ret = write(g_data.connfd[i], header, sizeof(header));
                        for(pos = 0,ret = 0; len > 0; len = len - ret)
                        {
#ifdef DEBUG
                              memset(send_buf, 0, sizeof(send_buf));
                              if( Max_Send_Length <= len )
                              {
                                    memcpy( send_buf, data+pos, Max_Send_Length );
                                    ret = write( g_data.connfd[i], send_buf, Max_Send_Length );
                              }
                              else
                              {
                                    memcpy(send_buf, data+pos,len);
                                    ret = write( g_data.connfd[i], send_buf, len );
                              }
#else
                              pthread_mutex_lock(&g_data.mutex);
                              ret = write( g_data.connfd[i], data+pos, len);
                              pthread_mutex_unlock(&g_data.mutex);
#endif
                              if (-1 == ret )
                              {
                                    perror("server->write");
                                    return FAILED;
                              }
                              log_p(LOG_T_DEBUG,"%x %x %x .len:%d  ret:%d  pos:%d \r\n",send_buf[0],send_buf[1],send_buf[2], len, ret, pos);
                              pos += ret;
                        }

                        pthread_mutex_unlock(&g_data.mutex);
                        log_p(LOG_T_DEBUG,"%x %x %x .len:%d  ret:%d  pos:%d \r\n",send_buf[0],send_buf[1],send_buf[2], len, ret, pos);
                        return pos;
                  }

            }
            else
                  continue;
      }
      return SUCCESS;
}

void *connect_process(void *arg)
{
      char buf[1024];
      int index = (int*)arg - g_data.connfd;
      int ret;
      log_p(LOG_T_INFO, "connfd index:%d In\n",index);
      while(1)
      {
            memset(buf,0,sizeof(buf));

            int ret = recv(g_data.connfd[index], buf, sizeof(buf), 0);
            {
                  if (-1 == ret)
                  {
                        log_p(LOG_T_ERROR, "data error!\n");
                        break;
                  }
                  if (0 == ret)
                  {
                        log_p(LOG_T_ERROR, "user shutdown!\n");
                        break;
                  }
            }
      }

      pthread_exit(NULL);

}

int service_run(void)
{
      int connfd;
      socklen_t addrlen;
      struct sockaddr_in cliaddr;
      char buf[1024];
      log_p(LOG_T_INFO, "service is running\r\n");

      while (1)
      {
            addrlen = sizeof(struct sockaddr_in);
            memset(&cliaddr, 0, sizeof(struct sockaddr_in));

            connfd = accept(g_socket_fd, (struct sockaddr *)&cliaddr, &addrlen);
            if (connfd == -1)
            {
                  //perror("accept error");
                  continue;
            }

#ifdef PRINTF_SIGN
            memset(buf,0,sizeof(buf));
            log_p(LOG_T_DEBUG, "accept info: client addr is %s,port %d\n", \
                   inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)),\
                   ntohs(cliaddr.sin_port));
#endif

            memset(buf,0,sizeof(buf));
            if(recv(connfd,buf,sizeof(buf),0)==-1)
            {
                  perror("recv");
                  exit(-1);
            }

            if(!strncmp(buf,"quit",4))
                  break;
            log_p(LOG_T_DEBUG, "received a message :%s",buf);

            if ( strstr(buf , "HTTP"))
            {
                  log_p(LOG_T_DEBUG, "recv a http request\n");
                  send(connfd,HTTP_HEADER_200,strlen(HTTP_HEADER_200),0);
                  if (strstr(buf, "GET"))
                  {
                        log_p(LOG_T_DEBUG, "recv a http GET request\n");
                        tcp_send_File(connfd, http_parser_GetFileName(buf,sizeof(buf)));
                  }
            }
#ifdef TEST
            memset(buf,0,sizeof(buf));
            strcpy(buf,"easy test\n");
            send(connfd,buf,strlen(buf),0);
#endif
            close(connfd);

      }

      close(g_socket_fd);
      return 1;
}


/*
int service_run(void)
{
      socklen_t addrlen;
      struct sockaddr_in cliaddr;
      char buf[1024];
      pthread_t newthread;
      static int index;
      log_p(LOG_T_INFO, "service is running\r\n");

      while (1)
      {
            index = get_node();
            if ( index == UN_USE )
            {
                  continue;
            }
            addrlen = sizeof(struct sockaddr_in);
            memset(&cliaddr, 0, sizeof(struct sockaddr_in));

            g_data.connfd[index] = accept(g_socket_fd, (struct sockaddr *)&cliaddr, &addrlen);
            if (g_data.connfd[index] == -1)
            {
                  perror("accept error");
                  if( free_node(index) == SUCCESS || free_node(index) == SUCCESS );
                  log_pf("free node failed!\n");
                  continue;
            }

#ifdef PRINTF_SIGN
            memset(buf,0,sizeof(buf));
            log_pf("accept info: client addr is %s,port %d\n", \
                   inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)),\
                   ntohs(cliaddr.sin_port));
#endif

            if ( pthread_create(&newthread , NULL, (void *)connect_process, (void *)&g_data.connfd[index]) )
                  perror("connect pthread_create");
            else
            {
                  g_data.status[index] = USED;
            }
      }

      pthread_mutex_destroy(&g_data.mutex);
      close(g_socket_fd);
      return 1;
}
*/
