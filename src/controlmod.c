#include "public.h"
#include "controlmod.h"
#include "tcpconnect.h"
#include "createhtml.h"
#include "search.h"

char SignInLockHtml[][__SHOPPING_STR_LEN_32__] = {
	"buy",
	"pay",
	"money",
};
int SignInLockHtmlLen;

LOGIN_USR_S login_usr_list[__LOGIN_USR_MAX_NUM_10__];
int LoginSuccess = 0;

int LoginUsrInit(void)
{
	int i = 0;
	int len = sizeof(login_usr_list) / sizeof(login_usr_list[0]);

	memset(login_usr_list, 0, sizeof(login_usr_list));

	for (i = 0; i < len; i++)
	{
		login_usr_list[i].stat = __LOGIN_USR_STAT_OFF__;
		login_usr_list[i].maxnum = len;
		strcpy(login_usr_list[i].list_email, "email");
		strcpy(login_usr_list[i].list_password, "password");
		strcpy(login_usr_list[i].table, "admin_user");
	}

	return 0;
}
int LoginUsrAdd(LOGIN_USR_S *pTmploginu_usr)
{
	int i = 0;
	srand(time(NULL));
	for (i < 0; i < sizeof(login_usr_list)/ sizeof(login_usr_list[0]); i++)
	{
		if (__LOGIN_USR_STAT_OFF__ == login_usr_list[i].stat)
		{
			login_usr_list[i].stat = __LOGIN_USR_STAT_COOKIE__;
			strcpy(login_usr_list[i].email, pTmploginu_usr->email);
			sprintf(login_usr_list[i].cookie, "%s:%d", pTmploginu_usr->email, rand()%100000);

			break;
		}
	}

	return 0;
}

int LoginUsrDel(LOGIN_USR_S *pTmploginu_usr)
{
	int i = 0;
	for (i = 0; i < sizeof(login_usr_list) / sizeof(login_usr_list[0]); i++)
	{
		if ( ! strcmp(login_usr_list[i].email, pTmploginu_usr->email))
		{
			login_usr_list[i].stat = __LOGIN_USR_STAT_OFF__;
			memset(login_usr_list[i].email, 0, sizeof(login_usr_list[i].email));
			memset(login_usr_list[i].cookie, 0, sizeof(login_usr_list[i].cookie));
			break;
		}
	}

	return 0;
}

int IsLoginStat(char *cookieArg, char *getEmailArg)
{
	if (NULL == cookieArg)
	{
		perror("IsLoginStat of cookieArg is NULL");
		return 0;
	}

	int	i = 0;

	for (i = 0; i < sizeof(login_usr_list) / sizeof(login_usr_list[0]); i++)
	{
		if (__LOGIN_USR_STAT_ON__ == login_usr_list[i].stat)
		{
			if (NULL != strstr(cookieArg, login_usr_list[i].cookie))
			{
				if (NULL != getEmailArg)
				{
					strcpy(getEmailArg, login_usr_list[i].email);
				}
				
				return 1;
				printf("\n\n\n\nLoginSuccessGetLoginoutButtun Cookie: %s\n\n\n\n", cookieArg);
			}
		}

	}

	return 0;
}



int IsSignInLockHtml(char *HtmlFileNameArg)
{
	int i = 0;
	
	for (i = 0; i < sizeof(SignInLockHtml) / sizeof(SignInLockHtml[0]); i++)
	{
		if (NULL != strstr(HtmlFileNameArg, SignInLockHtml[i]))
		{
			return 1;
		}
	}

	return 0;
}

int DeleteLoginStat(char *cookieArg)
{
	int	i = 0;

	for (i = 0; i < sizeof(login_usr_list) / sizeof(login_usr_list[0]); i++)
	{
		if (__LOGIN_USR_STAT_ON__ == login_usr_list[i].stat)
		{
			if (NULL != strstr(cookieArg, login_usr_list[i].cookie))
			{
				login_usr_list[i].stat = __LOGIN_USR_STAT_OFF__;
				memset(login_usr_list[i].cookie, 0, sizeof(login_usr_list[i].cookie));
			}
		}

	}

	return 0;
}


