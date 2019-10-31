#include "public.h"
#include "tcpconnect.h"
#include "controlsql.h"
#include "createhtml.h"
#include "search.h"
#include "controlmod.h"

REQUEST_NODE_INFO_S RequestNodeInfo[] = {
	{"../home/index.html", "<!--index_div_cargo_start-->", "<!--index_div_cargo_end-->", "../home/index_div_cargo.html"},
//	{"../home/index.html", "<!--index_div_login_start-->", "<!--index_div_login_start-->", "../home/index_div_login.html"},
};

char RequestDivInfo[][__SHOPPING_STR_LEN_128__] = {
	"<!--index_div_login_state-->"
};
char BinFile[][32] = {
	".jpg",
	".gif",
	".png",
	".css",
	".download",
};

int TcpInit(void)
{
	int Ret = 0;
	int sockfd = 0;
	struct sockaddr_in sAddrServersin;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("fail to socket");
		return -1;
	}
	sAddrServersin.sin_family = AF_INET;
	sAddrServersin.sin_port = htons(__HTTP_SERVER_PORT__);
	sAddrServersin.sin_addr.s_addr = inet_addr(__HTTP_SERVER_IP__);

	Ret = bind(sockfd, (struct sockaddr *)&sAddrServersin, sizeof(sAddrServersin));
	if (-1 == Ret)
	{
		perror("fail to bind");
		return -1;
	}

	return sockfd;
}


int ConnectBrowser(int sockfdArg)
{
	int Ret = 0;
	int confd;

	Ret = listen(sockfdArg, 10);
	if (-1 == Ret)
	{
		perror("fail to listen");
		return -1;
	}

	confd = accept(sockfdArg, NULL, NULL);
	if (-1 == confd)
	{
		perror("fail to accept");
		return -1;
	}

	return confd;
}


REQUESTHEAD_S *GetRequest(int confdArg)
{
	int i = 0;
	int Ret = 0;
	char *pTmpKey = NULL;
	char *pTmpStar = NULL;
	char *pTmpEnd = NULL;
	char RequestBuff[__SHOPPING_STR_LEN_4096__] = {0};
	REQUESTHEAD_S *prequest_head = NULL;

	prequest_head = malloc(sizeof(REQUESTHEAD_S));
	memset(prequest_head, 0, sizeof(REQUESTHEAD_S));

	Ret = recv(confdArg, RequestBuff, sizeof(RequestBuff), 0);
	if (-1 == Ret)
	{
		perror("fail to recv");
		return NULL;
	}
	printf("==================== RECV: =====================\n %s\n", RequestBuff);
	if ( ! strcmp(RequestBuff, ""))
	{
		return NULL;
	}


	if (NULL != (pTmpKey = strstr(RequestBuff, "search=")))
	{
		pTmpKey += strlen("search=");
		strcpy(prequest_head->post, strtok(pTmpKey, "&"));
	}
	else if (NULL != (pTmpKey = strstr(RequestBuff, "&END=")))
	{
		pTmpStar = strstr(RequestBuff, "\r\n\r\n");
		memcpy(prequest_head->post, pTmpStar, pTmpKey - pTmpStar);
	}
	if (NULL != (pTmpKey = strstr(RequestBuff, "Cookie")))
	{
		pTmpStar = pTmpKey + strlen("Cookie:");
		pTmpEnd = strstr(pTmpStar, "\r\n");
		strncpy(prequest_head->cookie, pTmpStar, pTmpEnd - pTmpStar);
		prequest_head->cookie[pTmpEnd - pTmpStar] = '\0';

		printf("pTmpKeyCookie: %s\n", pTmpKey);

	}

	strcpy(prequest_head->key, strtok(RequestBuff, " "));
	strcpy(prequest_head->url, strtok(NULL, " "));

	printf("key: %s\nurl: %s\ncookie: %s\n", prequest_head->key, prequest_head->url, prequest_head->cookie);
	if (NULL != prequest_head->post)
	{
		printf("POST Data: %s\n", prequest_head->post);
	}
	return prequest_head;
}

