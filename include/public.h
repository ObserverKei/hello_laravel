#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sqlite3.h"
#include "time.h"
#include "signal.h"
#include "sys/wait.h"
#include "sys/epoll.h"
#include "pthread.h"
#include "semaphore.h"

//#define __HTTP_SERVER_IP__		"192.168.0.168"
#define __HTTP_SERVER_IP__		"192.168.0.188"
#define __HTTP_SERVER_PORT__	8080

#define __HTTP_SERVER_HOME_PATH__ "../home"

#define __SHOPPING_STR_LEN_16__		16
#define __SHOPPING_STR_LEN_32__		32
#define __SHOPPING_STR_LEN_128__	128
#define __SHOPPING_STR_LEN_512__	512
#define __SHOPPING_STR_LEN_1024__	1024
#define __SHOPPING_STR_LEN_2048__	2048
#define __SHOPPING_STR_LEN_4096__	4096

#define __SHOPPING_SQL_MAX_LINE_50__	50
#define __SHOPPING_MAXEPOLL__		4096
struct tcp_confd_s
{
	int confd;
	char url[__SHOPPING_STR_LEN_2048__];
};


extern struct tcp_confd_s http_url_confd_map;
extern long long unsigned int MaxGoodsCnt;

#endif
