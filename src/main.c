#include "public.h"
#include "tcpconnect.h"
#include "controlsql.h"
#include "controlmod.h"
#include "search.h"

struct tcp_http_s
{
	int confd;
	int nready;
	int sockfd;
	struct epoll_event epl_evret[1024];
}tcp_http_data;


sem_t sem;
pthread_mutex_t lock;



struct tcp_confd_s http_url_confd_map;

void *pthread_1(void *arg)
{
	int i = 0;
	int confd = *((int *)arg);
						
	pthread_mutex_unlock(&lock);		

	while(1)
	{
		printf("pthread_1 start \n:");
		if(-1 == ControlMod(confd))
		{
			close(confd);
			sem_post(&sem);
			printf("ControlMod of pthread_1 free success!\n");
			return NULL;
		}
		else
		{
			close(confd);
		}
	}

	return NULL;
}

int main(int argc, const char *argv[])
{

	int i = 0;
	int sockfd = 0;
	int confd = 0;
	int evfd = 0;
	sqlite3 *pdb = NULL;
	struct epoll_event epl_evret[__SHOPPING_MAXEPOLL__];
	struct epoll_event epl_evfd;
	int nready = 0;	

	pid_t pid;
	pthread_attr_t attr;
	pthread_t tid;
	
//	signal(SIGPIPE, SIG_DFL);


	pdb = SqlInit(__SHOPPING_DB__);
	if (NULL == pdb)
	{
		perror("fail to SqlInit");
		return -1;
	}
	char TmpBuff[1024] = "%E8%AF%BA%E5%9F%BA%E4%BA%9A+KD876+%E8%80%B3%E6%9C%BA";
	//	Search(pdb, TmpBuff);

	//	LoginUpNewEmail(pdb, "email=123%40123&password=123&confirm_password=123");
	//	return 0;


	GOODS_S goods1;

	goods1 = ExecPostToGoods("goodsid=1&shopprice=1388.0", pdb);
	PayForGoods(&goods1, "1219545479@qq.com");
	PayForGoods(&goods1, "1219545479@qq.com");
	//	return 0;
	sockfd = TcpInit();
	if (-1 == sockfd)
	{
		perror("fail to TcpInit");
		return -1;
	}

	evfd = epoll_create(__SHOPPING_MAXEPOLL__);
	epl_evfd.events = EPOLLIN;
	epl_evfd.data.fd = sockfd;
	epoll_ctl(evfd, EPOLL_CTL_ADD, sockfd, &epl_evfd);

	pthread_mutex_init(&lock, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	sem_init(&sem, 0, 300);

	LoginUsrInit();

	while (1)
	{

#if 0 
		confd = ConnectBrowser(sockfd);
		if (-1 == confd)
		{
			continue;
		}
		else
		{

			if (-1 == ControlMod(confd))
			{
				continue;
			}
			close(confd);
		}
		continue;
#endif

		nready = epoll_wait(evfd, epl_evret, __SHOPPING_MAXEPOLL__, -1);
		if (-1 == nready)
		{
			perror("fail to epoll_wait");
			continue;	
		}
		else
		{
			for (i = 0; i < nready; i++)
			{
				if (sockfd == epl_evret[i].data.fd)
				{

					pthread_mutex_lock(&lock);
					sem_wait(&sem);

					confd = ConnectBrowser(sockfd);
					if (-1 == confd)
					{
						printf("main confd == -1\n");
						sem_post(&sem);
						pthread_mutex_unlock(&lock);		
						continue;
					}
					else
					{
						pthread_create(&tid, &attr, pthread_1, &confd);


						PRINTuserList();
						PRINTuserList();
						PRINTuserList();

					}
				}
			}
		}
	}


	sem_destroy(&sem);
	close(sockfd);
	sqlite3_close(pdb);
	pthread_mutex_destroy(&lock);
	pthread_attr_destroy(&attr);

	return 0;
}
