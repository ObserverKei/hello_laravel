#include "public.h"
#include "sqlite3.h"
#include "controlsql.h"
#include "search.h"

//解码Url sscanf 版本
char *UrlExecCode(char *HttpSearchStrArg)
{
	char *pTmpUrl = NULL;
	char *pTmpExec = NULL;
	char UrlHex[10] = {0};
	int Dec[3] = {0};
	int i = 0;

	pTmpUrl = HttpSearchStrArg;
	pTmpExec = HttpSearchStrArg;

	while ('\0' != *pTmpUrl)
	{
		if ('%' == *pTmpUrl)
		{
			memcpy(UrlHex, pTmpUrl, sizeof(UrlHex)-1);
			UrlHex[sizeof(UrlHex)-1] = '\0';
			
			sscanf(UrlHex, "%%%x%%%x%%%x", Dec + 0, Dec + 1, Dec + 2);

			for(i = 0; i < 3; i++)
			{
				*pTmpExec++ = (char)Dec[i];
			}
			pTmpUrl += strlen(UrlHex);
		}
		else if ('+' == *pTmpUrl)
		{
			*pTmpExec++ = ' ';
			pTmpUrl++;
		}
		else
		{
			*pTmpExec++ = *pTmpUrl++;
		}
	}
	
	*pTmpExec = '\0';

	return HttpSearchStrArg;
}


SEARCH_S *Search(sqlite3 *pdbArg, char *HttpSearchStrArg)
{
	int i = 0;
	int j = 0;
	char TmpBuff[1024] = "%E8%AF%BA%E5%9F%BA%E4%BA%9A+KD876+%E8%80%B3%E6%9C%BA";
	char *pTmp[__SHOPPING_SEARCH_MAX_ID_50__] = {NULL};
	SEARCH_S *pSearchId = NULL;
	sqlite3 *pdb = NULL;
	CTLSQL_S Tmpctlsql = {
		__CTLSQL_LINE_MAX__,
		pdbArg,
		"goods",
		"rowid",
		"1",
		"1",
	};
	CTLSQL_S TmpctlsqlSearch = {
		__CTLSQL_SEARCH_GOODS_NAME_RET_GOODS_ID__,
		pdbArg,
		"goods",
		"goods_name",
		"1",
		"1",
	};
	int k = 0;
	int SearchNodeKill = 0;

	pSearchId = malloc(sizeof(SEARCH_S));
	if (NULL == pSearchId)
	{
		perror("fail to malloc of Search");
		return NULL;
	}
	memset(pSearchId, 0, sizeof(SEARCH_S));


	puts(HttpSearchStrArg);

	puts(UrlExecCode(HttpSearchStrArg));

	i = 0;
	pTmp[i] = strtok(HttpSearchStrArg, " ");
	i++;
	while (NULL != (pTmp[i] = strtok(NULL, " ")))
	{
		i++;
	}

	for (i = 0; i < atoi(ControlSQL(&Tmpctlsql)->data); i++)
	{
		sprintf(TmpctlsqlSearch.goods_id, "%d", i+1);

		strcpy(TmpctlsqlSearch.data, pTmp[0]);

		for (j = 0; j < sizeof(pTmp) / sizeof(pTmp[0]); j++)
		{
			if (NULL == pTmp[j])
			{
				break;
			}

			ControlSQL(&TmpctlsqlSearch);
			if ( ! strcmp(TmpctlsqlSearch.data, pTmp[j]))
			{
				continue;
			}
			if (NULL != strstr(TmpctlsqlSearch.data, pTmp[j]))
			{
				SearchNodeKill = 0;
				for (k = 0; k < pSearchId->goods_id_len; k++)
				{
					if (! strcmp(pSearchId->goods_id_list[k], TmpctlsqlSearch.goods_id))
					{
						SearchNodeKill = 1;
						break;
					}
				}
				if (0 == SearchNodeKill)
				{
					printf("goods_id: %s\n", TmpctlsqlSearch.goods_id);
					strcpy(pSearchId->goods_id_list[pSearchId->goods_id_len++], TmpctlsqlSearch.goods_id);
				}
				else if (1 == SearchNodeKill)
				{
					SearchNodeKill = 0;
					continue;
				}
			}
		}
	}


	return pSearchId;
}