LOGIN_PWD_S *ExecPasswd(char *pTmpPostStrArg, LOGIN_PWD_S *pTmpUsrArg)
{
	char *pTmpKey = NULL;
	char *pTmpStar = NULL;
	char *pTmpEnd = NULL;

	pTmpStar = strstr(pTmpPostStrArg, "email=");
	pTmpStar += strlen("email=");
	pTmpEnd = strstr(pTmpStar, "%");
	strncpy(pTmpUsrArg->email, pTmpStar, pTmpEnd - pTmpStar);
	pTmpUsrArg->email[pTmpEnd - pTmpStar] = '\0';
	printf("pTmpUsrArg: %s\n", pTmpUsrArg->email);

	pTmpKey = pTmpUsrArg->email + strlen(pTmpUsrArg->email);
	*pTmpKey = '@';
	pTmpKey++;
	printf("pTmpStar: %s\n", pTmpStar);
	printf("pTmpUsrArg: %s\n", pTmpUsrArg->email);
	pTmpStar = pTmpEnd;
	pTmpStar += strlen("%40");
	pTmpEnd = strstr(pTmpStar, "&");
	strncpy(pTmpKey, pTmpStar, pTmpEnd - pTmpStar);
	pTmpKey[pTmpEnd - pTmpStar] = '\0';
	printf("pTmpStar: %s\n", pTmpStar);
	printf("pTmpUsrArg: %s\n", pTmpUsrArg->email);

	pTmpStar = strstr(pTmpPostStrArg, "passwd=");
	pTmpStar += strlen("passwd=");
	strcpy(pTmpUsrArg->passwd, pTmpStar);
	printf("pTmpStar: %s\n", pTmpStar);
	printf("pTmpUsrArg: %s\n", pTmpUsrArg->email);


	return pTmpUsrArg;
}

char *GetEmailPasswd(sqlite3 *pdbArg, char *pEmailArg)
{
	char *pSearchPasswd;
	CTLSQL_S TmpctlsqlSearch = {
		__CTLSQL_SEARCH_PASSWD__,
		pdbArg,
		"admin_user",
		"email",
	};

	pSearchPasswd = malloc(__SHOPPING_STR_LEN_1024__);
	if (NULL == pSearchPasswd)
	{
		perror("fail to malloc of Search");
		return NULL;
	}
	memset(pSearchPasswd, 0, sizeof(__SHOPPING_STR_LEN_1024__));

	strcpy(TmpctlsqlSearch.data, pEmailArg);

	ControlSQL(&TmpctlsqlSearch);

	if (1 == TmpctlsqlSearch.callbackfuncstat)
	{
		strcpy(pSearchPasswd, TmpctlsqlSearch.data);
	}
	else if (0 == TmpctlsqlSearch.callbackfuncstat)
	{
		char chEOF[2] = {EOF, '\0'};
		strcpy(pSearchPasswd, chEOF);
	}

	return pSearchPasswd;
}

void PRINTuserList(void)
{
	int i = 0;
	for (i = 0; i < sizeof(login_usr_list) / sizeof(login_usr_list[0]); i++)
	{
		if (__LOGIN_USR_STAT_ON__ == login_usr_list[i].stat)
		{
			printf("LOGIN_USR: %s--------------------------\n", login_usr_list[i].email);
			printf("LOGIN_USR: %s--------------------------\n", login_usr_list[i].email);
			printf("LOGIN_COOKIE: %s--------------------------\n", login_usr_list[i].cookie);
			printf("LOGIN_COOKIE: %s--------------------------\n", login_usr_list[i].cookie);
		}
	}

	return ;
}

