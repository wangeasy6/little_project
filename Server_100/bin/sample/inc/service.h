#ifndef __WEB__
#define __WEB__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <pthread.h>

#include "basic_define.h"
#include "log.h"
#include "get_ip.h"
#include "http.h"

#define MAX_THREAD 10

int service_init(void);
int service_run(void);

#endif