int ResponseHead(int confdArg)
{
	int i = 0;
	int Ret = 0;
	char responbuff[__SHOPPING_STR_LEN_4096__] = {0};

	sprintf(responbuff, "HTTP/1.1 200 OK\r\n");
	sprintf(responbuff, "%sServer: a light wight web server by linux\r\n", responbuff);

	if (1 == LoginSuccess)
	{
		for (i = 0; i < sizeof(login_usr_list) / sizeof(login_usr_list[0]); i++)
		{
			if (__LOGIN_USR_STAT_COOKIE__ == login_usr_list[i].stat)
			{
				break;
			}
		}
		sprintf(responbuff, "%sSet-Cookie: \"name=%s\"\r\n", responbuff, login_usr_list[i].cookie);
		login_usr_list[i].stat = __LOGIN_USR_STAT_ON__;
	}

	sprintf(responbuff, "%sConnection: keep-alive\r\n\r\n", responbuff);

	Ret = send(confdArg, responbuff, strlen(responbuff), 0);
	if (-1 == Ret)
	{
		perror("fail to send");
		return -1;
	}

	return 0;
}

char *IsRequestNodeInfd(char *TmpTcpData)
{
	if (NULL == TmpTcpData)
	{
		perror("fail to IsRequestNodeInfd of TmpTcpData is NULL");
		return NULL;
	}
	int i = 0;
	char *pTmpKey;
	int RequestNodeInfoLen = sizeof(RequestNodeInfo) / sizeof(RequestNodeInfo[0]);


	for (i = 0; i < RequestNodeInfoLen; i++)
	{
		if (NULL != (pTmpKey = strstr(TmpTcpData, RequestNodeInfo[i].start)))
		{
			printf("=========__LOGIN_USR_STAT_COOKIE__2\n");
			return RequestNodeInfo[i].filename;
		}
	}

	return NULL;
}

int IsUnASCIIStr(char *StrArg)
{
	int i = 0;
	for (i = 0; i < sizeof(BinFile) / sizeof(BinFile[0]); i++)
	{
		if (NULL != strstr(StrArg, BinFile[i]))
		{
			return 1;
		}
	}

	return 0;
}
int ResponseFile(int confdArg, char *FileNameArg, char *cookieArg)
{
	int i = 0;
	int IsBinFile = 0;
	if (access(FileNameArg, F_OK) || FileNameArg == NULL)
	{
		perror("fail to ResponseFile of access no exists");
		return -1;
	}
	else
	{
		printf("ResponseFile: fine file  to send %s\n", FileNameArg);
		for (i = 0; i < sizeof(BinFile) / sizeof(BinFile[0]); i++)
		{
			if (NULL != strstr(FileNameArg, BinFile[i]))
			{
				IsBinFile = 1;
			}
		}
		if (1 == IsBinFile)
		{
			printf("ResponseFile Switch To ResponseBin\n");
			ResponseBin(confdArg, FileNameArg);
			return 0;
		}
	}
	FILE *fp = NULL;
	int Ret = 0;
	int size = 0;
	char TmpEmail[__SHOPPING_STR_LEN_128__] = {0};
	char SendEmail[__SHOPPING_STR_LEN_1024__] = {0};
	char *pTmpKey = NULL;
	char TmpBuff[__SHOPPING_STR_LEN_4096__] = {0};
	int RequestDivInfoLen = sizeof(RequestDivInfo) / sizeof(RequestDivInfo[0]);	


	fp = fopen(FileNameArg, "r");
	if (NULL == fp)
	{
		perror("fail to fopen");
		return -1;
	}

	while (NULL != fgets(TmpBuff, sizeof(TmpBuff), fp))
	{
		for (i = 0; i < RequestDivInfoLen; i++)
		{
			if (NULL != (pTmpKey = strstr(TmpBuff, RequestDivInfo[i])))
			{
				printf("*************************RequestDivInfo*******************************\n");
				if (NULL != strstr(TmpBuff, "<!--index_div_login_state-->"))
				{
					if (IsLoginStat(cookieArg, TmpEmail))
					{	

						sprintf(SendEmail, "<br><p class=\"lead\">%s</p>", TmpEmail);
						Ret = send(confdArg, SendEmail, strlen(SendEmail), 0);
						if (-1 == Ret)
						{
							perror("fail to send in ResponseFile");
							return -1;
						}

						sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "index_div_login _on.html");
						printf("=====================-login dese !!-====================\n");
						printf("=====================-login dese !!-====================\n");
						printf("=====================-login dese !!-====================\n");
						printf("=====================-login dese !!-====================\n");

						ResponseFile(confdArg, FileNameArg, NULL);
					}
					else
					{
						sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "index_div_login _off.html");
						printf("=====================-unlugin dese !!-=====================\n");
						printf("=====================-unlugin dese !!-=====================\n");
						printf("=====================-unlugin dese !!-=====================\n");
						printf("=====================-unlugin dese !!-=====================\n");
						ResponseFile(confdArg, FileNameArg, NULL);
					}
				}
			}
		}


		Ret = send(confdArg, TmpBuff, strlen(TmpBuff), 0);
		if (-1 == Ret)
		{
			perror("fail to send in ResponseFile");
			return -1;
		}
		memset(TmpBuff, 0, sizeof(TmpBuff));
	}

	fclose(fp);


	return 0;
}

