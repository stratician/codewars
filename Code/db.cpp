// website: "ways of improving the game"
/*
Xdb_execute("ALTER TABLE ".USERS_TABLE." ADD user_rating_age int DEFAULT 0"); // age
Xdb_execute("ALTER TABLE ".USERS_TABLE." ADD user_rating_name varchar(64)"); // name
Xdb_execute("ALTER TABLE ".USERS_TABLE." ADD user_rating_disable tinyint(1) DEFAULT 0"); // disable rating
Xdb_execute("ALTER TABLE ".FORUMS_TABLE." ADD forum_imgtopics mediumint DEFAULT 0"); // imgtopics count //
*/
// db.cpp //
#include "../CodeSupp/Build.h"
#include "../CodeSupp/stdafx.h"
#include <direct.h>
#include "../CodeSupp/GameAppView.h"
#include "../Code/lobby.h"

#include "Lobby.h"

#include <mysql.h>

#include "db.h"

#define QUIET_LOG

MYSQL *g_db = NULL;
char g_db_host[256];
char g_db_login[256];
char g_db_pass[256];
char g_db_database[256];
char g_db_prefix[256];
int g_db_port;
BOOL g_db_setup = FALSE;

extern void db_debugmemberfile(Player *plr);

int db_reconnectmode = 1; // 0 - reconnect on each, 1 - don't reconnect.
char **db_stringgood_tmpstr;
char *db_pstringgood_tmpstr;
int db_pstringgood_tmpstr_idx;
int db_pstringgood_tmpstr_n;

void db_log(char *str)
{
	#ifdef QUIET_LOG
	if(g_gameAppView!=NULL)	ErrorMessage(g_gameAppView, str);
	#else
	AfxMessageBox(str);
	#endif	
}

void db_stringgood_init(void)
{
	int n;
	db_pstringgood_tmpstr_n = 64; // wraparound - aslong as we're doing quick access, and dont go more than 64 instantly then we're all good :) //
	db_pstringgood_tmpstr_idx = 0; 
	db_stringgood_tmpstr = new char *[db_pstringgood_tmpstr_n];
	for(n=0;n<db_pstringgood_tmpstr_n;n++)
	db_stringgood_tmpstr[n] = NULL;
}

void db_stringgood_reset(void)
{
	while(db_pstringgood_tmpstr_idx)
	{
	db_pstringgood_tmpstr_idx--;
	delete db_stringgood_tmpstr[db_pstringgood_tmpstr_idx];
	}
}

char *db_stringgood(char *str)
{
	int n;
	int m;
	int len;
	int len2;

	// count required size of destination buffer i.e. add 1 extra byte per '
	len = 0;
	len2 = 0;
	for(n=0;n<8192;n++)
	{
	if(!str[n]) break;
	len++;
	len2++;
	if(str[n]=='\'') len2++;
	}
	// allocate this required size
	if(db_stringgood_tmpstr[db_pstringgood_tmpstr_idx]!=NULL) delete db_stringgood_tmpstr[db_pstringgood_tmpstr_idx]; // delete if already accessed //
	db_stringgood_tmpstr[db_pstringgood_tmpstr_idx] = new char[len2+1];
	db_pstringgood_tmpstr = db_stringgood_tmpstr[db_pstringgood_tmpstr_idx];
	m = 0;
	for(n=0;n<len;n++)
	{
	db_pstringgood_tmpstr[m] = str[n];
	if(str[n]=='\'')
	{
	m++;
	db_pstringgood_tmpstr[m] = '\'';
	}
	m++;
	}
	db_pstringgood_tmpstr[len2] = 0; // add terminator.

	db_pstringgood_tmpstr_idx++;
	if(db_pstringgood_tmpstr_idx==db_pstringgood_tmpstr_n) db_pstringgood_tmpstr_idx = 0; // Wraparound //
	return db_pstringgood_tmpstr;
}

BOOL db_init(char *host, char *login, char *pass, char *database, int port)
{
#ifdef MAIN_SERVER
	db_stringgood_init();
	if(host==NULL); // don't override the parameters.
	else {
		strcpy(g_db_host,host);
		strcpy(g_db_login,login);
		strcpy(g_db_pass,pass);
		strcpy(g_db_database,database);
		g_db_port = port;
		}
	#ifdef THE_POOL_CLUB
		strcpy(g_db_prefix,"ipool_");
	#else 
		strcpy(g_db_prefix,"isnk_");
	#endif

	//sprintf(buf,"host:%s\nlogin:%s\npass:%s\ndatabase:%s\nport:%d", g_db_host, g_db_login, g_db_pass, g_db_database, g_db_port);
	

	if(db_reconnectmode==1) // reconnect everytime.
	{
	db_connect();
	}


	db_setup();
#endif
	return TRUE;
}

BOOL db_connect(void)
{
#ifdef MAIN_SERVER
	char buf[256];
	g_db = mysql_init(NULL);
	if(!g_db)
		{
		sprintf(buf,"error: mysql_init (%s)", mysql_error(g_db));
		db_log(buf);
		return FALSE;
		}

	if(g_db)
	{

	mysql_options(g_db, MYSQL_READ_DEFAULT_GROUP, "test_libmysqld_CLIENT");
	if(!mysql_real_connect(g_db, g_db_host, g_db_login, g_db_pass, g_db_database, 
		g_db_port, NULL, 0))
		{
		sprintf(buf,"error: mysql_real_connect (%s)", mysql_error(g_db));
		db_log(buf);

		return FALSE;
		}
	return TRUE;
	}
#endif
	return FALSE;
}

BOOL db_close(void)
{
#ifdef MAIN_SERVER
	if(!g_db) return FALSE;
	mysql_close(g_db);
	g_db = NULL;
#endif
	return TRUE;
}

MYSQL_RES *g_res = NULL;

MYSQL_RES *db_query(int *fields, char *query_str)
{
	char buf[1024];
	//MYSQL_RES *res;
#ifdef MAIN_SERVER
	if(g_res!=NULL) mysql_free_result(g_res);
	*fields = 0;
	if(db_reconnectmode==0) // reconnect everytime.
	{
	if(db_connect()==FALSE) return NULL;
	}
	if(!g_db) return NULL;
	
	g_res = NULL;
	if(mysql_query(g_db, query_str)!=0)
	{
	sprintf(buf,"error: mysql_query (%s)", mysql_error(g_db));
	db_log(buf);
	//return NULL;	
	}
	else { // query good
	if((*fields = mysql_field_count(g_db))>0) // one or more fields.
	{
	g_res = mysql_store_result(g_db);
	}
	//else return NULL;		
	}
	if(db_reconnectmode==0) // reconnect everytime.
	{
	db_close();
	}
#endif MAIN_SERVER
	return g_res;
}



// ---------------------- //
extern void TOMYSQLAllMembers(void);

