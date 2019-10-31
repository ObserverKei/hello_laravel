#ifndef __SEARCH_H__
#define __SEARCH_H__

#define __SHOPPING_SEARCH_MAX_ID_50__	(50)
#define __SHOPPING_SEARCH_MAX_ID_100__	(100)

typedef struct search_s
{
	int goods_id_len;
	char goods_id_list[__SHOPPING_SEARCH_MAX_ID_100__][__SHOPPING_STR_LEN_32__];
}SEARCH_S;

char *UrlExecCode(char *HttpSearchStrArg);
SEARCH_S *Search(sqlite3 *pdbArg, char *HttpSearchStrArg);


#endif