int ResponseBin(int confdArg, char *FileNameArg)
{
	if (access(FileNameArg, F_OK) || FileNameArg == NULL)
	{
		perror("fail to ResponseFile of access no exists");
		return -1;
	}
	else
	{
		printf("ResponseBin: fine file  to send %s\n", FileNameArg);
	}
	int i = 0;
	FILE *fp = NULL;
	int Ret = 0;
	int size = 0;
	char *pTmpKey = NULL;
	char TmpBuff[__SHOPPING_STR_LEN_1024__] = {0};

	fp = fopen(FileNameArg, "r");
	if (NULL == fp)
	{
		perror("fail to fopen");
		return -1;
	}

	while (0 != (size = fread(TmpBuff, sizeof(char), sizeof(TmpBuff), fp)))
	{

		Ret = send(confdArg, TmpBuff, size, 0);
		if (-1 == Ret)
		{
			perror("fail to send in ResponseFile");
			return -1;
		}
		memset(TmpBuff, 0, sizeof(TmpBuff));
	}

	if (NULL != fp)
	{
		fclose(fp);
	}


	return 0;
}

int ResponseData(int confdArg, REQUESTHEAD_S *prequest_headArg, sqlite3 *pdbArg)
{
	int k = 0;
	int i = 0;
	int Ret = 0;
	int size = 0;
	char ThisLoginUsrEmail[__SHOPPING_STR_LEN_128__] = {0};
	char TmpUrl[2048] = {0};
	char TmpTcpData[2048] = {0};
	char FileNameArg[__SHOPPING_STR_LEN_1024__] = {0};
	char TmpTcpDataDiv[__SHOPPING_STR_LEN_128__] = {0};
	char *pTmpPath = NULL;
	char *pTmpKey = NULL;
	char *pTmpStar = NULL;
	char *pTmpEnd = NULL;
	FILE *fp = NULL;
	FILE *fpdiv = NULL;
	DICT_S *pdict = NULL;
	int RequestNodeInfoLen = sizeof(RequestNodeInfo) / sizeof(RequestNodeInfo[0]);
	int RequestNodeInfoStatStart = 0;
	int RequestNodeInfoStatEnd = 0;
	CTLSQL_S Tmpctlsql = {
		__CTLSQL_LINE_MAX__,
		pdbArg,
		"goods",
		"rowid",
		"1",
		"1",
	};
	char TmpCreateFileNameSrc[__SHOPPING_STR_LEN_1024__] = {0};
	char TmpCreateFileNameDest[__SHOPPING_STR_LEN_1024__] = {0};
	char TmpDiv[__SHOPPING_STR_LEN_1024__] = {0};
	int IsBinFile = 0;
	SEARCH_S *pTmpSearchId = NULL;
	int PostSearchMode = 0;
	int FindPostSearchModeShow = 0;

	ResponseHead(confdArg);
	if (IsUnASCIIStr(prequest_headArg->url))
	{
		sprintf(FileNameArg, "%s%s", __HTTP_SERVER_HOME_PATH__, prequest_headArg->url);
		ResponseBin(confdArg, FileNameArg);
		return 0;
	}


	if ( ! IsLoginStat(prequest_headArg->cookie, ThisLoginUsrEmail))
	{
		if (IsSignInLockHtml(prequest_headArg->url))
		{
			{
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "login_lock.html");
				ResponseFile(confdArg, FileNameArg, NULL);
				return 0;
			}
		}		
	}
	else
	{
		printf(":::::::::::::::::::::ThisLoginUsrEmail: %s::::::::::::::::\n", ThisLoginUsrEmail);
	}

	if (! strcmp(prequest_headArg->key, "POST"))
	{
		if (NULL != strstr(prequest_headArg->url, "search"))
		{
			pTmpSearchId = Search(pdbArg, prequest_headArg->post);
			PostSearchMode = 1;
		}
	}
	if ( ! strcmp(prequest_headArg->key, "GET") || 1 == PostSearchMode)
	{
		if ( (! strcmp(prequest_headArg->url, "/")) || 1 == PostSearchMode || NULL != strstr(prequest_headArg->url, "index.html"))
		{
			sprintf(TmpUrl, "%s/index.html", __HTTP_SERVER_HOME_PATH__);
		}
		else
		{
			sprintf(TmpUrl, "%s%s", __HTTP_SERVER_HOME_PATH__, prequest_headArg->url);
		}
		printf("TmpUrl: %s\n", TmpUrl);



		for (i = 0; i < sizeof(BinFile) / sizeof(BinFile[0]); i++)
		{
			if (NULL != strstr(TmpUrl, BinFile[i]))
			{
				IsBinFile = 1;
			}
		}
		if (1 == IsBinFile)	
		{
			ResponseBin(confdArg, TmpUrl);

			IsBinFile = 0;
		}
		if (NULL != (pTmpKey = strstr(TmpUrl, "loginout.html")))
		{
			DeleteLoginStat(prequest_headArg->cookie);

			sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "loginout.html");
			ResponseFile(confdArg, FileNameArg, NULL);
		}
		else if (NULL != (pTmpKey = strstr(TmpUrl, "detail")))
		{
			if (access(TmpUrl, F_OK))
			{
				pTmpKey = TmpUrl + strlen(TmpUrl);
				while ('_' != *pTmpKey && pTmpKey > TmpUrl)
				{
					pTmpKey--;
				}
				pTmpKey++;

				i = 0;
				while ('.' != *pTmpKey && (pTmpKey < (TmpUrl + strlen(TmpUrl))))
				{
					Tmpctlsql.goods_id[i] = *pTmpKey;
					pTmpKey++;
					i++;
				}
				Tmpctlsql.goods_id[i] = '\0';

				sprintf(TmpCreateFileNameSrc, "%s/detail.html", __HTTP_SERVER_HOME_PATH__);

				if (NULL == CreateResponesHtml(TmpCreateFileNameSrc, TmpCreateFileNameDest, &Tmpctlsql, NULL))
				{
					perror("fial to CreateResponesHtml of NULL is no exists file ");
					return 0;;
				}
			}
			ResponseFile(confdArg, TmpUrl, prequest_headArg->cookie);
		}
		else if (NULL != (pTmpKey = strstr(TmpUrl, "login.html")))
		{
			fp = fopen(TmpUrl, "r");
			if (NULL == fp)
			{
				perror("fail to fopen of detail TmpCreateFileNameDest");
				return -1;
			}
			while (NULL != (fgets(TmpTcpData, sizeof(TmpTcpData), fp)))
			{
				Ret = send(confdArg, TmpTcpData, strlen(TmpTcpData), 0);
				if (-1 == Ret)
				{
					perror("fail to send of detail CreateResponesHtml");
					return -1;
				}
			}
			fclose(fp);
		}
		else if (NULL != (pTmpKey = strstr(TmpUrl, "loginup.html")))
		{			
			sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "loginup.html");
			ResponseFile(confdArg, FileNameArg, NULL);						
		}
		else if (NULL != (pTmpKey = strstr(TmpUrl, "buy")))
		{
			if (access(TmpUrl, F_OK) || IsSignInLockHtml(TmpUrl))
			{
				pTmpKey = TmpUrl + strlen(TmpUrl);
				while ('_' != *pTmpKey && pTmpKey > TmpUrl)
				{
					pTmpKey--;
				}
				pTmpKey++;

				i = 0;
				while ('.' != *pTmpKey && (pTmpKey < (TmpUrl + strlen(TmpUrl))))
				{
					Tmpctlsql.goods_id[i] = *pTmpKey;
					pTmpKey++;
					i++;
				}
				Tmpctlsql.goods_id[i] = '\0';

				sprintf(TmpCreateFileNameSrc, "%s/buy.html", __HTTP_SERVER_HOME_PATH__);

				if (NULL == CreateResponesHtml(TmpCreateFileNameSrc, TmpCreateFileNameDest, &Tmpctlsql, ThisLoginUsrEmail))
				{
					perror("fial to CreateResponesHtml of NULL is no exists file ");
					return 0;;

				}
			}

			fp = fopen(TmpUrl, "r");
			if (NULL == fp)
			{
				perror("fail to fopen of detail TmpCreateFileNameDest");
				return -1;
			}
			while (NULL != (fgets(TmpTcpData, sizeof(TmpTcpData), fp)))
			{
				Ret = send(confdArg, TmpTcpData, strlen(TmpTcpData), 0);
				if (-1 == Ret)
				{
					perror("fail to send of detail CreateResponesHtml");
					return -1;
				}
			}
			fclose(fp);

		}
		else if (NULL != (pTmpKey = strstr(TmpUrl, "chargemoney.html")))
		{	
			sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "chargemoney.html");
			ResponseFile(confdArg, FileNameArg, NULL);		
		}
		else
		{
			fp = fopen(TmpUrl, "r");
			if (NULL == fp)
			{
				perror("fail to fopen");
				return -1;
			}
			while (NULL != (fgets(TmpTcpData, sizeof(TmpTcpData), fp)))
			{
				if (NULL != strstr(TmpTcpData, "<!--index_div_login_state-->"))
				{
					char TmpEmail[__SHOPPING_STR_LEN_128__] = {0};
					char SendEmail[__SHOPPING_STR_LEN_1024__] = {0};
					if (IsLoginStat(prequest_headArg->cookie, TmpEmail))
					{	
						sprintf(SendEmail, "<br><p class=\"lead\">%s</p>", TmpEmail);
						Ret = send(confdArg, SendEmail, strlen(SendEmail), 0);
						if (-1 == Ret)
						{
							perror("fail to send in ResponseFile");
							return -1;
						}
						sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "index_div_login _on.html");
						printf("=====================-login dese !!-====================\n");
						printf("=====================-login dese !!-====================\n");
						printf("=====================-login dese !!-====================\n");
						printf("=====================-login dese !!-====================\n");
						ResponseFile(confdArg, FileNameArg, NULL);
					}
					else
					{
						sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "index_div_login _off.html");
						printf("=====================-unlugin dese !!-=====================\n");
						printf("=====================-unlugin dese !!-=====================\n");
						printf("=====================-unlugin dese !!-=====================\n");
						printf("=====================-unlugin dese !!-=====================\n");
						ResponseFile(confdArg, FileNameArg, NULL);
					}
				}
				if (-1 == RequestNodeInfoStatEnd)
				{
					for (i = 0; i < RequestNodeInfoLen; i++)
					{
						if (NULL != (pTmpKey = strstr(TmpTcpData, RequestNodeInfo[i].end)))
						{
							RequestNodeInfoStatEnd = 0;
							break;
						}
					}
					if (-1 == RequestNodeInfoStatEnd)
					{
						continue;	
					}
					else if (0 == RequestNodeInfoStatEnd)
					{
						continue;
					}
				}
				RequestNodeInfoStatStart = 0;
				for (i = 0; i < RequestNodeInfoLen; i++)
				{
					if (NULL != (pTmpKey = strstr(TmpTcpData, RequestNodeInfo[i].start)))
					{
						memset(TmpDiv, 0, sizeof(TmpDiv));

						printf("=========__LOGIN_USR_STAT_COOKIE__2\n");

						strcpy(TmpDiv, RequestNodeInfo[i].start);

						RequestNodeInfoStatStart = 1;
						RequestNodeInfoStatEnd = -1;
						break;
					}
				}
				if (0 == RequestNodeInfoStatStart)
				{
					Ret = send(confdArg, TmpTcpData, strlen(TmpTcpData), 0);
					if (-1 == Ret)
					{
						perror("fail to send");
						return -1;
					}

				}
				//				printf("__CTLSQL_LINE_MAX__\n");
				if (1 == RequestNodeInfoStatStart)
				{
#if 1
					if (NULL != strstr(TmpDiv, "<!--index_div_cargo_start-->"))
					{
						printf("<!--creatnode-->:===============================================%s======\n", ControlSQL(&Tmpctlsql)->data);
						if (NULL == ControlSQL(&Tmpctlsql))
						{
							perror("fail to ControlSQL");
							return -1;
						}
						for (i = 0; i < atoi(Tmpctlsql.data); i++)
						{
							sprintf(Tmpctlsql.goods_id, "%d", i+1);
							printf("RequestNodeInfo[i].filename:%s\n", RequestNodeInfo[0].filename);

							FindPostSearchModeShow = 0;	
							if (1 == PostSearchMode)
							{
								for (k = 0; k < pTmpSearchId->goods_id_len; k++)
								{
									if ( ! strcmp(pTmpSearchId->goods_id_list[k], Tmpctlsql.goods_id))
									{
										FindPostSearchModeShow = 1;
										break;																		
									}
								}

							}
							if (0 == FindPostSearchModeShow && 1 == PostSearchMode)
							{
								continue;
							}
							else if (1 == FindPostSearchModeShow && 1 == PostSearchMode)
							{
								FindPostSearchModeShow = 0;	
							}

							memset(TmpCreateFileNameDest, 0, sizeof(TmpCreateFileNameDest));

							if ( NULL == CreateResponesHtml(RequestNodeInfo[0].filename, TmpCreateFileNameDest, &Tmpctlsql, NULL))
							{
								perror("fail to CreateResponesHtml of ResponseData the file is no exists");
								continue;
							}
							if (access(TmpCreateFileNameDest, F_OK))
							{
								continue;
							}


							fpdiv = fopen(TmpCreateFileNameDest, "r");
							if (NULL == fpdiv)
							{
								perror("fail to fopen of fpdiv");
								continue;
							}
							while (fgets(TmpTcpDataDiv, sizeof(TmpTcpDataDiv), fpdiv))
							{
								Ret = send(confdArg, TmpTcpDataDiv, strlen(TmpTcpDataDiv), 0);
								if (-1 == Ret)
								{
									perror("fail to send of TmpTcpDataDiv");
									return -1;
								}
							}
							if (NULL != fpdiv)
							{
								fclose(fpdiv);
							}
						}
					}				
#endif
				}
				memset(TmpTcpData, 0, sizeof(TmpTcpData));
			}
			fclose(fp);
		}
		if (1 == PostSearchMode)
		{
			if(NULL != pTmpSearchId)
			{
				free(pTmpSearchId);	
			}

			PostSearchMode = 0;
		}
	}
	else if ( ! strcmp(prequest_headArg->key, "POST"))
	{
		printf("========POST STR: %s ==========\n", prequest_headArg->post);
		printf("========POST STR: %s ==========\n", prequest_headArg->post);
		if (NULL != (pTmpKey = strstr(prequest_headArg->url, "logininstat.html")))
		{
			if (1 == LoginSuccess)
			{
				sprintf(TmpCreateFileNameSrc, "%s/%s", __HTTP_SERVER_HOME_PATH__, "loginon.html");
				puts(TmpCreateFileNameSrc);
				ResponseFile(confdArg, TmpCreateFileNameSrc, prequest_headArg->cookie);
				LoginSuccess = 0;	
			}
			else
			{
				sprintf(TmpCreateFileNameSrc, "%s/%s", __HTTP_SERVER_HOME_PATH__, "loginoff.html");
				puts(TmpCreateFileNameSrc);
				ResponseFile(confdArg, TmpCreateFileNameSrc, prequest_headArg->cookie);
			}

		}
		else if (NULL != (pTmpKey = strstr(prequest_headArg->url, "loginupstat.html")))
		{
			int RetLoginUpStat = 0;

			RetLoginUpStat = LoginUpNewEmail(pdbArg, prequest_headArg->post);	
			if (__ADD_USR_OK__ == RetLoginUpStat)
			{
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "loginup_ok.html");
				ResponseFile(confdArg, FileNameArg, NULL);
			}
			else if (__ADD_USR_ERR__ == RetLoginUpStat)
			{
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "loginup_err.html");
				ResponseFile(confdArg, FileNameArg, NULL);
			}
			else if (__ADD_USR_REPEAT__ == RetLoginUpStat)
			{
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "loginup_respeat.html");
				ResponseFile(confdArg, FileNameArg, NULL);
			}
		}
		else if (NULL != (pTmpKey = strstr(prequest_headArg->url, "payforstat.html")))
		{
			GOODS_S goods1;
			goods1 = ExecPostToGoods(prequest_headArg->post, pdbArg);
			printf("PayForGoods before %s\n", ThisLoginUsrEmail);
			Ret = PayForGoods(&goods1, ThisLoginUsrEmail);
			if (__PAYFORGOODS_OK__ == Ret)
			{	
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "buy_ok.html");
				ResponseFile(confdArg, FileNameArg, NULL);	
			}
			else if (__PAYFORGOODS_DEF__ == Ret)
			{
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "buy_def.html");
				ResponseFile(confdArg, FileNameArg, NULL);
			}
			else
			{
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "buy_err.html");
				ResponseFile(confdArg, FileNameArg, NULL);
			}
		}
		else if (NULL != (pTmpKey = strstr(prequest_headArg->url, "chargemoneystat.html")))
		{
			Ret = ExecPostToChangeMoney(prequest_headArg->post, ThisLoginUsrEmail, pdbArg);
			if (0 == Ret)
			{
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "chargemoney_ok.html");
				ResponseFile(confdArg, FileNameArg, NULL);	
			}
			else
			{			
				sprintf(FileNameArg, "%s/%s", __HTTP_SERVER_HOME_PATH__, "chargemoney_err.html");
				ResponseFile(confdArg, FileNameArg, NULL);
			}
		}

	}

	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx\n");


	return 0;
}

