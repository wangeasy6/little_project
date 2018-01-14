# Server_100 开发历程
##### 作为网络编程以及并发编程的练习。开发一个可支持并发100的服务器。这个文档用于记录这整个过程。

* CS模型的实现很简单，此处暂时使用TCP协议。但是关于服务器提供的服务以及并发模型还待思考。
* 暂定提供的服务为文件下载，也可带有Web服务器功能。
* Client 端连接之后，服务器重复发送一段数据，测试同时连接的数量。

* 直接返回头和 easy_test 的单循环模式，使用 Webbench 测试居然可以。以下是测试数据
easy-w@ubuntu:~/software/webbench-1.5$ webbench -c 1000 -t 60 http://192.168.111.129:9999/index.html
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.

Benchmarking: GET http://192.168.111.129:9999/index.html
1000 clients, running 60 sec.

Speed=514323 pages/min, 471462 bytes/sec.
Requests: 514323 susceed, 0 failed.

这里的并发，明显不是真正的并发，因为如果已经有了一个连接，我的服务器不可能再接受其他的连接，只能说可能是发送和结束的比较快。
在后续的测试中，要测试真正的并发数。就算是用 webbench， 应该在速度上也会有提升。

* 还是单循环模式，但是这次返回了数据，ls: 8.9K Nov 25 18:36 service.c
webbench -c 1000 -t 60 http://192.168.111.130:9998/service.c
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.

Benchmarking: GET http://192.168.111.130:9998/service.c
1000 clients, running 60 sec.

Speed=1163012 pages/min, 32782048 bytes/sec.
Requests: 1162653 susceed, 359 failed.

* 对比了多进程、多线程、IO多路复用技术，多线程是最科学的，所以首先做多线程的并发。