int ExecNewPasswd(char *PostStrArg, char *Email, char *PassWord, char *RePassWord)
{
	char *pTmpKey = NULL;
	char *pTmpStar = NULL;
	char *pTmpEnd = NULL;

	pTmpStar = strstr(PostStrArg, "email=");
	pTmpStar += strlen("email=");
	pTmpEnd = strstr(pTmpStar, "%");
	strncpy(Email, pTmpStar, pTmpEnd - pTmpStar);
	Email[pTmpEnd - pTmpStar] = '\0';
	printf("SignUp Email: %s\n", Email);

	pTmpKey = Email + strlen(Email);
	*pTmpKey = '@';
	pTmpKey++;
	printf("pTmpStar: %s\n", pTmpStar);
	printf("SignUp Email: %s\n", Email);
	pTmpStar = pTmpEnd;
	pTmpStar += strlen("%40");
	pTmpEnd = strstr(pTmpStar, "&");
	strncpy(pTmpKey, pTmpStar, pTmpEnd - pTmpStar);
	pTmpKey[pTmpEnd - pTmpStar] = '\0';
	printf("pTmpStar: %s\n", pTmpStar);
	printf("SignUp Email: %s\n", Email);

	pTmpStar = strstr(PostStrArg, "password=");
	pTmpStar += strlen("password=");
	pTmpEnd = strstr(pTmpStar, "&");
	strncpy(PassWord, pTmpStar, pTmpEnd - pTmpStar);
	PassWord[pTmpEnd - pTmpStar] = '\0';
	printf("pTmpStar: %s\n", pTmpStar);
	printf("SignUp PassWord: %s\n", PassWord);

	pTmpStar = strstr(PostStrArg, "confirm_password=");
	pTmpStar += strlen("confirm_password=");
	strcpy(RePassWord, pTmpStar);
	printf("pTmpStar: %s\n", pTmpStar);
	printf("SignUp RePassWord: %s\n", RePassWord);
	
	return 0;	
}

int LoginUpNewEmail(sqlite3 *pdbArg, char *PostStrArg)
{
	char chEOF[2] = {EOF, '\0'};
	int MaxUserId = 0;
	char Email[__SHOPPING_STR_LEN_128__] = {0};
	char PassWord[__SHOPPING_STR_LEN_128__] = {0};
	char RePassWord[__SHOPPING_STR_LEN_128__] = {0};
	CTLSQL_S TmpctlsqlSearch = {
		__CTLSQL_ADD_NEWUSR__,
		pdbArg,
		"admin_user",
		"email",
	};
	CTLSQL_S Tmpctlsql = {
		__CTLSQL_LINE_MAX__,
		pdbArg,
		"admin_user",
		"user_id",
		"1",
		"1",
	};

	

	ExecNewPasswd(PostStrArg, Email, PassWord, RePassWord);

	if(strcmp(PassWord, RePassWord) || (NULL == strstr(Email, "@")))
	{
		perror("PassWord != RePassWord or Email Not @");
		return __ADD_USR_ERR__;
	}

	if(strcmp(GetEmailPasswd(pdbArg, Email), chEOF))
	{
		perror("Email is Repeat");
		return __ADD_USR_REPEAT__;
	}
	

	strcpy(TmpctlsqlSearch.email, Email);
	strcpy(TmpctlsqlSearch.passwd, PassWord);

	/* Set User Id */
	MaxUserId = atoi(ControlSQL(&Tmpctlsql)->data);
	sprintf(TmpctlsqlSearch.goods_id, "%d", MaxUserId+1);

	ControlSQL(&TmpctlsqlSearch);


	printf("Add New User Success !\n");
	return __ADD_USR_OK__;
}