void db_gamesetup(void)
{

	AfxMessageBox("About to Delete Entire Database and Create a New One, Are you Sure?");
	AfxMessageBox("IMPORTANT!!! (Check with Brandon before Proceeding)");


	char sql[4096];
	int fields;

	sprintf(sql,"DELETE FROM %smembers",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %smembers",g_db_prefix);db_query(&fields, sql);

	sprintf(sql,"CREATE TABLE %smembers (member_id int PRIMARY KEY AUTO_INCREMENT, name varchar(32), password varchar(32), \
		rating smallint, wins int, losses int, streak smallint, \
		sysid varchar(16), macid varchar(16), ip varchar(16), \
		banned tinyint, walled tinyint, denied tinyint, blocked tinyint, \
		_50s smallint, _60s smallint, _70s smallint, _80s smallint, _90s smallint, _100s smallint, \
		highbreak smallint, max147s smallint, \
		_p50s smallint, _p60s smallint, _p70s smallint, _p80s smallint, _p90s smallint, _p5100s smallint, \
		phighbreak smallint, pmax147s smallint, \
		bioaverageshottime float, bioaverageposition float, bioaveragesafety float, bioaveragebreak float, \
		pbioaverageshottime float, pbioaverageposition float, pbioaveragesafety float, pbioaveragebreak float, \
		winner smallint, runnerup smallint, semifinals smallint, quarterfinals smallint, last16 smallint, last32 smallint, \
		textformat varchar(16), state2 bigint, inventory varchar(16), highestrating smallint, \
		cash int, cuetype smallint, chalktype int, expiry varchar(16))",g_db_prefix);db_query(&fields, sql);


	//sprintf(sql,"ALTER TABLE %smembers ADD banned tinyint DEFAULT 0",g_db_prefix);db_query(&fields, sql); // banned flag/type
	sprintf(sql,"ALTER TABLE %smembers ADD account tinyint DEFAULT 0",g_db_prefix);db_query(&fields, sql); // account flag/type
	
	sprintf(sql,"ALTER TABLE %smembers ADD type varchar(16) DEFAULT 0",g_db_prefix);db_query(&fields, sql); // account flag/type
	sprintf(sql,"ALTER TABLE %smembers ADD rexpiry int DEFAULT 0",g_db_prefix);db_query(&fields, sql); // account flag/type
	sprintf(sql,"ALTER TABLE %smembers ADD updated int DEFAULT 0",g_db_prefix);db_query(&fields, sql); // account flag/type

	sprintf(sql,"ALTER TABLE %smembers ADD bandata int DEFAULT 0",g_db_prefix); db_query(&fields, sql); // account flag/type

	sprintf(sql,"ALTER TABLE %smembers ADD dbcookie varchar(64) DEFAULT 0",g_db_prefix);db_query(&fields, sql); // account flag/type

	sprintf(sql,"ALTER TABLE %smembers ADD level smallint DEFAULT 1, level_progress DEFAULT 10",g_db_prefix);db_query(&fields, sql); // account flag/type
	
	// Note 14803425 is RGB(225,225,225)
	sprintf(sql,"ALTER TABLE %smembers ADD textcolor bigint DEFAULT 14803425",g_db_prefix);db_query(&fields, sql); // account flag/type 
	
	// Ladder Points
	sprintf(sql, "ALTER TABLE %smembers ADD ladderpoints int DEFAULT 12000", g_db_prefix); db_query(&fields, sql); // account flag/type
	sprintf(sql, "ALTER TABLE %smembers ADD ladderwinslosses int DEFAULT 0", g_db_prefix); db_query(&fields, sql); // account flag/type
	sprintf(sql, "ALTER TABLE %smembers ADD ladderhighbreak int DEFAULT 0", g_db_prefix); db_query(&fields, sql); // account flag/type
	
	// High Streak Stat
	sprintf(sql, "ALTER TABLE %smembers ADD highstreak int DEFAULT 0", g_db_prefix); db_query(&fields, sql); // account flag/type
	sprintf(sql, "ALTER TABLE %smembers ADD realcash int DEFAULT 0", g_db_prefix); db_query(&fields, sql); // account flag/type
	

	// Create Comments Table //
	sprintf(sql,"DELETE FROM %smembers_comments",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %smembers_comments",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"CREATE TABLE %smembers_comments (member_id int, comment varchar(2048))",g_db_prefix);db_query(&fields, sql);


	// Create NewAccounts Table //
	sprintf(sql,"DELETE FROM %smembers_newaccounts",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %smembers_newaccounts",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"CREATE TABLE %smembers_newaccounts (member_id int, atype varchar(2), mcode varchar(20), email varchar(128), date varchar(16))",g_db_prefix);db_query(&fields, sql);


	// Create Walled Table //
	sprintf(sql,"DELETE FROM %smembers_walled",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %smembers_walled",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"CREATE TABLE %smembers_walled (sysid varchar(16))",g_db_prefix);db_query(&fields, sql);
	// Create Denied Table //
	sprintf(sql,"DELETE FROM %smembers_denied",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %smembers_denied",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"CREATE TABLE %smembers_denied (macid varchar(16))",g_db_prefix);db_query(&fields, sql);
	// Create Blocked Table //
	sprintf(sql,"DELETE FROM %smembers_blocked",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %smembers_blocked",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"CREATE TABLE %smembers_blocked (ip varchar(16))",g_db_prefix);db_query(&fields, sql);

	// Create LogTime Table //
	sprintf(sql,"DELETE FROM %slogtime",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %slogtime",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"CREATE TABLE %slogtime (logtime_id int PRIMARY KEY AUTO_INCREMENT, name varchar(32), time int, mode smallint)",g_db_prefix);db_query(&fields, sql);


	//sprintf(sql,"ALTER TABLE %smembers ADD winner smallint", g_db_prefix);db_query(&fields, sql);

	/*
	//TOMYSQLAllMembers();
	
	sprintf(sql,"CREATE TABLE %steams (team_id int PRIMARY KEY AUTO_INCREMENT, member_id int, name varchar(32), \
			formation smallint)",g_db_prefix);db_query(&fields, sql);

	sprintf(sql,"ALTER TABLE %steams ADD shirt mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD shorts mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD socks mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD shirtGK mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD shortsGK mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD socksGK mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD shirtAway mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD shortsAway mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD socksAway mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD shirtGKAway mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD shortsGKAway mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %steams ADD socksGKAway mediumint unsigned", g_db_prefix);db_query(&fields, sql);

	// -- New Setup for Server -- //

	sprintf(sql,"ALTER TABLE %smembers ADD banned tinyint DEFAULT 0",g_db_prefix);db_query(&fields, sql); // banned flag/type
	sprintf(sql,"ALTER TABLE %smembers ADD account tinyint DEFAULT 0",g_db_prefix);db_query(&fields, sql); // account flag/type
*/
	/*
	sprintf(sql,"CREATE TABLE %splayers (player_id int PRIMARY KEY AUTO_INCREMENT, team_id int, name varchar(32), \
			position smallint)",g_db_prefix);db_query(&fields, sql);

	sprintf(sql,"ALTER TABLE %splayers ADD speed tinyint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %splayers ADD stamina tinyint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %splayers ADD skill tinyint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %splayers ADD iq tinyint unsigned", g_db_prefix);db_query(&fields, sql);

	sprintf(sql,"ALTER TABLE %splayers ADD skin mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %splayers ADD hair mediumint unsigned", g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"ALTER TABLE %splayers ADD boots mediumint unsigned", g_db_prefix);db_query(&fields, sql);
*/

	// positions
	// 0-10 players on pitch
	// 11-13 subs
	// 14+ reserves

	AfxMessageBox("MYSQL DATABASE SETUP\r\n\r\nmake sure you remove the <db_setup> from settings.txt");
}

BOOL db_setup(void) // checks database to see if it's fresh.
{
#ifdef MAIN_SERVER
	if(g_db_setup) db_gamesetup();

	/*char sql[1024];
	char buf[256];
	MYSQL_RES *res;
	int fields;
	int tables;
	MYSQL_ROW row,end_row;
	res = db_query(&fields, "show tables");
	tables = 0;
	if(res==NULL) return FALSE; // failed to setup.

	if(fields)
	{
	while(row = mysql_fetch_row(res))
			{
			tables++;
			}
	}
	
	if(tables) {
			return TRUE; // already setup.
				}*/	
	
	// Setup new database /////////////////////////////////////////////////
	/*Player plr;

	strcpy(plr.name, "ichidan2");
	db_loadmemberfile(&plr, FALSE);
	plr.rating = 1234;
	db_savememberfile(&plr);
	return TRUE;*/
	// tiny/small/medium/int (8/16/24/32 bit in size) //

	// Create Members Structure //
/*	sprintf(sql,"DELETE FROM %smembers",g_db_prefix);db_query(&fields, sql);
	sprintf(sql,"DROP TABLE %smembers",g_db_prefix);db_query(&fields, sql);

	sprintf(sql,"CREATE TABLE %smembers (member_id int PRIMARY KEY AUTO_INCREMENT, name varchar(32), password varchar(32), \
		email varchar(255), rating smallint, wins smallint, losses smallint, streak smallint, \
		reracks smallint, draws smallint, expiry varchar(16))",g_db_prefix);db_query(&fields, sql);

	TOMYSQLAllMembers();
	AfxMessageBox("MYSQL DATABASE SETUP");
*/

	/*sprintf(sql,"CREATE TABLE %smembers (member_id int, name varchar(32), password varchar(32), \
		email varchar(255), rating smallint, wins smallint, losses smallint, streak smallint, \
		reracks smallint, draws smallint, expiry varchar(16), PRIMARY KEY (member_id), AUTO_INCREMENT (member_id))",g_db_prefix);db_query(&fields, sql);
*/
#endif
	return TRUE; // setup
}


void DateToStr2(char *str, int day, int month, int year)
{
	sprintf(str,"%d/%d/%d",day, month, year);
}




int db_savememberfile(Player *plr)
{	// if can't save dont worry //
#ifdef MAIN_SERVER
	char sql[8192];
	char email[64];


	int fields;
	BOOL update=FALSE;
	MYSQL_RES *res;
	int account;
	

	if(g_db_setup)
		{
		int banned = g_sql_banned;
		plr->control = 0;
		plr->control |= g_sql_banned;
		}

	// NEW // 
	char dstr[80];
	int d,m,y;
	d = plr->ExpDay;
	m = plr->ExpMonth;
	y = plr->ExpYear;
	y+=2000;
	DateToStr2(dstr, d, m, y);

	

	account = 0;
	if(plr->state&PLRSTATE_GUESTMEMBER) account = 1;
	g_sql_trial = account;

	char type[16];
	if(g_sql_trial==TRUE) sprintf(type, "TRIAL");
		else sprintf(type, "MEMBER");

	

	// CHECK IF ACCOUNT EXISTS //
	sprintf(sql,"select * from %smembers where name='%s'",g_db_prefix, db_stringgood(plr->name)); 
	res = db_query(&fields, sql);
	if(fields)
	{
	if(mysql_fetch_row(res)) update=TRUE;
	}
	


	// Update the Type - Setup Only for Mods / Admins //
	// Remove This Section Only Accounts have been Restored //
/*	

	account = g_sql_trial;

	char type[16];
	if(g_sql_trial==TRUE) sprintf(type, "TRIAL");
		else sprintf(type, "MEMBER");

  
	char path[256];	
	CModerator *m_moderator;
	m_moderator = new CModerator(128); // 128 Moderators max.
	sprintf(path,"%smoderators.txt", g_basedir);	
	m_moderator->LoadModerators(path);
	int mod_type = m_moderator->GetType(plr->name);

	if(mod_type==MODTYPE_ADMIN) sprintf(type, "ADMIN");
	else if(mod_type==MODTYPE_LEADMODERATOR) sprintf(type, "SENIORMOD");
	else if(mod_type==MODTYPE_SMODERATOR) sprintf(type, "MOD2");
	else if(mod_type==MODTYPE_MODERATOR) sprintf(type, "MOD");
	else;
*/
	/////////////////////////////////////////////////////

	
	// Store the Real Expiry Date (from After 1st Jan 2000)

	long rexpiry, updated;
	CTime m_time;
	CTime t2000(2000,1,1,0,0,0);
	CTimeSpan diff;
	m_time = m_time.GetCurrentTime();
	
	diff = m_time-t2000;
	rexpiry = diff.GetDays();
	rexpiry += TimeToExpire(plr);


	CTime t1970(1970,1,1,0,0,0);
	diff = m_time-t1970;
	updated = diff.GetTotalSeconds();

	

	

	
/*

	return DaysLeft(m_time.GetDay(),m_time.GetMonth(),m_time.GetYear(),
				    d,m,y);
*/
	 // = 2000+TimeToExpire(plr);

	char trialsysid_buf[16];
	sprintf(trialsysid_buf, "%lx", plr->trialsysid);
	char textFormat_buf[16];
	sprintf(textFormat_buf, "%lx", plr->textFormat);
	char trialmacid_u_buf[16];
	sprintf(trialmacid_u_buf, "%lx", plr->trialmacid_u);
	char trialmacid_l_buf[16];
	sprintf(trialmacid_l_buf, "%lx", plr->trialmacid_l);
	char inventory_buf[16];
	sprintf(inventory_buf, "%lx", plr->inventory);
	

	
	if(update) // UPDATE (just deletes the old record for now) [+] //
	{
	//sprintf(sql,"delete from %smembers where name='%s'",g_db_prefix, plr->name);
	sprintf(sql,"UPDATE %smembers set rating=%d, wins=%d, losses=%d, streak=%d, \
	_50s=%d, _60s=%d, _70s=%d, _80s=%d, _90s=%d, _100s=%d, highbreak=%d, max147s=%d, \
	_p50s=%d, _p60s=%d, _p70s=%d, _p80s=%d, _p90s=%d, _p100s=%d, phighbreak=%d, pmax147s=%d, \
	bioaverageshottime=%f, bioaverageposition=%f, bioaveragesafety=%f, bioaveragebreak=%f, \
	pbioaverageshottime=%f, pbioaverageposition=%f, pbioaveragesafety=%f, pbioaveragebreak=%f, \
	winner=%d, runnerup=%d, semifinals=%d, quarterfinals=%d, last16=%d, last32=%d, \
	textformat='%s', state2=%ld, inventory='%s', highestrating=%d, cash=%d, cuetype='%d', chalktype='%ld', \
	expiry='%s', account=%d, rexpiry=%ld, updated=%ld, bandata=%d, level=%d, level_progress=%d, textcolor=%ld, \
	ladderpoints=%d, ladderwinslosses=%d, ladderhighbreak=%d, highstreak=%d, realcash=%d WHERE name='%s'", \
	g_db_prefix, plr->rating, plr->wins, plr->losses, plr->streak, \
	plr->_50s, plr->_60s, plr->_70s, plr->_80s, plr->_90s, plr->_100s, plr->HighBreak, plr->Max147s, \
	plr->_p50s, plr->_p60s, plr->_p70s, plr->_p80s, plr->_p90s, plr->_p100s, plr->pHighBreak, plr->pMax147s, \
	plr->bioAverageShotTime, plr->bioAveragePosition, plr->bioAverageSafety, plr->bioAverageBreak, \
	plr->pBioAverageShotTime, plr->pBioAveragePosition, plr->pBioAverageSafety, plr->pBioAverageBreak, \
	plr->winner, plr->runnerUp, plr->semiFinals, plr->quarterFinals, plr->last16, plr->last32, \
	textFormat_buf, plr->state2, inventory_buf, plr->highestrating, plr->cash, plr->cuetype, plr->chalktype, \
	dstr, account, rexpiry, updated, plr->bandata, plr->level, plr->level_progress, plr->textColor, \
	plr->ladderPoints, plr->ladderWinsLosses, plr->ladderHighBreak, plr->highstreak, plr->realcash, db_stringgood(plr->name));

	db_query(&fields, sql);
	return 0;
	}
		

	BOOL banned=0, walled=0, denied=0, blocked=0;
	if(plr->control&2) banned = 1;
	if(plr->control&4) walled = 1;
	if(plr->control&8) denied = 1;
	if(plr->control&16) blocked = 1;



	sprintf(sql,"insert into %smembers (name,password,rating,wins,losses,streak,banned, walled, denied, blocked, \
										_50s, _60s, _70s, _80s, _90s, _100s, highbreak, max147s, \
										_p50s, _p60s, _p70s, _p80s, _p90s, _p100s, phighbreak, pmax147s, \
										bioaverageshottime, bioaverageposition, bioaveragesafety, bioaveragebreak, \
										pbioaverageshottime, pbioaverageposition, pbioaveragesafety, pbioaveragebreak, \
										winner, runnerup, semifinals, quarterfinals, last16, last32, \
										textformat, state2,	inventory, highestrating, \
										cash, cuetype, chalktype, expiry, account, type, rexpiry, updated, dbcookie, bandata, \
										level, level_progress, textcolor, ladderpoints, ladderwinslosses, ladderhighbreak, highstreak, realcash) \
										values ('%s','%s',%d,%d,%d,%d,%d,%d,%d,%d, \
										%d,%d,%d,%d,%d,%d,%d,%d,\
										%d,%d,%d,%d,%d,%d,%d,%d,\
										%f,%f,%f,%f,%f,%f,%f,%f, \
										%d,%d,%d,%d,%d,%d, \
										'%s',%ld,'%s',%d, \
										%d,'%d','%ld','%s',%d,'%s',%ld,%ld,'%s',%d,%d,%d,%ld,%d,%d,%d, %d, %d)", \
										g_db_prefix, db_stringgood(plr->name), db_stringgood(plr->password), plr->rating, plr->wins, plr->losses, plr->streak, \
										banned, walled, denied, blocked, \
										plr->_50s, plr->_60s, plr->_70s, plr->_80s, plr->_90s, plr->_100s, plr->HighBreak, plr->Max147s, \
										plr->_p50s, plr->_p60s, plr->_p70s, plr->_p80s, plr->_p90s, plr->_p100s, plr->pHighBreak, plr->pMax147s, \
										plr->bioAverageShotTime, plr->bioAveragePosition, plr->bioAverageSafety, plr->bioAverageBreak, \
										plr->pBioAverageShotTime, plr->pBioAveragePosition, plr->pBioAverageSafety, plr->pBioAverageBreak, \
										plr->winner, plr->runnerUp, plr->semiFinals, plr->quarterFinals, plr->last16, plr->last32, \
										textFormat_buf, plr->state2, inventory_buf, plr->highestrating, \
										plr->cash, plr->cuetype, plr->chalktype, dstr, account, type, rexpiry, updated, "", plr->bandata, plr->level, plr->level_progress, plr->textColor, \
										plr->ladderPoints, plr->ladderWinsLosses, plr->ladderHighBreak, plr->highstreak, plr->realcash);

						

	db_query(&fields, sql);

	
#endif 
	return 0;
}

int sscanf_int(char *str) // safer than sscanfing into a char/short/int - since it does 32 bit always
{
if(str == NULL) return 0;
int v32;
sscanf(str, "%d", &v32);
return v32;
}

float sscanf_float(char *str) // safer than sscanfing into a char/short/int - since it does 32 bit always
{
if(str == NULL) return 0;
float f32;
sscanf(str, "%f", &f32);
return f32;
}

int sscanf_d32(char *str) // safer than sscanfing into a char/short/int - since it does 32 bit always
{
if(str == NULL) return 0;
int d32;
sscanf(str, "%lx", &d32);
return d32;
}

// If account is Admin			- Return 0
// If account is Senior Mod		- Return 1
// If account is Special Mod	- Return 2
// If account is Moderator		- Return 3
// If account is Member			- Return 4
// If account is Trialist		- Return 5
// If account is Guest			- Return 6


// Lowest Rank is Best //
int sscanf_playertype(char *str) 
{
if(!strcmpi(str, "TRIAL")) return 5;
else if(!strcmpi(str, "MEMBER")) return 4;
else if(!strcmpi(str, "MOD")) return 3;
else if(!strcmpi(str, "MOD2")) return 2;
else if(!strcmpi(str, "SENIORMOD")) return 1;
else if(!strcmpi(str, "ADMIN")) return 0;
else return 6; // Guest Rank 
}


int db_loadmemberfile(Player *plr, BOOL bCheck) // set the player name field prior to entry.
{
#ifdef MAIN_SERVER
	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bWalled=FALSE;
	BOOL bDenied=FALSE;
	BOOL bBlocked=FALSE;

	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;

	if(bCheck==FALSE) // Initialise the player structure.
		{
		char name[22];
		strcpy(name, plr->name);
		InitPlayer(plr);
		strcpy(plr->name, name);

		plr->ExpDay = g_config.m_day;//SPECIAL_DAY;
		plr->ExpMonth = g_config.m_month;//SPECIAL_MONTH;
		plr->ExpYear = g_config.m_year;//SPECIAL_YEAR;
		}
	plr->state = 0;
	sprintf(buf,"select * from %smembers where name='%s'",g_db_prefix, db_stringgood(plr->name)); 
	res = db_query(&fields, buf);
	if(fields)
	{	
	while(row = mysql_fetch_row(res))
			{
			bFile=TRUE;
			//if(bCheck) return 1; // (just checking the existances of the file, no saving) - always gets loading into a player struct anyway
			idx = 0;
			for(end_row = row+fields; row<end_row;++row)
				{
				if(row)
				{
				str = (char *) *row;
				fldrow = mysql_fetch_field_direct(res, idx);
				fld = (*fldrow).name;
				if(!strcmpi(fld,"name")) 
				{
					strcpy(plr->name, str);
				}

				if(!strcmpi(fld,"password")) strcpy(plr->password, str);
				if(!strcmpi(fld,"rating")) plr->rating = sscanf_int(str);
				if(!strcmpi(fld,"wins")) plr->wins = sscanf_int(str);
				if(!strcmpi(fld,"losses")) plr->losses = sscanf_int(str);
				if(!strcmpi(fld,"streak")) plr->streak = sscanf_int(str);
				if(!strcmpi(fld,"reracks")) plr->reracks = sscanf_int(str);
				if(!strcmpi(fld,"bandata")) plr->bandata = sscanf_int(str);
				if(!strcmpi(fld,"expiry")) {
								int d,m,y;
								char str_t[80];
								sprintf(str_t,"[%s]",str);
								StrToDate(str_t, &d, &m, &y);							
								plr->ExpDay = d;
								plr->ExpMonth = m;
								plr->ExpYear = y-2000; // make relative to y2k
				}
				//if(!strcmpi(fld,"member_id")) plr->member_id = sscanf_int(str);
				if(!strcmpi(fld,"banned")) {
										sscanf(str, "%d", &v32);
										if(v32==1) bBanned = TRUE;
										}
				if(!strcmpi(fld,"walled")) {
										sscanf(str, "%d", &v32);
										if(v32==1) bWalled = TRUE;
										}
				if(!strcmpi(fld,"denied")) {
										sscanf(str, "%d", &v32);
										if(v32==1) bDenied = TRUE;
										}
				if(!strcmpi(fld,"blocked")) {
										sscanf(str, "%d", &v32);
										if(v32==1) bBlocked = TRUE;
										}
				if(!strcmpi(fld,"account")) {
										sscanf(str, "%d", &v32);
										if(v32==1) plr->state|=PLRSTATE_GUESTMEMBER;
										}

				if(!strcmpi(fld,"level")) plr->level = sscanf_int(str);
				if(!strcmpi(fld,"level_progress")) plr->level_progress = sscanf_int(str);
				if(!strcmpi(fld,"textcolor")) plr->textColor = sscanf_int(str);

				if(!strcmpi(fld,"_50s")) plr->_50s = sscanf_int(str);
				if(!strcmpi(fld,"_60s")) plr->_60s = sscanf_int(str);
				if(!strcmpi(fld,"_70s")) plr->_70s = sscanf_int(str);
				if(!strcmpi(fld,"_80s")) plr->_80s = sscanf_int(str);
				if(!strcmpi(fld,"_90s")) plr->_90s = sscanf_int(str);
				if(!strcmpi(fld,"_100s")) plr->_100s = sscanf_int(str);
				if(!strcmpi(fld,"highbreak")) plr->HighBreak = sscanf_int(str);
				if(!strcmpi(fld,"max147s")) plr->Max147s = sscanf_int(str);
								
				
				if(!strcmpi(fld,"_p50s")) plr->_p50s = sscanf_int(str);
				if(!strcmpi(fld,"_p60s")) plr->_p60s = sscanf_int(str);
				if(!strcmpi(fld,"_p70s")) plr->_p70s = sscanf_int(str);
				if(!strcmpi(fld,"_p80s")) plr->_p80s = sscanf_int(str);
				if(!strcmpi(fld,"_p90s")) plr->_p90s = sscanf_int(str);
				if(!strcmpi(fld,"_p100s")) plr->_p100s = sscanf_int(str);
				if(!strcmpi(fld,"phighbreak")) plr->pHighBreak = sscanf_int(str);
				if(!strcmpi(fld,"pmax147s")) plr->pMax147s = sscanf_int(str);
				

				if(!strcmpi(fld,"bioaverageshottime")) plr->bioAverageShotTime = sscanf_float(str);
				if(!strcmpi(fld,"bioaverageposition")) plr->bioAveragePosition = sscanf_float(str);
				if(!strcmpi(fld,"bioaveragesafety")) plr->bioAverageSafety = sscanf_float(str);
				if(!strcmpi(fld,"bioaveragebreak")) plr->bioAverageBreak = sscanf_float(str);

				if(!strcmpi(fld,"pbioaverageshottime")) plr->pBioAverageShotTime = sscanf_float(str);
				if(!strcmpi(fld,"pbioaverageposition")) plr->pBioAveragePosition = sscanf_float(str);
				if(!strcmpi(fld,"pbioaveragesafety")) plr->pBioAverageSafety = sscanf_float(str);
				if(!strcmpi(fld,"pbioaveragebreak")) plr->pBioAverageBreak = sscanf_float(str);

				if(!strcmpi(fld,"winner")) plr->winner = sscanf_int(str);
				if(!strcmpi(fld,"runnerup")) plr->runnerUp = sscanf_int(str);
				if(!strcmpi(fld,"semifinals")) plr->semiFinals = sscanf_int(str);
				if(!strcmpi(fld,"quarterfinals")) plr->quarterFinals = sscanf_int(str);
				if(!strcmpi(fld,"last16")) plr->last16 = sscanf_int(str);
				if(!strcmpi(fld,"last32")) plr->last32 = sscanf_int(str);

				if(!strcmpi(fld,"sysid")) 
									{
									plr->trialcpuid = 0;
									plr->trialsysid = 0;
									// Last 4 Digits is CPU ID //
									if(str)
									if(strlen(str)>=6)
										{
										char cpuID_str[6];
										cpuID_str[0] = str[strlen(str)-4];
										cpuID_str[1] = str[strlen(str)-3];
										cpuID_str[2] = str[strlen(str)-2];
										cpuID_str[3] = str[strlen(str)-1];
										cpuID_str[4] = 0;
										plr->trialcpuid = sscanf_d32(cpuID_str);

										char sysID_str[32];
										strcpy(sysID_str, str);
										sysID_str[strlen(sysID_str)-4] = 0;
										plr->trialsysid = sscanf_d32(sysID_str);
										}
									}
				if(!strcmpi(fld,"macid")) 
									{
									plr->trialmacid_u = 0;
									plr->trialmacid_l = 0;
									if(str)
									if(strlen(str)>=6)
										{
										// Last 4 Digits is CPU ID //
										char macID_L_str[16];
										macID_L_str[0] = str[strlen(str)-4];
										macID_L_str[1] = str[strlen(str)-3];
										macID_L_str[2] = str[strlen(str)-2];
										macID_L_str[3] = str[strlen(str)-1];
										macID_L_str[4] = 0;
										plr->trialmacid_l = sscanf_d32(macID_L_str);

										char macID_U_str[16];
										strcpy(macID_U_str, str);
										macID_U_str[strlen(macID_U_str)-4] = 0;
										plr->trialmacid_u = sscanf_d32(macID_U_str);
										}
									}
				if(!strcmpi(fld,"textformat")) plr->textFormat = sscanf_d32(str);
				if(!strcmpi(fld,"state2")) plr->state2 = sscanf_d32(str);
				if(!strcmpi(fld,"inventory")) plr->inventory = sscanf_d32(str);
				if(!strcmpi(fld,"highestrating")) plr->highestrating = sscanf_int(str);
				if(!strcmpi(fld,"cash")) plr->cash = sscanf_int(str);
				if(!strcmpi(fld,"cuetype")) plr->cuetype = sscanf_int(str);
				if(!strcmpi(fld,"chalktype")) plr->chalktype = sscanf_int(str);
				if(!strcmpi(fld,"type")) plr->playertype = sscanf_playertype(str);
				if(!strcmpi(fld,"ladderpoints")) plr->ladderPoints = sscanf_int(str);
				if(!strcmpi(fld,"ladderwinslosses")) plr->ladderWinsLosses = sscanf_int(str);
				if(!strcmpi(fld,"ladderhighbreak")) plr->ladderHighBreak = sscanf_int(str);
				if(!strcmpi(fld,"highstreak")) plr->highstreak = sscanf_int(str);
				if(!strcmpi(fld,"realcash")) plr->realcash = sscanf_int(str);
				//[BUG FIX] //
				if(!strcmpi(fld,"sysid")) if(str) strcpy(plr->ssysid, str);
				if(!strcmpi(fld,"macid")) if(str) strcpy(plr->smacid, str);
				if(!strcmpi(fld,"ip")) if(str) strcpy(plr->sip, str);
				if(!strcmpi(fld,"activated")) plr->activated = sscanf_int(str);
				} // end if row
				idx++;
			} // end for
			} // end while
	} // end if fields


	if(!bFile) return 0;


	if(plr->highestrating < plr->rating) plr->highestrating = plr->rating;
	if(plr->highestrating < 1200) plr->highestrating = 1200;
	// Create a Special return status based on
	// bBanned: 2
	// bWalled: 4
	// bDenied: 8
	// bBlocked: 16
	char retval=1; // Set Bit1, since this account exists
	retval |= bBanned*2;
	retval |= bWalled*4;
	retval |= bDenied*8;
	retval |= bBlocked*16;
	return retval;

//	if(bBanned) return 2;
#endif
	return 1;
}


void db_banmemberfile(char *name, int flag)
{
	char sql[1024];
	int fields;
	sprintf(sql,"UPDATE %smembers SET banned=%d where name='%s'",g_db_prefix, flag,db_stringgood(name));
	db_query(&fields, sql);
}


// Returns member_id
int db_getidmemberfile(char *name)
{
#ifdef MAIN_SERVER
	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;


	sprintf(buf,"SELECT member_id from %smembers where name='%s'",g_db_prefix, db_stringgood(name)); 
	res = db_query(&fields, buf);
	if(fields)
	{	
	while(row = mysql_fetch_row(res))
			{
			bFile=TRUE;
			//if(bCheck) return 1; // (just checking the existances of the file, no saving) - always gets loading into a player struct anyway
			idx = 0;
			for(end_row = row+fields; row<end_row;++row)
				{
				if(row)
				{
				str = (char *) *row;
				fldrow = mysql_fetch_field_direct(res, idx);
				fld = (*fldrow).name;
				if(!strcmpi(fld,"member_id")) return sscanf_int(str);
				} // end if row
				idx++;
			} // end for
			} // end while
	} // end if fields
	
	
#endif
	return -1;
}
 

// Returns 0: NOT TRIALIST
// Returns 1: TRIALIST
BOOL db_checktrial(int member_id)
{
#ifdef MAIN_SERVER
	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;


	sprintf(buf,"SELECT * from %smembers where member_id=%d AND type='TRIAL'", g_db_prefix, member_id); 
	res = db_query(&fields, buf);
	if(fields) 
	if(row = mysql_fetch_row(res)) return 1; 
		else return 0; 

#endif
return 0;
}



void db_newaccountlog(char *name, char *atype, char *mcode, char *email, char *date)
{
#ifdef MAIN_SERVER

	long updated;
	CTime m_time;
	CTimeSpan diff;
	m_time = m_time.GetCurrentTime();
	CTime t1970(1970,1,1,0,0,0);
	diff = m_time-t1970;
	updated = diff.GetTotalSeconds();
	
	
	char sql[1024];
	int fields, member_id;
	// Get Member ID for New Account //
	member_id = db_getidmemberfile(name);
	// Insert New Account Log //
	if(member_id>=0)
		{
		sprintf(sql,"INSERT INTO %smembers_newaccounts (member_id, atype, mcode, email, date, time) values(%d,'%s','%s','%s','%s', %ld)",g_db_prefix, member_id, atype, mcode, email, date, updated);
		db_query(&fields, sql);
		
		// If we are Topping-Up with a Membership Code then change TYPE from 'TRIAL' change to 'MEMBER' //
		//
		if(atype[0] != 'T')		// Make sure this isn't a Trial Account being created //
		if(db_checktrial(member_id))
			{
			sprintf(sql,"UPDATE %smembers SET type='MEMBER' WHERE member_id=%d",g_db_prefix, member_id);
			db_query(&fields, sql);
			}
		}
#endif
}




// Returns 0: UnWalled ID
// Returns 1: Walled ID
int db_checkwalled(char *sysid)
{
#ifdef MAIN_SERVER

	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;

	// Check members_walled table, to see if this SYSID is walled //
	sprintf(buf,"SELECT * FROM %smembers_walled WHERE sysid='%s'", g_db_prefix, sysid); 
	res = db_query(&fields, buf);
	if(fields) 
	if(row = mysql_fetch_row(res)) return 1; 
		else return 0; 
	
#endif
	return 0;
}


void db_wall(char *sysid, int flag)
{
#ifdef MAIN_SERVER

	char sql[1024];
	int fields;
	// Insert blocked ip into members_walled table //
	if(flag==1) sprintf(sql,"INSERT INTO %smembers_walled (sysid) values('%s')",g_db_prefix, sysid);
		else if(flag==0) sprintf(sql,"DELETE FROM %smembers_walled WHERE sysid='%s'",g_db_prefix, sysid);
	db_query(&fields, sql);
	
#endif
}



// Returns 0: UnDenied ID
// Returns 1: Denied ID
int db_checkdenied(char *macid)
{
#ifdef MAIN_SERVER

	if(strlen(macid) <= 4) return 0;		// If MACID is less than 4 characters then it isn't a Valid one so can't really deny this //

	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;

	// Check members_walled table, to see if this SYSID is walled //
	sprintf(buf,"SELECT * FROM %smembers_denied WHERE macid='%s'", g_db_prefix, macid); 
	res = db_query(&fields, buf);
	if(fields) 
	if(row = mysql_fetch_row(res)) return 1; 
		else return 0; 
#endif
	return 0;
}



void db_deny(char *macid, int flag)
{
#ifdef MAIN_SERVER

	char sql[1024];
	int fields;
	// Insert blocked ip into members_walled table //
	if(flag==1) sprintf(sql,"INSERT INTO %smembers_denied (macid) values('%s')",g_db_prefix, macid);
		else if(flag==0) sprintf(sql,"DELETE FROM %smembers_denied WHERE macid='%s'",g_db_prefix, macid);
	db_query(&fields, sql);
	
#endif
}




// Returns 0: UnBlocked ID
// Returns 1: Blocked ID
int db_checkblocked(char *ip)
{
#ifdef MAIN_SERVER

	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;

	// Check members_blocked table, to see if this IP is blocked //
	sprintf(buf,"SELECT * FROM %smembers_blocked WHERE ip='%s'", g_db_prefix, ip); 
	res = db_query(&fields, buf);
	if(fields) 
	if(row = mysql_fetch_row(res)) return 1; 
		else return 0; 

#endif
	return 0;
}




void db_block(char *ip, int flag)
{
#ifdef MAIN_SERVER

	char sql[1024];
	int fields;
	// Insert blocked ip into members_walled table //
	if(flag==1) sprintf(sql,"INSERT INTO %smembers_blocked (ip) values('%s')",g_db_prefix, ip);
		else if(flag==0) sprintf(sql,"DELETE FROM %smembers_blocked WHERE ip='%s'",g_db_prefix, ip);
	db_query(&fields, sql);
	
#endif
}




void db_debugmemberfile(Player *plr)
{
#ifdef MAIN_SERVER
	char buffer[1024];
	char dstr[80];
	int d,m,y;
	d = plr->ExpDay;
	m = plr->ExpMonth;
	y = plr->ExpYear;
	y+=2000;
	DateToStr2(dstr, d, m, y);

	sprintf(buffer, "Name:%s\nPassword:%s\nWins:%d\nLosses:%d\nStreak:%d\nReracks:%d\nExpiry:%s",plr->name, plr->password,
		plr->wins,plr->losses,plr->streak,plr->reracks,dstr);
	
#endif
}



int db_getplayertype(char *name) // Find the PlayerType
{
#ifdef MAIN_SERVER
	char buf[128];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;

	char tguest[256];
	strcpy(tguest, name);
	tguest[5] = 0;
	if(!strcmpi(tguest, "guest")) return 6;

	sprintf(buf,"select type from %smembers where name='%s'",g_db_prefix, db_stringgood(name)); 
	res = db_query(&fields, buf);
	if(fields)
	{	
	while(row = mysql_fetch_row(res))
			{
			bFile=TRUE;
			//if(bCheck) return 1; // (just checking the existances of the file, no saving) - always gets loading into a player struct anyway
			idx = 0;
			for(end_row = row+fields; row<end_row;++row)
				{
				if(row)
				{
				str = (char *) *row;
				fldrow = mysql_fetch_field_direct(res, idx);
				fld = (*fldrow).name;
				if(!strcmpi(fld,"type")) return sscanf_playertype(str);
				} // end if row
				idx++;
			} // end for
			} // end while
	} // end if fields
	
	
#endif
	return -1;
}




// Returns 0: UnDenied
// Returns 1: Denied
// Returns -1: Account Doesn't Exist


int sscanf_var(char *str) 
{
#ifdef MAIN_SERVER
	if(!strcmpi(str, "1Month")) return MCODE_1MONTH;
	else if(!strcmpi(str, "3Months")) return MCODE_3MONTHS;
	else if(!strcmpi(str, "6Months")) return MCODE_6MONTHS;
	else if(!strcmpi(str, "1Year")) return MCODE_1YEAR;
	return MCODE_UNDEFINED;
#endif
return 0;
}



int CheckSerialdb(char *mCode, char *email)
{
#ifdef MAIN_SERVER	
	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;

	int type_idx = MCODE_UNDEFINED;

	sprintf(buf,"SELECT * from %scodes where mcode='%s' AND used=0",g_db_prefix, mCode); 
	res = db_query(&fields, buf);
	if(fields)
	{	
	while(row = mysql_fetch_row(res))
			{
			bFile=TRUE;
			//if(bCheck) return 1; // (just checking the existances of the file, no saving) - always gets loading into a player struct anyway
			idx = 0;
			for(end_row = row+fields; row<end_row;++row)
				{
				if(row)
				{
				str = (char *) *row;
				fldrow = mysql_fetch_field_direct(res, idx);
				fld = (*fldrow).name;
				if(!strcmpi(fld,"var")) type_idx = sscanf_var(str);
				else if(!strcmpi(fld,"email")) sprintf(email, "%s", str);
				} // end if row
				idx++;
			} // end for
			} // end while
	// Set Membership Code to Deleted //
	sprintf(buf,"UPDATE %scodes SET used=1 WHERE mcode='%s'", g_db_prefix, mCode);
	res = db_query(&fields, buf);
	} // end if fields
	
	return type_idx;
#endif
return 0;
}




void DeleteSerialdb(char *mCode)
{
#ifdef MAIN_SERVER
	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;
	
	sprintf(buf,"UPDATE %scodes SET used=1 where mcode='%s'",g_db_prefix, mCode); 
	res = db_query(&fields, buf);

	// We Also Need To Update the New Account Create (by 
#endif
}


// Returns 0: Doesn't Exist
// Returns 1: Exists
int db_checkaccountexist(char *name)
{
#ifdef MAIN_SERVER
	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;


	sprintf(buf,"SELECT * from %smembers where name='%s'", g_db_prefix, name); 
	res = db_query(&fields, buf);
	if(fields) 
	if(row = mysql_fetch_row(res)) return 1; // Account Exists
		else return 0; // Account Doesn't Exist
#endif	
return 0;
}


// Returns 0: UnWalled ID
// Returns 1: Walled ID
BOOL db_checkpassword(char *name, char *pass)
{
#ifdef MAIN_SERVER

	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;


	sprintf(buf,"SELECT * from %smembers where name='%s' AND password='%s'", g_db_prefix, name, pass); 
	res = db_query(&fields, buf);
	if(fields) 
	if(row = mysql_fetch_row(res)) return 1; 
		else return 0; 
	
	
#endif
	return 0;
}


void db_sysmacipmemberfile(char *name, char *sysid, char *macid, char *ip)
{
#ifdef MAIN_SERVER
	char sql[1024];
	int fields;
	sprintf(sql,"UPDATE %smembers SET sysid='%s', macid='%s', ip='%s' WHERE name='%s'",g_db_prefix, sysid, macid, ip, db_stringgood(name));
	db_query(&fields, sql);

#endif
}



void db_cuechanged(char *name, int cuetype)
{
#ifdef MAIN_SERVER
	char sql[1024];
	int fields;
	sprintf(sql,"UPDATE %smembers SET cuetype=%d where name='%s'",g_db_prefix, cuetype, db_stringgood(name));
	db_query(&fields, sql);
#endif
}

void db_chalkchanged(char *name, int chalktype)
{
#ifdef MAIN_SERVER
	char sql[1024];
	int fields;
	sprintf(sql,"UPDATE %smembers SET chalktype=%ld where name='%s'",g_db_prefix, chalktype, db_stringgood(name));
	db_query(&fields, sql);
#endif
}

void db_textchanged(char *name, int textformat, int textcolor)
{
#ifdef MAIN_SERVER
	char sql[1024];
	int fields;
	sprintf(sql,"UPDATE %smembers SET textformat='%lx', textcolor='%ld' where name='%s'",g_db_prefix, textformat, textcolor, db_stringgood(name));
	db_query(&fields, sql);
#endif
}


//
void db_updategamehistory(char *name, char *opponent, int gameinfo, int time, int update_hid)
{
#ifdef MAIN_SERVER	
	// Update the Oldests to this result being reported //


	// Get Member ID for this name
	int member_id = db_getidmemberfile(name);
	char sql[1024];
	int fields;
	if(update_hid == -1)
		{
		// INSERT NEW ENTRY //
		sprintf(sql,"INSERT INTO %shistory (member_id, gameinfo, time, opponent) values(%d, %d, %d, '%s')", g_db_prefix, member_id, gameinfo, time, opponent);
		db_query(&fields, sql);
		}
	else{
		// UPDATE ENTRY (oldest Entry) //
		sprintf(sql,"UPDATE %shistory SET member_id=%d, gameinfo=%d, time=%d, opponent='%s' WHERE history_id=%d", g_db_prefix, member_id, gameinfo, time, opponent, update_hid);
		db_query(&fields, sql);
		}
	
#endif MAIN_SERVER
}

void *db_getgamehistory(char *name)
{
#ifdef MAIN_SERVER
	// Test, just send back some data //
	HistoryData *data;
	data = new HistoryData;

	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned=FALSE;
	BOOL bFile=FALSE;
	BOOL update=FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row,end_row;
	MYSQL_FIELD *fldrow;
	char opponent[10][22];
	int n;
	int columns = 0;


	// Clear History Data, by setting time to 0 //
	for(n=0;n<10;n++) data->time[n] = 0;
	
	// Get Member ID for this name
	int member_id = db_getidmemberfile(name);

	sprintf(buf,"SELECT * FROM %shistory WHERE member_id=%d ORDER BY time DESC LIMIT 10", g_db_prefix, member_id); 
	res = db_query(&fields, buf);
	if(fields)
	{	
	while(row = mysql_fetch_row(res))
			{
			
			bFile=TRUE;
			//if(bCheck) return 1; // (just checking the existances of the file, no saving) - always gets loading into a player struct anyway
			idx = 0;
			for(end_row = row+fields; row<end_row;++row)
				{
				if(row)
				{
				str = (char *) *row;
				fldrow = mysql_fetch_field_direct(res, idx);
				fld = (*fldrow).name;
				if(!strcmpi(fld,"opponent")) strcpy(opponent[columns], str);
				if(!strcmpi(fld,"gameinfo")) data->gameinfo[columns] = sscanf_int(str);
				if(!strcmpi(fld,"time")) data->time[columns] = sscanf_int(str);
				if(!strcmpi(fld,"history_id")) data->history_id[columns] = sscanf_int(str);
				} // end if row
				idx++;
			} // end for
			columns++;
			} // end while
	} // end if fields
	
	for(int i=0;i<columns;i++)
		{
		// Bit 1 tells us if This is a Win or Loss for member_id / name
		if(data->gameinfo[i]&1)	
			{
			
			strcpy(data->winner[i], name); // Win 
			strcpy(data->loser[i], opponent[i]); // Loser 
			}
		else{
			strcpy(data->winner[i], opponent[i]); // Loser
			strcpy(data->loser[i], name); // Winner 
			}
		}

	return data;
#endif MAIN_SERVER

	return 0;
}



void db_updatereracks(char *name, int reracks)
{
#ifdef MAIN_SERVER	

	// Get Member ID for this name
	int member_id = db_getidmemberfile(name);
	if(member_id != -1)
		{
		char sql[1024];
		int fields;
		sprintf(sql,"UPDATE %smembers SET reracks=%d WHERE member_id=%d", g_db_prefix, reracks, member_id);
		db_query(&fields, sql);
		}
#endif MAIN_SERVER
}



void db_logtime(char *name, bool mode)
{
#ifdef MAIN_SERVER
	
	if(name[0] != 0)
	{

		long TotalSeconds;
		CTime m_time;
		CTimeSpan diff;
		m_time = m_time.GetCurrentTime();
		
		CTime t1970(1970,1,1,0,0,0);
		diff = m_time-t1970;
		TotalSeconds = diff.GetTotalSeconds();

		
		char sql[128];
		int fields;

		// Add New Log Entry //
		sprintf(sql,"INSERT INTO %slogtime (name, time, mode) values('%s', %ld, %d)", g_db_prefix, name, TotalSeconds, mode);
		db_query(&fields, sql);

	}


#endif MAIN_SERVER
}


void db_logtimecleanup(void)
{
#ifdef MAIN_SERVER
	
	long TotalSeconds, CutOffSeconds;
	CTime m_time;
	CTimeSpan diff;
	m_time = m_time.GetCurrentTime();
	
	CTime t1970(1970,1,1,0,0,0);
	diff = m_time-t1970;
	TotalSeconds = diff.GetTotalSeconds();
	CutOffSeconds = TotalSeconds - 3600*24*60;	// 60 days

	char sql[128];
	int fields;
	// Delete any Log Entries that are older than 60 days //
	sprintf(sql,"DELETE FROM %slogtime WHERE time < %ld",g_db_prefix, CutOffSeconds);
	db_query(&fields, sql);


#endif MAIN_SERVER
}



// Returns the current rank of our ladder points
BOOL db_getLadderRank(char *name, int ladderPoints)
{
#ifdef MAIN_SERVER

	// Get our Ladder Division (ceiling value)

	// We only want active players
	long rexpiry, updated;
	CTime m_time;
	CTime t2000(2000, 1, 1, 0, 0, 0);
	CTimeSpan diff;
	m_time = m_time.GetCurrentTime();
	diff = m_time - t2000;
	rexpiry = diff.GetDays();

	int cutOffPoints;

	if (ladderPoints < 11000) cutOffPoints = 11000; // Bronze division
	else if (ladderPoints < 14000) cutOffPoints = 14000; // Silver division
	else if (ladderPoints < 17000) cutOffPoints = 17000; // Gold division
	else if (ladderPoints < 21000) cutOffPoints = 21000; // Platinum division
	else cutOffPoints = 99999; // Diamond division


	char buf[256];
	int fields;
	int rank;
	char *str;
	char *fld;
	int v32;
	float f32;
	
	MYSQL_RES *res;
	MYSQL_ROW row, end_row;
	MYSQL_FIELD *fldrow;

	rank = -1;

	// sprintf(buf, "SELECT COUNT(*) as rank FROM %smembers WHERE rexpiry>%d AND account=0 AND ladderpoints < %d AND ladderpoints > (SELECT ladderpoints FROM %smembers WHERE name='%s')", g_db_prefix, rexpiry, cutOffPoints, g_db_prefix, name);
	sprintf(buf, "SELECT COUNT(*) as rank FROM %smembers WHERE rexpiry>%d AND account=0 AND ladderpoints > (SELECT ladderpoints FROM %smembers WHERE name='%s')", g_db_prefix, rexpiry, g_db_prefix, name);
	res = db_query(&fields, buf);
	if (fields)
	if (row = mysql_fetch_row(res)) 
	{
		str = (char *) *row;
		fldrow = mysql_fetch_field_direct(res, 0);
		fld = (*fldrow).name;
		if (!strcmpi(fld, "rank")) rank = sscanf_int(str);
	} // end if row

	return (rank+1);

#endif
	return -1;
}



// Returns the current rank of our ladder points
std::array<ladderObject, 20> db_getLadderRankings()
{

	std::array<ladderObject, 20> rankData;
	rankData.empty();

	// Clear Rank Data
	for (int i = 0; i < rankData.size(); i++)
	{
		strcpy(rankData[i].name, "");
		rankData[i].ladderPoints = 0;
		rankData[i].ladderRank = 0;
		rankData[i].ladderHighBreak = 0;
		rankData[i].ladderWinsLosses = 0;
	}

#ifdef MAIN_SERVER


	long rexpiry, updated;
	CTime m_time;
	CTime t2000(2000, 1, 1, 0, 0, 0);
	CTimeSpan diff;
	m_time = m_time.GetCurrentTime();
	diff = m_time - t2000;
	rexpiry = diff.GetDays();
	// rexpiry += TimeToExpire(plr);

	char buf[256];
	int fields;
	int idx;
	char *str;
	char *fld;
	int v32;
	float f32;
	BOOL bBanned = FALSE;
	BOOL bFile = FALSE;
	BOOL update = FALSE;
	MYSQL_RES *res;
	MYSQL_ROW row, end_row;
	MYSQL_FIELD *fldrow;
	
	int n;
	int columns = 0;


	sprintf(buf, "SELECT name, ladderpoints, ladderhighbreak, ladderwinslosses FROM %smembers WHERE account=0 AND rexpiry > %d ORDER BY ladderpoints DESC LIMIT 20", g_db_prefix, rexpiry);
	res = db_query(&fields, buf);
	if (fields)
	{
		while (row = mysql_fetch_row(res))
		{

			bFile = TRUE;
			//if(bCheck) return 1; // (just checking the existances of the file, no saving) - always gets loading into a player struct anyway
			idx = 0;
			for (end_row = row + fields; row<end_row; ++row)
			{
				if (row)
				{
					str = (char *)*row;
					fldrow = mysql_fetch_field_direct(res, idx);
					fld = (*fldrow).name;
					if (!strcmpi(fld, "name"))
					{
						strcpy_s(rankData[columns].name, 32, str);
						rankData[columns].name[31] = 0;
						// Make sure our name isnt over 32 characters, so it doesn't overflow rankData member
					}
					if (!strcmpi(fld, "ladderpoints")) rankData[columns].ladderPoints = sscanf_int(str);
					if (!strcmpi(fld, "ladderhighbreak")) rankData[columns].ladderHighBreak = sscanf_int(str);
					if (!strcmpi(fld, "ladderwinslosses")) rankData[columns].ladderWinsLosses = sscanf_int(str);
				} // end if row
				idx++;
			} // end for
			rankData[columns].ladderRank = columns + 1;
			columns++;
		} // end while
	} // end if fields


	

#endif MAIN_SERVER

	return rankData;
}




// resets the ladder Rankings (done once a week usually)
void db_resetLadderRankings()
{
#ifdef MAIN_SERVER

	char sql[128];
	int fields;
	// Delete any Log Entries that are older than 60 days //
	sprintf(sql, "UPDATE %smembers SET ladderpoints=12000, ladderhighbreak=0, ladderwinslosses=0", g_db_prefix);
	db_query(&fields, sql);

#endif
	
}


// Update Real Cash - Database
void db_updateCash(char *name, int cash)
{
#ifdef MAIN_SERVER
	char sql[1024];
	int fields;
	sprintf(sql, "UPDATE %smembers SET realcash=%d where name='%s'", g_db_prefix, cash, db_stringgood(name));
	db_query(&fields, sql);
#endif
}