#ifndef __TCPCONNECT_H___
#define __TCPCONNECT_H__

typedef struct requesthead_s
{
	char key[__SHOPPING_STR_LEN_32__];
	char url[__SHOPPING_STR_LEN_2048__];
	char cookie[__SHOPPING_STR_LEN_1024__];
	char post[__SHOPPING_STR_LEN_1024__];
}REQUESTHEAD_S;

typedef struct responsehtmlnode_s
{
	char htmlname[__SHOPPING_STR_LEN_1024__];
	char start[__SHOPPING_STR_LEN_1024__];
	char end[__SHOPPING_STR_LEN_1024__];
	char filename[__SHOPPING_STR_LEN_1024__];
}REQUEST_NODE_INFO_S;

extern int TcpInit(void);
extern int ConnectBrowser(int sockfdArg);
extern REQUESTHEAD_S *GetRequest(int confdArg);
extern int ResponseHead(int confdArg);
extern int ResponseData(int confdArg, REQUESTHEAD_S *prequest_headArg, sqlite3 *pdbArg);
extern int ResponseFile(int confdArg, char *FileNameArg, char *cookieArg);
extern int ResponseBin(int confdArg, char *FileNameArg);

extern REQUEST_NODE_INFO_S RequestNodeInfo[]; 
extern char RequestDivInfo[][__SHOPPING_STR_LEN_128__];

#endif

