// db.h //

#include <mysql.h>

extern MYSQL *g_db;
extern char g_db_host[256];
extern char g_db_login[256];
extern char g_db_pass[256];
extern char g_db_database[256];
extern int g_db_port;
extern BOOL g_db_setup;

extern void db_log(char *str);
extern BOOL db_init(char *host = NULL, char *login = NULL, char *pass = NULL, char *database = NULL, int port = 0);
extern BOOL db_connect(void);
extern BOOL db_close(void);
extern MYSQL_RES *db_query(int *fields, char *query_str);

extern BOOL db_setup(void);
extern int db_savememberfile(Player *plr);
extern int db_loadmemberfile(Player *plr, BOOL bCheck=FALSE);
extern void db_banmemberfile(char *name, int flag=1);

extern int db_checkwalled(char *sysid);
extern void db_wall(char *sysid, int flag=1);
extern int db_checkdenied(char *macid);
extern void db_deny(char *macid, int flag=1);
extern int db_checkblocked(char *ip);
extern void db_block(char *ip, int flag=1);

extern void db_sysmacipmemberfile(char *name, char *sysid, char *macid, char *ip);
extern int db_getplayertype(char *name); // Find the PlayerType
extern void db_newaccountlog(char *name, char *atype, char *mcode, char *email, char *date);
extern int CheckSerialdb(char *mCode, char *email);
extern void DeleteSerialdb(char *mCode);
extern int db_checkaccountexist(char *name);
extern int db_getidmemberfile(char *name);
extern BOOL db_checkpassword(char *name, char *pass);
extern int db_getblocklist(void);

extern void db_textchanged(char *name, int textformat, int textcolor);
extern void db_cuechanged(char *name, int cuetype);
extern void db_chalkchanged(char *name, int chalktype);
extern void *db_getgamehistory(char *name);
extern void db_updategamehistory(char *name, char *opponent, int gameinfo, int time, int update = -1);
extern void db_updatereracks(char *name, int reracks);
extern void db_logtime(char *name, bool mode);
extern void db_logtimecleanup(void);

extern int db_getLadderRank(char *name, int ladderPoints);
extern std::array<ladderObject, 20> db_getLadderRankings();
extern void db_resetLadderRankings();
extern void db_updateCash(char *name, int cash);
