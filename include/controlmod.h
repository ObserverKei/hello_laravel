#ifndef __CONTROLMOD_H__
#define __CONTROLMOD_H__

#define __LOGIN_USR_STAT_ON__		 1
#define __LOGIN_USR_STAT_COOKIE__	 0
#define __LOGIN_USR_STAT_OFF__ 		-1

#define __LOGIN_USR_MAX_NUM_10__	10

#define __ADD_USR_ERR__ 			-1
#define __ADD_USR_OK__				 0
#define __ADD_USR_REPEAT__			-2

#define __PAYFORGOODS_OK__			0
#define __PAYFORGOODS_ERR__			-1
#define __PAYFORGOODS_DEF__			-2

typedef struct login_usr_s 
{
	int stat;
	int maxnum;
	char email[__SHOPPING_STR_LEN_128__];
	char cookie[__SHOPPING_STR_LEN_512__];
	char admin_id[__SHOPPING_STR_LEN_32__];
	char list_email[__SHOPPING_STR_LEN_32__];
	char list_password[__SHOPPING_STR_LEN_32__];
	char table[__SHOPPING_STR_LEN_32__];
}LOGIN_USR_S;

typedef struct log_pwd_s
{
	char email[__SHOPPING_STR_LEN_128__];
	char passwd[__SHOPPING_STR_LEN_128__];
}LOGIN_PWD_S;

typedef struct goods_s
{
	sqlite3 *pdb;
	char goods_id[__SHOPPING_STR_LEN_32__];
	char price[__SHOPPING_STR_LEN_32__];
}GOODS_S;






extern LOGIN_USR_S login_usr_list[__LOGIN_USR_MAX_NUM_10__];
extern int LoginSuccess;
extern char SignInLockHtml[][__SHOPPING_STR_LEN_32__];
extern int SignInLockHtmlLen;

extern int LoginUsrInit(void);
extern char *GetEmailPasswd(sqlite3 *pdbArg, char *pEmailArg);
extern void PRINTuserList(void);

extern int ExecNewPasswd(char *PostStrArg, char *Email, char *PassWord, char *RePassWord);
extern int LoginUpNewEmail(sqlite3 *pdbArg, char *PostStrArg);
extern int IsLoginStat(char *cookieArg, char *getEmailArg);
extern int IsSignInLockHtml(char *HtmlFileNameArg);
extern GOODS_S ExecPostToGoods(char *pPostStrArg, sqlite3 *pdbArg);
extern int PayForGoods(GOODS_S *GoodsArg, char *EmailArg);
extern int ExecPostToChangeMoney(char *pPostStrArg, char *EmailArg, sqlite3 *pdbArg);
extern int DeleteLoginStat(char *cookieArg);

extern int ControlMod(int confdArg);


#endif
