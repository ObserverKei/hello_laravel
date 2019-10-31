#ifndef __CREATHTML_H__
#define __CREATHTML_H__

#include "public.h"
#include "controlsql.h"

typedef struct urlnodelist_key_s
{
	char key[32];
	char values[32];
}HTML_HREF_URL_MAP_S;

typedef struct
{
	char table[__SHOPPING_STR_LEN_32__];
	char search_id_line[__SHOPPING_STR_LEN_32__];
}HTML_SWITCH_ID_LIST_S;

#define __INDEX_DIV_CARGO__		"../home/index_div_cargo.html"
#define __INDEX_DIV_CARGO_TMP__ "../home/index_div_cargo_tmp/"

extern char *CreateResponesHtml(char *CreateHtmlNameSrcArg, char *CreateHtmlNameDestArg, CTLSQL_S *pctlsqlArg, char *CreateUsrEmailArg);

extern HTML_HREF_URL_MAP_S UrlHrefMapList[];
extern HTML_SWITCH_ID_LIST_S SwichtSqlValuesList[];
#endif