GOODS_S ExecPostToGoods(char *pPostStrArg, sqlite3 *pdbArg)
{
	GOODS_S TmpGoods = {
		pdbArg,
	};
	char *pTmpKey = NULL;
	char *pTmpStar = NULL;
	char *pTmpEnd = NULL;

	pTmpStar = strstr(pPostStrArg, "goodsid=");
	pTmpStar += strlen("goodsid=");
	pTmpEnd = strstr(pTmpStar, "&");
	strncpy(TmpGoods.goods_id, pTmpStar, pTmpEnd - pTmpStar);
	TmpGoods.goods_id[pTmpEnd - pTmpStar] = '\0';
	printf("TmpGoodsId: %s\n", TmpGoods.goods_id);

	pTmpStar = strstr(pPostStrArg, "shopprice=");
	pTmpStar += strlen("shopprice=");
	strcpy(TmpGoods.price, pTmpStar);
	printf("TmpGoodsPrice: %s\n", TmpGoods.price);

	return TmpGoods;
}



int ExecPostToChangeMoney(char *pPostStrArg, char *EmailArg, sqlite3 *pdbArg)
{
	char *pTmpKey = NULL;
	char *pTmpStar = NULL;
	char *pTmpEnd = NULL;
	CTLSQL_S TmpMaxLine;
	unsigned long long int UserMoney = 0;
	long long int ChargeMoney = 0;
	char changemoneystr[__SHOPPING_STR_LEN_32__] = {0};


	pTmpStar = strstr(pPostStrArg, "chargemoney=");
	pTmpStar += strlen("chargemoney=");
	strcpy(changemoneystr, pTmpStar);
	printf("TmpUsr: %s\n", changemoneystr);

	ChargeMoney = atoi(changemoneystr);
	if (ChargeMoney <= 0)
	{
		return -1;
	}


	TmpMaxLine.flag = __CTLSQL_SEARCH__;

	TmpMaxLine.pdb = pdbArg;

	sprintf(TmpMaxLine.goods_id, "\"%s\"", EmailArg);
	strcpy(TmpMaxLine.table, "admin_user");
	strcpy(TmpMaxLine.line, "role_id");
	strcpy(TmpMaxLine.search_id_line, "email");

	ControlSQL(&TmpMaxLine);

	printf("role_id: %s\n", TmpMaxLine.data);

	UserMoney = atoi(TmpMaxLine.data);

	TmpMaxLine.flag = __CTLSQL_UPDATE__;
	TmpMaxLine.pdb = pdbArg;
	sprintf(TmpMaxLine.goods_id, "\"%s\"", EmailArg);
	strcpy(TmpMaxLine.table, "admin_user");
	strcpy(TmpMaxLine.line, "role_id");
	strcpy(TmpMaxLine.search_id_line, "email");
	sprintf(TmpMaxLine.data, "%llu", UserMoney + ChargeMoney);

	ControlSQL(&TmpMaxLine);

	printf("ChargeMoney Success\n");


	return 0;
}



int PayForGoods(GOODS_S *GoodsArg, char *EmailArg)
{
	CTLSQL_S TmpMaxLine;
	int UserMoney = 0;
	int GoodsPrice = 0;

	printf("0\n");
	TmpMaxLine.flag = __CTLSQL_SEARCH__;
	TmpMaxLine.pdb = GoodsArg->pdb;
	printf("0.6\n");
	printf("EmailArg%p\n", EmailArg);
	printf("EmailArg:%s\n", EmailArg);

	printf("0.7\n");
	sprintf(TmpMaxLine.goods_id, "\"%s\"", EmailArg);
	printf("1\n");
	strcpy(TmpMaxLine.table, "admin_user");
	strcpy(TmpMaxLine.line, "role_id");
	strcpy(TmpMaxLine.search_id_line, "email");

	printf("2\n");
	ControlSQL(&TmpMaxLine);

	printf("role_id: %s\n", TmpMaxLine.data);

	UserMoney = atoi(TmpMaxLine.data);
	GoodsPrice = atoi(GoodsArg->price);
	printf("3\n");

	printf("UserMoney:%d\n GoodsPrice:%d\n", UserMoney, GoodsPrice);

	if (UserMoney >= GoodsPrice)
	{

		TmpMaxLine.flag = __CTLSQL_UPDATE__;
		TmpMaxLine.pdb = GoodsArg->pdb;
		sprintf(TmpMaxLine.goods_id, "\"%s\"", EmailArg);
		strcpy(TmpMaxLine.table, "admin_user");
		strcpy(TmpMaxLine.line, "role_id");
		strcpy(TmpMaxLine.search_id_line, "email");
		sprintf(TmpMaxLine.data, "%d", UserMoney - GoodsPrice);

		ControlSQL(&TmpMaxLine);

		printf("PayForGoods\n");
		printf("4\n");
		return __PAYFORGOODS_OK__;
	}
	else if (UserMoney < GoodsPrice)
	{
		return __PAYFORGOODS_DEF__;
	}
	else
	{
		return __PAYFORGOODS_ERR__;
	}


	return -1;
}

