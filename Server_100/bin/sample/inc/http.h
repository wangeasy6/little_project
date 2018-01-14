#ifndef __HTTP_H__
#define __HTTP_H__

#define HTTP_HEADER_200 \
"HTTP/1.0 200 OK\n\
Server:EasyW\n\
Content-Type:text/html\r\n\r\n"
//Connection:keep-alive\r\n
//Content-Length:\r\n

char * http_parser_GetFileName(const char* buf,const int buf_len);
int tcp_send_File(const int connfd,const char* file_name);
int tcp_send_data(const int connfd,const char* buf,const int buf_len);


#endif
