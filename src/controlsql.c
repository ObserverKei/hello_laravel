#include "public.h"
#include "controlsql.h"


sqlite3 *SqlInit(char *DictSqlPathArg)
{
	char cmdbuff[1024] = {0};
	sqlite3 *pdb = NULL;
	char *perrmsg = NULL;

	if (SQLITE_OK != sqlite3_open(DictSqlPathArg, &pdb))
	{
		fprintf(stderr, 
				"fail to sqlite3_open %s\n", 
				DictSqlPathArg
		);
		return NULL;
	}

	return pdb;
}

CTLSQL_S *ControlSQL(CTLSQL_S *pctlsqlArg)
{
	char *perrmsg = NULL;
	char cmdbuff[1024] = {0};
#if 0
	printf("%d\n", pctlsqlArg->flag);
	printf("table %s\n", pctlsqlArg->table);
	printf("line %s\n", pctlsqlArg->line);
	printf("goods_id %s", pctlsqlArg->goods_id);
#endif
	if (__CTLSQL_SEARCH__ == pctlsqlArg->flag)
	{
		/* select goods_name from goods where goods_name like "%诺%"; */
		if (strlen(pctlsqlArg->search_id_line) < 1)
		{
			strcpy(pctlsqlArg->search_id_line, "goods_id");
		}
		sprintf(cmdbuff, "select %s from %s where %s=%s;", pctlsqlArg->line, pctlsqlArg->table, pctlsqlArg->search_id_line, pctlsqlArg->goods_id);
	}
	else if (__CTLSQL_LINE_MAX__ == pctlsqlArg->flag)
	{
		sprintf(cmdbuff, "select MAX(%s) from %s;", pctlsqlArg->line, pctlsqlArg->table);
	}
	else if (__CTLSQL_EXEC_LINE_NAME__ == pctlsqlArg->flag)
	{
		sprintf(cmdbuff, "select * from %s where rowid=1;", pctlsqlArg->table);
	}
	else if (__CTLSQL_SEARCH_ROWID__ == pctlsqlArg->flag)
	{		
		sprintf(cmdbuff, "select %s from %s where rowid=%s;", pctlsqlArg->line, pctlsqlArg->table, pctlsqlArg->goods_id);
	}
	else if (__CTLSQL_SEARCH_GOODS_NAME_RET_GOODS_ID__ == pctlsqlArg->flag)
	{
		sprintf(cmdbuff, "select %s from %s where goods_id=%s;", pctlsqlArg->line, pctlsqlArg->table, pctlsqlArg->goods_id);
	}
	else if (__CTLSQL_SEARCH_PASSWD__ == pctlsqlArg->flag)
	{
		sprintf(cmdbuff, "select password,%s from %s where email=\"%s\";", pctlsqlArg->line, pctlsqlArg->table, pctlsqlArg->data);
	}
	else if (__CTLSQL_ADD_NEWUSR__ == pctlsqlArg->flag)
	{
		sprintf(cmdbuff, "insert into admin_user values(%s, 0, \"%s\", \"%s\", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);", pctlsqlArg->goods_id, pctlsqlArg->email, pctlsqlArg->passwd);
	}
	else if (__CTLSQL_UPDATE__ == pctlsqlArg->flag)
	{
		sprintf(cmdbuff, "update %s set %s=%s where %s=%s;", pctlsqlArg->table, pctlsqlArg->line, pctlsqlArg->data, pctlsqlArg->search_id_line, pctlsqlArg->goods_id);
	}
	
//printf("%s\n", cmdbuff);
	if (SQLITE_OK != sqlite3_exec(pctlsqlArg->pdb, cmdbuff, CallBackFunc, pctlsqlArg, &perrmsg))
	{
		printf("ERROR!!!!!!!!!!!!!!!!!!!!!!!!%s\n", cmdbuff);
		fprintf(stderr,	"fail to sqlite3_exec:%s\n", perrmsg);
		sqlite3_free(perrmsg);
		return NULL;
	}


	return pctlsqlArg;
}

int CallBackFunc(void *arg, int num, char **content, char **title)
{
	CTLSQL_S *pctlsqlArg = (CTLSQL_S *)arg;

	memset(pctlsqlArg->linelist, 0, sizeof(pctlsqlArg->linelist));
	for (pctlsqlArg->linelistcnt = 0; pctlsqlArg->linelistcnt < num; pctlsqlArg->linelistcnt++)
	{
		strcpy(pctlsqlArg->linelist[pctlsqlArg->linelistcnt], title[pctlsqlArg->linelistcnt]);
	}
	strcpy(pctlsqlArg->data, *content);
	pctlsqlArg->callbackfuncstat = 1;

	return 0;
}