int ControlMod(int confdArg)
{

	int i = 0;
	CTLSQL_S ctlsql = {
		__CTLSQL_LINE_MAX__,
		NULL,
		"goods",
		"rowid",
		"1",
		"1",
	};
	char CreateHtmlNameSrc[__SHOPPING_STR_LEN_1024__] = {0};
	char CreateHtmlNameDst[__SHOPPING_STR_LEN_1024__] = {0};
	sqlite3 *pdb = NULL;
	REQUESTHEAD_S *prequest_head = NULL;
	LOGIN_PWD_S TmpUsr;
	LOGIN_USR_S TmpUsrLogin;
	char *pTmpKey = NULL;
	char *pTmpStar = NULL;
	char *pTmpEnd = NULL;
	CTLSQL_S Tmpctlsql = {
		__CTLSQL_LINE_MAX__,
		NULL,
		"goods",
		"rowid",
		"1",
		"1",
	};

	SignInLockHtmlLen = sizeof(SignInLockHtml) / sizeof(SignInLockHtml[0]);

	LoginSuccess = 0;

	pdb = SqlInit(__SHOPPING_DB__);
	if (NULL == pdb)
	{
		perror("fail to SqlInit");
		return -1;
	}

	Tmpctlsql.pdb = pdb;
	ctlsql.pdb = pdb;


	prequest_head = GetRequest(confdArg);
	if (NULL == prequest_head)
	{
		perror("fail to GetRequest");
		return -1;
	}
	else
	{	
		if ( ! strcmp(prequest_head->key, "POST"))
		{
			if (NULL != (pTmpKey = strstr(prequest_head->url, "logininstat.html")))
			{
				printf("??????????????????????????????\n");
				if (NULL == strstr(prequest_head->post, "%40"))
				{
					return -1;
				}

				printf("ControlMod-----------------------------------ControlMod\n");

				ExecPasswd(prequest_head->post, &TmpUsr);

				if (!strcmp(GetEmailPasswd(pdb, TmpUsr.email), TmpUsr.passwd))
				{
					puts("yes");
					printf("====================登录成功=====-=-\n");
					strcpy(TmpUsrLogin.email, TmpUsr.email);
					LoginUsrAdd(&TmpUsrLogin);					
					LoginSuccess = 1;
				}
				else
				{
					puts("noo");
				}

				printf("ControlMod EXEC: TmpUsr.email: %s, TmpUsr.passwd: %s\n", TmpUsr.email, TmpUsr.passwd);

			}
			if (NULL != (pTmpKey = strstr(prequest_head->url, "loginupstat.html")))
			{

			}
		}
		ResponseData(confdArg, prequest_head, pdb);
		if (NULL != prequest_head)
		{
			free(prequest_head);		
		}
		LoginSuccess = 0;
	}

	PRINTuserList();

	sqlite3_close(pdb);


	return 0;
}
