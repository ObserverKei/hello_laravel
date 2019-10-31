#include "createhtml.h"


HTML_HREF_URL_MAP_S UrlHrefMapList[] = {
	{"open_detail_href", "detail_tmp/detail_"},
	{"open_buy_href", "buy_tmp/buy_"},
};

HTML_SWITCH_ID_LIST_S SwichtSqlValuesList[] = {
	{"goods", "goods_id"},
	{"admin_user", "admin_id"},
};



char *CreateResponesHtml(char *CreateHtmlNameSrcArg, char *CreateHtmlNameDestArg, CTLSQL_S *pctlsqlArg, char *CreateUsrEmailArg)
{
	int i = 0;
	int k = 0;
	FILE *fpsrc = NULL;
	FILE *fpdest = NULL;
	char TmpBuff[__SHOPPING_STR_LEN_1024__] = {0};
	int Ret = 0;
	CTLSQL_S *pTmpMaxLine;
	char *pTmp = NULL;
	char *pTmpEnd = NULL;
	char *pTmpKey = NULL;
	char TmpLineName[__SHOPPING_STR_LEN_32__] = {0};
	char TmpName[__SHOPPING_STR_LEN_512__] = {0};
	char TmpHref[__SHOPPING_STR_LEN_128__] = {0};
	int FindReplace = 0;
	char SaveGoodId[__SHOPPING_STR_LEN_32__] = {0};
	int FindReplaceHref = 0;
//	CTLSQL_S TmpMaxLine;	
//	CTLSQL_S *pTmpMaxLine = &TmpMaxLine;
	
	if (NULL != CreateUsrEmailArg)
	{
		printf("CreateResponesHtml of ThisLoginUsrEmail : %s\n", CreateUsrEmailArg);
	}
	printf("CreateHtmlNameSrcArg: %s\n", CreateHtmlNameSrcArg);
	printf("=======================================goods_id: %s------------------------------------------\n", pctlsqlArg->goods_id);

	pTmpMaxLine = malloc(sizeof(CTLSQL_S));
	memcpy(pTmpMaxLine, pctlsqlArg, sizeof(CTLSQL_S));

	puts(strcpy(pTmpMaxLine->data, "==="));
	pTmpMaxLine->flag = 1;

	ControlSQL(pTmpMaxLine);
	if (! strncmp(pTmpMaxLine->data, "===", 3))
	{
		printf("9999999999999999999999999\n");
		return NULL;
	}
	pTmpMaxLine->flag = 2;
	printf("\n\n\n***********************************%s##########################\n", pTmpMaxLine->data);
	
	printf("CreateResponesHtml: goods_id: %s\n", pctlsqlArg->goods_id);
	strcpy(SaveGoodId, pctlsqlArg->goods_id);

	pTmpEnd = CreateHtmlNameSrcArg + strlen(CreateHtmlNameSrcArg)-1;
	while ('.' != *pTmpEnd && pTmpEnd > CreateHtmlNameSrcArg)
	{
		pTmpEnd--;
	}
	if (pTmpEnd == CreateHtmlNameSrcArg)
	{
		perror("fail to CreateResponesHtml");
		return NULL;
	}
	strncpy(CreateHtmlNameDestArg, CreateHtmlNameSrcArg, pTmpEnd - CreateHtmlNameSrcArg);

	pTmp = CreateHtmlNameSrcArg + strlen(CreateHtmlNameSrcArg)-1;
	while ('/' != *pTmp && pTmp > CreateHtmlNameSrcArg)
	{
		pTmp--;
	}
	pTmp++;
	
	i = 0;
	while ('.' != *pTmp && i < sizeof(TmpName))
	{
		TmpName[i] = *pTmp;
		pTmp++;
		i++;
	}
	TmpName[i] = '\0';
	printf("TmpName:%s\n", TmpName);

	sprintf(CreateHtmlNameDestArg, "%s_tmp/%s_%s.html", CreateHtmlNameDestArg, TmpName, pctlsqlArg->goods_id);
	
	if (( ! access(CreateHtmlNameDestArg, F_OK)) && ( ! IsSignInLockHtml(CreateHtmlNameSrcArg)))
	{
		printf("not is lock file html\n");
		return CreateHtmlNameDestArg;
	}

	fpsrc = fopen(CreateHtmlNameSrcArg, "r");
	if (NULL == fpsrc)
	{
		perror("fiail to fopen of src of CreateResponesHtml");
		return "../home/index.html";
	}
	fpdest = fopen(CreateHtmlNameDestArg, "w");
	if (NULL == fpsrc)
	{
		perror("fail to fopen of dest of CreateResponesHtml");
		return "../home/index.html";
	}

	printf(" src:%s\n", CreateHtmlNameSrcArg);
	printf("dest:%s\n", CreateHtmlNameDestArg);

	while (NULL != fgets(TmpBuff, sizeof(TmpBuff), fpsrc))
	{
		fprintf(stderr, "fgets ing: fpsrc: %s\n", TmpBuff);
		int ss = 0;
		printf("miao!%d ", ss);
		for (k = 0; k < sizeof(SwichtSqlValuesList) / sizeof(SwichtSqlValuesList[0]); k++)
		{

			if ( ! strcmp(SwichtSqlValuesList[k].table, "admin_user"))
			{
				strcpy(pTmpMaxLine->table, SwichtSqlValuesList[k].table);
				strcpy(pTmpMaxLine->search_id_line, SwichtSqlValuesList[k].search_id_line);
			}
			else
			{
				strcpy(pTmpMaxLine->goods_id, SaveGoodId);
				strcpy(pTmpMaxLine->table, SwichtSqlValuesList[k].table);
				strcpy(pTmpMaxLine->search_id_line, SwichtSqlValuesList[k].search_id_line);
			}
			
			FindReplace = 0;
			pTmpMaxLine->flag = __CTLSQL_EXEC_LINE_NAME__;
			for (i = 0; i < ControlSQL(pTmpMaxLine)->linelistcnt; i++)
			{
				if (NULL != (pTmpKey = strstr(TmpBuff, pTmpMaxLine->linelist[i])))
				{
					strcpy(TmpLineName, pTmpMaxLine->linelist[i]);
					printf("SwichtSqlValuesList[k].table: %s\n", SwichtSqlValuesList[k].table);
					printf("SwichtSqlValuesList[k].table: %s\n", SwichtSqlValuesList[k].table);
					printf("SwichtSqlValuesList[k].table: %s\n", SwichtSqlValuesList[k].table);
						
					FindReplace = 1;break;
				}
			}
			if (1 == FindReplace)
			{
				break;
			}
		}

		if (0 == FindReplace)
		{
			FindReplaceHref = 0;
			for (i = 0; i < sizeof(UrlHrefMapList) / sizeof(UrlHrefMapList[0]); i++)
			{
				if (NULL != (pTmpKey = strstr(TmpBuff, UrlHrefMapList[i].key)))
				{
					FindReplaceHref = 1;break;
				}
			}
			if (0 == FindReplaceHref)
			{
				fwrite(TmpBuff, sizeof(char), strlen(TmpBuff), fpdest);
			}
			else if(1 == FindReplaceHref)
			{	
				fwrite(TmpBuff, sizeof(char), pTmpKey - TmpBuff, fpdest);

				sprintf(TmpHref, "%s%s.html", UrlHrefMapList[i].values, pctlsqlArg->goods_id);

				fwrite(TmpHref, sizeof(char), strlen(TmpHref), fpdest);

				pTmpKey += strlen(UrlHrefMapList[i].key);
				fwrite(pTmpKey, sizeof(char), strlen(pTmpKey), fpdest);

				FindReplaceHref = 0;
			}
		}
		else if (1 == FindReplace)
		{
			fwrite(TmpBuff, sizeof(char), pTmpKey - TmpBuff, fpdest);

			pTmpMaxLine->flag = __CTLSQL_SEARCH__;
			strcpy(pTmpMaxLine->line, pTmpMaxLine->linelist[i]);
			if ((NULL != CreateUsrEmailArg) && ( ! strcmp(pTmpMaxLine->table, "admin_user")))
			{
				printf("&&&&&&&&&&&&&&&&&&&&&fine the admin_user table &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

				sprintf(pTmpMaxLine->goods_id, "\"%s\"", CreateUsrEmailArg);
				strcpy(pTmpMaxLine->search_id_line, "email");
			}
			printf("Map table: %s\n", pTmpMaxLine->table);

			pTmp = ControlSQL(pTmpMaxLine)->data;

			fwrite(pTmp, sizeof(char), strlen(pTmp), fpdest);

			pTmpKey += strlen(TmpLineName);
			fwrite(pTmpKey, sizeof(char), strlen(pTmpKey), fpdest);

			FindReplace = 0;
		}		
	}
	if (NULL != fpsrc)
	{
		fclose(fpsrc);
	}
	if (NULL != fpdest)
	{
		fclose(fpdest);
	}
	if (NULL != pTmpMaxLine)
	{
		free(pTmpMaxLine);
	}


	return CreateHtmlNameDestArg;
}

