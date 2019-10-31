#ifndef __DICTSQLITE3_H__
#define __DICTSQLITE3_H__

typedef struct ctlsql_s
{
	int	flag;
	sqlite3 *pdb;
	char table[__SHOPPING_STR_LEN_32__];
	char line[__SHOPPING_STR_LEN_32__];
	char row[__SHOPPING_STR_LEN_32__];
	char goods_id[__SHOPPING_STR_LEN_128__];
	char search_id_line[__SHOPPING_STR_LEN_32__];
	int linelistcnt;
	char linelist[__SHOPPING_SQL_MAX_LINE_50__][15];
	int callbackfuncstat;
	char email[__SHOPPING_STR_LEN_128__];
	char passwd[__SHOPPING_STR_LEN_128__];
	char data[__SHOPPING_STR_LEN_4096__];
}CTLSQL_S;

extern CTLSQL_S *ControlSQL(CTLSQL_S *pctlsqlArg);
extern sqlite3 *DictSqlInit(int argc, const char *argv[]);
extern int CallBackFunc(void *arg, int num, char **content, char **title);

#define __SHOPPING_DB__				"../home/shopping.db"
#define __CTLSQL_SEARCH__							1
#define __CTLSQL_LINE_MAX__							2
#define __CTLSQL_EXEC_LINE_NAME__					3
#define __CTLSQL_SEARCH_ROWID__						4
#define __CTLSQL_SEARCH_GOODS_NAME_RET_GOODS_ID__	5
#define __CTLSQL_SEARCH_PASSWD__					6
#define __CTLSQL_ADD_NEWUSR__						7
#define __CTLSQL_SEARCH_EMAIL_RET_GOODS_ID__		8
#define __CTLSQL_UPDATE__							9





typedef struct dict_s
{
	int flag;
	char word[__SHOPPING_STR_LEN_32__];
	char explain[__SHOPPING_STR_LEN_1024__];
}DICT_S;

extern sqlite3 *SqlInit(char *DictSqlPathArg);
extern int AddDictToSql(const char *DictTextPathArg, sqlite3 *pdbArg);
extern DICT_S *SqlDictSearch(sqlite3 *pdbArg, char *wordArg);
extern int callBackfunc(void *arg, int num, char **content, char **title);


#define __DICTPATH_TXT__ "dict.txt"
#define __DICTPATH_DB__ "dict.db"

#endif
