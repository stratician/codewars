#include "../CodeSupp/Build.h"
#include "../CodeSupp/stdafx.h"
#include <direct.h>
#include "../CodeSupp/GameAppView.h"
#include <afxinet.h>
//#include "../CodeSupp/Picture.h"
#include "Lobby.h"
#include "db.h"
#include "ctime"
// lets try this again!
#ifdef THE_POOL_CLUB
// 1st December 2003
#define SPECIAL_DAY		1
#define SPECIAL_MONTH	12
#define SPECIAL_YEAR	3
#else // The Snooker Club
// 1st October 2003
#define SPECIAL_DAY		1
#define SPECIAL_MONTH	10
#define SPECIAL_YEAR	3
#endif

BOOL g_sql_banned = FALSE;
BOOL g_sql_trial = FALSE;

char TableLostConnectionFlag[50];

#define FORCE_HIGHID	// Give everyone High ID, (no checking)

// table icons for iPool
char *tableFName[] = {
"Graphics/HUD/Table8BallBlue.bmp",
"Graphics/HUD/Table8BallRed.bmp",
"Graphics/HUD/Table8BallGreen.bmp",
"Graphics/HUD/Table8BallBlack.bmp",
"Graphics/HUD/Table8BallYellow.bmp",
"Graphics/HUD/Table8BallPurple.bmp",
"Graphics/HUD/Table8BallTeal.bmp",
"Graphics/HUD/Table8BallBrown.bmp",
"Graphics/HUD/Table8BallWhite.bmp",

"Graphics/HUD/Table9BallBlue.bmp",
"Graphics/HUD/Table9BallRed.bmp",
"Graphics/HUD/Table9BallGreen.bmp",
"Graphics/HUD/Table9BallBlack.bmp",
"Graphics/HUD/Table9BallYellow.bmp",
"Graphics/HUD/Table9BallPurple.bmp",
"Graphics/HUD/Table9BallTeal.bmp",
"Graphics/HUD/Table9BallBrown.bmp",
"Graphics/HUD/Table9BallWhite.bmp",

"Graphics/HUD/TableRBallBlue.bmp",
"Graphics/HUD/TableRBallRed.bmp",
"Graphics/HUD/TableRBallGreen.bmp",
"Graphics/HUD/TableRBallBlack.bmp",
"Graphics/HUD/TableRBallYellow.bmp",
"Graphics/HUD/TableRBallPurple.bmp",
"Graphics/HUD/TableRBallTeal.bmp",
"Graphics/HUD/TableRBallBrown.bmp",
"Graphics/HUD/TableRBallWhite.bmp",

"Graphics/HUD/TableBowlsBlue.bmp",
"Graphics/HUD/TableBowlsRed.bmp",
"Graphics/HUD/TableBowlsGreen.bmp",
"Graphics/HUD/TableBowlsBlack.bmp",
"Graphics/HUD/TableBowlsYellow.bmp",
"Graphics/HUD/TableBowlsPurple.bmp",
"Graphics/HUD/TableBowlsTeal.bmp",
"Graphics/HUD/TableBowlsBrown.bmp",
"Graphics/HUD/TableBowlsWhite.bmp",

"Graphics/HUD/Table6BallBlue.bmp",
"Graphics/HUD/Table6BallRed.bmp",
"Graphics/HUD/Table6BallGreen.bmp",
"Graphics/HUD/Table6BallBlack.bmp",
"Graphics/HUD/Table6BallYellow.bmp",
"Graphics/HUD/Table6BallPurple.bmp",
"Graphics/HUD/Table6BallTeal.bmp",
"Graphics/HUD/Table6BallBrown.bmp",
"Graphics/HUD/Table6BallWhite.bmp",

"Graphics/HUD/Table10BallBlue.bmp",
"Graphics/HUD/Table10BallRed.bmp",
"Graphics/HUD/Table10BallGreen.bmp",
"Graphics/HUD/Table10BallBlack.bmp",
"Graphics/HUD/Table10BallYellow.bmp",
"Graphics/HUD/Table10BallPurple.bmp",
"Graphics/HUD/Table10BallTeal.bmp",
"Graphics/HUD/Table10BallBrown.bmp",
"Graphics/HUD/Table10BallWhite.bmp",

"Graphics/HUD/TableBreakBlue.bmp",  // 54
"Graphics/HUD/TableBreakRed.bmp",
"Graphics/HUD/TableBreakGreen.bmp",
"Graphics/HUD/TableBreakBlack.bmp",
"Graphics/HUD/TableBreakYellow.bmp",
"Graphics/HUD/TableBreakPurple.bmp",
"Graphics/HUD/TableBreakTeal.bmp",
"Graphics/HUD/TableBreakBrown.bmp",
"Graphics/HUD/TableBreakWhite.bmp",	

"Graphics/HUD/TableUKPoolBlue.bmp",	// 63
"Graphics/HUD/TableUKPoolRed.bmp",
"Graphics/HUD/TableUKPoolGreen.bmp",
"Graphics/HUD/TableUKPoolBlack.bmp",
"Graphics/HUD/TableUKPoolYellow.bmp",
"Graphics/HUD/TableUKPoolPurple.bmp",
"Graphics/HUD/TableUKPoolTeal.bmp",
"Graphics/HUD/TableUKPoolBrown.bmp",
"Graphics/HUD/TableUKPoolWhite.bmp",

"Graphics/HUD/TableWILDBlue.bmp",	// 72
"Graphics/HUD/TableWILDRed.bmp",
"Graphics/HUD/TableWILDGreen.bmp",
"Graphics/HUD/TableWILDBlack.bmp",
"Graphics/HUD/TableWILDYellow.bmp",
"Graphics/HUD/TableWILDPurple.bmp",
"Graphics/HUD/TableWILDTeal.bmp",
"Graphics/HUD/TableWILDBrown.bmp",
"Graphics/HUD/TableWILDWhite.bmp",	// 80
};

// table icons for iSnooker
char *tableSnookerFName[] = {
"Graphics/HUD/TableOff.bmp",
"Graphics/HUD/Table15.bmp",
"Graphics/HUD/Table10.bmp",
"Graphics/HUD/Table6.bmp",
"Graphics/HUD/TableBlackBall.bmp",
"Graphics/HUD/TableBilliards50.bmp",
"Graphics/HUD/TableBilliards100.bmp",
"Graphics/HUD/TableBilliards200.bmp",
"Graphics/HUD/TableWild.bmp"
};

char *g_status_strlst[] = {
"AVAILABLE",
"BUSY",
"BRB (Be Right Back)",
"AWAY",
"PHONE",
"LUNCH",
"DND (Do Not Disturb)",
"AWAY",
};

Player m_player[MAX_LOBBY_PLAYERS];
CExtendedPlayerData *g_extendedPlayerData[MAX_LOBBY_PLAYERS]; // what you know of all the other players currently on the lobby
CExtendedPlayerData *g_extendedPlayerDataLocal;	// what you know of yourself (what you want to tell everyone about when data is reqd, e.g. Avatar info when ur in a game)
CExtendedPlayerManager *g_extendedPlayerManagerLocal;	// handles the sending of packages (large packages are sent over a period of time)
MuteList m_mutelist[32]; // up to 32 mutes for now

// [CExtendedPlayerData] /////////////////////////////////////////////////////////////////////////////////////////////////////

CExtendedPlayerData::CExtendedPlayerData()
{
	m_next = NULL;
	m_data = NULL;
	m_data_n = 0;
	m_tag = 0;
}

CExtendedPlayerData::~CExtendedPlayerData()
{
	if(m_next) {
			delete m_next;
			m_next = NULL;
			}
	if(m_data) {
			delete m_data;
			m_data = NULL;
			m_data_n = 0;
			}
}

int CExtendedPlayerData::AddPackage(void *pData, int size, DWORD tag, int ref)
{
	if(m_data) // if this already has data, put it in child/make a child.
	{
	if(m_next==NULL) m_next = new CExtendedPlayerData;
	// Pass on to child epd
	return m_next->AddPackage(pData, size,tag,ref+1);
	}
	m_data = new char[size];
	m_data_n = size;
	m_tag = tag;
	memcpy(m_data, pData, size);
	return ref;
}

void *CExtendedPlayerData::GetPackageData(DWORD tag)
{
	if(m_data) 
	{
	if(m_tag==tag) {
				   return m_data;
				   }
	}
	
	if(m_next) return m_next->GetPackage(tag);
	
	return NULL;
}

CExtendedPlayerData *CExtendedPlayerData::GetPackage(DWORD tag)
{
	if(m_data) 
	{
	if(m_tag==tag) {
				   return this;
				   }
	}
	
	if(m_next) return m_next->GetPackage(tag);
	
	return NULL;
}

int CExtendedPlayerData::GetNumberOfPackages(int ref)
{
	if(m_next) return m_next->GetNumberOfPackages(ref+1);
	// last node.
	if(m_data) return ref+1;
	return ref;
}

// --------------------- /////////////////////////////////////////////////////////////////////////////////////////////////////

// [CExtendedPlayerManager] /////////////////////////////////////////////////////////////////////////////////////////////////////

CExtendedPlayerManager::CExtendedPlayerManager(CLobby *m_parent)
{
	m_sendpackage = NULL;
	m_offset = 0;
	m_lobbyParent = m_parent;
}

CExtendedPlayerManager::~CExtendedPlayerManager()
{

}

BOOL CExtendedPlayerManager::SendPackage(CExtendedPlayerData *ptr, DWORD tag, char *name, int mode)
{
	DWORD targetid;
	DWORD sourceid;

	int plrIdx;
	plrIdx = m_lobbyParent->FindPlayerIdx(name);
	if(plrIdx==-1) return FALSE; // couldn't find target.

	targetid = m_player[plrIdx].id;
	sourceid = ((CGameAppView *)m_lobbyParent->m_parent)->m_playerId;

	return SendPackage(ptr, tag, targetid, sourceid, mode);
}

BOOL CExtendedPlayerManager::SendPackage(CExtendedPlayerData *ptr, DWORD tag, DWORD targetid, DWORD sourceid, int mode)
{
	if(m_sendpackage) return FALSE;	// still busy sending last package.
	m_sendpackage = ptr->GetPackage(tag);
	if(m_sendpackage==NULL) return FALSE; // failed to find.

	m_offset = 0;
	m_mode = mode;
	m_targetid = targetid;
	m_sourceid = sourceid;


	return TRUE;
}

void CExtendedPlayerManager::ReceivePackage(void *pData)
{
	char *buffer;
	char *realdata;
	int *v;
	int siz;
	int bytesleft;
	int realdata_off;
	DWORD tag;
	DWORD offset;
	DWORD size;
	char name[32];
	CExtendedPlayerData *m_receivepackage;

	//m_receivepackage

	buffer = (char *) pData;

	v = (int *) &buffer[4];
	tag = *v;
	v = (int *) &buffer[8];
	offset = *v;
	v = (int *) &buffer[12];
	size = *v;

	strcpy(name, &buffer[16]);

	realdata_off = 16+strlen(name)+1;
	realdata = &buffer[realdata_off];

}

void CExtendedPlayerManager::Process(void)
{
	if(m_sendpackage)
		{
		// Prefix all messages with 'name' (of target) [server will handle name of source],
		// Then 4 byte Tag,
		// Then 4 byte offset
		// Then 4 byte size
		// Then data.

		char buffer[256];
		char *realdata;
		int *v;
		int siz;
		int bytesleft;
		int realdata_off;

	bytesleft = m_sendpackage->m_data_n-m_offset;

	buffer[0] = 0x7f;	// MSGCODE_GAME Game message.
	buffer[1] = 75;		// MSGGAME_EPD
	buffer[2] = 0;
	buffer[3] = 0;
	v = (int *) &buffer[4];
	*v = m_sendpackage->m_tag;
	v = (int *) &buffer[8];
	*v = m_offset;
	//v = (int *) &buffer[12];
	//!!!!*v = siz;
	v = (int *) &buffer[16];
	*v = m_targetid;
	v = (int *) &buffer[22];
	*v = m_sourceid;
	// 24 from
	
	realdata_off = 24;
	realdata = &buffer[realdata_off];
	siz = 128-realdata_off; // = maximum data we can send.
	if(bytesleft<siz) siz = bytesleft;
	v = (int *) &buffer[12];
	*v = siz;
	memcpy(realdata, &m_sendpackage->m_data[m_offset], siz);
	
	//sprintf(buffer,"%d (%d)", realdata_off+siz, (&realdata[siz])-buffer);
	// Around 32bytes is just header, so the remaining 96 bytes are da shit. (75% eff.)
		
		if(mainShell) // send to mainserver, he'll re-route it.
		{
		mainShell->Client->SendToQueue(buffer, realdata_off+siz, 0);
		} 
		m_offset+=siz;

		if(m_offset==siz) m_sendpackage = 0;
		}
}

// --------------------- /////////////////////////////////////////////////////////////////////////////////////////////////////

Player m_myself;		// the two players playing in this game
Player m_opponent;		// 

Table m_table[MAX_TABLEITEMS];
Details m_details;
CSerialCode *g_serialcode;

char *g_mcode_type_lst[] = {
"Undefined",	// MCODE_UNDEFINED		0
"Special",		// MCODE_SPECIAL		1
"1Year",		// MCODE_1YEAR			2
"6Months",		// MCODE_6MONTHS		3
"3Months",		// MCODE_3MONTHS		4
"1Month",		// MCODE_1MONTH			5
"Trial",		// MCODE_TRIAL			6
"GuestMember",	// MCODE_GUESTMEMBER	7
"Trial2",		// MCODE_TRIAL2			8
NULL,
};

char *g_db_type_lst[] = {
"U",		// MCODE_UNDEFINED		0
"S",		// MCODE_SPECIAL		1
"Y",		// MCODE_1YEAR			2
"6",		// MCODE_6MONTHS		3
"3",		// MCODE_3MONTHS		4
"1",		// MCODE_1MONTH			5
"T",		// MCODE_TRIAL			6
"GM",		// MCODE_GUESTMEMBER	7
"T2",		// MCODE_TRIAL2			8
NULL,
};



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//


CLobby::CLobby(CWnd *pParent)
{
	m_parent = pParent;



}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
CLobby::~CLobby()
{

/*
	if(m_table)
	{
		for(int n=0;n<MAX_TABLEITEMS;n++)
			delete m_table[n];
		delete m_table;
		m_table = 0;
	}

	if(m_player)
	{
		for(int n=0;n<50;n++)
			delete m_player[n];
		delete m_player;
		m_player = 0;
	}
*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL First_Init = TRUE;



void CLobby::Init(void)
{


	//GAMEAPPVIEW->m_htmltest->Navigate2(_T("http://www.google.com"), NULL, NULL);
	//test.Navigate2(_T("http://www.google.com"), NULL, NULL);
	//test.Navigate2(_T("http://www.google.com"), NULL, NULL);

int n;
// initialise vars
m_cx = m_cy = 0;
m_bInLobby = FALSE;//TRUE;
m_bLobbyReady = FALSE;
m_bInTable = FALSE;
m_bDeclineAllGuests = g_gfxDetails.bDeclineAllGuests;//FALSE;
m_bDeclineAll = g_gfxDetails.bDeclineAll;//FALSE;
m_PLCwait = 0;


// Initialize LobbyTVInfo values //
g_LobbyTVInfo.lobbytablewatch = -1;		// Defaultly Not Watching any Table //
g_LobbyTVInfo.bShowShot = 0;			// Defaultly we are not displaying the shots //
strcpy(g_LobbyTVInfo.name1P, "Player 1");
strcpy(g_LobbyTVInfo.name2P, "Player 2");
g_LobbyTVInfo.score1P = 0;
g_LobbyTVInfo.score2P = 0;
g_LobbyTVInfo.break1P = 0;
g_LobbyTVInfo.break2P = 0;
g_LobbyTVInfo.bTurn1P = 10;

// Create Welcome Table Text //
//Message(m_parent, "Welcome to iSnooker Ver. 1.810");



#ifndef MAIN_SERVER
	char buffer[256];

#ifndef THE_POOL_CLUB
	#ifdef WIN_VISTA
		sprintf(buffer,"Welcome to iSnooker Version %s\n", GAME_VERSION);
	#else
		sprintf(buffer,"Welcome to iSnooker Version %s\n", GAME_VERSION);
	#endif



	GAMEAPPVIEW->Message(buffer, 10);


	sprintf(buffer,"Welcome to iSnooker. Create a 14 day Trial Account (FREE), by Clicking on 'Create Account' or 'Register Menu -> New Account'.\n");
	GAMEAPPVIEW->Message(buffer, 70);

#else
	#ifdef WIN_VISTA
		sprintf(buffer,"Welcome to iPool Version %s", GAME_VERSION);
	#else
		sprintf(buffer,"Welcome to iPool Version %s", GAME_VERSION);
	#endif
	GAMEAPPVIEW->Message(buffer, 10);
	sprintf(buffer,"Welcome to iPool. Create a 21 day Trial Account (FREE), by Clicking on 'Create Account' or 'Register Menu -> New Account'.");
	GAMEAPPVIEW->Message(buffer, 70);
#endif

	
if(First_Init)
	{
	CMenu *subMenu = g_menu->GetSubMenu(GAME_MENU);
	First_Init = FALSE;
			
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(GAME_INVITATIONS);
		if(subSubMenu)
		{
			if(m_bDeclineAll == FALSE)
			{
				subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALL, MF_UNCHECKED | MF_BYPOSITION);
				for(int n=0;n<MAX_INVITATIONDLGS;n++)
					GAMEAPPVIEW->m_invitationDlg[n].m_bDeclineAll = FALSE;
			}
			else
			{
				subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALL, MF_CHECKED | MF_BYPOSITION);
				for(int n=0;n<MAX_INVITATIONDLGS;n++)
					GAMEAPPVIEW->m_invitationDlg[n].m_bDeclineAll = TRUE;
			}

			if(m_bDeclineAllGuests == FALSE)
			{
				subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALLGUESTS, MF_UNCHECKED | MF_BYPOSITION);
				for(n=0;n<MAX_INVITATIONDLGS;n++)
					GAMEAPPVIEW->m_invitationDlg[n].m_bDeclineAllGuests = FALSE;
			}
			else
			{
				subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALLGUESTS, MF_CHECKED | MF_BYPOSITION);
				for(n=0;n<MAX_INVITATIONDLGS;n++)
					GAMEAPPVIEW->m_invitationDlg[n].m_bDeclineAllGuests = TRUE;
			}
		}
	}
#endif //!MAIN_SERVER 
	

#ifndef MAIN_SERVER
	if(g_build.gameType == GAMETYPE_POOL)
		{
		int n;
		for(n=0;n<81;n++)
			{
			GAMEAPPVIEW->m_bitmapTable[n] = (HBITMAP)LoadImage(0, tableFName[n], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			//GAMEAPPVIEW->m_bitmapTable[n].LoadBitmap(tableFName[n]);
			}
		}
	else if(g_build.gameType == GAMETYPE_SNOOKER)
		{
		int n;
		for(n=0;n<9;n++)
			{	
			GAMEAPPVIEW->m_bitmapTable2[n] = (HBITMAP)LoadImage(0, tableSnookerFName[n], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			//GAMEAPPVIEW->m_bitmapTable2[n].LoadBitmap(tableSnookerFName[n]);
			}
		}
	}
#endif //!MAIN_SERVER

	
	// empty players (nobody here)
	for(n=0;n<MAX_LOBBY_PLAYERS/*50*/;n++)
	{
		ClearPlayer(n);
	g_extendedPlayerData[n] = NULL;
	}
	g_extendedPlayerDataLocal = NULL;

	for(n=0;n<32;n++)
		m_mutelist[n].name[0] = 0;

	

	#ifndef MAIN_SERVER
//	SetupLocalEPD(); // note: if using this in the future, make sure to add cleanup/destroy code
	#endif //!MAIN_SERVER
	
	ClearPlayer(&m_myself);
	ClearPlayer(&m_opponent);

	//// clean all tables (nobody here)
	for(n=0;n<MAX_TABLEITEMS;n++)
		ClearTable(n);

	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::Deinit(void)
{
	// idle...
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::Process(void)
{
	int plrIdx = GetPlayerIdx(((CGameAppView *)m_parent)->m_playerSockIdx);
	char buffer[256];
		
	// Client code
	// create/join a table code
	if(!m_bLobbyReady || m_bInTable)
		((CGameAppView *)m_parent)->m_tableNo = -1;
	
	
	if(m_bInTable == FALSE)
	if( ((CGameAppView *)m_parent)->m_tableNo !=-1 )
	{
		// if this joiner cannot host this empty table
		if(EmptyTable(((CGameAppView *)m_parent)->m_tableNo))
			if(plrIdx != -1)
				if(!m_player[plrIdx].bCanHost)
				{
					((CGameAppView *)m_parent)->m_tableNo = -1;
					goto _skipJoin;
				}
			////
			// for returning lost connection player
			if(m_table[((CGameAppView *)m_parent)->m_tableNo].bLockSeats) ////NEW
			if( ((CGameAppView *)m_parent)->m_tableEnter ) // if trying to sit
			{
				//if(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P2StartAtTable)
				int tblIdx = ((CGameAppView *)m_parent)->m_tableNo;
				
				if(!strcmpi(m_table[tblIdx].P1StartName, m_player[plrIdx].name) || !strcmpi(m_table[tblIdx].P2StartName, m_player[plrIdx].name))
				{
					//ErrorMessage(m_parent, "ABCD!");
					// ok to proceed
					//;
				}
				else			
				{
					// dont sit!
					//ErrorMessage(m_parent, "WXYZ!");
					LostConnectionMessageCantSit();
					((CGameAppView *)m_parent)->m_tableNo = -1;
					goto _skipJoin;
				}
			}
			////
			
		
			// default...
			m_bInTable = TRUE;
			((CGameAppView *)m_parent)->SendPlayerJoin(FALSE,
													   ((CGameAppView *)m_parent)->m_tableNo, 
													   ((CGameAppView *)m_parent)->m_tableEnter, 
													   ((CGameAppView *)m_parent)->m_playerSockIdx,
													   FALSE);

			((CGameAppView *)m_parent)->m_tableNo = -1;
	}
_skipJoin:;

#ifdef MAIN_SERVER
		  
	// Server just joined lobby message
	static BOOL firstTime = TRUE;
	if(firstTime)
	{
		firstTime = FALSE;
		char _buffer[MAX_CHARS];
		char *timestr;
		// Get (Local) time
		GAMEAPPVIEW->GetTime();
		timestr = GAMEAPPVIEW->m_stringFullTime.GetBuffer(0);
		sprintf(_buffer,"*** %s joined the lobby at %s ***", g_serverHandle.GetBuffer(0), timestr);
		GAMEAPPVIEW->Message(_buffer, 10); // personal message
	}

	// server send-over player data to newly joined players
	if(  ( ((CGameAppView *)m_parent)->m_cyc&0x03 )  ) // 1 in 4 frames (Odd)
	{
		// check & start sending player details to an individual that has just joined
		int n = 0;
		int m = 0;
		for(n=0;n<MAX_LOBBY_PLAYERS;n++)
		if(m_player[n].status)
		{
			if(m_player[n].getAllPlayers >=0)
			{
				// send server info. 'once' to player (i.e. if this is a tournament lobby, handle etc...)
				((CGameAppView *)m_parent)->SendServerData(m_player[n].playerSocketIdx);
				// give player server configuration data (second batch of configuration data - could've been combined in the above but im doing this seperate - colin)
				((CGameAppView *)m_parent)->SendServerConfig(m_player[n].playerSocketIdx);


				for(m=m_player[n].getAllPlayers;m<MAX_LOBBY_PLAYERS;m++)
				if(m_player[m].status)
				if(m!=n)
				{
					
					((CGameAppView *)m_parent)->SendPlayerDetailsCompact(TRUE,
																  &m_player[m],
																  m_player[n].playerSocketIdx);
					

				//	m_player[n].getAllPlayers = m+1;
				//	goto endGetAllPlayers;
				}

				//break;
				// check if this player has received all players details
				//if(m == MAX_LOBBY_PLAYERS)
				//{
					m_player[n].getAllPlayers = -1;
					m_player[n].getAllTables = 0; // for server only (tells server to start sending all table details)
					//send(...);
				//}

			}
		}
		//endGetAllPlayers:;

	}
	else if(  ( (((CGameAppView *)m_parent)->m_cyc+2)&0x03 )  ) // 1 in 4 frames (Even)
	{
		// check & start sending table details to an individual that has just joined
		int n = 0;
		int m = 0;
		for(n=0;n<MAX_LOBBY_PLAYERS;n++)
		if(m_player[n].status)
		{
			if(m_player[n].getAllTables >=0)
			{
				for(m=m_player[n].getAllTables;m<MAX_TABLEITEMS;m++)
				if(m_table[m].status || m_table[m].tournament)
				{				
					m_table[m].idx = m; // NEW: safety code
					// usabap
					((CGameAppView *)m_parent)->SendTableDetails(TRUE,
																 &m_table[m],
																 m_player[n].playerSocketIdx);
				}

			m_player[n].getAllTables = -1;
			//send(...);
			// tell player all server lobby-data has been sent
			((CGameAppView *)m_parent)->SendServerTime(m_player[n].playerSocketIdx);
			((CGameAppView *)m_parent)->SendLobbyReady(TRUE, m_player[n].playerSocketIdx);
			// send high breaks
			((CGameAppView *)m_parent)->SendAllHighBreaks(m_player[n].playerSocketIdx);
			LobbyJoinMessage(m_player[n].playerSocketIdx);
			((CGameAppView *)m_parent)->UpdateSponsor(m_player[n].playerSocketIdx);
			// Send Client the Abuse Lst //
			//((CGameAppView *)m_parent)->SendAbuseLst(m_player[n].playerSocketIdx);
			// For now, Client will use a local list (less to transfer to Client when loggin in)

			// Randomly Choose an active Table to Watch (if any), else set to watch Table 1 //
			// Get a list of all tables with games in progress //
			int total_active_tables = 0;
			int p;
			unsigned char active_tables[MAX_LOBBY_TABLES];
			for(p=0;p<MAX_LOBBY_TABLES;p++) active_tables[p] = 0;

			for(p=0;p<MAX_LOBBY_TABLES;p++)
			if(m_table[p].bGameStarted)
				{
				active_tables[total_active_tables] = p;
				total_active_tables++;
				}

			if(total_active_tables)
				{
				int tblIdx = GetTableIdxFromPlayer(m_player[n].playerSocketIdx);
				if(tblIdx != -1) m_player[n].lobbytablewatch = -1; // If we are at a Table, then set this to -1, so this player doesn't receive MSGGAME_BALLS for the Lobby Table Watch
				else{
					// Randomly asign player's lobbytablewatch variable to an active table
					m_player[n].lobbytablewatch = active_tables[(rand()%total_active_tables)];
					}
				}
			else m_player[n].lobbytablewatch = -1;
			
			// Now we need to Update the Client, so it knows what Table it's Defaultly Watching //
			if(m_player[n].lobbytablewatch != -1)
				{	
				((CGameAppView *)m_parent)->UpdateLobbyTVClient(m_player[n].playerSocketIdx, m_player[n].lobbytablewatch);
				}
			}
		}
		
		
	}

#endif //MAIN_SERVER


	#ifndef MAIN_SERVER
	if(g_bTournamentLobby)
	{
		if(m_PLCwait)
		{
			if(LostConnectionReturned(TRUE))
				goto _PLCend;
		}
	}
	else
	{
		///////////////////////////////
		// for normal lobbies -- for every individual to do, check for autoinvites and clear them if they are out of date
		for(int i=0;i<MAX_INVITATIONDLGS;i++)
		if(GAMEAPPVIEW->m_invitationDlg[i].m_hostSockIdx == 0)
		if(GAMEAPPVIEW->m_invitationDlg[i].IsWindowEnabled())
		{
			int invitedToTbl = GAMEAPPVIEW->m_invitationDlg[i].m_table;
			if(invitedToTbl != -1)
			{
				if ((m_table[invitedToTbl].status && m_table[invitedToTbl].bLockSeats) || m_table[invitedToTbl].bLadderMatch)
				{
					// do nothing...
				}
				else
				{
					// destroy this invitation, as it has no use now!
					GAMEAPPVIEW->m_invitationDlg[i].DestroyWindow();
				}
			}
		}
		///////////////////////////////
		if(m_PLCwait)
		{
			// check if opponent has returned
			if(LostConnectionReturned(TRUE))
				goto _PLCend;
			
			int timeElapsed = g_cyc - m_PLCwait;
			
			// Only Need to Display this message, if the Foul Dialog is not showing //
			if((timeElapsed >= 2*60*1000) || (timeElapsed < 0))  // now 2mins for opponent to return!
			{
				m_PLCwait = g_cyc; // Restart Timer again //
				// bring up force forfeit dialog
				GAMEAPPVIEW->m_forfeitDlg.m_forfeitType = 0;
				GAMEAPPVIEW->m_forfeitDlg.DestroyWindow();
				GAMEAPPVIEW->m_forfeitDlg.Create(IDD_FORFEITDLG);
				GAMEAPPVIEW->m_forfeitDlg.ShowWindow(SW_SHOW);  // newfix This appears when the applicaiton starts
				

				//GAMEAPPVIEW->SetFocus();
				GAMEAPPVIEW->PreviousWndEditSetFocus();
			}
		}
	}

_PLCend:;

	// *NEW - check for people just sitting at a table a doing nothing for 5mins
	CheckTableHogger();

	#endif //!MAIN_SERVER

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::AddMute(char *name)
{
	int n;

	// check if 'name' is already in MuteList
	for(n=0;n<32;n++)
	{
		// if this 'name' has already been added to the mutelist
		if(!strcmpi(name, m_mutelist[n].name) == 0)
			return;		
	}

	// ...otherwise just add onto end of list
	for(n=0;n<32;n++)
	{
		if(m_mutelist[n].name[0] == 0)
		{
			strcpy(m_mutelist[n].name, name);
			return;
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::RemoveMute(char *name)
{
	int n;

	// check if 'name' is in MuteList
	for(n=0;n<32;n++)
	{
		// if this 'name' is present in the mutelist
		if(!strcmpi(name, m_mutelist[n].name) == 0)
		{
			m_mutelist[n].name[0] = 0;
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CLobby::IsMuted(char *name)
{
	int n;

	// check if 'name' is already in MuteList
	for(n=0;n<32;n++)
	{
		// if this 'name' has already been added to the mutelist
		if(!strcmpi(name, m_mutelist[n].name) == 0)
			return TRUE;		
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::CheckTableHogger(void)
{
	// if at a table, and 1) game hasn't started and 2) 'Host' or 'Sitting' then boot if was idle for 5mins.
	if(GAMEAPPVIEW->m_bIdleAway) // idle for 5mins
	{
		int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);

		if(myTblIdx != -1) // if at a table
		if(!GameStarted(GAMEAPPVIEW->m_playerSockIdx)) // game not started
		if(HostOfTable(GAMEAPPVIEW->m_playerSockIdx) || SeatedAtTable(GAMEAPPVIEW->m_playerSockIdx))
		{			
			GAMEAPPVIEW->ExitTable2Lobby();

			// display idle message in lobby - for idle-booted player only:
			char _buffer[MAX_CHARS];
			sprintf(_buffer,"(You have been auto-booted from table #%d, for being idle too long!)", myTblIdx+1);
			GAMEAPPVIEW->Message(_buffer,10); // system message		

			// send main server message that we've left the table
			GAMEAPPVIEW->SendPlayerExit(FALSE, GAMEAPPVIEW->m_playerSockIdx);
		}
	}
}



// Pairs up players who are waiting for a Match. Sets up table for this.
// Set's the player's status to Found Match, and their Table & Seat Index to invite to

void CLobby::ProcessFindMatchSetup(void)
{
#ifdef MAIN_SERVER

	int i;
	int searchGameStatus;

	// Cancel player search status if they are at a table
	for (i = 0; i < MAX_LOBBY_PLAYERS; i++)
	{
		searchGameStatus = m_player[i].searchgamestatus & 0xf;

		if (searchGameStatus > 0)	// Either Searching or a Game or Found a Game
		{
			if (m_player[i].status)
			if (m_player[i].table != -1)
			{
				// in a game, so cancel Ladder Match Search
				m_player[i].searchgamestatus &= 0xfffffff0;

				// Cancel our search
				g_gameAppView->SendClientFindMatchStatus(m_player[i].playerSocketIdx, 0);
			}

			/*
			// Check if we're in a game.. If so, stop us searching for Ladder Matches
			for (int n = 0; n < MAX_LOBBY_TABLES; n++)
			{
				if (g_gameAppView->m_lobby->SeatedAtTableIdx(m_player[i].playerSocketIdx, n))
				if (g_gameAppView->m_lobby->PressedStartAtTable(m_player[i].playerSocketIdx))
				{
					// in a game, so cancel Ladder Match Search
					m_player[i].searchgamestatus &= 0xfffffff0;

					// Cancel our search
					g_gameAppView->SendClientFindMatchStatus(m_player[i].playerSocketIdx, 0);
					break;
				}
			}
			*/
		}
	}





	bool bCheckedAll = false;

	int tbl;
	int P1Div;
	int P2Div;

	char player1_pro;
	char player2_pro;
	char player1_RaceToTwo;
	char player2_RaceToTwo;
	char player1_exactMatch;
	char player2_exactMatch;


	// Get the player who's been waiting for a game longest
	long stime;
	int sidxP1;
	int sidxP2;


	// Get our current time..
	long currentTime;
	CTime m_time;
	CTimeSpan diff;
	m_time = m_time.GetCurrentTime();
	CTime t1970(1970, 1, 1, 0, 0, 0);
	diff = m_time - t1970;
	currentTime = diff.GetTotalSeconds();


	bool bChecked[MAX_LOBBY_PLAYERS];
	for (i = 0; i < MAX_LOBBY_PLAYERS; i++) bChecked[i] = false;

	// Check if we have any ladder matches to add
	while(bCheckedAll == false)
	{
		if (!g_bTournamentLobby)
		{
			stime = LONG_MAX;
			sidxP1 = -1;
			sidxP2 = -1;

			// For each player that is searching for a game, we will try to pair them up with the most suitable match
			for (i = 0; i < MAX_LOBBY_PLAYERS; i++)
			if (bChecked[i] == false)
			if (m_player[i].status)
			if (m_player[i].searchgamestatus & 0x01)
			{
				if (m_player[i].searchgametime < stime)
				{
					stime = m_player[i].searchgametime;
					sidxP1 = i;
				}
			}

			// If we found someone, don't want to recheck this 'first' player
			if (sidxP1 != -1)
			{
				bChecked[sidxP1] = true;
			}


			// If we didn't find a player searching and we've checked all players, return
			if (sidxP1 == -1 && i == MAX_LOBBY_PLAYERS)
			{
				bCheckedAll = true;
				return;
			}

			

			// Get Division of this first player..
			if (m_player[sidxP1].ladderPoints < 11000) P1Div = 0;
			else if (m_player[sidxP1].ladderPoints < 14000) P1Div = 1;
			else if (m_player[sidxP1].ladderPoints < 17000) P1Div = 2;
			else if (m_player[sidxP1].ladderPoints < 21000) P1Div = 3;
			else P1Div = 4;

	
			
			// First try to find an exact match for what we want (i.e. Pro / Semi Pro, and Race to Match), and closest Division


			// First go through other players.. and see if we can find anyone who's within one Division of us (to play)
			// Next check if anyone has been waiting at least 20 seconds. If So, Match them up first..


			// Look for a player within one Division of us
			// Division (i.e. 0-10999: Bronze, 11000-13999: Silver, 14000-16999: Gold, 17000-20999: Platinum, 21000+ Diamond)

			
			stime = LONG_MAX;
			for (i = 0; i < MAX_LOBBY_PLAYERS; i++)
			if (i != sidxP1)
			if (m_player[i].status)
			if (m_player[i].searchgamestatus & 0x01)
			{

				// Ignore this player if they haven't been waiting at least 3 seconds. This will increase the chance of us getting a good match (with someone)
				// instead of taking the first find
				// if ((currentTime - m_player[i].searchgametime) < 5) continue;

				// Get Exact Match status (of 1st Player)
				player1_exactMatch = (m_player[sidxP1].searchgamestatus & 0x40) >> 6;
				player2_exactMatch = (m_player[i].searchgamestatus & 0x40) >> 6;

				// If either player is looking for exact match, make sure their search criteria matches or skip 'continue'
				if (player1_exactMatch == 1 || player2_exactMatch == 1)
				{
					// Get Pro Game status & race to
					player1_pro = (m_player[sidxP1].searchgamestatus & 0x20) >> 5;
					player1_RaceToTwo = (m_player[sidxP1].searchgamestatus & 0x10) >> 4;

					player2_pro = (m_player[i].searchgamestatus & 0x20) >> 5;
					player2_RaceToTwo = (m_player[i].searchgamestatus & 0x10) >> 4;

					// skip (continue) if pro and race to don't match
					if ((player1_pro != player2_pro) || (player1_RaceToTwo != player2_RaceToTwo)) continue;
				}
				
									
				// Get Division of P2
				if (m_player[i].ladderPoints < 11000) P2Div = 0;
				else if (m_player[i].ladderPoints < 14000) P2Div = 1;
				else if (m_player[i].ladderPoints < 17000) P2Div = 2;
				else if (m_player[i].ladderPoints < 21000) P2Div = 3;
				else P2Div = 4;

				int diff = P1Div - P2Div;
				if (diff < 0) diff = -diff;

				// If we're been waiting over 15 seconds, take the first available find (within 2 Divisions of us

				if ((currentTime - m_player[sidxP1].searchgametime) < (2 + 5) &&
					(currentTime - m_player[i].searchgametime) < (2 + 5) )	// If both player have been searching under 5 seconds, check within same division for a match
				{
					if (diff == 0)	// Same Division
					if (m_player[i].searchgametime < stime)
					{
						stime = m_player[i].searchgametime;
						sidxP2 = i;
					}
				}
				else if ((currentTime - m_player[sidxP1].searchgametime) < (2 + 10) &&
						 (currentTime - m_player[i].searchgametime) < (2 + 10))	// if both players have been searching between 5-10 seconds (look within 1 divisions of us for match)
				{
					if (diff <= 1)	// Within 1 Division
					if (m_player[i].searchgametime < stime)
					{
						stime = m_player[i].searchgametime;
						sidxP2 = i;
					}
				}
				else if ((currentTime - m_player[sidxP1].searchgametime) < (2 + 15) &&
					(currentTime - m_player[i].searchgametime) < (2 + 15))	// if both plyers have been searching between 10-15 seconds (look within 2 divisions of us for match)
				{
					if (diff <= 2)	// Within 2 Division
					if (m_player[i].searchgametime < stime)
					{
						stime = m_player[i].searchgametime;
						sidxP2 = i;
					}
				}
				else // if we've been searching more than (2+10) seconds, find anyone available
				{
					if (m_player[i].searchgametime < stime)
					{
						stime = m_player[i].searchgametime;
						sidxP2 = i;
					}
				}
			}

		

			// Do we have two players waiting for a match, If so Find a table
			if (sidxP1 != -1 && sidxP2 != -1)
			{
				// find vacant table for this Match

				for (tbl = 0; tbl < MAX_TABLEITEMS; tbl++)
				if (g_gameAppView->m_lobby->EmptyTable(tbl))
				{
					// found a vacant table!
					break;
				}
				if (tbl == MAX_TABLEITEMS) return; // no spare table

				int idx = tbl;
				

				////////////////////////////////////////////////////////////////////////
				/////// We found a Table so update player's Find Match Status //////////
				////////////////////////////////////////////////////////////////////////

				// If either player has set Semi Pro (make game Semi Pro)
				char pro;
				player1_pro = (m_player[sidxP1].searchgamestatus & 0x20) >> 5;
				player2_pro = (m_player[sidxP2].searchgamestatus & 0x20) >> 5;
				if (player1_pro == 1 && player2_pro == 1) pro = 1;
				else pro = 0;


				// Get Race to
				char RaceToTwo;
				player1_RaceToTwo = (m_player[sidxP1].searchgamestatus & 0x10) >> 4;
				player2_RaceToTwo = (m_player[sidxP2].searchgamestatus & 0x10) >> 4;
				if (player1_RaceToTwo == 1 && player2_RaceToTwo == 1) RaceToTwo = 1;
					else RaceToTwo = 0;

				// Get Exact Match
				player1_exactMatch = (m_player[sidxP1].searchgamestatus & 0x40) >> 6;
				player2_exactMatch = (m_player[sidxP2].searchgamestatus & 0x40) >> 6;



				// Set up Tournament Object, which we use for Ladder Matches
				g_tournament.AddName("Ladder Matches", idx);
				g_tournament.AddDesc("iSnooker Ladder League", idx);
				// g_tournament.AddSponsor("Ladder Match", idx);
				// g_tournament.AddSponsorURL("http://www.thesnookerclub.com", idx);
				
				char sGameType[32];

				// If we're iPool, pro option is used for 9-Ball game being on or not
				if (g_build.gameType == GAMETYPE_POOL)
				{
					
					if (pro == 0) sprintf(sGameType, "%s", "8Ball");
					else sprintf(sGameType, "%s", "9Ball");

					g_tournament.AddOptionsLevel("Pro", idx);
					g_tournament.AddOptionsWhoBreaks("Alternate", idx);
					g_tournament.AddOptionsTableColour("Blue", idx);
					g_tournament.AddOptionsTableType("Pro", idx); // (iPool)
					g_tournament.AddOptionsBallSize("Normal", idx);
					g_tournament.AddOptionsRackType("Normal", idx);
					// g_tournament.AddOptionsAddTimer(AddTimer->GetString(), idx);
					// g_tournament.AddOptionsShotTime(ShotTime->GetString(), idx);
					g_tournament.AddOptionsRated("Yes", idx);
					g_tournament.AddOptionsCallShots("Yes", idx);
					g_tournament.AddOptionsMissRule("Yes", idx);
					g_tournament.AddOptionsHighB("0", idx);
					g_tournament.AddOptionsHighBCash("0", idx);
					
				}
				else
				{
					sprintf(sGameType, "%s", "Snooker");

					if (pro == 0) g_tournament.AddOptionsLevel("SemiPro", idx);
					else g_tournament.AddOptionsLevel("Pro", idx);
					
					g_tournament.AddOptionsWhoBreaks("Alternate", idx);
					g_tournament.AddOptionsTableColour("Green", idx);
					// g_tournament.AddOptionsTableType(TableType->GetString(), idx); // (iPool)
					g_tournament.AddOptionsBallSize("Normal", idx);
					g_tournament.AddOptionsRackType("Normal", idx);
					// g_tournament.AddOptionsAddTimer(AddTimer->GetString(), idx);
					// g_tournament.AddOptionsShotTime(ShotTime->GetString(), idx);
					g_tournament.AddOptionsRated("Yes", idx);
					// g_tournament.AddOptionsCallShots(CallShots->GetString(), idx);
					g_tournament.AddOptionsMissRule("Yes", idx);
					g_tournament.AddOptionsHighB("0", idx);
					g_tournament.AddOptionsHighBCash("0", idx);

					
				}

				



				// Set Search Status to 2 (Found Match)
				searchGameStatus = 2;	// Found Match!
				searchGameStatus |= (player1_RaceToTwo << 4);		// Race to
				searchGameStatus |= (player1_pro << 5);		// Set the pro state
				searchGameStatus |= (player1_exactMatch << 6);
				m_player[sidxP1].searchgamestatus = searchGameStatus;

				searchGameStatus = 2;	// Found Match!
				searchGameStatus |= (player2_RaceToTwo << 4);		// Race to
				searchGameStatus |= (player2_pro << 5);		// Set the pro state
				searchGameStatus |= (player2_exactMatch << 6);
				m_player[sidxP2].searchgamestatus = searchGameStatus;

				// Update Clients HERE!


				// Set the Table and Seats to invite players to
				m_player[sidxP1].searchgametableinvite = tbl;
				m_player[sidxP1].searchgameseatinvite = 0;

				m_player[sidxP2].searchgametableinvite = tbl;
				m_player[sidxP2].searchgameseatinvite = 1;


				// Tell Client that they are in a Match
				g_gameAppView->SendClientFindMatchStatus(m_player[sidxP1].playerSocketIdx, searchGameStatus);
				g_gameAppView->SendClientFindMatchStatus(m_player[sidxP2].playerSocketIdx, searchGameStatus);


				// Create Match
				int gameLevel = 1; // Semi Pro by default
				if (pro == 1) gameLevel = 2;
				char raceToStr[8];
				if (RaceToTwo)
				{
					if (g_build.gameType == GAMETYPE_POOL) sprintf(raceToStr, "5");
						else sprintf(raceToStr, "2");
				}
				else
				{
					if (g_build.gameType == GAMETYPE_POOL) sprintf(raceToStr, "3");
						else sprintf(raceToStr, "1");
				}


				g_tournament.AddMatch("0", raceToStr, m_player[sidxP1].name, m_player[sidxP2].name, "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "1", "0", "0", "300", sGameType, idx, true);
			}
			else
			{
				// We didn't find any Matches to Create for this player
			}
		}
	}
#endif //MAIN_SERVER
}


// Invite Players awaiting invite to table for Match
void CLobby::ProcessFindMatchInvites(void)
{
#ifdef MAIN_SERVER
	
	// *NEW - server to handle autoinvites for players who lost connection, in normal lobbies	
	if (!g_bTournamentLobby)
	{
		int i;

		// Invite any player to their Match (that hasn't accepted invite yet
		for (i = 0; i < MAX_LOBBY_PLAYERS; i++)
		if (m_player[i].status)
		if (m_player[i].searchgamestatus & 0x02)
		{


			int n;
			bool bAlreadyInGame;

			int tableIdx = m_player[i].searchgametableinvite;
			int seatIdx = m_player[i].searchgameseatinvite + 1;

			// 1P
			int plr1SockIdx = m_player[i].playerSocketIdx;
			if (plr1SockIdx != -1)
			{
				// If Player is already involved in another game, skip this invite //
				bAlreadyInGame = false;
				for (n = 0; n < MAX_LOBBY_TABLES; n++)
				{
					if (g_gameAppView->m_lobby->SeatedAtTableIdx(plr1SockIdx, n))
					if (g_gameAppView->m_lobby->PressedStartAtTable(plr1SockIdx))
					{
						bAlreadyInGame = true;
						break;
					}
				}

				if (!bAlreadyInGame)
				if (!g_gameAppView->m_lobby->SeatedAtTableIdx(plr1SockIdx, tableIdx))
				{
					// ErrorMessage(((CGameAppView *)g_wnd), "Find Match Invite");
					// Auto-Invite player-1 to table
					g_gameAppView->SendInvitePressed(0, plr1SockIdx, g_serverHandle.GetBuffer(0), m_player[i].name, FALSE, NULL, tableIdx, tableIdx, seatIdx); // network game only!
				}
			}

		}

	}
#endif //MAIN_SERVER
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// REAL


void CLobby::LostConnectionAutoInvites(void) // [for server to call only]
{
	#ifdef MAIN_SERVER
	// *NEW - server to handle autoinvites for players who lost connection, in normal lobbies	
	if(!g_bTournamentLobby)
	{
		int tbl, plr;
		for(tbl=0;tbl<MAX_TABLEITEMS;tbl++)
		if(m_table[tbl].status)
		if(m_table[tbl].bLadderMatch == false)	// If not a Ladder Match
		if(m_table[tbl].bLockSeats) // if someone lost connection at a table
		{
			// Make sure this isn't a Conceded Game //
			// If we conceded this game, then don't invite us back //
			if(TableLostConnectionFlag[tbl])
			{
				// 1P seat...
				if(m_table[tbl].player1SockIdx == -1) // if seat is empty, it should always be when tbl is locked!
				if(m_table[tbl].P1StartName[0] != 0)
				{
					for(plr=0;plr<MAX_LOBBY_PLAYERS;plr++)
					if(m_player[plr].status)
					if(!strcmpi(m_table[tbl].P1StartName, m_player[plr].name))
					{
						//ErrorMessage(((CGameAppView *)g_wnd), "P1 AUTO-Invited!");
						g_gameAppView->SendInvitePressed(0, m_player[plr].playerSocketIdx, g_serverHandle.GetBuffer(0), m_player[plr].name, FALSE, NULL, tbl, tbl, 1); // network game only!
					}
				}

				// 2P seat...
				if(m_table[tbl].player2SockIdx == -1) // if seat is empty, it should always be when tbl is locked!
				if(m_table[tbl].P2StartName[0] != 0)
				{
					for(plr=0;plr<MAX_LOBBY_PLAYERS;plr++)
					if(m_player[plr].status)
					if(!strcmpi(m_table[tbl].P2StartName, m_player[plr].name))
					{
						//ErrorMessage(((CGameAppView *)g_wnd), "P2 AUTO-Invited!");
						g_gameAppView->SendInvitePressed(0, m_player[plr].playerSocketIdx, g_serverHandle.GetBuffer(0), m_player[plr].name, FALSE, NULL, tbl, tbl, 2); // network game only!
					}
				}
			}
		}
	}
	#endif //MAIN_SERVER
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::LostConnectionMessageCantSit(void)
{
	char buffer[MAX_CHARS];							 
	sprintf(buffer, "Cannot 'Sit' at this table! Waiting for player to rejoin...");//, m_PLCname);
	GAMEAPPVIEW->Message(buffer, 10); // personal message	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CLobby::LostConnectionReturned(BOOL bSendReturnMsg)
{
	
	int oppSockIdx = FindPlayerSockIdx(m_PLCname);		
	if(oppSockIdx != -1)
	{
		int oppTblIdx = GetTableIdxFromPlayer(oppSockIdx);
		int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if(oppTblIdx == myTblIdx)
		if(SeatedAtTable(oppSockIdx)) // *NEW line added, player must sit first for reconnect to be complete!!!
		{
			if(bSendReturnMsg)
			{				
				m_PLCwait = 0; // this player has now rejoined us, so dont go any further with this 5min. countdown
				GAMEAPPVIEW->SendPlayerConnection(oppSockIdx,  GAMEAPPVIEW->m_snooker->m_gameOver, GAMEAPPVIEW->m_snooker->m_gameOverIWon, GAMEAPPVIEW->m_playerSockIdx, myTblIdx);
			}
			return TRUE;
		}
	}

	return FALSE;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::LostConnectionWaitMore(void)
{
	if(!LostConnectionReturned())
	{
		m_PLCwait = g_cyc;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::LostConnectionGameOver(void)
{
	if(!LostConnectionReturned())
		{
		// game over!...
		if(!GAMEAPPVIEW->m_snooker->m_gameOver)
			{
			// if i'm the winner, send gameover msg to all
			int tblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(tblIdx != -1) // check i'm on a valid table
				{
					// make sure the first gameOver winner wins! (Not the the later gameOvers)
					int plrIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);
					if(plrIdx==-1) return;
					if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1StartName)) GAMEAPPVIEW->m_snooker->m_gameOver = 2; // 1P wins
						else GAMEAPPVIEW->m_snooker->m_gameOver = 1; // 2P wins

					if(IsPlayerWinner(GAMEAPPVIEW->m_playerSockIdx))
						{
						GAMEAPPVIEW->m_snooker->m_bConcededGame = FALSE; // **
						GAMEAPPVIEW->SendGameOverForAll(tblIdx, GAMEAPPVIEW->m_snooker->m_gameOver, TRUE); // lose entire match/raceto!
						}
				}
			
			GAMEAPPVIEW->m_snooker->m_gameOver = 0;
			GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;
			}
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::MaxTimeUpWaitMore(void)
{
	GAMEAPPVIEW->m_snooker->m_MaxShotTime = 60*50;		// wait 1 minute longer, before bringing up this dialog again...
	GAMEAPPVIEW->m_snooker->m_MaxShotTimeOld = GAMEAPPVIEW->m_snooker->m_MaxShotTime;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::MaxTimeUpGameOver(void)
{	
	// game over!...
	if(!GAMEAPPVIEW->m_snooker->m_gameOver)
		{
		// if i'm the winner, send gameover msg to all
		int tblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if(tblIdx != -1) // check i'm on a valid table
			{
				// make sure the first gameOver winner wins! (Not the the later gameOvers)
				int plrIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);
				if(plrIdx==-1) return;
				if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1StartName)) GAMEAPPVIEW->m_snooker->m_gameOver = 2; // 1P wins
					else GAMEAPPVIEW->m_snooker->m_gameOver = 1; // 2P wins

				if(IsPlayerWinner(GAMEAPPVIEW->m_playerSockIdx))
					{
					GAMEAPPVIEW->m_snooker->m_bConcededGame = FALSE; // **
					GAMEAPPVIEW->SendGameOverForAll(tblIdx, GAMEAPPVIEW->m_snooker->m_gameOver, FALSE); // NEW: 21/FEB/2004
					}
			}
		
		GAMEAPPVIEW->m_snooker->m_gameOver = 0;
		GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//


void CLobby::LobbyJoinMessage(int plrSockIdx)
{


	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx==-1) return;

	char _buffer[MAX_CHARS];
	CString timestr;

	// Display Server Time when Loggin In, so we are all consistent on GMT //
	// Get (Local) time
#ifdef MAIN_SERVER	
	GAMEAPPVIEW->GetTime();
	timestr = GAMEAPPVIEW->m_stringFullTime.GetBuffer(0);
#else

	CTime time;
	time = GAMEAPPVIEW->GetServerTime();
	timestr = time.Format("%d-%m-%Y %H:%M:%S (GMT)");


#endif

	
	if(timestr)
	{
		// Display Login Time & Welcome Message //
		sprintf(_buffer,"*** %s joined the lobby at %s ***", m_player[plrIdx].name, timestr);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		g_concede_time = 0;

#ifndef MAIN_SERVER		


		// Reset Find Match Ctrl status
		GAMEAPPVIEW->m_findMatchCtrl->UpdateStatus(0);

		// Display IgnoreList Info //
		sprintf(_buffer,"*** %d player(s) on ignore. Replays, Ignore and other game files are saved: %s ***", GAMEAPPVIEW->m_ignorelst->m_total, g_build.savepath);
		GAMEAPPVIEW->Message(_buffer, 60); // personal message


		// GAMEAPPVIEW->ShowNews();

		// If this user has hacked the game, Log their Username, ip, sysid, macid //
		if(g_hacked) 
		{
			TemperSendData TemperData;
			// Send Message to Server, telling Server To Login this user's details //
			TemperData.buffer[0] = 0x7f;	// MSGCODE_GAME Game message.
			TemperData.buffer[1] = 104;		// MSGGAME_TEMPERLOG
			TemperData.buffer[2] = 0;		// 
			TemperData.buffer[3] = 0;		// 

			TemperData.m_plrSockIdx = m_player[plrIdx].playerSocketIdx;

			if(mainShell)
				{	// this client to send this msg to the server. (the server will decide what to do with it)
				mainShell->Client->SendToQueue((void *)&TemperData, sizeof(TemperSendData_typ));
				
				}
		}




#endif
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::ServerDataReceive(BOOL bTournament, BOOL bOrginalVersion, char handle[22])
{
	// update client with Server's info...
	g_bTournamentLobby = bTournament;
	g_bOriginalVersion = bOrginalVersion;
	g_serverHandle =  _T(handle);
	//strcpy(g_serverHandle.GetBuffer(0), "[TBot]");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//

void CLobby::LobbyReady(void)
{
	
	m_bLobbyReady = TRUE;

	::ShowWindow(g_hwndDX, SW_HIDE);

#ifndef MAIN_SERVER
	// navigate to webpages - 'once only!' //[NEW] CP
	static BOOL firstTime = TRUE;
	

	
	if(firstTime)
	{
		firstTime = FALSE;
	}
	
	// Show the Ticker Board and Banner URL
	GAMEAPPVIEW->m_htmlViewTickerBoard.Navigate2(g_URLlobbyTickerBoard, NULL, NULL);
	GAMEAPPVIEW->m_htmlViewBanner.Navigate2(g_URLBanner,NULL,NULL);
	

	GAMEAPPVIEW->m_htmlViewTickerBoard.ShowWindow(SW_SHOW);
	GAMEAPPVIEW->m_htmlViewBanner.ShowWindow(SW_SHOW);

	// Always reload following pages //
	char buf[128];
	int len;
	strcpy(buf, g_URLlobbyLeaderBoardA);
	len = strlen(buf);
	buf[len-5] = 0;
	sprintf(buf, "%smenu.html", buf);
	GAMEAPPVIEW->m_htmlViewLeaderBoardA.Navigate2(buf, NULL, NULL);

	strcpy(buf, g_URLlobbyLeaderBoardB);
	len = strlen(buf);
	buf[len-5] = 0;
	sprintf(buf, "%smenu.html", buf);
	GAMEAPPVIEW->m_htmlViewLeaderBoardB.Navigate2(buf, NULL, NULL);
	
	GAMEAPPVIEW->m_htmlViewMB.Navigate2(g_lobbyMessageBoard,NULL,NULL);



	// Initialize LobbTyVInfo values //
	g_LobbyTVInfo.lobbytablewatch = -1;			// Defaultly Not Watching any Table //
	strcpy(g_LobbyTVInfo.name1P, "Player 1");
	strcpy(g_LobbyTVInfo.name2P, "Player 2");
	g_LobbyTVInfo.score1P = 0;
	g_LobbyTVInfo.score2P = 0;
	g_LobbyTVInfo.break1P = 0;
	g_LobbyTVInfo.break2P = 0;
	g_LobbyTVInfo.bTurn1P = 10;


#endif //!MAIN_SERVER

	RefreshPlayerItem();
	//GAMEAPPVIEW->m_bootDlg.RefreshPlayerItem();
	//GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
	LobbyJoinMessage(GAMEAPPVIEW->m_playerSockIdx);


	// get my bio stats
	int plrIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);
	if(plrIdx!=-1)
		GAMEAPPVIEW->SendRequestPlayerDetails(plrIdx, FALSE); // Request stats, then get back to us.

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::Draw(void)
{
	// idle...
}


void CLobby::ReceiveTMatchTimerLeft(int timeRemaining, int type, char *str, int plrSockIdx, int tblIdx)
{ // FOR TOURNEY LOBBIES ONLY or Ladder Match

	
	// if (g_bTournamentLobby)
	{
		char _buffer[MAX_CHARS];

		if(type == 0 || type == 1)
		{
			int mins = timeRemaining/60;
			if(mins > 1)
				sprintf(_buffer,"WARNING! '%s' HAS %d MINUTES REMAINING TO START/CONTINUE MATCH...", str, mins);
			else
				sprintf(_buffer,"WARNING! '%s' HAS %d SECONDS REMAINING TO START/CONTINUE MATCH...", str, timeRemaining);
			
			GAMEAPPVIEW->Message(_buffer,10); // personal message
		}
		else if(type == 10)
		{
			int mins = timeRemaining/60;
			if(mins > 1)
				sprintf(_buffer,"WARNING! THIS TABLE WILL BE CLEARED IN %d MINUTES...", mins);
			else
				sprintf(_buffer,"WARNING! THIS TABLE WILL BE CLEARED IN %d SECONDS...", timeRemaining);
			
			GAMEAPPVIEW->Message(_buffer,10); // personal message
		}
	}
}

void CLobby::ReceiveTextMsg(char *str, int type, int plrSockIdx, int tblIdx, int lobbyAndTblIdx)
{
	//char buf[256];
	
	if(lobbyAndTblIdx != -1)
	{
		// this msg is intended for people in the lobby and at table #'lobbyAndTblIdx'
		int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if((myTblIdx != -1) && (myTblIdx != lobbyAndTblIdx))
			return;		
	}

	if(type == 0)
		GAMEAPPVIEW->Message(str, 10); // normal 'red' personal message
	else if(type == 1)
	{
		//sprintf(buf, "ANNOUNCEMENT --> %s", str);
		GAMEAPPVIEW->Message(str, 60); // 'cyan' message
	}
}

void CLobby::SetupLocalEPD(void) // Avatars/Personal Details/homepage url/etc. etc.
{
	#ifndef MAIN_SERVER
	g_extendedPlayerManagerLocal = new CExtendedPlayerManager(this);


	g_extendedPlayerDataLocal = new CExtendedPlayerData;
	EPDPersonalLayout epd_pl;

	epd_pl.avatar_idx = 0; // Get from avatar file.
	g_extendedPlayerDataLocal->AddPackage(&epd_pl,sizeof(EPDPersonalLayout),EPD_PERSONALLAYOUT);
	
//	pEpd_pl = (EPDPersonalLayout *) g_extendedPlayerDataLocal->GetPackage(EPD_PERSONALLAYOUT);

	#endif //!MAIN_SERVER
}

void CLobby::ReRouteEPD(unsigned char *buffer, int plrSockIdx)
{
	int *v;
	DWORD size;
	int tplrIdx;
	int tplrSockIdx;

	v = (int *) &buffer[12];
	size = *v;
	size+=24;

	v = (int *) &buffer[16]; // target_id
	tplrIdx = FindPlayerIdxFromId(*v);
	if(tplrIdx==-1) return; // Allow it
	else {
	tplrSockIdx = m_player[tplrIdx].playerSocketIdx;

	if(mainShell) // send to mainclient, he'll eat it.
		{
		mainShell->Server->SendToQueue(buffer, size, tplrSockIdx);
		}
	}
	


}

void CLobby::ReceiveEPD(unsigned char *buffer)
{
	/*
	char *realdata;
	int *v;
	int siz;
	int bytesleft;
	int realdata_off;
	DWORD tag;
	DWORD offset;
	DWORD size;
	char name[32];
	int plrIdx;
	CExtendedPlayerData *m_receivepackage;

	v = (int *) &buffer[4];
	tag = *v;
	v = (int *) &buffer[8];
	offset = *v;
	v = (int *) &buffer[12];
	size = *v;

	strcpy(name, (char *) &buffer[16]);

	realdata_off = 16+strlen(name)+1;
	realdata = (char *) &buffer[realdata_off];

	// ------------------ //

	plrIdx = FindPlayerIdx(name);
	if(plrIdx==-1)	{
					char buffer[80];
					sprintf(buffer,"Couldn't Find Player %s", name);

					return;
					}
	if(g_extendedPlayerData[plrIdx]==NULL) g_extendedPlayerData[plrIdx] = new CExtendedPlayerData;
	
	// [+] Upgrade for multiple additions - we need to know total size prior to this i.e. in the first sent packet - since offset = 0, we should know/the 2 free bytes!!! don't forget colin!
	g_extendedPlayerData[plrIdx]->AddPackage(realdata, size, tag);
	//g_extendedPlayerData[plrIdx]->AddPackageByInstallments(realdata, offset, size, DWORD tag, int ref)


	*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CLobby::LoggedInAlready(char *plrname)
{
	
	for(int n=0; n<MAX_LOBBY_PLAYERS; n++)
	if(m_player[n].status)
	if(m_player[n].textFormat != -2)	// Special Condition, we use when disconnecting a player, to tell us this immediately //
	if(!strcmpi(plrname, m_player[n].name))
	{
		// this player is already on the server (lobby)
		return TRUE;
	}

	// must be a new player
	return FALSE;
}

//usa
BOOL CLobby::LoggedInAlreadySYSIDMACID(char *name, DWORD sysID, char *ip, DWORD macID_U, DWORD macID_L)
{
	
	int i;

	for(int n=0; n<MAX_LOBBY_PLAYERS/*50*/; n++)
	if(m_player[n].status)
	if(sysID == m_player[n].sysID) // this computer's SYSID is already on server
	if(macID_U == m_player[n].macID_U) // this computer's MACID Upper is already on server
	if(macID_L == m_player[n].macID_L) // this computer's MACID Lower is already on server
	if(m_player[n].textFormat != -2)	// Special Condition, we use when disconnecting a player, to tell us this immediately //
	if(strcmpi(name, m_player[n].name) !=0) // Must be trying to login with a different account
	{
		BOOL ok;
		ok = FALSE;

		if(!m_player[n].bIsGuest) // Non Guests //
		{			
		//switch(GAMEAPPVIEW->m_moderator->GetType(m_player[n].name))
		switch(m_player[n].playertype)
			{
			case 0:ok = TRUE; // Admin
				   break;
			case 1:ok = TRUE; // Mod
				   break;
			case 2:ok = TRUE; // Senior Mods
				   break;
			case 3:ok = TRUE; // Mod2
				   break;
			}
		}

		if(!ok) 
		{
		// Check if IP matches //
		//char buffer[256];

		CString rPA = "Crap";
		UINT rPP = 1;
		int plrSockIdx;

		plrSockIdx = m_player[n].playerSocketIdx;


		// Get External IP (rPeerAddress, rPeerPort) ///////////////////////////////////////////////
		if(plrSockIdx!=-1)
		mainShell->Server->m_socketList[plrSockIdx]->GetPeerName(rPA, rPP);

		if(!strcmpi(rPA.GetBuffer(0), ip)) 
			{
			//sprintf(buffer,"%d:<%s:%s>TRUE",n, rPA.GetBuffer(0), ip);
			//ErrorMessage(m_parent, buffer);
			return TRUE; // sysID matches with a guest or member (admins/mods can log in multiple times)
			}
		else {
			//sprintf(buffer,"%d:<%s:%s>FALSE",n, rPA.GetBuffer(0), ip);
			//ErrorMessage(m_parent, buffer);
			}
		}
	}

	// must be a new player
	return FALSE;
}

void CLobby::HostSimEndInit(void)
{
	int n;

	int hostTbl = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		//m_player[n].type = Player_typ::TYPE_SIMEND;
		m_player[n].type = Player_typ::TYPE_ARRIVED;
	}
}

void CLobby::HostSimStartReceive(int playerSockIdx)
{	
	int plrIdx = GetPlayerIdx(playerSockIdx);
	if(plrIdx == -1) return;

	m_player[plrIdx].type = Player_typ::TYPE_SIM;
}


void CLobby::HostSimEndReceive(int playerSockIdx)
{
	int plrIdx = GetPlayerIdx(playerSockIdx);
	if(plrIdx == -1) return;

	m_player[plrIdx].type = Player_typ::TYPE_SIMEND;
}



BOOL CLobby::HostSimEndComplete(void)
{
	int n;

	int hostTbl = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	if(m_player[n].table == hostTbl)
	if(m_player[n].type == Player_typ::TYPE_SIM || m_player[n].type == Player_typ::TYPE_ARRIVED)
	{
		// someone hasn't finished their simulation
		return FALSE;
	}
	
	// everyone at this table has sent a SIMEND to the host.
	return TRUE;
}


void CLobby::SimEndWaiting(void)
{
	return; //16 march 2004
	// everyone at this table has sent a SIMEND to the host.
}


	
// Player functions
void CLobby::ClearPlayer(int idx)
{	
	ClearPlayer(&m_player[idx]);
}

void CLobby::ClearPlayer(Player *pPlr)
{	
	if(pPlr==NULL) return;

	InitPlayer(pPlr);
	pPlr->playerPort = -1;
	pPlr->playerIP[0] = 0;
	pPlr->playerLocalIP[0] = 0;
	pPlr->playerSocketIdx = -1;	

	pPlr->sysID = 0;
	pPlr->macID_L = 0;
	pPlr->macID_U = 0;
	pPlr->sip[0] = 0;
	pPlr->smacid[0] = 0;
	pPlr->ssysid[0] = 0;
}

void CLobby::ClearPlayerNonRating(Player *pPlr)
{
	if(pPlr==NULL) return;

//	pPlr->wins = 0;
//	pPlr->losses = 0;
//	pPlr->streak = 0;
	
	pPlr->_p50s = 0;
	pPlr->_p60s = 0;
	pPlr->_p70s = 0;
	pPlr->_p80s = 0;
	pPlr->_p90s = 0;
	pPlr->_p100s = 0;
	pPlr->pHighBreak = 0;
	pPlr->pMax147s = 0;

	///////////
	pPlr->_50s = 0;
	pPlr->_60s = 0;
	pPlr->_70s = 0;
	pPlr->_80s = 0;
	pPlr->_90s = 0;
	pPlr->_100s = 0;
	pPlr->HighBreak = 0;
	pPlr->Max147s = 0;
			
	
	//pPlr->pBioAverageShotTime = 0;
	//pPlr->pBioAveragePosition = 0;
	//pPlr->pBioAverageSafety = 0;
	//////////////
	//pPlr->bioAverageShotTime = 0;
	//pPlr->bioAveragePosition = 0;
	//pPlr->bioAverageSafety = 0;
}


void CLobby::AddPlayerNonRating(Player *pDPlr, Player *pSPlr)
{
	if(pDPlr==NULL) return;
	if(pSPlr==NULL) return;

	/*
	pDPlr->wins += pSPlr->wins;
	pDPlr->losses = += pSPlr->wins;
		
	if( (pDPlr->streak > 0) && (pSPlr->streak > 0) )
		pDPlr->streak++;
	else if( (pDPlr->streak < 0) && (pSPlr->streak < 0) )
		pDPlr->streak--;
	else 
		pDPlr->streak = pSPlr->streak;
	*/
	
	pDPlr->_p50s += pSPlr->_p50s;
	pDPlr->_p60s += pSPlr->_p60s;
	pDPlr->_p70s += pSPlr->_p70s;
	pDPlr->_p80s += pSPlr->_p80s;
	pDPlr->_p90s += pSPlr->_p90s;
	pDPlr->_p100s += pSPlr->_p100s;
	pDPlr->pHighBreak += pSPlr->pHighBreak;
	pDPlr->pMax147s += pSPlr->pMax147s;
	///////////
	pDPlr->_50s += pSPlr->_50s;
	pDPlr->_60s += pSPlr->_60s;
	pDPlr->_70s += pSPlr->_70s;
	pDPlr->_80s += pSPlr->_80s;
	pDPlr->_90s += pSPlr->_90s;
	pDPlr->_100s += pSPlr->_100s;
	pDPlr->HighBreak += pSPlr->HighBreak;
	pDPlr->Max147s += pSPlr->Max147s;
}


void CLobby::AddPlayer(Player *plr)
{
	if(plr==NULL) return;
	
	int n;

	// see if this player already exists
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		if(!strcmpi(plr->name, m_player[n].name))
		{
			// player has been added previously
			AddPlayerData(n, plr);
			return;

		}
	}

	// new player, find a new slot...
	//int count = 0;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(!m_player[n].status)
	{
		// reserve slot [0] for myself
		if(n==0)
		if(GAMEAPPVIEW->m_playerSockIdx != plr->playerSocketIdx)
			continue;

		AddPlayerData(n, plr);
		return;
		//count++;
		//if(count == 20) return;
	}
}


void CLobby::AddPlayerData(int n, Player *plr)
{
	if(plr==NULL) return;

	if(n<0 || n>=MAX_LOBBY_PLAYERS) return;
	
	//BOOL bIgnore = m_player[n].bIgnore;

	memcpy(&m_player[n], plr, sizeof(Player_typ));
	
	//m_player[n].bIgnore = bIgnore;

	m_player[n].status = 1;
//	m_player[n].idx = n;
	m_player[n].getAllPlayers = 0; // for server only (tells server to start sending all player details)

	// add player details to player list
	//AddPlayerItem(m_player[n].name, m_player[n].rating, m_player[n].table+1);

	//NEXTFIX 
	
	/*
	GAMEAPPVIEW->m_inviteDlg.DestroyWindow();
	GAMEAPPVIEW->m_inviteDlg.Create(IDD_INVITE);
	GAMEAPPVIEW->m_playerListDlg.DestroyWindow();
	GAMEAPPVIEW->m_playerListDlg.Create(IDD_PLAYERLIST);
	*/
	

	RefreshPlayerItem();
	GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
	GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();
	
	

	/*
	// set and broadcast player profile (nick for chat msgs)
	if(GAMEAPPVIEW->m_playerSockIdx == m_player[n].playerSocketIdx) // myself added to player list
	{
		if(mainShell)
		{
			mainShell->SetNick(m_player[n].name);
			mainShell->SendProfile();
		}
	}
	*/
}


// This function seems to be called up twice when a player disconnects (during a game).. could create a timing issue //
void CLobby::RemovePlayer(int playerSocketIdx)
{
	// find player from socket idx
	int n;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		//char buff[80];
		//sprintf(buff,"m_plr[%d] = %d, plr = %d", n, m_player[n].playerSocketIdx, playerSocketIdx);
		//MessageBox(0,buff,"",MB_OK);

		if(m_player[n].playerSocketIdx == playerSocketIdx)
		{
			//char buff[80];
			//sprintf(buff,"n = %d",n);
			//MessageBox(0,buff,"",MB_OK);

			ClearPlayer(n);
			RefreshPlayerItem();
			GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
			GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();
			break;
		}
		if(g_extendedPlayerData[n])
			{
			delete g_extendedPlayerData[n];
			g_extendedPlayerData[n] = NULL;
			}
	}
}

int CLobby::GetPlayerIdx(int playerSocketIdx)
{
	if(playerSocketIdx == -1) return -1;

	int n;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		if(m_player[n].playerSocketIdx == playerSocketIdx)	
			return n;
	}

	return -1;
}

int CLobby::FindPlayerIdxFromId(DWORD id)
{
	int n;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
	if(m_player[n].id==id) return n;
	}
	return -1;
}

char *CLobby::FindPlayerNameFromId(DWORD id)
{
	int idx = FindPlayerIdxFromId(id);
	if(idx==-1) return ""; // blank i.e. no name
	return m_player[idx].name;
}

int CLobby::FindPlayerIdx(CString name)
{
	int n;
	CString plrName ;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		plrName = m_player[n].name;
		//if(plrName == name) return n;
		if(plrName.CompareNoCase(name) == 0) return n;
	}
	return -1;
}

int CLobby::FindPlayerSockIdx(CString name)
{
	int n;
	CString plrName;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		plrName = m_player[n].name;
		//if(plrName == name) return m_player[n].playerSocketIdx;
		if(plrName.CompareNoCase(name) == 0) return m_player[n].playerSocketIdx;
	}
	return -1;
}

int CLobby::FindPlayerSockIdxi(CString name)
{
	int n;
	CString plrName;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		if(!strcmpi(m_player[n].name, name.GetBuffer(0))) return m_player[n].playerSocketIdx;
		//plrName = m_player[n].name;
		//if(plrName == name) return m_player[n].playerSocketIdx;		
	}
	return -1;
}



////////// Ignore the Socket Idx parament //
int CLobby::FindPlayerSockIdx_Ignore(CString name, int IgnoreIdx)
{
	int n;
	CString plrName;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	if((m_player[n].playerSocketIdx)!=IgnoreIdx)
	{
		plrName = m_player[n].name;
		if(plrName.CompareNoCase(name) == 0) return m_player[n].playerSocketIdx;
	}
	return -1;
}

int CLobby::FindPlayerSockIdxi_Ignore(CString name, int IgnoreIdx)
{
	int n;
	CString plrName;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	if((m_player[n].playerSocketIdx)!=IgnoreIdx)
	{
		if(!strcmpi(m_player[n].name, name.GetBuffer(0))) return m_player[n].playerSocketIdx;
	}
	return -1;
}


int CLobby::GetPlayerLevel(int idx)
{
	if(idx<0 || idx>=MAX_LOBBY_PLAYERS) return -1;

	if(m_player[idx].status)
		return m_player[idx].level;
	else return -1;
}

int CLobby::GetPlayerLevelProgress(int idx)
{
	if(idx<0 || idx>=MAX_LOBBY_PLAYERS) return -1;

	if(m_player[idx].status)
		return m_player[idx].level_progress;
	else return -1;
}


void CLobby::SetPlayerLevel(int idx, int level)
{
	if(idx<0 || idx>=MAX_LOBBY_PLAYERS) return;

	if(m_player[idx].status)
	{
		m_player[idx].level = level;
	}
	
}

void CLobby::SetPlayerLevelProgress(int idx, int level_progress)
{
	if(idx<0 || idx>=MAX_LOBBY_PLAYERS) return;

	if(m_player[idx].status)
	{
		m_player[idx].level_progress = level_progress;
	}
}



int CLobby::GetPlayerRating(int idx)
{
	if(idx<0 || idx>=MAX_LOBBY_PLAYERS) return -1;

	if(m_player[idx].status)
		return m_player[idx].rating;
	else return -1;
}


int CLobby::GetPlayerSeat(int plrSockIdx)
{
	if(plrSockIdx==-1) return 0; // not at table (invalid)

	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);

	if(tblIdx == -1) return 0; // not at table

	if(m_table[tblIdx].player1SockIdx == plrSockIdx) return 1; // P1
	if(m_table[tblIdx].player2SockIdx == plrSockIdx) return 2; // P2
	if(m_table[tblIdx].player1bSockIdx == plrSockIdx) return 3; // P1b
	if(m_table[tblIdx].player2bSockIdx == plrSockIdx) return 4; // P2b

	return 5; // a watcher
}


void CLobby::ProcessPlayers(void)
{

}

int CLobby::CountPlayers(void)
{
	int n;
	int count = 0;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status) count++;
	
	return count;
}

// Table functions
void CLobby::ClearTable(int idx)
{

	if(idx<0 || idx>=MAX_TABLEITEMS) return; // NEW 4/FEB/2004

	m_table[idx].status = 0;
	m_table[idx].idx = -1;
	
	m_table[idx].hostSockIdx = -1;	
	m_table[idx].player1SockIdx = -1;
	m_table[idx].player2SockIdx = -1;
	m_table[idx].player1bSockIdx = -1;
	m_table[idx].player2bSockIdx = -1;
	for(int n=0;n<MAX_TABLE_PLAYERS;n++)
		m_table[idx].playersSockIdx[n] = -1;
	//m_table[idx].playersSockIdxN = 0;

	m_table[idx].P1StartAtTable = -1;
	m_table[idx].P2StartAtTable = -1;
	m_table[idx].P1StartName[0] = 0;
	m_table[idx].P2StartName[0] = 0;
	m_table[idx].P1bStartAtTable = -1;
	m_table[idx].P2bStartAtTable = -1;
	m_table[idx].P1bStartName[0] = 0;
	m_table[idx].P2bStartName[0] = 0;

	m_table[idx].bGameStarted = FALSE;
	m_table[idx].bReRackRequest = FALSE;
	m_table[idx].bSwitchBreakTo1P = TRUE; // 1P to go first in first frame
	m_table[idx].racksWon1P = 0;
	m_table[idx].racksWon2P = 0;

	m_table[idx].startScore1P = 0;
	m_table[idx].startScore2P = 0;
	m_table[idx].score1P = 0;
	m_table[idx].score2P = 0;
	m_table[idx].break1P = 0;
	m_table[idx].break2P = 0;
		
	m_table[idx].m_lostConn1P = 0;
	m_table[idx].m_lostConn2P = 0;

	#ifdef CUEERROR_DEFAULTON
	m_table[idx].cueError = 1; // start on semi-pro
	#else
	m_table[idx].cueError = 0;
	#endif
		
	m_table[idx].bRated = TRUE;
	m_table[idx].type = Table_typ::TYPE_PUBLIC;
	m_table[idx].bMissRule = FALSE;

	m_table[idx].shotTime = 20;
	m_table[idx].addTimer = 0;
	m_table[idx].doubles = 0;
	// Make sure Doubles Buttons are not showing //
	if(g_gameAppView)
	{
		//g_gameAppView->m_sit1PButtonB.ShowWindow(SW_HIDE);
		//g_gameAppView->m_sit2PButtonB.ShowWindow(SW_HIDE);
	}
				

	if(g_build.gameType == GAMETYPE_POOL) m_table[idx].tableColour = 0; // blue table default
	else if(g_build.gameType == GAMETYPE_SNOOKER) m_table[idx].tableColour = 2; // green table default
	m_table[idx].gameType = 0;

	m_table[idx].raceTo = 0;	// 0 => race to 1
	m_table[idx].whoBreaks = 2;	// 0 => 1P breaks 'Always'

	m_table[idx].ballSize = 0;	// 0 => Normal {0 = Normal, 1 = BIG, 2 = small}
	m_table[idx].rackType = 0;
	if(g_build.gameType == GAMETYPE_POOL) m_table[idx].tableType = 1;
		else m_table[idx].tableType = 1;
	m_table[idx].proTable = 0;

	m_table[idx].bHostComplete = FALSE;

	m_table[idx].bLockSeats = FALSE;	
	m_table[idx].bCanRerack = TRUE;
	m_table[idx].bLadderMatch = FALSE;

	TableLostConnectionFlag[idx] = 0;

	m_table[idx].bLadderMatch = FALSE;				

	// check if tournament table
	if(g_bTournamentLobby)
	{
		m_table[idx].tournament = 1;					// make this table a tourney table
		m_table[idx].type = Table_typ::TYPE_PRIVATE;	// override table type
	}
	else
		m_table[idx].tournament = 0;

	g_tournament.ClearMatchTableNo(idx); // this is redundant for non-tournament lobbies! (should have no effect)
	

	//m_table[idx].bSimEndMsgBalls = FALSE;
	m_table[idx].nSimEndMsgBalls = 0;

	UpdateTableItemButtons(idx);

	
	AddTableOptionsItem(idx);

	
}

void CLobby::ClearTableReceive(int tblIdx)
{
	#ifdef MAIN_SERVER // for tournament lobbies only...

	bool bLadderMatch = false;

	if (tblIdx != -1)
	{
		bLadderMatch = m_table[tblIdx].bLadderMatch;
	}

	if (g_bTournamentLobby || bLadderMatch)
	{
		// If Ladder Match, then update m_player structure for any players involved in match, to not searching for match
		int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
		if (matchNo != -1)
		{
			char *name1P = g_tournament.m_match[matchNo].m_name1P;
			char *name2P = g_tournament.m_match[matchNo].m_name2P;

			int pIdx;
			pIdx = FindPlayerIdx(name1P);
			if (pIdx != -1)
			{
				m_player[pIdx].searchgamestatus &= 0xFFFFFFF0; // Set last 4 bits to 0, which is our searchgamestatus

				// Cancel this player's search
				g_gameAppView->SendClientFindMatchStatus(m_player[pIdx].playerSocketIdx, 0);
				
			}

			pIdx = FindPlayerIdx(name2P);
			if (pIdx != -1)
			{
				m_player[pIdx].searchgamestatus &= 0xFFFFFFF0; // Set last 4 bits to 0, which is our searchgamestatus

				// Cancel this player's search
				g_gameAppView->SendClientFindMatchStatus(m_player[pIdx].playerSocketIdx, 0);
			}

		}


		// clear match logically from server's list
		g_tournament.ClearMatchTable(tblIdx);
	}
	#endif


	// (special admin command) : e.g. "/cleartable 4" in lobby
	ClearTable(tblIdx);
	AddTableItem(tblIdx, 1);
	AddTableItem(tblIdx, 2);		
	AddTableItem(tblIdx, 3);
	AddTableItem(tblIdx, 4);		
	AddTableItem(tblIdx, 0);
	
	if(GAMEAPPVIEW->m_playerSockIdx != -1)
	{
		int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
		if(myTblIdx == tblIdx)
		{
			GAMEAPPVIEW->ExitTable2Lobby();
			// send main server message that we've left the table (destroyed the game shell)
			GAMEAPPVIEW->SendPlayerExit(FALSE, GAMEAPPVIEW->m_playerSockIdx);
		}
	}
	 
	UpdateTableItemButtons(tblIdx); //!! // note: these two lines are also in 'ClearTable()' above.
	AddTableOptionsItem(tblIdx);
}

int CLobby::CountPlayersAtTable(int idx)
{
	if(idx<0 || idx>=MAX_TABLEITEMS) return 0; // NEW 15/FEB/2004

	int count = 0;

	if(m_table[idx].player1SockIdx != -1) count++;
	if(m_table[idx].player2SockIdx != -1) count++;
	if(m_table[idx].player1bSockIdx != -1) count++;
	if(m_table[idx].player2bSockIdx != -1) count++;
	
	for(int n=0;n<MAX_TABLE_PLAYERS;n++)
		if(m_table[idx].playersSockIdx[n] != -1) count++;
	
	return count;
}

BOOL CLobby::EmptyTable(int idx)
{
	if(idx<0 || idx>=MAX_TABLEITEMS) return FALSE; // NEW 15/FEB/2004

	if(!m_table[idx].status) return TRUE;
	else return FALSE;
}

void CLobby::TakeTable(int idx)
{
	if(idx<0 || idx>=MAX_TABLEITEMS) return; // NEW 15/FEB/2004
	
	m_table[idx].status = 1;
	m_table[idx].idx = idx; // NEW	
}


BOOL CLobby::SeatedAtTable(int plrSockIdx)
{
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);

	if(tblIdx == -1) return FALSE;

	if(m_table[tblIdx].player1SockIdx == plrSockIdx) return TRUE;
	if(m_table[tblIdx].player2SockIdx == plrSockIdx) return TRUE;
	if(m_table[tblIdx].player1bSockIdx == plrSockIdx) return TRUE;
	if(m_table[tblIdx].player2bSockIdx == plrSockIdx) return TRUE;
	
	return FALSE;
}

BOOL CLobby::SeatedAtTableIdx(int plrSockIdx, int tblIdx)
{		
	if(tblIdx == -1) return FALSE;

	int plrTblIdx = GetTableIdxFromPlayer(plrSockIdx);	
	if(plrTblIdx != tblIdx) return FALSE;

	if(m_table[tblIdx].player1SockIdx == plrSockIdx) return TRUE;
	if(m_table[tblIdx].player2SockIdx == plrSockIdx) return TRUE;
	if(m_table[tblIdx].player1bSockIdx == plrSockIdx) return TRUE;
	if(m_table[tblIdx].player2bSockIdx == plrSockIdx) return TRUE;
	
	return FALSE;
}

BOOL CLobby::PressedStartAtTable(int plrSockIdx)
{
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);

	if(tblIdx == -1) return FALSE;
	
	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx==-1) return FALSE;
	if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1StartName)) return TRUE;
	else if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P2StartName)) return TRUE;
	else return FALSE;


	return FALSE;
}

BOOL CLobby::BothSeatsTaken(int plrSockIdx)
{
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);

	if(tblIdx == -1) return FALSE;

	if(m_table[tblIdx].player1SockIdx != -1 && m_table[tblIdx].player2SockIdx != -1) return TRUE;
	
	return FALSE;
}


BOOL CLobby::PressedStartReceive(int plrSockIdx)
{
	if(plrSockIdx == -1) return FALSE;
	
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return FALSE;

	char ebuff[256];

	// have we already started a frame?
	if(m_table[tblIdx].bGameStarted) return FALSE;

	//////////////////////////////////////////////////////////////////////
	// if this is a tourney table, dont allow players to start another game, if they've just played on this table!
	if(m_table[tblIdx].tournament == 3) return FALSE;
	//////////////////////////////////////////////////////////////////////

	if(m_table[tblIdx].status)
	{
		// ensure both 1P & 2P are seated if Two Player Game (Singles)
		if(!m_table[tblIdx].doubles)
		{
			if((m_table[tblIdx].player1SockIdx != -1) && (m_table[tblIdx].player2SockIdx != -1))
			{	
				// Get the name of the player who pressed Start - "m_player[plrIdx].name" //
				int plrIdx = GetPlayerIdx(plrSockIdx);
				if(plrIdx==-1) return FALSE;
						
				// who pressed 'start' - 1P or 2P
				if(m_table[tblIdx].player1SockIdx == plrSockIdx) 
					{
					strcpy(m_table[tblIdx].P1StartName, m_player[plrIdx].name);
					m_table[tblIdx].P1StartAtTable = plrSockIdx;
					}
				if(m_table[tblIdx].player2SockIdx == plrSockIdx) 
					{
					strcpy(m_table[tblIdx].P2StartName, m_player[plrIdx].name);
					m_table[tblIdx].P2StartAtTable = plrSockIdx;
					}
			}
		}
		// If Four Player Game (Doubles), Ensure that all 4 players are seated //
		else
		{
			if(	(m_table[tblIdx].player1SockIdx != -1) && (m_table[tblIdx].player2SockIdx != -1) &&
				(m_table[tblIdx].player1bSockIdx != -1) && (m_table[tblIdx].player2bSockIdx != -1) )
			{	
				// Get the name of the player who pressed Start - "m_player[plrIdx].name" //
				int plrIdx = GetPlayerIdx(plrSockIdx);
				if(plrIdx==-1) return FALSE;
						
				// who pressed 'start' - 1P, 2P, 3P or 4P
				if(m_table[tblIdx].player1SockIdx == plrSockIdx) 
					{
					strcpy(m_table[tblIdx].P1StartName, m_player[plrIdx].name);
					m_table[tblIdx].P1StartAtTable = plrSockIdx;
					}
				else if(m_table[tblIdx].player2SockIdx == plrSockIdx) 
					{
					strcpy(m_table[tblIdx].P2StartName, m_player[plrIdx].name);
					m_table[tblIdx].P2StartAtTable = plrSockIdx;
					}
				else if(m_table[tblIdx].player1bSockIdx == plrSockIdx) 
					{
					strcpy(m_table[tblIdx].P1bStartName, m_player[plrIdx].name);
					m_table[tblIdx].P1bStartAtTable = plrSockIdx;
					}
				else if(m_table[tblIdx].player2bSockIdx == plrSockIdx) 
					{
					strcpy(m_table[tblIdx].P2bStartName, m_player[plrIdx].name);
					m_table[tblIdx].P2bStartAtTable = plrSockIdx;
					}
			}

		}
		

	}

	char AllPressedStart = 0;

	if(!m_table[tblIdx].doubles)
		{
		if((m_table[tblIdx].P1StartAtTable != -1) && (m_table[tblIdx].P2StartAtTable != -1)) AllPressedStart = 1;
		}
	else{
		if(	(m_table[tblIdx].P1StartAtTable != -1) && (m_table[tblIdx].P2StartAtTable != -1) &&
			(m_table[tblIdx].P1bStartAtTable != -1) && (m_table[tblIdx].P2bStartAtTable != -1) ) AllPressedStart = 1;
		}


	// If we are watching This Table, then Update the Lobby TV //
	GAMEAPPVIEW->UpdateLobbyTableWatch(tblIdx);

	// Have all players pressed 'Start' ? //
	if(AllPressedStart)
	{
		m_table[tblIdx].bGameStarted = TRUE;

		// Match Sure Ladder Match search is reset (if we've started a game)
		int pIdx;
		pIdx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);
		if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0; // Set last 4 bits to 0, which is our searchgamestatus
		pIdx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);
		if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0;
		pIdx = GetPlayerIdx(m_table[tblIdx].player1bSockIdx);
		if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0;
		pIdx = GetPlayerIdx(m_table[tblIdx].player2bSockIdx);
		if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0;
		
		// If we are one of the players who started their match, cancel the Find Match Search
		if (GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].player1SockIdx || 
			GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].player2SockIdx)
		if (g_gameAppView)
		if (g_gameAppView->m_findMatchCtrl)
		{
			g_gameAppView->m_findMatchCtrl->UpdateStatus(0);
		}
		
	
		if(!g_bTournamentLobby) // note: tourney server will have already sent an "UpdateTableHostDetails()", so setting racksWon1P&2P
		{
			m_table[tblIdx].racksWon1P = 0;
			m_table[tblIdx].racksWon2P = 0;
			m_table[tblIdx].startScore1P = 0; // we shdn't need these lines, but let's be safe.
			m_table[tblIdx].startScore2P = 0; // ...
		}
		m_table[tblIdx].score1P = m_table[tblIdx].startScore1P;
		m_table[tblIdx].score2P = m_table[tblIdx].startScore2P;

#ifndef MAIN_SERVER
		// Update LastTurn variable for clients //
		g_bLastTurn = GAMEAPPVIEW->m_snooker->m_bTurn1P;
#endif


		#ifdef MAIN_SERVER	
		if(g_bTournamentLobby)
			{
			// reset the player disconnect counts at start of frame/rack
			int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
			if(matchNo != -1)
				{
				g_tournament.m_match[matchNo].m_lostConn1P = 0;
				g_tournament.m_match[matchNo].m_lostConn2P = 0;
				}
			}
		#endif
		
		// P1 records the two players playing
		if(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].player1SockIdx)
			{
			int myIdx = GetPlayerIdx(m_table[tblIdx].P1StartAtTable);	// get player that clicked start (1P)
			if(myIdx != -1)
				{
				memcpy(&m_myself, &m_player[myIdx], sizeof(Player_typ));
				}
			int oppIdx = GetPlayerIdx(m_table[tblIdx].P2StartAtTable);	// get player that clicked start (2P)
			if(oppIdx != -1)
				{
				memcpy(&m_opponent, &m_player[oppIdx], sizeof(Player_typ));
				}
			}

		// P2 records the two players playing
		if(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].player2SockIdx)
			{
			int myIdx = GetPlayerIdx(m_table[tblIdx].P2StartAtTable);	// get player that clicked start (2P)
			if(myIdx != -1)
				{
				memcpy(&m_myself, &m_player[myIdx], sizeof(Player_typ));
				}
			int oppIdx = GetPlayerIdx(m_table[tblIdx].P1StartAtTable);	// get player that clicked start (1P)
			if(oppIdx != -1)
				{
				memcpy(&m_opponent, &m_player[oppIdx], sizeof(Player_typ));
				}
			}

		// check i'm on the same table before restarting table [[BUGFIX!] FOR A bugfix]
		int myTableIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if(myTableIdx == -1) return FALSE;
		if(myTableIdx != tblIdx) return FALSE;
		
		// set initial bio averages for start of frame
		if(m_table[tblIdx].cueError == 1)
		{			
			g_bioAverageShotTime = g_sbioAverageShotTime;
			g_bioAveragePosition = g_sbioAveragePosition;
			g_bioAverageSafety = g_sbioAverageSafety;			
		}
		else if(m_table[tblIdx].cueError == 2)
		{
			g_bioAverageShotTime = g_pbioAverageShotTime;
			g_bioAveragePosition = g_pbioAveragePosition;
			g_bioAverageSafety = g_pbioAverageSafety;			
		}

		GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;
		GAMEAPPVIEW->m_snooker->m_bShotTimeUp = FALSE;

		if(g_build.gameType == GAMETYPE_POOL)
		if(m_table[tblIdx].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
		{
				GAMEAPPVIEW->m_tableSetupDlg.DestroyWindow();
		}

		if(g_build.gameType == GAMETYPE_SNOOKER)
		if(m_table[tblIdx].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
		{
				GAMEAPPVIEW->m_tableSetupDlg2.DestroyWindow();
		}

		
		// Tell Clients that 3 Disconnects is a loss of a Rack / Frame
		char buff[256];
		#ifdef THE_POOL_CLUB
		sprintf(buff, "Note: 3 Disconnections is a loss of a Rack!");
		#else
		sprintf(buff, "Note: 3 Disconnections is a loss of a Frame!");
		#endif
		GAMEAPPVIEW->Message(buff,10); // personal message
	
		// *NEW - disable stand button during game [standdisable]
		//GAMEAPPVIEW->m_standButton.EnableWindow(FALSE);
		GAMEAPPVIEW->m_standButtonEnabled = FALSE;


		// only return TRUE if at this particular table
		return TRUE;
	}

	return FALSE;
}


BOOL CLobby::PlayingAGame(int plrSockIdx)
{
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1) return FALSE;

	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx==-1) return FALSE;
	

	// Singles Check for playign a Game //
	if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1StartName)) return TRUE;
	if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P2StartName)) return TRUE;
	
	// If Doubles Game, then we also need to check P1b and P2b Seats //
	if(m_table[tblIdx].doubles)
		{
		if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1bStartName)) return TRUE;
		if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P2bStartName)) return TRUE;
		}
	
	return FALSE;
}

BOOL CLobby::IsPlayerWinner(int plrSockIdx)
{
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);

	if(tblIdx != -1) // check if on a valid table
		{
		int plrIdx = GetPlayerIdx(plrSockIdx);
		if(plrIdx==-1) return FALSE;
		
		// is this player the winner?...
		if( (GAMEAPPVIEW->m_snooker->m_gameOver == 1) && !(strcmpi(m_player[plrIdx].name, m_table[tblIdx].P2StartName)) ) 
			{
			if(GAMEAPPVIEW->m_snooker->m_gameOverIWon >= 0)	return TRUE;
			}

		if( (GAMEAPPVIEW->m_snooker->m_gameOver == 2) && !(strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1StartName)) )
			{
			if(GAMEAPPVIEW->m_snooker->m_gameOverIWon >= 0)	return TRUE;
			}
		
		// if i'm playing in this game, and i won before
		if( (plrSockIdx == m_table[tblIdx].P1StartAtTable) || (plrSockIdx == m_table[tblIdx].P2StartAtTable) )
			if(GAMEAPPVIEW->m_snooker->m_gameOverIWon == 1) // ENSURE ONLY PLAYER AT THIS TABLE CAN BE 'TRUE' HERE!
				return TRUE;
	}

	return FALSE;
}




// Only Used for Disconnections //
// To Notify the Players
void CLobby::ReceivePlayerLostConnection(int playerSockIdx, char name[22], int tblIdx)
{
	if(playerSockIdx == -1) return;
	if(tblIdx == -1) return;

	// Tell everyone to clear this player's profile //
	RemovePlayer(playerSockIdx);


	// Only Do following code if game is still in progress //
	// It's possible for the game to be over now, if the player disconnected just after GAME_OVER message was issued //
	// Which would cause a bug if the following code was then run //
	if(m_table[tblIdx].bGameStarted)
		{
		m_table[tblIdx].bLockSeats = TRUE; // lock seats, so nobody else can use them

		// Make the Join Button for any seats that are showing Diabled //
		// This will let others in the Lobby Know that the Player has lost connection //
		GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*4+2, FALSE);
		GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*4+3, FALSE);
	
		TableLostConnectionFlag[tblIdx] = 1;
		}
	
	int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(myTblIdx != tblIdx) return;

	// If we've started a frame
	if(m_table[tblIdx].bGameStarted)
		{
		// Notify the player that his/her opponent has just disconnected //
		char _buffer[MAX_CHARS];
		sprintf(_buffer,"*** %s has lost connection! ***", name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		if(g_bTournamentLobby) sprintf(_buffer,"(%s has 5 minutes to reconnect, before server forces forfeit!)", name);
		else sprintf(_buffer,"(%s has 2 minutes to reconnect, before opponent can force forfeit)", name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message

		// Update m_PLCname //
		strcpy(m_PLCname, name);
		// Start lost connection timer, m_PLCwait
		// if i'm the opponent that is waiting, then start timer
		if(PlayingAGame(GAMEAPPVIEW->m_playerSockIdx)) m_PLCwait = g_cyc; // start timer
		}
	
}


// ReConnection Code, for Players in games //
// Update The Server and Both Player's game details //
void CLobby::ReceivePlayerConnection(int playerSockIdx, int gameOver, int gameOverIWon, int playerSockIdx2)
{
	// playerSockIdx = Player who has returned to table
	
	int n;
	int tblIdx = GetTableIdxFromPlayer(playerSockIdx);
	if(tblIdx == -1) return;
	
	int plrIdx = GetPlayerIdx(playerSockIdx);
	if(plrIdx==-1) return;
	
	
	// Update StartAtTable Socket
	if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1StartName)) m_table[tblIdx].P1StartAtTable = playerSockIdx;
	if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P2StartName)) m_table[tblIdx].P2StartAtTable = playerSockIdx;
	
	// UnLock Seats //
	m_table[tblIdx].bLockSeats = FALSE;
	TableLostConnectionFlag[tblIdx] = 0;

	// Enable the Join Buttons again, so people can use them when needed //
	GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*4+2, TRUE);
	GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*4+3, TRUE);
	

	// only let the person that has just rejoined, do the following		
	if(playerSockIdx != GAMEAPPVIEW->m_playerSockIdx) return;

	
	// Only for the Players, Update m_myself and m_opponent structues //
#ifndef MAIN_SERVER
	if(m_table[tblIdx].bGameStarted)
		{			
		int myIdx = GetPlayerIdx(playerSockIdx);
		if(myIdx != -1)
			{
			memcpy(&m_myself, &m_player[myIdx], sizeof(Player_typ));
			}


		int oppIdx = GetPlayerIdx(playerSockIdx2);
		if(oppIdx != -1)
		{
			memcpy(&m_opponent, &m_player[oppIdx], sizeof(Player_typ));
		}
		
		// did the sender (i.e. my waiting opponent) win already?
		GAMEAPPVIEW->m_snooker->m_gameOver = gameOver;
		
		if(gameOverIWon == 1) GAMEAPPVIEW->m_snooker->m_gameOverIWon = -1;	// I lost before I loss connection
		else if(gameOverIWon == -1)	GAMEAPPVIEW->m_snooker->m_gameOverIWon = 1;		// I won before I loss connection
		else GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;		// nobody has won yet

		}
#endif //MAIN_SERVER
}



void CLobby::ReceivePlayerTextFormat(int playerSockIdx, char name[22], int textFormat)
{
	int plrIdx = GetPlayerIdx(playerSockIdx);
	if(plrIdx == -1) return;

	if(m_player[plrIdx].status)
	if( strcmpi(m_player[plrIdx].name, name) == 0 )
		m_player[plrIdx].textFormat = textFormat;
}

BOOL CLobby::PressedConcedeReceive(int plrSockIdx, BOOL bAndExit, int tblIdx)
{

	if(tblIdx < 0 || tblIdx >= MAX_LOBBY_TABLES) return false;

	int matchNo;

	/////////////////////////
/*	#ifdef MAIN_SERVER
	if(bAndExit)
	{
		// exit table
		//ExitTable(plrSockIdx);
		GAMEAPPVIEW->SendPlayerExit(FALSE, plrSockIdx);
	}
	#endif // MAIN_SERVER
	/////////////////////////*/


	// Ladder Match, so 

	///////////////
	// if player has clicked 'exit' and chosen to concede - end the 'raceTo' type game
	// for all to update, including server!!
	if (bAndExit)
	{
		if (tblIdx != -1)
		if (m_table[tblIdx].raceTo > 0) // note: 0 => 1 rack (if we are racing to 2 or more racks)
		{
			// *TODO - should change GetPlayerSeat(...) here and further down to 'm_table[tblIdx].P1StartAtTable'
			if (GetPlayerSeat(plrSockIdx) == 1)
				m_table[tblIdx].racksWon2P = m_table[tblIdx].raceTo;
			else if (GetPlayerSeat(plrSockIdx) == 2)
				m_table[tblIdx].racksWon1P = m_table[tblIdx].raceTo;
			if (GetPlayerSeat(plrSockIdx) == 3)
				m_table[tblIdx].racksWon2P = m_table[tblIdx].raceTo;
			else if (GetPlayerSeat(plrSockIdx) == 4)
				m_table[tblIdx].racksWon1P = m_table[tblIdx].raceTo;
		}

	#ifdef MAIN_SERVER
		if (g_bTournamentLobby)
		{
			int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
			g_tournament.m_match[matchNo].m_bStopTiming = TRUE; // Tell Tourney Timers to Stop for this match //
			g_tournament.m_match[matchNo].m_bMatchConceded = TRUE; // conceded match (so we don't invite player that conceded anymore) not really needed, used for ladder
		}
	#endif
	}

#ifdef MAIN_SERVER

	// If this is a ladder match, check if the match is now over due to this concede.. If so, set the bMatchConceded flag
	// This is used to stop inviting players after they concede and leave the table...
	if (m_table[tblIdx].bLadderMatch)
	{
		bool bGameOver = false;

		if (GetPlayerSeat(plrSockIdx) == 1)
		{
			// Player 1 conceded, so check if player 2 has won
			if (m_table[tblIdx].racksWon2P >= m_table[tblIdx].raceTo) bGameOver = true;
		}
		else if(GetPlayerSeat(plrSockIdx) == 2)
		{
			if (m_table[tblIdx].racksWon1P >= m_table[tblIdx].raceTo) bGameOver = true;
		}
		else;

		if (bGameOver)
		{
			// Game is Over due to this concede
			matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
			if (matchNo != -1)
			{
				g_tournament.m_match[matchNo].m_bStopTiming = TRUE; // Tell Tourney Timers to Stop for this match // 
				g_tournament.m_match[matchNo].m_bMatchConceded = TRUE; // conceded match (so we don't invite player that conceded anymore) used for ladder matches
			}
		}
	}

#endif
	
	///////////////
	
	if(plrSockIdx == -1) return bAndExit;

	if(tblIdx == -1)
		return bAndExit;

	// check i'm on the same table before restarting table
	int myTableIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	
	if(myTableIdx == -1) return bAndExit;

	if(myTableIdx != tblIdx) return bAndExit;

	
	// if we haven't started a frame
	if(!m_table[tblIdx].bGameStarted) return bAndExit;
	#ifndef MAIN_SERVER
	if(GAMEAPPVIEW->m_snooker->m_gameOver != 0) return bAndExit;
	#endif //!MAIN_SERVER

	// do gameover now
	if(m_table[tblIdx].status)
	{				
		// go into "Game Over!" state
		if(GetPlayerSeat(plrSockIdx) == 1)
		{
			#ifndef MAIN_SERVER
			GAMEAPPVIEW->m_snooker->m_bConcededGame = TRUE;
			GAMEAPPVIEW->m_snooker->m_gameOver = 1;
			#endif //!MAIN_SERVER
		}
		else if(GetPlayerSeat(plrSockIdx) == 2)
		{
			#ifndef MAIN_SERVER
			GAMEAPPVIEW->m_snooker->m_bConcededGame = TRUE;
			GAMEAPPVIEW->m_snooker->m_gameOver = 2;
			#endif //!MAIN_SERVER
		}
 		else if(GetPlayerSeat(plrSockIdx) == 3)
		{
			#ifndef MAIN_SERVER
			GAMEAPPVIEW->m_snooker->m_bConcededGame = TRUE;
			GAMEAPPVIEW->m_snooker->m_gameOver = 1;
			#endif //!MAIN_SERVER
		}
		else if(GetPlayerSeat(plrSockIdx) == 4)
		{
			#ifndef MAIN_SERVER
			GAMEAPPVIEW->m_snooker->m_bConcededGame = TRUE;
			GAMEAPPVIEW->m_snooker->m_gameOver = 2;
			#endif //!MAIN_SERVER
		}
		else if(m_table[tblIdx].player1SockIdx != -1 || m_table[tblIdx].player2SockIdx != -1 )
		{
			#ifndef MAIN_SERVER
			// If the conceded player is not at the table, then give the win to the player seated
			if(m_table[tblIdx].doubles == false)
			{
					if(m_table[tblIdx].player1SockIdx != -1) // Player 1 Wins
					{
						GAMEAPPVIEW->m_snooker->m_bConcededGame = TRUE;
						GAMEAPPVIEW->m_snooker->m_gameOver = 2;
					}
					else if(m_table[tblIdx].player2SockIdx != -1) // Player 2 Wins
					{
						GAMEAPPVIEW->m_snooker->m_bConcededGame = TRUE;
						GAMEAPPVIEW->m_snooker->m_gameOver = 1;
					}
			}
			#endif //!MAIN_SERVER
			
		}
		else return bAndExit;

		int plr1Idx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);
		int plr2Idx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);
		int plr1bIdx = GetPlayerIdx(m_table[tblIdx].player1bSockIdx);
		int plr2bIdx = GetPlayerIdx(m_table[tblIdx].player2bSockIdx);
		
		char _buffer[MAX_CHARS];
		

		#ifndef MAIN_SERVER			
		// if i'm playing and i'm the winner, tell all my opponent has conceeded too early!

/*	// derem if u want it to be too brief for ratings
		///////////////////////////////////////////////////////////////
		if(!g_bTournamentLobby) // dont do for tourney lobbies!
		if(IsPlayerWinner(GAMEAPPVIEW->m_playerSockIdx))
		if(g_build.gameType == GAMETYPE_SNOOKER) // *TODO - fix this for pool games aswell!
		{
			//ErrorMessage(g_wnd, "111");
			int totalPtsLeft = GAMEAPPVIEW->m_snooker->TotalPtsLeft();
			//int ptsfor10reds = 10*8 + 27; // 107pts
			int ptsafter5redspotted = 1000;
			switch(GAMEAPPVIEW->m_snooker->m_customTable->m_type)
			{
				case CTTYPE_SNOOKER:
					ptsafter5redspotted = 10*8 + 27; // 107pts
					break;
				case CTTYPE_SNOOKER10:
					ptsafter5redspotted = 5*8 + 27; // 67pts
					break;
				case CTTYPE_SNOOKER6:
					ptsafter5redspotted = 1*8 + 27; // 35pts
					break;
				case CTTYPE_RESPOTBLACK:
					ptsafter5redspotted = 1000;
					break;
			}

		}
		///////////////////////////////////////////////////////////////
*/
		if(GAMEAPPVIEW->m_snooker->m_gameOver == 1)
		{
			if(m_table[tblIdx].doubles) sprintf(_buffer,"*** Team %s / %s conceded! ***", m_table[tblIdx].P1StartName, m_table[tblIdx].P1bStartName);
			else sprintf(_buffer,"*** %s conceded! *** (Please note: conceding / losing intentionally can result in a Ban)", m_table[tblIdx].P1StartName);
			GAMEAPPVIEW->Message(_buffer,10); // personal message
			// If Player1 is Not a Guest, Display a Warning Message for Virtual Cash being lost after game ends //
			if(m_table[tblIdx].bRated)
			{
				sprintf(_buffer,"*** %s will lose Rating Points & Virtual Cash after this game finishes. ***", m_table[tblIdx].P1StartName);
				GAMEAPPVIEW->Message(_buffer,10); // personal message
			}
		}
		else
		{
			if(m_table[tblIdx].doubles) sprintf(_buffer,"*** Team %s / %s conceded! ***",  m_table[tblIdx].P2StartName, m_table[tblIdx].P2bStartName);
				else sprintf(_buffer,"*** %s conceded! *** (Please note: conceding / losing intentionally can result in a Ban)", m_table[tblIdx].P2StartName);
			GAMEAPPVIEW->Message(_buffer,10); // personal message
			if(m_table[tblIdx].bRated)
			{
				sprintf(_buffer,"*** %s will lose Rating Points & Virtual Cash after this game finishes. ***", m_table[tblIdx].P2StartName);
				GAMEAPPVIEW->Message(_buffer,10); // personal message
			}
		}
		
		if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
			Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[64], 200); // "frame conceeded"

		#endif //!MAIN_SERVER
	}

	return bAndExit;
}




void CLobby::PressedReRackReceive(int plrSockIdx, BOOL bAbortRaceTo)
{
	if(plrSockIdx == -1) return;
	
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return;

	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1)
		return;

	// if we haven't started a frame
	if(!m_table[tblIdx].bGameStarted) return;

	// make a note
	m_table[tblIdx].bReRackRequest = TRUE;

	// check i'm on the same table before going any further.
	int myTableIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(myTableIdx == -1) return;
	if(myTableIdx != tblIdx) return;

	// has opponent requested an abort of 'Race-To' match
	if(bAbortRaceTo && m_table[tblIdx].raceTo > 0) // note: 0 => 1 rack (if we are racing to 2 or more racks)
	{
		// for all at this table to see:
		char _buffer[MAX_CHARS];
		sprintf(_buffer,"*** %s has asked to abort the 'Race-To' match... ***", m_player[plrIdx].name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}
	else
	{
		bAbortRaceTo = FALSE;
		// for all at this table to see:
		char _buffer[MAX_CHARS];
		sprintf(_buffer,"*** %s has asked for a Re-Rack... ***", m_player[plrIdx].name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}

	// if i'm the opponent player
	int oppPlrSockIdx = GAMEAPPVIEW->m_playerSockIdx;
	int oppPlrIdx = GetPlayerIdx(oppPlrSockIdx);
	if(oppPlrIdx == -1)
		return;
	
	if(oppPlrIdx == plrIdx) return; // this is not the oppenent

	// get seat idxs
	int plr1Idx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);
	int plr2Idx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);

	if((oppPlrIdx == plr1Idx) || (oppPlrIdx == plr2Idx))
	{
		// display "rerack accpet" request dialog box,
		GAMEAPPVIEW->ReRackAcceptDlg(bAbortRaceTo);
	
		// ****debug code, to be called inside 'InvitationDlg':
		//GAMEAPPVIEW->SendReRackPressed(oppPlrSockIdx, TRUE);
	}
}




void CLobby::ReRackAccpetedReceive(int plrSockIdx, int acceptType, BOOL bAbortRaceTo)
{
	if(plrSockIdx == -1) return;
	
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return;

	int idx;
	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1)
		return;


	// bypass the following code if acceptType == 8 //
	if(acceptType != 8)
		{
		// make sure a request has just been made 
		if(!m_table[tblIdx].bReRackRequest) return;
		}

	m_table[tblIdx].bReRackRequest = FALSE;

	// if we haven't started a frame
	if(!m_table[tblIdx].bGameStarted) return;

	////////////////////////////////////////////////////
	BOOL bRacksStillToPlay = FALSE;
	//if(m_table[tblIdx].raceTo > 0) // note: 0 => 1 rack (if we are racing to 2 or more racks)
	//{
		int raceTo = m_table[tblIdx].raceTo + 1;
		if((m_table[tblIdx].racksWon1P < raceTo) &&
		   (m_table[tblIdx].racksWon2P < raceTo))
		{
			bRacksStillToPlay = TRUE;
		}
	//}
	////////////////////////////////////////////////////

	
	if(bAbortRaceTo) // ???? is this ok <-------------------- (BUGFIXED NOW: by odin)
	if(acceptType == 0 || acceptType == 8) //<-- bugfix!
	{
		bRacksStillToPlay = FALSE;
		
		m_table[tblIdx].racksWon1P = 0;
		m_table[tblIdx].racksWon2P = 0;
	}

	if(!bRacksStillToPlay)
	if(acceptType == 0 || acceptType == 8)
	{
		m_table[tblIdx].bGameStarted = FALSE;
		m_table[tblIdx].P1StartAtTable = -1;
		m_table[tblIdx].P2StartAtTable = -1;
		m_table[tblIdx].P1StartName[0] = 0;
		m_table[tblIdx].P2StartName[0] = 0;
		m_table[tblIdx].P1bStartAtTable = -1;
		m_table[tblIdx].P2bStartAtTable = -1;
		m_table[tblIdx].P1bStartName[0] = 0;
		m_table[tblIdx].P2bStartName[0] = 0;
		m_table[tblIdx].score1P = m_table[tblIdx].startScore1P;
		m_table[tblIdx].score2P = m_table[tblIdx].startScore2P;
		m_table[tblIdx].m_lostConn1P = 0;
		m_table[tblIdx].m_lostConn2P = 0;
	}


	// If this was a Re-Rack then we should update the Re-Rack Stat //
	if(acceptType == 0)
		{

		m_table[tblIdx].bCanRerack = FALSE;	// Don't allow another Re-Rack for this Frame (at this Table)
		

		if(m_table[tblIdx].player1SockIdx != -1)
			{
			idx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);
			m_player[idx].reracks++;
			#ifdef MAIN_SERVER
			db_updatereracks(m_player[idx].name, m_player[idx].reracks);
			#endif
			}
		// Player 2 ReRacks //
		if(m_table[tblIdx].player2SockIdx != -1)
			{
			idx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);
			m_player[idx].reracks++;
			#ifdef MAIN_SERVER
			db_updatereracks(m_player[idx].name, m_player[idx].reracks);
			#endif
			}
		// Player 1b ReRacks //
		if(m_table[tblIdx].player1bSockIdx != -1)
			{
			idx = GetPlayerIdx(m_table[tblIdx].player1bSockIdx);
			m_player[idx].reracks++;
			#ifdef MAIN_SERVER
			db_updatereracks(m_player[idx].name, m_player[idx].reracks);
			#endif
			}
		// Player 2b ReRacks //
		if(m_table[tblIdx].player2bSockIdx != -1)
			{
			idx = GetPlayerIdx(m_table[tblIdx].player2bSockIdx);
			m_player[idx].reracks++;
			#ifdef MAIN_SERVER
			db_updatereracks(m_player[idx].name, m_player[idx].reracks);
			#endif
			}
		}



	// check i'm on the same table before going any further.
	int myTableIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(myTableIdx == -1) return;
	if(myTableIdx != tblIdx) return;

	// for all at table to see:
	char _buffer[MAX_CHARS];
	if(acceptType == 1)
	{
		if(!bAbortRaceTo)
			sprintf(_buffer,"*** %s has NOT accepted a Re-Rack ***", m_player[plrIdx].name);
		else
			sprintf(_buffer,"*** %s has NOT agreed to an Abort ***", m_player[plrIdx].name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		return;
	}
	else if(acceptType == 8)	// Special Type used when Player Decides to Abort Match when opponent hasn't returned from a disconnection
	{
		sprintf(_buffer,"*** Match Aborted ***");
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		if(!bRacksStillToPlay)
		{
			//if(g_build.gameType == GAMETYPE_POOL)
				if(m_table[tblIdx].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
					GAMEAPPVIEW->m_button[BUTTON_SETUP].EnableWindow(TRUE);

			// ensure mouse cursor is visible (just in-case someone is mid-way thru the line-up process)
			GAMEAPPVIEW->FreeMouse();

			// *NEW - disable stand button during game [standdisable]
			if(!g_bTournamentLobby)
				{
				//GAMEAPPVIEW->m_standButton.EnableWindow(TRUE);
				GAMEAPPVIEW->m_standButtonEnabled = TRUE;
				}

			// *NEW - bugfix for abort raceto, stand, and let someone else sit in ur seat and start a game, then exit.
			// get ready for next frame
			ClearPlayer(&m_myself);
			ClearPlayer(&m_opponent);
			m_PLCwait = 0;
		}

		#ifndef MAIN_SERVER
		// check this player is 'sitting' at a table and was the accpeterd
		if(GAMEAPPVIEW->m_playerSockIdx == plrSockIdx)
		//	if(SeatedAtTable(plrSockIdx))
		//		if(BothSeatsTaken(plrSockIdx))
				{
					// ^
					// POSSIBLE BUG HERE!!!!! WHAT IF ONE PLAYERS STANDS UP AFTER RERACK IS ACCEPTED, THEN THE OTHERE PLAYER WON'T GET IN HERE (OAP)
			
						//if(gameShell)
						GAMEAPPVIEW->SendReset(); // network game only! //SendStartPressed(m_playerSockIdx); // network game only!
				}
		#endif //!MAIN_SERVER
		return;


	}
	else
	{

		// Here is Where the Re-Rack stat Augments //


		// Set Camera View back to Default //
		g_gameAppView->m_snooker->m_3dtoggle = 3;				// Default View is 3
		///// Set Default Camera Position /////
		g_gameAppView->m_TVcameraX_pos[0] = 5000;
		g_gameAppView->m_TVcameraY_pos[0] = 0;
		g_gameAppView->m_TVcameraZ_pos[0] = 1600;
		g_gameAppView->m_TVcameraX_at[0] = 500;
		g_gameAppView->m_TVcameraY_at[0] = 0;
		g_gameAppView->m_TVcameraZ_at[0] = 0;
		g_gameAppView->m_TVClipType = 0;
		/////////////////////////////////////// 

		if(!bAbortRaceTo)
			sprintf(_buffer,"*** %s has agreed to a Re-Rack ***", m_player[plrIdx].name);
		else
			sprintf(_buffer,"*** %s has agreed to an Abort ***", m_player[plrIdx].name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message



		if(!bRacksStillToPlay)
		{
			//if(g_build.gameType == GAMETYPE_POOL)
				if(m_table[tblIdx].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
					GAMEAPPVIEW->m_button[BUTTON_SETUP].EnableWindow(TRUE);

			// ensure mouse cursor is visible (just in-case someone is mid-way thru the line-up process)
			GAMEAPPVIEW->FreeMouse();

			// *NEW - disable stand button during game [standdisable]
			if(!g_bTournamentLobby)
				{
				//GAMEAPPVIEW->m_standButton.EnableWindow(TRUE);
				GAMEAPPVIEW->m_standButtonEnabled = TRUE;
				}

			// *NEW - bugfix for abort raceto, stand, and let someone else sit in ur seat and start a game, then exit.
			// get ready for next frame
			ClearPlayer(&m_myself);
			ClearPlayer(&m_opponent);
			m_PLCwait = 0;
		}

		#ifndef MAIN_SERVER
		// check this player is 'sitting' at a table and was the accpeterd
		if(GAMEAPPVIEW->m_playerSockIdx == plrSockIdx)
		//	if(SeatedAtTable(plrSockIdx))
		//		if(BothSeatsTaken(plrSockIdx))
				{
					// ^
					// POSSIBLE BUG HERE!!!!! WHAT IF ONE PLAYERS STANDS UP AFTER RERACK IS ACCEPTED, THEN THE OTHERE PLAYER WON'T GET IN HERE (OAP)
			
						//if(gameShell)
						GAMEAPPVIEW->SendReset(); // network game only! //SendStartPressed(m_playerSockIdx); // network game only!
				}
		#endif //!MAIN_SERVER
		return;
	}
}


// This function will end the game for the following table //
void CLobby::EndGameReceive(int tblIdx)
{
	if(tblIdx == -1)
		return;

	// if we haven't started a frame
	if(!m_table[tblIdx].bGameStarted) return;

	////////////////////////////////////////////////////
	BOOL bRacksStillToPlay;

	
	bRacksStillToPlay = FALSE;
	m_table[tblIdx].racksWon1P = 0;
	m_table[tblIdx].racksWon2P = 0;
	
	m_table[tblIdx].bGameStarted = FALSE;

	m_table[tblIdx].P1StartAtTable = -1;
	m_table[tblIdx].P1StartName[0] = 0;

	m_table[tblIdx].P2StartAtTable = -1;
	m_table[tblIdx].P2StartName[0] = 0;
	
	m_table[tblIdx].P1bStartAtTable = -1;
	m_table[tblIdx].P1bStartName[0] = 0;

	m_table[tblIdx].P2bStartAtTable = -1;
	m_table[tblIdx].P2bStartName[0] = 0;
	
	m_table[tblIdx].score1P = m_table[tblIdx].startScore1P;
	m_table[tblIdx].score2P = m_table[tblIdx].startScore2P;

	m_table[tblIdx].m_lostConn1P = 0;
	m_table[tblIdx].m_lostConn2P = 0;

	// If I'm not at the table, then return here //
	int mytblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(mytblIdx == -1) return;
	if(mytblIdx != tblIdx) return;
	
#ifndef MAIN_SERVER
	char _buffer[128];
	if(m_table[tblIdx].doubles)	sprintf(_buffer,"*** A player has left this Doubles Match, so the game has been aborted. ***");
		else sprintf(_buffer,"*** Your opponent left the table at the same time you clicked Start. The Match has therefore been aborted ***");
	GAMEAPPVIEW->Message(_buffer,10); // personal message
	
	if(m_table[tblIdx].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
		GAMEAPPVIEW->m_button[BUTTON_SETUP].EnableWindow(TRUE);

	// ensure mouse cursor is visible (just in-case someone is mid-way thru the line-up process)
	GAMEAPPVIEW->FreeMouse();

	// *NEW - disable stand button during game [standdisable]
	if(!g_bTournamentLobby)
		{
		GAMEAPPVIEW->m_standButtonEnabled = TRUE;
		}

	// *NEW - bugfix for abort raceto, stand, and let someone else sit in ur seat and start a game, then exit.
	// get ready for next frame
	ClearPlayer(&m_myself);
	ClearPlayer(&m_opponent);
	m_PLCwait = 0;
	
	
	GAMEAPPVIEW->SendReset(); // network game only! //SendStartPressed(m_playerSockIdx); // network game only!



	// Set Camera View back to Default //
	g_gameAppView->m_snooker->m_3dtoggle = 3;				// Default View is 3
	///// Set Default Camera Position //////////
	g_gameAppView->m_TVcameraX_pos[0] = 5000;
	g_gameAppView->m_TVcameraY_pos[0] = 0;
	g_gameAppView->m_TVcameraZ_pos[0] = 1600;
	g_gameAppView->m_TVcameraX_at[0] = 500;
	g_gameAppView->m_TVcameraY_at[0] = 0;
	g_gameAppView->m_TVcameraZ_at[0] = 0;
	g_gameAppView->m_TVClipType = 0;
	////////////////////////////////////////////
#endif //!MAIN_SERVER


return;


}


void CLobby::TableClearReRackRequest(int plrSockIdx)
{
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return;

	// remove ReRack request message
	GAMEAPPVIEW->m_rerackDlg.DestroyWindow();
	GAMEAPPVIEW->m_rerackAccept2Dlg.DestroyWindow();
	m_table[tblIdx].bReRackRequest = FALSE;		
}

void CLobby::PressedInviteReceive(int plrSockIdx, int invitedPlrSockIdx, char *hostName, int initialHostTbl, int table, int enterType, int reracks, int totalGames)
{
	BOOL bAutoInvite = FALSE;
	BOOL bServerInvite = FALSE;
	if(table != -1) {
		bAutoInvite = TRUE;		
		if(plrSockIdx == 0)
			bServerInvite = TRUE;
	}
//	else
//		table = initialHostTbl;

	// note plrSockIdx => host of table or inviter
	if(plrSockIdx == -1) return;
	
	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1 && !bServerInvite)
		return;

	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(bAutoInvite) tblIdx = table;

	if(tblIdx == -1)
		return;

	// check if this invitee is still in the process of logging-in? **NEW**
	if(!m_bLobbyReady)
	{
		// ignore this invitation
		return;
	}

	if(!bAutoInvite)
	{

	if(m_bDeclineAll)
	{
		// "not accepting invitations"
		GAMEAPPVIEW->SendInvitePressed(plrSockIdx, GAMEAPPVIEW->m_playerSockIdx, NULL, NULL, TRUE,
													   "NOT accepting Invitations at this time.",
													   initialHostTbl, table, enterType); // network game only!
		return;
	}
	else if(m_bDeclineAllGuests)
	{
		if(m_player[plrIdx].bIsGuest) // if the inviter is a guest
		{
			// "not accepting invitations"
			GAMEAPPVIEW->SendInvitePressed(plrSockIdx, GAMEAPPVIEW->m_playerSockIdx, NULL, NULL, TRUE,
													   "NOT accepting Invitations from guests at this time.",
													   initialHostTbl, table, enterType); // network game only!
			return;	
		}
	}
	else 
	{
		if(GameStarted(GAMEAPPVIEW->m_playerSockIdx))
		if(PlayingAGame(GAMEAPPVIEW->m_playerSockIdx))
		{
			// "not accepting invitations, whilst playing a game"
			GAMEAPPVIEW->SendInvitePressed(plrSockIdx, GAMEAPPVIEW->m_playerSockIdx, NULL, NULL, TRUE,
													   "Currently playing a game, try again later please.",
													   initialHostTbl, table, enterType); // network game only!
			return;
		}
	}
	
	if(m_player[plrIdx].bIgnore) // if the inviter has been ignored
	{
		// "ignoring invitations"
		GAMEAPPVIEW->SendInvitePressed(plrSockIdx, GAMEAPPVIEW->m_playerSockIdx, NULL, NULL, TRUE,
												   "NOT accepting your Invitation at this time.",
												   initialHostTbl, table, enterType); // network game only!
		return;	
	}

	} // !bAutoInvite

	// display invitation dialog box, 
	GAMEAPPVIEW->InvitationDlg(plrSockIdx, hostName, initialHostTbl, table, enterType, reracks, totalGames);

	// ****debug code, to be called inside 'InvitationDlg':
	//GAMEAPPVIEW->SendInvitePressed(plrSockIdx, invitedPlrSockIdx, TRUE);
}

void CLobby::InvitationAcceptedReceive(int plrSockIdx, int invitedPlrSockIdx, char *str, int initialHostTbl, int table, int enterType)
{
	BOOL bAutoInvite = FALSE;
	BOOL bServerInvite = FALSE;
	if(table != -1)
	{
		bAutoInvite = TRUE;
		if(plrSockIdx == 0)
			bServerInvite = TRUE;
	}
	
	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1 && !bServerInvite)
		return;
	
	int invitedPlrIdx = GetPlayerIdx(invitedPlrSockIdx);
	if(invitedPlrIdx == -1)
		return;

	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);

	if(bAutoInvite) tblIdx = table;



	// ***cancel msg type 1***
	//if(tblIdx != -1) // if inviter is still at a table...
	if(initialHostTbl == tblIdx) // check if the inviter is still at the initial table they invited from
	{
		// ...And invitation was declined!
		if(*str)
		if(str[0])
		{
			// host of table, who invited (now displays opponent's decline reply)
			if(GAMEAPPVIEW->m_playerSockIdx == plrSockIdx)
			{
				char buff[MAX_CHARS];
				sprintf(buff,"%s has declined your invitation:\n\nReply: %s",m_player[invitedPlrIdx].name, str);
				GAMEAPPVIEW->AlertMessage(buff);
				// Also put this message in the Chat Area, so Abuse can be reported //
				GAMEAPPVIEW->Message(buff, 10); // personal message
			}
			return;
		}									  
	}
	
	// ***cancel msg type 2***
	if(initialHostTbl != tblIdx) // check that inviter has moved table since this invite was issued
	{
		// ...And invitation was accepted!
		//if(str == NULL)
		if(str[0] == 0)
		{
			if(GAMEAPPVIEW->m_playerSockIdx == invitedPlrSockIdx)
			{
				// message to say inviter has left lobby, so ignore this invitation
				char _buffer[MAX_CHARS];
				sprintf(_buffer,"*** Invitation cancelled! (Inviter/Host has moved table) ***");
				GAMEAPPVIEW->Message(_buffer,10); // personal message	
			}
		}
		return;
	}

	if(tblIdx == -1)
		return;

	/* WONT WORK!, as everyone get this message, see down below...
	// check if host of table, who invited, is still in the process of logging-in? **NEW**
	// (note: we shouldn't need this, but due to SocketIdx, hoest may have logged off, and someone lese logged in his socket place) 
	if(GAMEAPPVIEW->m_playerSockIdx == plrSockIdx)
		if(!m_bLobbyReady)
		{
			// ignore this invitation reply
			return;
		}
	*/	
	
	// check if the invitee is playing a game
	//if(GAMEAPPVIEW->m_playerSockIdx == invitedPlrSockIdx)
	//{		
		int iTblIdx = GetTableIdxFromPlayer(invitedPlrSockIdx);
		//if(iTblIdx != -1)
		if(iTblIdx != -1 && iTblIdx != tblIdx) // new 20/feb/2004 : if this disconn. invitee has returned to this table, let invitation work
		{
			// if game has already started AND i'm playing in it
			if(m_table[iTblIdx].bGameStarted)
			if( (m_table[iTblIdx].P1StartAtTable == invitedPlrSockIdx) ||
			    (m_table[iTblIdx].P2StartAtTable == invitedPlrSockIdx) )
			{
				return; // ignore this 'invite' as invitee is playing in a game already
			}			
		}
	//}

	// invitation was accepted
	bool bLadderMatch = m_table[iTblIdx].bLadderMatch;

	int invitedTblIdx = GetTableIdxFromPlayer(invitedPlrSockIdx);	
	if( (invitedTblIdx == tblIdx) && (!bAutoInvite || !g_bTournamentLobby) && !bLadderMatch)
	{
		// this 'invitee' must already be on the same table as the 'inviter'
		if(GAMEAPPVIEW->m_playerSockIdx == invitedPlrSockIdx)
			GAMEAPPVIEW->EnableSeats(invitedPlrIdx, TRUE);	// allow invitee to sit at all table types (i.e. PUB, PRO, PRI)
		
		m_player[invitedPlrIdx].bInvited = TRUE;
	}
	else
	{
		// goto to table!
		if(GAMEAPPVIEW->m_playerSockIdx == invitedPlrSockIdx)
		{
			GAMEAPPVIEW->DestroyGameShell();
			GAMEAPPVIEW->m_tableEnter = 0;
		}


		ExitTable(invitedPlrSockIdx);					// if at table, exit table

		// check if table is already active
		if(m_table[tblIdx].status == 1)	JoinTable(tblIdx, enterType, invitedPlrSockIdx, FALSE);	// join table as a watcher (as non-host)
			else JoinTable(tblIdx, enterType, invitedPlrSockIdx, TRUE);	// join table as a watcher (as host!)

		if(GAMEAPPVIEW->m_playerSockIdx == invitedPlrSockIdx)
			GAMEAPPVIEW->EnableSeats(invitedPlrIdx, TRUE);	// allow invitee to sit at all table types (i.e. PUB, PRO, PRI)
		
		m_player[invitedPlrIdx].bInvited = TRUE;

		if(invitedTblIdx == tblIdx) // special case for bAutoInvite == TRUE
			if(GAMEAPPVIEW->m_playerSockIdx == invitedPlrSockIdx) // only let the 'invitee' do this...
				UpdateMyselfOpponent(tblIdx); // update 'm_myself' & 'm_opponent' since the ExitTable and JoinTable cleared them!
	}
}


void CLobby::UpdateMyselfOpponent(int tblIdx)
{
	if(tblIdx == -1) return;

	// NOTE: BOTH PLAYERS MUST BE SEATED, AND BOTH PRESSED START!

	// P1 records the two players playing
	if(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].player1SockIdx)
	{
		//int myIdx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);	// get player that is sitting (1P) - wrong!
		int myIdx = GetPlayerIdx(m_table[tblIdx].P1StartAtTable);	// get player that clicked start (1P)
		if(myIdx != -1)
		{
			memcpy(&m_myself, &m_player[myIdx], sizeof(Player_typ));
		}
		//int oppIdx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);// get player that is sitting (2P) - wrong!
		int oppIdx = GetPlayerIdx(m_table[tblIdx].P2StartAtTable);	// get player that clicked start (2P)
		if(oppIdx != -1)
		{
			memcpy(&m_opponent, &m_player[oppIdx], sizeof(Player_typ));
		}
	}

	// P2 records the two players playing
	if(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].player2SockIdx)
	{
		//int myIdx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);	// get player that is sitting (2P) - wrong!
		int myIdx = GetPlayerIdx(m_table[tblIdx].P2StartAtTable);	// get player that clicked start (2P)
		if(myIdx != -1)
		{
			memcpy(&m_myself, &m_player[myIdx], sizeof(Player_typ));
		}
		//int oppIdx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);// get player that is sitting (1P) - wrong!
		int oppIdx = GetPlayerIdx(m_table[tblIdx].P1StartAtTable);	// get player that clicked start (1P)
		if(oppIdx != -1)
		{
			memcpy(&m_opponent, &m_player[oppIdx], sizeof(Player_typ));
		}
	}
}

void CLobby::PressedBootReceive(int plrSockIdx, int bootedPlrSockIdx)
{
	if(plrSockIdx == -1) return;
	
	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1)
		return;

	int bootedPlrIdx = GetPlayerIdx(bootedPlrSockIdx);
	if(bootedPlrIdx == -1)
		return;

	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return;

	// [NEW] don't boot player that is sitting in a 'Tournament' table (i.e. in a Tournament lobby)
	if(g_bTournamentLobby)
		if(SeatedAtTable(bootedPlrSockIdx))		
		{				
			// only for host of table (the booter)
			if(GAMEAPPVIEW->m_playerSockIdx == plrSockIdx)
			{
				char _buffer[MAX_CHARS];
				sprintf(_buffer,"*** Tournament game: can't boot seated player! ***");
				GAMEAPPVIEW->Message(_buffer,10); // personal message	
			}
			
			return;
		}

	// don't boot player that is sitting and playing a 'started' game
	if(SeatedAtTable(bootedPlrSockIdx))
		if(GameStarted(bootedPlrSockIdx))
		{				
			// only for host of table (the booter)
			if(GAMEAPPVIEW->m_playerSockIdx == plrSockIdx)
			{
				char _buffer[MAX_CHARS];
				sprintf(_buffer,"*** Unfinished game: can't boot %s ***", m_player[bootedPlrIdx].name);
				GAMEAPPVIEW->Message(_buffer,10); // personal message	
			}
			
			return;
		}

	// for all to see:
	char _buffer[MAX_CHARS];
	sprintf(_buffer,"*** %s has booted %s! ***", m_player[plrIdx].name, m_player[bootedPlrIdx].name);
	GAMEAPPVIEW->Message(_buffer,10); // personal message

	// exit table (you're booted!)
	if(GAMEAPPVIEW->m_playerSockIdx == bootedPlrSockIdx)
		GAMEAPPVIEW->BootPlayer(bootedPlrSockIdx, 2);	
}


BOOL CLobby::GameStarted(int plrSockIdx)
{
	if(plrSockIdx == -1) return FALSE;
	
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return FALSE;

	return m_table[tblIdx].bGameStarted;
}

/*  i dont think we need this!
BOOL CLobby::IAmPlaying(int plrSockIdx)
{
	int plrIdx = GetPlayerIdx(plrSockIdx);

	if(plrIdx == -1)
		return FALSE;

	if(m_player[plrIdx]. 
}
*/

void CLobby::PlayerWin(Player *pPlr)
{
	if(pPlr == NULL) return;

	// win
	pPlr->wins++;
	
	// streak
	if(pPlr->streak < 0) pPlr->streak = 1;
	else pPlr->streak++;
}

void CLobby::PlayerLose(Player *pPlr)
{
	if(pPlr == NULL) return;

	// lose
	pPlr->losses++;
	
	// streak
	if(pPlr->streak > 0) pPlr->streak = -1;
	else pPlr->streak--;
}

void CLobby::PlayerDraw(Player *pPlr)
{
	if(pPlr == NULL) return;
	// streak
	pPlr->streak = 0;
}

/*
ELO difference Expected score 
0 0.50 
20 0.53 
40 0.58 
60 0.62 
80 0.66 
100 0.69 
120 0.73 
140 0.76 
160 0.79 
180 0.82 
200 0.84 
300 0.93 
400 0.97

Rn = Ro + C * (S - Se) (1)

where: 
Rn = new rating 
Ro = old rating 
S  = score 
Se = expected score 
C  = constant 

Rating		Rank						C 										
< 2200 30	kyu, ..., 4 dan				30 
2200-2500	4, ..., 7 dan				(2200-R)/15 + 30 
> 2500		7 dan (1p), ..., 9p			10 

http://gobase.org/studying/articles/elo/
*/

// Updates rating always!!
void CLobby::CalcNewELORatings(int gameOver, Player *pPlr1, Player *pPlr2)
{
	if(pPlr1 == NULL) return;
	if(pPlr2 == NULL) return;

	float Rn;
	float Ro;
	float C;
	float S;
	float Se;

	BOOL bNeg = FALSE;
	float diff = pPlr1->rating - pPlr2->rating;	
	if(diff < 0) {
		diff = -diff;
		bNeg = TRUE;
	}

	if(diff < 20*2) Se = 0.50;
	else if(diff < 40*2) Se = 0.53;
	else if(diff < 60*2) Se = 0.58;
	else if(diff < 80*2) Se = 0.62;
	else if(diff < 100*2) Se = 0.66;
	else if(diff < 120*2) Se = 0.69;
	else if(diff < 140*2) Se = 0.73;
	else if(diff < 160*2) Se = 0.76;
	else if(diff < 180*2) Se = 0.79;
	else if(diff < 200*2) Se = 0.82;
	else if(diff < 300*2) Se = 0.84;
	else if(diff < 400*2) Se = 0.93;
	else Se = 0.97;

	if(gameOver == 2) // 1P won!
	{
		// 1P's new rating...
		PlayerWin(pPlr1);

		if(bNeg) Se = 1 - Se;
		S = 1.0;
			
		if(pPlr1->rating < 2200) C = 30;
		else if(pPlr1->rating > 2500) C = 10;
		else C = (2200-pPlr1->rating)/15 + 30;
		Ro = pPlr1->rating;
		Rn = Ro + C * (S - Se);
		Rn += 0.5;
		pPlr1->rating = Rn;
		if(pPlr1->rating < 0) pPlr1->rating = 0; // lower rating limit

		// 2P's new rating...
		PlayerLose(pPlr2);
		Ro = pPlr2->rating;
		Rn = Ro - C * (S - Se);
		Rn += 0.5;
		pPlr2->rating = Rn;
		if(pPlr2->rating < 0) pPlr2->rating = 0; // lower rating limit
	}
	else if(gameOver == 1) // 2P won!
	{
		// 2P's new rating...
		PlayerWin(pPlr2);
		if(!bNeg) Se = 1 - Se;
		S = 1.0;

		if(pPlr2->rating < 2200) C = 30;
		else if(pPlr2->rating > 2500) C = 10;
		else C = (2200-pPlr2->rating)/15 + 30;
		Ro = pPlr2->rating;
		Rn = Ro + C * (S - Se);
		Rn += 0.5;
		pPlr2->rating = Rn;
		if(pPlr2->rating < 0) pPlr2->rating = 0; // lower rating limit

		// 1P's new rating...
		PlayerLose(pPlr1);
		Ro = pPlr1->rating;
		Rn = Ro - C * (S - Se);
		Rn += 0.5;
		pPlr1->rating = Rn;
		if(pPlr1->rating < 0) pPlr1->rating = 0; // lower rating limit
	}
	else // draw (Note: we prolly won't need this for snooker)
		{
		// Draw Case
		}

}

void CLobby::PlayerResultLadder(Player *pPlr, bool bWin, int highBreak, bool bPro)
{
	int lWins;
	int lLosses;
	int lHighBreak;

	int lHighBreakPro;

	// Get our Wins and Losses, High Breaks
	lWins = (pPlr->ladderWinsLosses & 0xFFFF0000) >> 16;
	lLosses = (pPlr->ladderWinsLosses & 0xFFFF);
	lHighBreakPro = (pPlr->ladderHighBreak & 0xFFFF0000) >> 16;
	lHighBreak = (pPlr->ladderHighBreak & 0xFFFF);

	// Update Wins or Losses
	if (bWin) lWins++;
		else lLosses++;

#ifndef THE_POOL_CLUB
	// Update High Break if needed
	if (bPro)
	{
		if (highBreak > lHighBreakPro) lHighBreakPro = highBreak;
	}
	else
	{
		if (highBreak > lHighBreak)	lHighBreak = highBreak;
	}

	
#else
	// For iPool, the bPro flag tells us if this is 9ball Game. highBreak is a runout made
	if (bPro)
	{
		// 9-Ball Game Type
		if (highBreak)
		{
			// A Run-Out was made
			lHighBreakPro++;
		}
	}
	else
	{
		// 8-Ball Game Type
		if (highBreak)
		{
			// A Run-Out was made
			lHighBreak++;
		}
	}

#endif

	// Finally update new stats
	pPlr->ladderWinsLosses = (lWins << 16) | lLosses;
	pPlr->ladderHighBreak = (lHighBreakPro << 16) | lHighBreak;
}



// Updates rating always!!
void CLobby::CalcNewELOLadderPoints(int gameOver, int highBreakP1, int highBreakP2, bool bPro, Player *pPlr1, Player *pPlr2, bool bLadderMatch)
{
	if (pPlr1 == NULL) return;
	if (pPlr2 == NULL) return;

	float Rn;
	float Ro;
	float C;
	float S;
	float Se;


	BOOL bNeg = FALSE;
	float diff = pPlr1->ladderPoints - pPlr2->ladderPoints;
	if (diff < 0) {
		diff = -diff;
		bNeg = TRUE;
	}

	if (diff < 200 * 3) Se = 0.50;
	else if (diff < 400 * 3) Se = 0.53;
	else if (diff < 600 * 3) Se = 0.58;
	else if (diff < 800 * 3) Se = 0.62;
	else if (diff < 1000 * 3) Se = 0.66;
	else if (diff < 1200 * 3) Se = 0.69;
	else if (diff < 1400 * 3) Se = 0.73;
	else if (diff < 1600 * 3) Se = 0.76;
	else if (diff < 1800 * 3) Se = 0.79;
	else if (diff < 2000 * 3) Se = 0.82;
	else if (diff < 3000 * 3) Se = 0.84;
	else if (diff < 4000 * 3) Se = 0.93;
	else Se = 0.97;

	int gamesP1;
	int gamesP2;
	int sM = 3;

	// Get Wins and Losses.. 
	gamesP1 = ((pPlr1->ladderWinsLosses & 0xFFFF0000) >> 16) + (pPlr1->ladderWinsLosses & 0xFFFF);
	gamesP2 = ((pPlr2->ladderWinsLosses & 0xFFFF0000) >> 16) + (pPlr2->ladderWinsLosses & 0xFFFF);


	if (gameOver == 2) // 1P won!
	{
		// 1P's new ladderPoints...
		PlayerResultLadder(pPlr1, true, highBreakP1, bPro);

		if (bNeg) Se = 1 - Se;
		S = 1.0;
		C = 600;
		// If this is a Ladder Match (i.e. Random opponent, then more points exchanged...)
		if (bLadderMatch) C = 800;


		Ro = pPlr1->ladderPoints;

		
		// Use 3x rate of Ladder Points transfer if first 5 games..
		if (gamesP1 < 5) Rn = Ro + (C * sM) * (S - Se);
			else Rn = Ro + C * (S - Se);

		Rn += 0.5;
		pPlr1->ladderPoints = Rn;
		if (pPlr1->ladderPoints < 0) pPlr1->ladderPoints = 0; // lower ladderPoints limit

		
		// 2P's new ladderPoints...
		PlayerResultLadder(pPlr2, false, highBreakP2, bPro);

		Ro = pPlr2->ladderPoints;

		if (gamesP2 < 5) Rn = Ro - (C * sM) * (S - Se);
			else Rn = Ro - C * (S - Se);
		Rn += 0.5;
		pPlr2->ladderPoints = Rn;
		if (pPlr2->ladderPoints < 0) pPlr2->ladderPoints = 0; // lower ladderPoints limit
	}
	else if (gameOver == 1) // 2P won!
	{
		// 2P's new ladderPoints...
		PlayerResultLadder(pPlr2, true, highBreakP2, bPro);

		if (!bNeg) Se = 1 - Se;
		S = 1.0;
		C = 600;
		// If this is a Ladder Match (i.e. Random opponent, then more points exchanged...)
		if (bLadderMatch) C = 800;
		
		
		Ro = pPlr2->ladderPoints;
		// Use 3x rate of Ladder Points transfer if first 5 games..
		if (gamesP2 < 5) Rn = Ro + (C * sM) * (S - Se);
			else Rn = Ro + C * (S - Se);
		Rn += 0.5;
		pPlr2->ladderPoints = Rn;
		if (pPlr2->ladderPoints < 0) pPlr2->ladderPoints = 0; // lower rating limit

		// 1P's new rating...
		PlayerResultLadder(pPlr1, false, highBreakP1, bPro);
		Ro = pPlr1->ladderPoints;
		if (gamesP1 < 5) Rn = Ro - (C * sM) * (S - Se);
			else Rn = Ro - C * (S - Se);
		Rn += 0.5;
		pPlr1->ladderPoints = Rn;
		if (pPlr1->ladderPoints < 0) pPlr1->ladderPoints = 0; // lower rating limit
	}
	else // draw (Note: we prolly won't need this for snooker)
	{
		// Draw Case
	}
	
}



void CLobby::GetHighBreak(Player *pPlr, Player *pPlrOpponent, int cueError, int highBreak, int high4, int tblIdx)
{
	
	if(pPlr == NULL) return;
	if(pPlrOpponent == NULL) return;
	if(pPlr->bIsGuest || pPlrOpponent->bIsGuest) return;
	//////////////////////////////////////////////////////////////////////////////


//	char buf[80];
//	sprintf(buf, "GetHighBreak(...) => highBreak = %d", highBreak);
//	ErrorMessage(m_parent, buf);

	//( note: iPool's cueError = 1 always )

	if(cueError == 2)
	{

//		sprintf(buf, "GetHighBreak(...) 2");
//		ErrorMessage(m_parent, buf);

		if(g_build.gameType == GAMETYPE_SNOOKER)
		if(GAMEAPPVIEW->m_snooker->m_customTable->m_type != CTTYPE_BILLIARDS)	// Don't Do High Breaks for Billiards //
		if(!pPlr->bIsGuest)
		if(!pPlrOpponent->bIsGuest)
		{
		
			// Always Send the high Break, since the server now holds up to 64 Breaks //
			GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 2, high4);

			if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[0])
			{
				GAMEAPPVIEW->m_sLobbyPHighBreak[0] = highBreak;
				//GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 2, high4);
			}
			else if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[1])
			{
				GAMEAPPVIEW->m_sLobbyPHighBreak[1] = highBreak;
				//GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 2, high4);
			}		
			else if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[2])
			{
				GAMEAPPVIEW->m_sLobbyPHighBreak[2] = highBreak;
				//GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 2, high4);
			}
			//////////////////////////////////////////////////////////////
		}



		// check if this beats current high break
		if(highBreak > pPlr->pHighBreak)
			pPlr->pHighBreak = highBreak;
		
		
		// only include breaks >= 50
		if(highBreak >= 50)
			{
			if(highBreak <= 59)
				pPlr->_p50s++;
			else if(highBreak <= 69)
				pPlr->_p60s++;
			else if(highBreak <= 79)
				pPlr->_p70s++;
			else if(highBreak <= 89)
				pPlr->_p80s++;
			else if(highBreak <= 99)
				pPlr->_p90s++;
			else if(highBreak >= 100)
				pPlr->_p100s++;

			if(highBreak == 147)
				pPlr->pMax147s++;	
			}

			

	}
	else if(cueError == 1)
	{
//		sprintf(buf, "GetHighBreak(...) 1");
//		ErrorMessage(m_parent, buf);

		if(g_build.gameType == GAMETYPE_SNOOKER)
		if(GAMEAPPVIEW->m_snooker->m_customTable->m_type != CTTYPE_BILLIARDS)	// Don't Do High Breaks for Billiards //
		if(!pPlr->bIsGuest)
		if(!pPlrOpponent->bIsGuest)
			{
			// Always Send the high Break, since the server now holds up to 64 Breaks //
			GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 1, high4);
			
			if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[0])
				{
				GAMEAPPVIEW->m_sLobbyHighBreak[0] = highBreak;
				//GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 1, high4);
				}
			else if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[1])
				{
				GAMEAPPVIEW->m_sLobbyHighBreak[1] = highBreak;
				//GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 1, high4);
				}		
			else if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[2])
				{
				GAMEAPPVIEW->m_sLobbyHighBreak[2] = highBreak;
				//GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 1, high4);
				}
		

			// Add Breaks For All Matches //
			// check if this beats current high break
			if(highBreak > pPlr->HighBreak)
				pPlr->HighBreak = highBreak;

			// only include breaks >= 50
			if(highBreak >= 50)
				{
				if(highBreak <= 59)
					pPlr->_50s++;
				else if(highBreak <= 69)
					pPlr->_60s++;
				else if(highBreak <= 79)
					pPlr->_70s++;
				else if(highBreak <= 89)
					pPlr->_80s++;
				else if(highBreak <= 99)
					pPlr->_90s++;
				else if(highBreak >= 100)
					pPlr->_100s++;

				if(highBreak == 147)
					pPlr->Max147s++;
				}

			//////////////////////////////////////////////////////////////
			}

		if(g_build.gameType == GAMETYPE_POOL)
		if(!pPlr->bIsGuest)
		if(!pPlrOpponent->bIsGuest)
			{
			if(highBreak == 50 || highBreak == 51 || highBreak == 52)	GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 1, 1);		  // CueError=1 for 8-Ball
			else if(highBreak == 60 || highBreak == 61 || highBreak == 62) GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 2, 1);  // CueError=2 for 9-Ball
			else if(highBreak == 70 || highBreak == 71 || highBreak == 72) GAMEAPPVIEW->SendHighBreak(FALSE, pPlr->playerSocketIdx, highBreak, 3, 1);  // CueError=3 for UK-Pool
			
			
			// Update Run-Out Stats //
			if(highBreak == 50) pPlr->_50s++;
			else if(highBreak == 51) pPlr->_80s++;
			else if(highBreak == 52) pPlr->_p60s++;

			else if(highBreak == 60) pPlr->_60s++;
			else if(highBreak == 61) pPlr->_90s++;
			else if(highBreak == 62) pPlr->_p70s++;

			else if(highBreak == 70) pPlr->_70s++;
			else if(highBreak == 71) pPlr->_p50s++;
			else if(highBreak == 72) pPlr->_p80s++;

			}
			
	}
}

/*
BOOL CLobby::HighBreakReceive(int plrSockIdx, int highBreak, int cueError, int high4)
{

	BOOL bRetval = FALSE;

	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1)
		return FALSE;

	if(g_build.gameType == GAMETYPE_POOL)
	{
		int statColumn = 0;
		CServerPlayerStats plrStats;
		plrStats.Clear();

		strcpy(plrStats.name, m_player[plrIdx].name);
		if(highBreak == 50)
		{
			statColumn = 0;
			plrStats.runOuts8ball = 1;
		}
		else if(highBreak == 60)
		{
			statColumn = 1;
			plrStats.runOuts9ball = 1;
		}
		else if(highBreak == 80)
		{
			statColumn = 2;
			plrStats.runOutsUKPool = 1;
		}
		else
			return FALSE;
		
		plrStats = GAMEAPPVIEW->m_serverAllPlayersStats->AddPlrStats(plrStats);

	
		//m_plrList = new CServerPlayerStats[g_gameAppView->m_serverAllPlayersStats->m_plrList_n];
	
		
		if(plrStats.name[0] == 0) // failed to add player to list. (Check size of list)
			return FALSE;

		if(statColumn == 0)
			highBreak = plrStats.runOuts8ball;
		else if(statColumn == 1)
			highBreak = plrStats.runOuts9ball;
		else if(statColumn == 2)
			highBreak = plrStats.runOutsUKPool;
		


		int i,n;
		int MostRunouts;
		char *MostRunout_Checked;
		int MostRunouts_idx;
		
		MostRunout_Checked = new char[g_gameAppView->m_serverAllPlayersStats->m_plrList_n];
		for(n=0;n<(g_gameAppView->m_serverAllPlayersStats->m_plrList_n);n++) MostRunout_Checked[n] = 0;
			



		// Update m_sLobbyHighBreak and m_sLobbyHighBreakName with the Top 3 MostRunouts //
		if(statColumn == 0) // 8ball runouts
			{
			if(highBreak>GAMEAPPVIEW->m_sLobbyHighBreak[0]) bRetval = TRUE;
 			for(n=0;n<g_gameAppView->m_serverAllPlayersStats->m_plrList_n;n++) MostRunout_Checked[n]=0; // Clear the Checked List //

			for(i=0;i<3;i++)
				{
				MostRunouts = 0;
				MostRunouts_idx = 0;
				// Find the MostRunouts name/number that hasn't been checked
				for(n=0;n<g_gameAppView->m_serverAllPlayersStats->m_plrList_n;n++)	
					{
					if(MostRunout_Checked[n]==0)
					if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].name[0] != 0)
					if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts8ball>MostRunouts) 
						{
						MostRunouts = g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts8ball;
						MostRunouts_idx = n;
						}
					}
				// MostRunouts is the next highest number of runouts in the list
				MostRunout_Checked[MostRunouts_idx] = 1; // Don't want to Check this again
				
				// Update Lobby Stats, if we found someone with RunOuts //
				if(MostRunouts>0)
					{
					GAMEAPPVIEW->m_sLobbyHighBreak[i] = MostRunouts;
					GAMEAPPVIEW->m_sLobbyHighBreakName[i] = g_gameAppView->m_serverAllPlayersStats->m_plrList[MostRunouts_idx].name;
					}
				}

			
			}
		else if(statColumn == 1) // 9ball runouts
			{
			if(highBreak>GAMEAPPVIEW->m_sLobbyPHighBreak[0]) bRetval = TRUE;
			

			for(i=0;i<3;i++)
				{
				MostRunouts = 0;
				MostRunouts_idx = 0;
				// Find the MostRunouts name/number that hasn't been checked
				for(n=0;n<g_gameAppView->m_serverAllPlayersStats->m_plrList_n;n++)	
					{
					if(MostRunout_Checked[n]==0)
					if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].name[0] != 0)
					if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts9ball>MostRunouts) 
						{
						MostRunouts = g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts9ball;
						MostRunouts_idx = n;
						}
					}
				// MostRunouts is the next highest number of runouts in the list
				MostRunout_Checked[MostRunouts_idx] = 1; // Don't want to Check this again
				
				// Update Lobby Stats, if we found someone with RunOuts //
				if(MostRunouts>0)
					{
					GAMEAPPVIEW->m_sLobbyPHighBreak[i] = MostRunouts;
					GAMEAPPVIEW->m_sLobbyPHighBreakName[i] = g_gameAppView->m_serverAllPlayersStats->m_plrList[MostRunouts_idx].name;
					}
				}
			}
		if(statColumn == 2) // UK Pool runouts
			{
			if(highBreak>GAMEAPPVIEW->m_sLobbyUKHighBreak[0]) bRetval = TRUE;
			
			for(i=0;i<3;i++)
				{
				MostRunouts = 0;
				MostRunouts_idx = 0;
				// Find the MostRunouts name/number that hasn't been checked

				for(n=0;n<(g_gameAppView->m_serverAllPlayersStats->m_plrList_n);n++)	
					{
					if(MostRunout_Checked[n]==0)
					if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].name[0] != 0)
					if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOutsUKPool>MostRunouts) 
						{
						MostRunouts = g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOutsUKPool;
						MostRunouts_idx = n;
						}
					}
				// MostRunouts is the next highest number of runouts in the list
				MostRunout_Checked[MostRunouts_idx] = 1; // Don't want to Check this again
				
				// Update Lobby Stats, if we found someone with RunOuts //
				if(MostRunouts>0)
					{
					GAMEAPPVIEW->m_sLobbyUKHighBreak[i] = MostRunouts;
					GAMEAPPVIEW->m_sLobbyUKHighBreakName[i] = g_gameAppView->m_serverAllPlayersStats->m_plrList[MostRunouts_idx].name;
					}
				}
			}

		if(MostRunout_Checked) delete MostRunout_Checked;


		return bRetval;   
	}

	// iSnooker only...
	if(cueError == 1) // amatuer high breaks
	{
		// shuffle high breaks downwards...
		if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[0])
		{
			if(high4 == 1)
				bRetval = TRUE;

			GAMEAPPVIEW->m_sLobbyHighBreak[2] = GAMEAPPVIEW->m_sLobbyHighBreak[1];
			GAMEAPPVIEW->m_sLobbyHighBreakName[2] = GAMEAPPVIEW->m_sLobbyHighBreakName[1];

			GAMEAPPVIEW->m_sLobbyHighBreak[1] = GAMEAPPVIEW->m_sLobbyHighBreak[0];
			GAMEAPPVIEW->m_sLobbyHighBreakName[1] = GAMEAPPVIEW->m_sLobbyHighBreakName[0];
			
			GAMEAPPVIEW->m_sLobbyHighBreak[0] = highBreak;
			GAMEAPPVIEW->m_sLobbyHighBreakName[0] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[1])
		{
			GAMEAPPVIEW->m_sLobbyHighBreak[2] = GAMEAPPVIEW->m_sLobbyHighBreak[1];
			GAMEAPPVIEW->m_sLobbyHighBreakName[2] = GAMEAPPVIEW->m_sLobbyHighBreakName[1];		

			GAMEAPPVIEW->m_sLobbyHighBreak[1] = highBreak;
			GAMEAPPVIEW->m_sLobbyHighBreakName[1] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[2])
		{
			GAMEAPPVIEW->m_sLobbyHighBreak[2] = highBreak;
			GAMEAPPVIEW->m_sLobbyHighBreakName[2] = m_player[plrIdx].name;
		}
	}
	else if(cueError == 2) // pro high breaks
	{
		// shuffle high breaks downwards...
		if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[0])
		{
			if(high4 == 1)
				bRetval = TRUE;

			GAMEAPPVIEW->m_sLobbyPHighBreak[2] = GAMEAPPVIEW->m_sLobbyPHighBreak[1];
			GAMEAPPVIEW->m_sLobbyPHighBreakName[2] = GAMEAPPVIEW->m_sLobbyPHighBreakName[1];

			GAMEAPPVIEW->m_sLobbyPHighBreak[1] = GAMEAPPVIEW->m_sLobbyPHighBreak[0];
			GAMEAPPVIEW->m_sLobbyPHighBreakName[1] = GAMEAPPVIEW->m_sLobbyPHighBreakName[0];
			
			GAMEAPPVIEW->m_sLobbyPHighBreak[0] = highBreak;
			GAMEAPPVIEW->m_sLobbyPHighBreakName[0] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[1])
		{
			GAMEAPPVIEW->m_sLobbyPHighBreak[2] = GAMEAPPVIEW->m_sLobbyPHighBreak[1];
			GAMEAPPVIEW->m_sLobbyPHighBreakName[2] = GAMEAPPVIEW->m_sLobbyPHighBreakName[1];		

			GAMEAPPVIEW->m_sLobbyPHighBreak[1] = highBreak;
			GAMEAPPVIEW->m_sLobbyPHighBreakName[1] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[2])
		{
			GAMEAPPVIEW->m_sLobbyPHighBreak[2] = highBreak;
			GAMEAPPVIEW->m_sLobbyPHighBreakName[2] = m_player[plrIdx].name;
		}
	}

	return bRetval;
}
*/



BOOL CLobby::HighBreakReceive(int plrSockIdx, int highBreak, int cueError, int high4)
{

	BOOL bRetval = FALSE;

	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1)
		return FALSE;

	if(g_build.gameType == GAMETYPE_POOL)
		{
		int statColumn = 0;
		CServerPlayerStats plrStats;
		plrStats.Clear();

		strcpy(plrStats.name, m_player[plrIdx].name);
		// 8 Ball //
		if(highBreak == 50)
			{
			statColumn = 0;
			plrStats.runOuts8ballStd = 1;
			}
		else if(highBreak == 51)
			{
			statColumn = 0;
			plrStats.runOuts8ballPro = 1;
			}
		else if(highBreak == 52)
			{
			statColumn = 0;
			plrStats.runOuts8ballUK = 1;
			}
		// 9 Ball //
		else if(highBreak == 60)
			{
			statColumn = 1;
			plrStats.runOuts9ballStd = 1;
			}
		else if(highBreak == 61)
			{
			statColumn = 1;
			plrStats.runOuts9ballPro = 1;
			}
		else if(highBreak == 62)
			{
			statColumn = 1;
			plrStats.runOuts9ballUK = 1;
			}
		// English Pool
		else if(highBreak == 70)
			{
			statColumn = 2;
			plrStats.runOutsEnglishStd = 1;
			}
		else if(highBreak == 71)
			{
			statColumn = 2;
			plrStats.runOutsEnglishPro = 1;
			}
		else if(highBreak == 72)
			{
			statColumn = 2;
			plrStats.runOutsEnglishUK = 1;
			}
		else
			return FALSE;
		
		plrStats = GAMEAPPVIEW->m_serverAllPlayersStats->AddPlrStats(plrStats);

	

		//m_plrList = new CServerPlayerStats[g_gameAppView->m_serverAllPlayersStats->m_plrList_n];
	
		
		if(plrStats.name[0] == 0) // failed to add player to list. (Check size of list)
			return FALSE;

		if(statColumn == 0)
			highBreak = (plrStats.runOuts8ballStd + plrStats.runOuts8ballPro + plrStats.runOuts8ballUK);
		else if(statColumn == 1)
			highBreak = (plrStats.runOuts9ballStd + plrStats.runOuts9ballPro + plrStats.runOuts9ballUK);
		else if(statColumn == 2)
			highBreak = (plrStats.runOutsEnglishStd + plrStats.runOutsEnglishPro + plrStats.runOutsEnglishUK);
		


		int i,n;
		int MostRunouts;
		char *MostRunout_Checked;
		int MostRunouts_idx;
		
		MostRunout_Checked = new char[g_gameAppView->m_serverAllPlayersStats->m_plrList_n];
		for(n=0;n<(g_gameAppView->m_serverAllPlayersStats->m_plrList_n);n++) MostRunout_Checked[n] = 0;
			



		// Update m_sLobbyHighBreak and m_sLobbyHighBreakName with the Top 3 MostRunouts //
		if(statColumn == 0) // 8ball runouts
			{
			if(highBreak>GAMEAPPVIEW->m_sLobbyHighBreak[0]) bRetval = TRUE;
 			for(n=0;n<g_gameAppView->m_serverAllPlayersStats->m_plrList_n;n++) MostRunout_Checked[n]=0; // Clear the Checked List //

			for(i=0;i<3;i++)
				{
				MostRunouts = 0;
				MostRunouts_idx = 0;
				// Find the MostRunouts name/number that hasn't been checked
				for(n=0;n<g_gameAppView->m_serverAllPlayersStats->m_plrList_n;n++)	
				if(MostRunout_Checked[n]==0)
				if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].name[0] != 0)
					{
					int TotalRunouts = g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts8ballStd;
					TotalRunouts += g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts8ballPro;
					TotalRunouts += g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts8ballUK;
					if(TotalRunouts >MostRunouts) 
						{
						MostRunouts = TotalRunouts;
						MostRunouts_idx = n;
						}
					}
				// MostRunouts is the next highest number of runouts in the list
				MostRunout_Checked[MostRunouts_idx] = 1; // Don't want to Check this again
				
				// Update Lobby Stats, if we found someone with RunOuts //
				if(MostRunouts>0)
					{
					GAMEAPPVIEW->m_sLobbyHighBreak[i] = MostRunouts;
					GAMEAPPVIEW->m_sLobbyHighBreakName[i] = g_gameAppView->m_serverAllPlayersStats->m_plrList[MostRunouts_idx].name;
					}
				}

			
			}
		else if(statColumn == 1) // 9ball runouts
			{
			if(highBreak>GAMEAPPVIEW->m_sLobbyPHighBreak[0]) bRetval = TRUE;
			

			for(i=0;i<3;i++)
				{
				MostRunouts = 0;
				MostRunouts_idx = 0;
				// Find the MostRunouts name/number that hasn't been checked
				for(n=0;n<g_gameAppView->m_serverAllPlayersStats->m_plrList_n;n++)	
				if(MostRunout_Checked[n]==0)
				if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].name[0] != 0)
					{
					int TotalRunouts = g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts9ballStd;
					TotalRunouts += g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts9ballPro;
					TotalRunouts += g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOuts9ballUK;
					
					if(TotalRunouts>MostRunouts) 
						{
						MostRunouts = TotalRunouts;
						MostRunouts_idx = n;
						}
					}
				// MostRunouts is the next highest number of runouts in the list
				MostRunout_Checked[MostRunouts_idx] = 1; // Don't want to Check this again
				
				// Update Lobby Stats, if we found someone with RunOuts //
				if(MostRunouts>0)
					{
					GAMEAPPVIEW->m_sLobbyPHighBreak[i] = MostRunouts;
					GAMEAPPVIEW->m_sLobbyPHighBreakName[i] = g_gameAppView->m_serverAllPlayersStats->m_plrList[MostRunouts_idx].name;
					}
				}
			}
		if(statColumn == 2) // UK Pool runouts
			{
			if(highBreak>GAMEAPPVIEW->m_sLobbyUKHighBreak[0]) bRetval = TRUE;
			
			for(i=0;i<3;i++)
				{
				MostRunouts = 0;
				MostRunouts_idx = 0;
				// Find the MostRunouts name/number that hasn't been checked

				for(n=0;n<(g_gameAppView->m_serverAllPlayersStats->m_plrList_n);n++)	
				if(MostRunout_Checked[n]==0)
				if(g_gameAppView->m_serverAllPlayersStats->m_plrList[n].name[0] != 0)
					{
					int TotalRunouts = g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOutsEnglishStd;
					TotalRunouts += g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOutsEnglishPro;
					TotalRunouts += g_gameAppView->m_serverAllPlayersStats->m_plrList[n].runOutsEnglishUK;

					if(TotalRunouts>MostRunouts) 
						{
						MostRunouts = TotalRunouts;
						MostRunouts_idx = n;
						}
					}
				// MostRunouts is the next highest number of runouts in the list
				MostRunout_Checked[MostRunouts_idx] = 1; // Don't want to Check this again
				
				// Update Lobby Stats, if we found someone with RunOuts //
				if(MostRunouts>0)
					{
					GAMEAPPVIEW->m_sLobbyUKHighBreak[i] = MostRunouts;
					GAMEAPPVIEW->m_sLobbyUKHighBreakName[i] = g_gameAppView->m_serverAllPlayersStats->m_plrList[MostRunouts_idx].name;
					}
				}
			}

		if(MostRunout_Checked) delete MostRunout_Checked;


		return bRetval;   
		}
	else{	
		/// Snooker High Breaks //
		if(cueError == 1)	// Semi Pro //
			{
			CServerPlayerBreak plrBreak;
			plrBreak.Clear();
			strcpy(plrBreak.name, m_player[plrIdx].name);
			plrBreak.highbreak = highBreak;
			plrBreak = GAMEAPPVIEW->m_serverAllPlayersSBreaks->AddPlrBreak(plrBreak);
			GAMEAPPVIEW->m_serverAllPlayersSBreaks->OrderBreaks();	// Now order the Breaks //

			// Once We've Updated the Breaks, we need to Update the HTML files for both SemiPro and Pro Breaks //
			GAMEAPPVIEW->m_serverAllPlayersSBreaks->UpdateHTML(g_FNamelobbyLeaderBoardA.GetBuffer(0));
			// Send Message To Clients, telling them to Update Message-Board (if they are in the Lobby)
			//GAMEAPPVIEW->RefreshLeaderBoardsClients();
			}
		else if(cueError == 2)	// Pro //
			{
			CServerPlayerBreak plrBreak;
			plrBreak.Clear();
			strcpy(plrBreak.name, m_player[plrIdx].name);
			plrBreak.highbreak = highBreak;
			plrBreak = GAMEAPPVIEW->m_serverAllPlayersPBreaks->AddPlrBreak(plrBreak);
			GAMEAPPVIEW->m_serverAllPlayersPBreaks->OrderBreaks();	// Now order the Breaks //

			// Once We've Updated the Breaks, we need to Update the HTML files for both SemiPro and Pro Breaks //
			GAMEAPPVIEW->m_serverAllPlayersPBreaks->UpdateHTML(g_FNamelobbyLeaderBoardB.GetBuffer(0));
			// Send Message To Clients, telling them to Update Message-Board (if they are in the Lobby)
			//GAMEAPPVIEW->RefreshLeaderBoardsClients();

			}

		}

	if(cueError == 1) // amatuer high breaks
	{
		// shuffle high breaks downwards...
		if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[0])
		{
			if(high4 == 1)
				bRetval = TRUE;

			GAMEAPPVIEW->m_sLobbyHighBreak[2] = GAMEAPPVIEW->m_sLobbyHighBreak[1];
			GAMEAPPVIEW->m_sLobbyHighBreakName[2] = GAMEAPPVIEW->m_sLobbyHighBreakName[1];

			GAMEAPPVIEW->m_sLobbyHighBreak[1] = GAMEAPPVIEW->m_sLobbyHighBreak[0];
			GAMEAPPVIEW->m_sLobbyHighBreakName[1] = GAMEAPPVIEW->m_sLobbyHighBreakName[0];
			
			GAMEAPPVIEW->m_sLobbyHighBreak[0] = highBreak;
			GAMEAPPVIEW->m_sLobbyHighBreakName[0] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[1])
		{
			GAMEAPPVIEW->m_sLobbyHighBreak[2] = GAMEAPPVIEW->m_sLobbyHighBreak[1];
			GAMEAPPVIEW->m_sLobbyHighBreakName[2] = GAMEAPPVIEW->m_sLobbyHighBreakName[1];		

			GAMEAPPVIEW->m_sLobbyHighBreak[1] = highBreak;
			GAMEAPPVIEW->m_sLobbyHighBreakName[1] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyHighBreak[2])
		{
			GAMEAPPVIEW->m_sLobbyHighBreak[2] = highBreak;
			GAMEAPPVIEW->m_sLobbyHighBreakName[2] = m_player[plrIdx].name;
		}
	}
	else if(cueError == 2) // pro high breaks
	{
		// shuffle high breaks downwards...
		if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[0])
		{
			if(high4 == 1)
				bRetval = TRUE;

			GAMEAPPVIEW->m_sLobbyPHighBreak[2] = GAMEAPPVIEW->m_sLobbyPHighBreak[1];
			GAMEAPPVIEW->m_sLobbyPHighBreakName[2] = GAMEAPPVIEW->m_sLobbyPHighBreakName[1];

			GAMEAPPVIEW->m_sLobbyPHighBreak[1] = GAMEAPPVIEW->m_sLobbyPHighBreak[0];
			GAMEAPPVIEW->m_sLobbyPHighBreakName[1] = GAMEAPPVIEW->m_sLobbyPHighBreakName[0];
			
			GAMEAPPVIEW->m_sLobbyPHighBreak[0] = highBreak;
			GAMEAPPVIEW->m_sLobbyPHighBreakName[0] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[1])
		{
			GAMEAPPVIEW->m_sLobbyPHighBreak[2] = GAMEAPPVIEW->m_sLobbyPHighBreak[1];
			GAMEAPPVIEW->m_sLobbyPHighBreakName[2] = GAMEAPPVIEW->m_sLobbyPHighBreakName[1];		

			GAMEAPPVIEW->m_sLobbyPHighBreak[1] = highBreak;
			GAMEAPPVIEW->m_sLobbyPHighBreakName[1] = m_player[plrIdx].name;
		}
		else if(highBreak > GAMEAPPVIEW->m_sLobbyPHighBreak[2])
		{
			GAMEAPPVIEW->m_sLobbyPHighBreak[2] = highBreak;
			GAMEAPPVIEW->m_sLobbyPHighBreakName[2] = m_player[plrIdx].name;
		}
	}

	return bRetval;
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//

// Allow High Break of Note for Unrated Games 
// BAP25 //
void CLobby::WinnerDoGameOver(int plrSockIdx, int gameOver, int highBreaks1P0, int highBreaks1P1, int highBreaks2P0, int highBreaks2P1, BOOL bInvalidGame, BOOL bRacksStillToPlay, BOOL IWon)
{
	

 	if(plrSockIdx == -1) return;
	
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return;

	
	// Disable RunOut Code for iPool //
	if(g_build.gameType == GAMETYPE_POOL)
		{
		// If Table Type is Not Std, Pro or UK then clear Run-Out stats //
		if(m_table[tblIdx].tableType>2)
			{
			highBreaks1P0 = 0;
			highBreaks1P1 = 0;
			highBreaks2P0 = 0;
			highBreaks2P1 = 0;
			}
		else{
			// Adjust highBreaks1P0 & highBreaks2P0 accordingly for Table Type //
			// Remember this valud stores the Run-Outs for iPool
			// 50 = 8Ball Std, 51 = 8Ball Pro, 52 = 8Ball UK
			// 60 = 9Ball Std, 61 = 9Ball Pro, 62 = 9Ball UK
			// 70 = English Std, 71 = English Pro, 72 = English UK
			highBreaks1P0 += m_table[tblIdx].tableType;
			highBreaks2P0 += m_table[tblIdx].tableType;

			// These are not used for iPool //
			highBreaks1P1 = 0;
			highBreaks2P1 = 0;
			}



		// If Game Type isn't 8-Ball, 9-Ball or English Pool
		if(m_table[tblIdx].gameType!=0 && m_table[tblIdx].gameType!=1 && m_table[tblIdx].gameType!=7)
			{
			highBreaks1P0 = 0;
			highBreaks1P1 = 0;
			highBreaks2P0 = 0;
			highBreaks2P1 = 0;
			}
		}

	// If UnRated, Set High Break / Runouts to 0 again //
	if(!m_table[tblIdx].bRated)
		{
		// Set High Breaks / Runouts to 0 //
		highBreaks1P0 = 0;
		highBreaks1P1 = 0;
		highBreaks2P0 = 0;
		highBreaks2P1 = 0;
		}


	if(m_table[tblIdx].bGameStarted)
	{
		BOOL bRated = TRUE;
		int v_gameOver = gameOver;

		// check this player clicked start at this table and has opponents player data
		if(!m_myself.status || !m_opponent.status)
			return;

		// ***NEW - clear both player structures of wins / breaks
		ClearPlayerNonRating(&m_myself);
		ClearPlayerNonRating(&m_opponent);

		int oldRatingMyself = m_myself.rating;
		int oldRatingOpponent = m_opponent.rating;

		BOOL bMg = FALSE;
		BOOL bOg = FALSE;
		BOOL bUpdateGamesPlayed = FALSE;

		if(m_myself.bIsGuest) bMg = TRUE;
		if(m_myself.state&PLRSTATE_GUESTMEMBER) bMg = FALSE;
		
		if(m_opponent.bIsGuest) bOg = TRUE;		
		if(m_opponent.state&PLRSTATE_GUESTMEMBER) bOg = FALSE;

		// if either player is a guest, don't give ratings
		//if(m_myself.bIsGuest || m_opponent.bIsGuest)
		if(bMg || bOg)
		{
			bRated = FALSE;
		}
		else if(GAMEAPPVIEW->m_snooker->m_customTable->m_type == CTTYPE_RESPOTBLACK)
		{
			// if players were playing a "respotted black ball game". Dont give ratings!
			bRated = FALSE;
		}
		else if(m_table[tblIdx].doubles)
			{
			// Don't allow stats or ratings for Doubles. It would require a lot of Code Restructuring
			// Month's work maybe, since the entire game it coded to work with 2 players only //
			bRated = FALSE;
			}
		else
		{

			if(!m_table[tblIdx].bRated || bInvalidGame)
			{
				bRated = FALSE;
			}


			/*
			// calc new ELO ratings
			if(m_table[tblIdx].P1StartAtTable == m_myself.playerSocketIdx)
				CalcNewELORatings(gameOver, &m_myself, &m_opponent);
			else
				CalcNewELORatings(gameOver, &m_opponent, &m_myself);
		
			// *NEW: don't update ratings: server will do this later
			m_myself.rating = oldRatingMyself;
			m_opponent.rating = oldRatingOpponent;

			/////

			if(!m_table[tblIdx].bRated || bInvalidGame)
			{
				// don't update ratings:
				m_myself.rating = oldRatingMyself;
				m_opponent.rating = oldRatingOpponent;
				
				if(m_table[tblIdx].bRated); // don't clear bRated in this case
				else bRated = FALSE; // default
			}
			//else
				//RefreshPlayerItem(); // do this later for all

			
			if(bRacksStillToPlay)
			{
				// don't update ratings:
				m_myself.rating = oldRatingMyself;
				m_opponent.rating = oldRatingOpponent;
			}
			*/


		}			

		// end of game extra msg...
		int EOGmsg = 0;
		
		if(bRated && m_table[tblIdx].bRated)
		{
			// if we are dealing with a WILD type game...
			// Only Normal Games are Rated ...
			if(!(IsTableWILD(tblIdx)))
			{
				// high breaks in snooker...
				int highest; // highest break in this frame of snooker
				int high4;
				highest = highBreaks1P0, high4 = 0;
				if(highest < highBreaks1P1) highest = highBreaks1P1, high4 = 1;
				if(highest < highBreaks2P0) highest = highBreaks2P0, high4 = 2;
				if(highest < highBreaks2P1) highest = highBreaks2P1, high4 = 3;

				
				if(m_table[tblIdx].P1StartAtTable == m_myself.playerSocketIdx)
					{			
					if(high4 == 0) GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks1P0, 1, tblIdx);
						else GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks1P0, 0, tblIdx);
					
					// Only iSnooker needs to deal with highBreaks1P1 //
					if(g_build.gameType != GAMETYPE_POOL)
						{
						if(high4 == 1) GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks1P1, 1, tblIdx);
							else GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks1P1, 0, tblIdx);
						}

					// If opponent is at the table //
					if(m_opponent.name[0] != 0)
						{
						if(high4 == 2) GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks2P0, 1, tblIdx);
							else GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks2P0, 0, tblIdx);
							
						// Only iSnooker needs to deal with highBreaks1P1 //
						if(g_build.gameType != GAMETYPE_POOL)
							{
							if(high4 == 3) GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks2P1, 1, tblIdx);
								else GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks2P1, 0, tblIdx);
							}
						}	
					}
				else{
					// If opponent is at the table //
					if(m_opponent.name[0] != 0)
						{
						if(high4 == 0) GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks1P0, 1, tblIdx);
							else GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks1P0, 0, tblIdx);
						
						// Only iSnooker needs to deal with highBreaks1P1 //
						if(g_build.gameType != GAMETYPE_POOL)	
							{
							if(high4 == 1) GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks1P1, 1, tblIdx);
								else GetHighBreak(&m_opponent, &m_myself, m_table[tblIdx].cueError, highBreaks1P1, 0, tblIdx);
							}
						}
					
					if(high4 == 2) GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks2P0, 1, tblIdx);
						else GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks2P0, 0, tblIdx);
					// Only iSnooker needs to deal with highBreaks1P1 //
					if(g_build.gameType != GAMETYPE_POOL)
						{
						if(high4 == 3) GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks2P1, 1, tblIdx);
							else GetHighBreak(&m_myself, &m_opponent, m_table[tblIdx].cueError, highBreaks2P1, 0, tblIdx);
						}
					}
				}



			////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////
			// check for golden breaks; run-outs etc...
			if(g_build.gameType == GAMETYPE_POOL)
			{
				int highBreak0 = 0;

				if(gameOver == 2) // 1P Won			
					highBreak0 = highBreaks1P0;
				else			
					highBreak0 = highBreaks2P0;

				EOGmsg = highBreak0;

				// DEBUG - REMOVE!
				//char buf[80];
				//sprintf(buf, "1) EOGmsg = %d", EOGmsg);
				//ErrorMessage(m_parent, buf);
			}
			else if(g_build.gameType == GAMETYPE_SNOOKER)
			{
				int highestBreak = 0;

				if(gameOver == 2) // 1P Won			
				{
					highestBreak = highBreaks1P0;
					if(highestBreak < highBreaks1P1)
						highestBreak = highBreaks1P1;
				}
				else
				{
					highestBreak = highBreaks2P0;
					if(highestBreak < highBreaks2P1)
						highestBreak = highBreaks2P1;
				}

				
				if(m_table[tblIdx].cueError == 1)
				{
					// only take 100+'s on Semi-Pro...
					if(highestBreak < 100)
						highestBreak = 0;
				}
				else if(m_table[tblIdx].cueError == 2)
				{
					// only take 100+'s on Pro... (was 50+'s before)
					if(highestBreak < 100)
						highestBreak = 0;
				}
				else
				{
					highestBreak = 0;
				}


				//highestBreak = 123; // DEBUG@!!!!!! REMOVE!!!!!!!!!!!

				// add skill level 0,1,2 as factors of 1000
				highestBreak += m_table[tblIdx].cueError * 1000;


				EOGmsg = highestBreak;
			}
		//////////////////////
			//////////////////////////////////////////
		////////////////////////////////////////////////////////////////
		} // end of if(bRated && m_table[tblIdx].bRated)

		// send 'GameOver!' data to all
		if(m_myself.playerSocketIdx == m_table[tblIdx].P1StartAtTable)
		{
			if(gameOver == 1)
				GAMEAPPVIEW->SendGameOver(&m_myself, &m_opponent, FALSE, bRated, bInvalidGame, tblIdx, oldRatingMyself, oldRatingOpponent, bRacksStillToPlay, EOGmsg);
			else // gameOver == 2
				GAMEAPPVIEW->SendGameOver(&m_myself, &m_opponent, TRUE, bRated, bInvalidGame, tblIdx, oldRatingMyself, oldRatingOpponent, bRacksStillToPlay, EOGmsg);
		}
		else
		{
			if(gameOver == 1)
				GAMEAPPVIEW->SendGameOver(&m_opponent, &m_myself, FALSE, bRated, bInvalidGame, tblIdx, oldRatingOpponent, oldRatingMyself, bRacksStillToPlay, EOGmsg);
			else
				GAMEAPPVIEW->SendGameOver(&m_opponent, &m_myself, TRUE, bRated, bInvalidGame, tblIdx, oldRatingOpponent, oldRatingMyself, bRacksStillToPlay, EOGmsg);
		}


	}

}


void CLobby::GameOverSaveFrame(char *name1P, char *name2P, BOOL ftp)
{
	if(name1P == NULL) return;
	if(name2P == NULL) return;

	// Replays are now always saved //
	// Save Replay to file  ->  i.e. odin_p vs carl_2000 at 2003-12-10 23h59m59s.rep
	
	char gPath[256];
	sprintf(gPath, "%s%s", g_build.savepath, "Replays\\");
	_mkdir(gPath);
	
	CString str;
	CString fname;
	CTime time;

	str.Format("%s%s vs %s at ", gPath, name1P, name2P);
	fname.Format("htdocs/Replays/%s vs %s at ", name1P, name2P);
	//time = CTime::GetCurrentTime();
	time = time.GetCurrentTime();
	str += time.Format("%Y-%m-%d %Hh%Mm%Ss");
	fname += time.Format("%Y-%m-%d %Hh%Mm%Ss");
	str += _T(".rep");
	fname += _T(".rep");

	char buffer[256];
	sprintf(buffer,"Saving game - '%s'...", str.GetBuffer(0));
	GAMEAPPVIEW->Message(buffer,10); // personal message

	

	GAMEAPPVIEW->m_replay->SaveRaw(str.GetBuffer(0));


	// create a session object to initialize WININET library
	// This will access www.isnooker.net //

	// try //
	/*
	you're right.. it would be better to write it in TCP / IP form
	Brandon - Working says:
	I might try that this week
	Colin - WWW.PLAYONLINESOCCER.COM says:
	coz i think the msserver has two socket types in it.
	Colin - WWW.PLAYONLINESOCCER.COM says:
	the standard SocketEx thing for the client/servers in the game
	Colin - WWW.PLAYONLINESOCCER.COM says:
	and also this HTTPSocket thing i made.
	Colin - WWW.PLAYONLINESOCCER.COM says:
	so you can just take that and make an FTPsocket thing.
	Colin - WWW.PLAYONLINESOCCER.COM says:
	the httpsocket thing being used for when it communicates with the gateway index.netgenie etc.
	*/

	// Only Upload to Website if the break is 100 or more //
	if(ftp)
	{
		CInternetSession sess(_T("MyProgram/1.0"));
		CFtpConnection* pConnect = NULL;
		pConnect = sess.GetFtpConnection("ftp.isnooker.net", "isnooker.net", "s21678", 21, TRUE);
		if(pConnect->PutFile(str, fname, FTP_TRANSFER_TYPE_BINARY)) ;//AfxMessageBox("Great Success2!");
		// if the connection is open, close it
		if (pConnect != NULL)
		  pConnect->Close();
		delete pConnect;
	}


}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::GameOverForAllReceive(int tblIdx, int gameOver, BOOL bForceRacksEnd, BOOL bMatchTimeUp)
{
	int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	int winnerSockIdx = -1;
	int vcash;

	if(tblIdx == -1) return;	// new: safety 6/FEB/2004

	int p=5;
	

	// Match Sure Ladder Match search is reset (if we've started a game)
	// Update Ladder for both players involved in match
	int pIdx;
	pIdx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);
	if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0;
	pIdx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);
	if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0;
	pIdx = GetPlayerIdx(m_table[tblIdx].player1bSockIdx);
	if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0;
	pIdx = GetPlayerIdx(m_table[tblIdx].player2bSockIdx);
	if (pIdx != -1) m_player[pIdx].searchgamestatus &= 0xFFFFFFF0;
	int highestBreakThisFrame = -1;
	char proceed = 0;

	if(myTblIdx == tblIdx)	// check i'm on this table
	{	
		if(m_table[tblIdx].doubles)
		{
			if( (GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P2StartAtTable) ||
				(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P1StartAtTable) ||
				(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P2bStartAtTable) ||
				(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P1bStartAtTable) ) proceed = 1;
		}
	else
	{
		if( (GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P2StartAtTable) ||
			(GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P1StartAtTable) ) proceed = 1;
		}
	}


	if(proceed)
	{	
		// Reset Find Match Ctrl
		GAMEAPPVIEW->m_findMatchCtrl->UpdateStatus(0);

		// Decrypt High Breaks, Scores //		
		
		p = 0;
		// Make Sure We are Decoded //
		GAMEAPPVIEW->m_snooker->m_customTable->Decode();		

		#ifndef THE_POOL_CLUB
		if(m_table[tblIdx].cueError != 0) // don't include beginner mode
		if(!IsTableWILD(tblIdx)) // don't include WILD types
		if(GAMEAPPVIEW->m_snooker->m_customTable->m_type != CTTYPE_RESPOTBLACK) // don't include re-spot blacks
		{


			
			//if(!IsTableWILD(tblIdx)) // don't use WILD breaks
			//{
				if( (GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P1StartAtTable) )
				{
					highestBreakThisFrame = GAMEAPPVIEW->m_snooker->m_highBreaks1P[0];
					if(GAMEAPPVIEW->m_snooker->m_highBreaks1P[1] > highestBreakThisFrame)
						highestBreakThisFrame = GAMEAPPVIEW->m_snooker->m_highBreaks1P[1];
				}
				else
				{
					highestBreakThisFrame = GAMEAPPVIEW->m_snooker->m_highBreaks2P[0];
					if(GAMEAPPVIEW->m_snooker->m_highBreaks2P[1] > highestBreakThisFrame)
						highestBreakThisFrame = GAMEAPPVIEW->m_snooker->m_highBreaks2P[1];
				}
			//}
			
			// update start frame bio averages
			if(m_table[tblIdx].cueError == 1)
			{
				g_sbioAverageBreak = highestBreakThisFrame;
				g_sbioAverageShotTime = g_bioAverageShotTime;
				g_sbioAveragePosition = g_bioAveragePosition;
				g_sbioAverageSafety = g_bioAverageSafety;
			}
			else if(m_table[tblIdx].cueError == 2)
			{
				g_pbioAverageBreak = highestBreakThisFrame;
				g_pbioAverageShotTime = g_bioAverageShotTime;
				g_pbioAveragePosition = g_bioAveragePosition;
				g_pbioAverageSafety = g_bioAverageSafety;
			}

			GAMEAPPVIEW->SendPlayerBioData(m_table[tblIdx].cueError);	
		}
		#else
		
		//{
		//char buf[256];
		//sprintf(buf, "m_highBreaks1P[1] = %d, m_highBreaks2P[1] = %d", GAMEAPPVIEW->m_snooker->m_highBreaks1P[1], GAMEAPPVIEW->m_snooker->m_highBreaks2P[1]);
		//ErrorMessage(g_wnd, buf);
		//}


		if(!IsTableWILD(tblIdx)) // don't include WILD types
		if(GAMEAPPVIEW->m_snooker->m_customTable->m_type == CTTYPE_8BALL ||
		   GAMEAPPVIEW->m_snooker->m_customTable->m_type == CTTYPE_9BALL ||
		   GAMEAPPVIEW->m_snooker->m_customTable->m_type == CTTYPE_UKPOOL) // only include 8-ball & 9-ball & UK Pool types
		{
			int highestPotsThisRack = -1;
			if( (GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P1StartAtTable) )
			{
				highestPotsThisRack = GAMEAPPVIEW->m_snooker->m_highBreaks1P[1];
			}
			else
			{
				highestPotsThisRack = GAMEAPPVIEW->m_snooker->m_highBreaks2P[1];
			}
			
			// update start frame bio averages
			int cueError = -1;
			if(GAMEAPPVIEW->m_snooker->m_customTable->m_type == CTTYPE_8BALL)
			{
				g_sbioAverageBreak = highestPotsThisRack;
				g_sbioAverageShotTime = g_bioAverageShotTime;
				g_sbioAveragePosition = g_bioAveragePosition;
				g_sbioAverageSafety = g_bioAverageSafety;
				cueError = 1; // this signifies 8-ball averages vars
			}
			else if(GAMEAPPVIEW->m_snooker->m_customTable->m_type == CTTYPE_9BALL)
			{
				g_pbioAverageBreak = highestPotsThisRack;
				g_pbioAverageShotTime = g_bioAverageShotTime;
				g_pbioAveragePosition = g_bioAveragePosition;
				g_pbioAverageSafety = g_bioAverageSafety;
				cueError = 2; // this signifies 9-ball averages vars
			}
			else if(GAMEAPPVIEW->m_snooker->m_customTable->m_type == CTTYPE_UKPOOL)
			{
				//g_pbioAverageBreak = highestPotsThisRack;
			//	g_pbioAverageShotTime = g_bioAverageShotTime;
			//	g_pbioAveragePosition = g_bioAveragePosition;
			//	g_pbioAverageSafety = g_bioAverageSafety;
				//cueError = 3; // Might Change to a new type for UK Pool **BAP**
			}
		
			//{
			//char buf[256];
			//sprintf(buf, "g_sbioAverageBreak = %f, g_pbioAverageBreak = %f", g_sbioAverageBreak, g_pbioAverageBreak);
			//ErrorMessage(g_wnd, buf);
			//}

			GAMEAPPVIEW->SendPlayerBioData(cueError);	
		}
		#endif

		// Encode again //
		GAMEAPPVIEW->m_snooker->m_customTable->Encode();
	}

	
	BOOL bTimedOut = FALSE;
	BOOL bCancelGame = FALSE;
	BOOL bRacksStillToPlay = FALSE; 
	BOOL bTournamentMatchCancel = FALSE;
	BOOL bInvalidRaceToGame = FALSE;
	

	if(gameOver > 16)	// I THINK THIS IS NOW REDUNDANT!!!!
	{
		gameOver -= 16;
		bCancelGame = TRUE;
	}
	else if(gameOver > 8)
	{
		gameOver -= 8;
		bTimedOut = TRUE;
	}
	else if(gameOver == 5)
	{
		bForceRacksEnd = FALSE;
		bTournamentMatchCancel = TRUE;
	}

	// update wins for opponents
	if(gameOver == 1) m_table[tblIdx].racksWon2P++;
	else if(gameOver == 2) m_table[tblIdx].racksWon1P++;

/*	 // derem if u want "too brief for ratings"
	// *NEW - 28/OCT/04 - 6 reds concede gives win BUG in a raceto > 1 match	
	if(!g_bTournamentLobby) // dont do for tourney lobbies!
	{
		//ErrorMessage(g_wnd, "-333a-");
		if(g_build.gameType == GAMETYPE_SNOOKER) // *TODO - fix this for pool games aswell!
		{
			//ErrorMessage(g_wnd, "-333b-");
			if( (m_table[tblIdx].racksWon1P == 1 && m_table[tblIdx].racksWon2P == 0) ||
				(m_table[tblIdx].racksWon1P == 0 && m_table[tblIdx].racksWon2P == 1) )
		}
	}
*/	

	// check for force end of game
	if(bForceRacksEnd)
	{
		if(gameOver == 1)
			m_table[tblIdx].racksWon2P = m_table[tblIdx].raceTo + 1;
		else if(gameOver == 2)
			m_table[tblIdx].racksWon1P = m_table[tblIdx].raceTo + 1;
	}

	// see if there are more Racks/Frames to play
	if(!bTournamentMatchCancel)
	if(!bTimedOut) // if this game has timed out, end it immediately (i.e. dont do the following)
	if(m_table[tblIdx].raceTo > 0) // note: 0 => 1 rack (if we are racing to 2 or more racks)
	{
		int raceTo = m_table[tblIdx].raceTo + 1;
		if((m_table[tblIdx].racksWon1P < raceTo) &&
		   (m_table[tblIdx].racksWon2P < raceTo))
		{
			bRacksStillToPlay = TRUE;
		}
	}





	#ifdef MAIN_SERVER
	
	
	
	bool bLadderMatch = false;
	if (tblIdx != -1)
	{
		bLadderMatch = m_table[tblIdx].bLadderMatch;
	}

	
	// ******************************************************************************************************
	// ------------------------------------------------------------------------------------------------------	
	// ******************************************************************************************************	
	if(g_bTournamentLobby || bLadderMatch)
	{
		// reset the player disconnect counts at end of each frame/rack
		int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
		if(matchNo != -1)
		{
			g_tournament.m_match[matchNo].m_lostConn1P = 0;
			g_tournament.m_match[matchNo].m_lostConn2P = 0;
		}
	}


	// handle tourney gameover-netgenie stuff!
	if(bTournamentMatchCancel)
	{
		// SERVER to post result back to tourney netgenie, if this is a tourney lobby?
		if (g_bTournamentLobby || bLadderMatch)
		{
			
			int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
			if(matchNo == -1)
				return;
			if(g_tournament.m_match[matchNo].m_bMatchOver)
				return;

			int tid = g_tournament.m_match[matchNo].m_tid;

			char *name1P = g_tournament.m_match[matchNo].m_name1P;
			char *name2P = g_tournament.m_match[matchNo].m_name2P;

			g_tournament.m_match[matchNo].m_bMatchOver = TRUE;
			g_tournament.m_match[matchNo].m_matchOverCount = MATCH_TIMETOCLEARTABLE;

			// If Ladder Match
			if (g_tournament.m_match[matchNo].m_bLadderMatch)
			{
				g_tournament.m_match[matchNo].m_matchOverCount = LADDERMATCH_TIMETOCLEARTABLE;
			}
			else
			{
				// report match score (if not ... Ladder Match
				GAMEAPPVIEW->ReportMatchTourney(tid, name1P, 99, name2P, 99);
			}

//			{
//			char buf[256];
//			sprintf(buf, "name1P = %s, name2P = %s", name1P, name2P);
//			ErrorMessage(((CGameAppView *)g_wnd), buf);
//			}

			{
			char buf[200];
			sprintf(buf, "Tbl#%d Both players forfeit for NOT starting on time! (%s vs %s)", tblIdx+1, name1P, name2P);
			GAMEAPPVIEW->SendTextMsg(buf, 1, -1, -1, tblIdx);
			
			

			////////////*NEW LATE PENALTY* - Only If Game hasn't started/////////////////
			if(!m_table[tblIdx].bGameStarted)
			if(g_tournament.m_match[matchNo].m_penalty > 0)
			{
				char *name1P = g_tournament.m_match[matchNo].m_name1P;
				int plr1SockIdx = FindPlayerSockIdx(name1P);
				if(plr1SockIdx != -1)
				{
					if (g_tournament.m_match[matchNo].m_bLadderMatch) sprintf(buf, "Late penalty of %d ladder pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name1P);
						else sprintf(buf, "Late penalty of %d pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name1P);
					GAMEAPPVIEW->SendTextMsg(buf, 1, plr1SockIdx, -1);
				}

				// Fine Player for no show
				if (g_tournament.m_match[matchNo].m_bLadderMatch) ServerGivePlayerPts(name1P, 0, 0, -1, true, -g_tournament.m_match[matchNo].m_penalty);
					else ServerGivePlayerPts(name1P, -g_tournament.m_match[matchNo].m_penalty, 0);
				
				
				char *name2P = g_tournament.m_match[matchNo].m_name2P;
				int plr2SockIdx = FindPlayerSockIdx(name2P);
				if(plr2SockIdx != -1)
				{
					if (g_tournament.m_match[matchNo].m_bLadderMatch) sprintf(buf, "Late penalty of %d ladder pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name2P);
						else sprintf(buf, "Late penalty of %d pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name2P);
					GAMEAPPVIEW->SendTextMsg(buf, 1, plr2SockIdx, -1);
				}

				// Fine Player for no show
				if (g_tournament.m_match[matchNo].m_bLadderMatch) ServerGivePlayerPts(name2P, 0, 0, -1, true, -g_tournament.m_match[matchNo].m_penalty);
					else ServerGivePlayerPts(name2P, -g_tournament.m_match[matchNo].m_penalty, 0);
			}				
			///////////////////////////////////////////////////////////////////
			}			
		}
	}
	else
	{ // normal end of frame scores...
		if (g_bTournamentLobby || bLadderMatch)
		{
			int racks1P = m_table[tblIdx].racksWon1P;
			int racks2P = m_table[tblIdx].racksWon2P;
			int raceTo = m_table[tblIdx].raceTo + 1;

			int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
			if(matchNo == -1)
				return;
			if(g_tournament.m_match[matchNo].m_bMatchOver)
				return;


			int tid = g_tournament.m_match[matchNo].m_tid;
			
			char *name1P = g_tournament.m_match[matchNo].m_name1P;
			char *name2P = g_tournament.m_match[matchNo].m_name2P;

			if(bRacksStillToPlay)
			{				
//				{
//					char buf[256];
//					sprintf(buf, "name1P = %s score1P = %d, name2P = %s score2P = %d", name1P, racks1P, name2P, racks2P);
//					ErrorMessage(m_parent, buf);
//				}

				// report match score...
				if (!g_tournament.m_match[matchNo].m_bLadderMatch)
				{
					GAMEAPPVIEW->ReportMatchTourney(tid, name1P, racks1P, name2P, racks2P);
				}
			}
			else
			{ 
				// RaceTo/Single match is complete!
				
				// clear match logically from server's list
				//int matchNo = g_tournament.GetMatchFromTableNo(tblIdx);
				//if(matchNo != -1)
				//{
					g_tournament.m_match[matchNo].m_bMatchOver = TRUE;
					g_tournament.m_match[matchNo].m_matchOverCount = MATCH_TIMETOCLEARTABLE;

					// If Ladder Match
					if (g_tournament.m_match[matchNo].m_bLadderMatch)
					{
						g_tournament.m_match[matchNo].m_matchOverCount = LADDERMATCH_TIMETOCLEARTABLE;
					}
				//}					
				/*	
					g_tournament.m_match[matchNo].Clear();
				*/

//					{
//					char buf[256];
//					sprintf(buf, "(*) name1P = %s score1P = %d, name2P = %s score2P = %d", name1P, racks1P, name2P, racks2P);
//					ErrorMessage(m_parent, buf);
//					}
				// report 'final' match score...
					if (!g_tournament.m_match[matchNo].m_bLadderMatch)
					{
						GAMEAPPVIEW->ReportMatchTourney(tid, name1P, racks1P, name2P, racks2P);
					}
					
				{
				char buf[200];
				if(bMatchTimeUp)
				{
					if(gameOver == 1)
						sprintf(buf,"Tbl#%d '%s' forfeits for NOT arriving/starting on time!", tblIdx+1, name1P);
					else
						sprintf(buf,"Tbl#%d '%s' forfeits for NOT arriving/starting on time!", tblIdx+1, name2P);
					GAMEAPPVIEW->SendTextMsg(buf, 1, -1, -1, tblIdx);						

				////////////*NEW LATE PENALTY* - Only If Game hasn't started/////////////////
				if(!m_table[tblIdx].bGameStarted)
				if(g_tournament.m_match[matchNo].m_penalty > 0)
				{
					if(gameOver == 1)
					{
						char *name1P = g_tournament.m_match[matchNo].m_name1P;
						int plr1SockIdx = FindPlayerSockIdx(name1P);
						if(plr1SockIdx != -1)
						{
							if (g_tournament.m_match[matchNo].m_bLadderMatch) sprintf(buf, "Late penalty of %d ladder pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name1P);
								else sprintf(buf, "Late penalty of %d pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name1P);
							GAMEAPPVIEW->SendTextMsg(buf, 1, plr1SockIdx, -1);
						}
							
						// Fine Player for no show
						if (g_tournament.m_match[matchNo].m_bLadderMatch) ServerGivePlayerPts(name1P, 0, 0, -1, true, -g_tournament.m_match[matchNo].m_penalty);
							else ServerGivePlayerPts(name1P, -g_tournament.m_match[matchNo].m_penalty, 0);
					}
					else
					{
						char *name2P = g_tournament.m_match[matchNo].m_name2P;
						int plr2SockIdx = FindPlayerSockIdx(name2P);
						if(plr2SockIdx != -1)
						{
							if (g_tournament.m_match[matchNo].m_bLadderMatch) sprintf(buf, "Late penalty of %d ladder pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name2P);
								else sprintf(buf, "Late penalty of %d pts has been taken from '%s'", g_tournament.m_match[matchNo].m_penalty, name2P);
							GAMEAPPVIEW->SendTextMsg(buf, 1, plr2SockIdx, -1);
						}
						// Fine Player for no show
						if (g_tournament.m_match[matchNo].m_bLadderMatch) ServerGivePlayerPts(name2P, 0, 0, -1, true, -g_tournament.m_match[matchNo].m_penalty);
							else  ServerGivePlayerPts(name2P, -g_tournament.m_match[matchNo].m_penalty, 0);
					}
				}				
				///////////////////////////////////////////////////////////////////						

				}
				if (g_tournament.m_match[matchNo].m_bLadderMatch) sprintf(buf, "Tbl#%d Ladder Match Result: %s (%d) vs (%d) %s", tblIdx + 1, name1P, racks1P, racks2P, name2P);
				else sprintf(buf, "Tbl#%d Match Result: %s (%d) vs (%d) %s", tblIdx+1, name1P, racks1P, racks2P, name2P);					
				GAMEAPPVIEW->SendTextMsg(buf, 1, -1, -1, tblIdx);					
				}					
				
				// If a Tournament Match (not Ladder Match)
				if (g_tournament.m_match[matchNo].m_bLadderMatch == false)
				{

					////////////////////////////////////////////////////
					// award tournament round pts if applicable here...
					if (g_tournament.m_match[matchNo].m_wpts > 0) // winner gets pts?
					{
						char buf[200];
						if (gameOver == 1) // 1P lost! So 2P wins here
						{
							vcash = g_tournament.m_match[matchNo].m_wcash;
							sprintf(buf, "Tbl#%d Winner '%s' is awarded %d pts! and $%ld.%02ld Virtual Cash!", tblIdx + 1, name2P, g_tournament.m_match[matchNo].m_wpts, vcash / 100, vcash % 100);
							GAMEAPPVIEW->SendTextMsg(buf, 1, -1, -1);
							ServerGivePlayerPts(name2P, g_tournament.m_match[matchNo].m_wpts, g_tournament.m_match[matchNo].m_wcash, 100 + g_tournament.m_match[matchNo].m_round, false);
						}
						else // 2P lost! So 1P wins here
						{
							vcash = g_tournament.m_match[matchNo].m_wcash;
							sprintf(buf, "Tbl#%d Winner '%s' is awarded %d pts! and $%ld.%02ld Virtual Cash!", tblIdx + 1, name1P, g_tournament.m_match[matchNo].m_wpts, vcash / 100, vcash % 100);
							GAMEAPPVIEW->SendTextMsg(buf, 1, -1, -1);
							ServerGivePlayerPts(name1P, g_tournament.m_match[matchNo].m_wpts, g_tournament.m_match[matchNo].m_wcash, 100 + g_tournament.m_match[matchNo].m_round, false);
						}

						// BP_FIX 13Nov05 //
						// Award the HighBreak or MostRunOut prize //
						//g_tournament.CheckGiveHighBreakPrize();
					}
					else // no pts, but round incs
					{
						char buf[200];
						if (gameOver == 1) // 1P lost! So 2P wins here
						{
							ServerGivePlayerPts(name2P, 0, 0, 100 + g_tournament.m_match[matchNo].m_round);
						}
						else // 2P lost! So 1P wins here
						{
							ServerGivePlayerPts(name1P, 0, 0, 100 + g_tournament.m_match[matchNo].m_round);
						}
					}

					if (g_tournament.m_match[matchNo].m_lpts > 0) // loser gets pts?
					{
						char buf[200];
						if (gameOver == 1) // 1P lost! So 2P wins here
						{
							vcash = g_tournament.m_match[matchNo].m_lcash;
							sprintf(buf, "Tbl#%d Loser '%s' is awarded %d pts! and $%ld.%02ld Virtual Cash!", tblIdx + 1, name1P, g_tournament.m_match[matchNo].m_lpts, vcash / 100, vcash % 100);
							GAMEAPPVIEW->SendTextMsg(buf, 1, -1, -1);

							ServerGivePlayerPts(name1P, g_tournament.m_match[matchNo].m_lpts, g_tournament.m_match[matchNo].m_lcash, g_tournament.m_match[matchNo].m_round, false);
						}
						else // 2P lost! So 1P wins here
						{
							vcash = g_tournament.m_match[matchNo].m_lcash;
							sprintf(buf, "Tbl#%d Loser '%s' is awarded %d pts! and $%ld.%02ld Virtual Cash!", tblIdx + 1, name2P, g_tournament.m_match[matchNo].m_lpts, vcash / 100, vcash % 100);
							GAMEAPPVIEW->SendTextMsg(buf, 1, -1, -1);
							ServerGivePlayerPts(name2P, g_tournament.m_match[matchNo].m_lpts, g_tournament.m_match[matchNo].m_lcash, g_tournament.m_match[matchNo].m_round, false);
						}
					}
					else // no pts, but round incs
					{
						char buf[200];
						if (gameOver == 1) // 1P lost! So 2P wins here
						{
							ServerGivePlayerPts(name1P, 0, 0, g_tournament.m_match[matchNo].m_round);
						}
						else // 2P lost! So 1P wins here
						{
							ServerGivePlayerPts(name2P, 0, 0, g_tournament.m_match[matchNo].m_round);
						}
					}


					////////////////////////////////////////////////////
					// *NEW - 7/NOV/2004 - ACCOUNT EXTENSIONS...				
					if (g_tournament.m_match[matchNo].m_waccext > 0)
					{
						if (gameOver == 1) // 1P lost! So 2P wins here
							GAMEAPPVIEW->TopUpPlayer(name2P, g_tournament.m_match[matchNo].m_waccext);
						else
							GAMEAPPVIEW->TopUpPlayer(name1P, g_tournament.m_match[matchNo].m_waccext);
					}

					if (g_tournament.m_match[matchNo].m_laccext > 0)
					{
						if (gameOver == 1) // 1P lost! So 2P wins here
							GAMEAPPVIEW->TopUpPlayer(name1P, g_tournament.m_match[matchNo].m_laccext);
						else
							GAMEAPPVIEW->TopUpPlayer(name2P, g_tournament.m_match[matchNo].m_laccext);
					}
					///////////////////////////////////////////////
				}
				
////*/
			}
		}

	}
	#endif //MAIN_SERVER



	#ifndef MAIN_SERVER
	// if i'm the winner, do the gameover code
	char IWon = 0;
	if(!bTournamentMatchCancel)
	if(!bCancelGame)
	if(myTblIdx == tblIdx)	// check i'm on this table
	if( ((gameOver == 1) && (GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P2StartAtTable)) ||
		((gameOver == 2) && (GAMEAPPVIEW->m_playerSockIdx == m_table[tblIdx].P1StartAtTable)) )
	{
		winnerSockIdx = GAMEAPPVIEW->m_playerSockIdx;
		IWon = 1;

		// stop 'pt cheats' from racking-up points (only do/give ratings pts, if at least 5 reds have been potted)
		BOOL bInvalidGame = FALSE;		
		if(!g_bTournamentLobby) // dont do for tourney lobbies!
		{			
			if(g_build.gameType == GAMETYPE_SNOOKER) // *TODO - fix this for pool games aswell!
			{
				//ErrorMessage(g_wnd, "222");
				int totalPtsLeft = GAMEAPPVIEW->m_snooker->TotalPtsLeft();
				//int ptsfor10reds = 10*8 + 27; // 107pts		
				int ptsafter5redspotted = 1000;
				switch(GAMEAPPVIEW->m_snooker->m_customTable->m_type)
				{
					case CTTYPE_SNOOKER:
						ptsafter5redspotted = 10*8 + 27; // 107pts
						break;
					case CTTYPE_SNOOKER10:
						ptsafter5redspotted = 5*8 + 27; // 67pts
						break;
					case CTTYPE_SNOOKER6:
						ptsafter5redspotted = 1*8 + 27; // 35pts
						break;
					case CTTYPE_RESPOTBLACK:
						ptsafter5redspotted = 1000;
						break;
				}
				/* // derem following to do >= 5 reds left check...
				if(totalPtsLeft > ptsafter5redspotted)
				{
					bInvalidGame = TRUE;
					
					// - *NEW: if RaceTo match and first frame, then InValid
					// - *NEW: if RaceTo match and this is 2nd, 3rd, 4th,.. etc frame then not InValid
					int frameNo = m_table[tblIdx].racksWon1P + m_table[tblIdx].racksWon2P;
					if(frameNo == 1)
					{
						bInvalidGame = TRUE;
						//bRacksStillToPlay = FALSE; // *NEW - to fix [6 Reds Concede give win in RaceTo match] 19/OCT/04 !!won't work cos var. is used below!!
					}
					else
						bInvalidGame = FALSE;
				}

				if(bInvalidRaceToGame) // *NEW 3/11/2004 - override
					bInvalidGame = TRUE;
				*/
			}
		} //!g_bTournamentLobby
		
		// Make Sure We are Decoded //
		GAMEAPPVIEW->m_snooker->m_customTable->Decode();
		//////////////////////////////////////////////////////////////////
	p++;

		
		// calc. new ratings & breaks, streaks and SEND! out data to all		
		WinnerDoGameOver(GAMEAPPVIEW->m_playerSockIdx, gameOver,
							GAMEAPPVIEW->m_snooker->m_highBreaks1P[0],
							GAMEAPPVIEW->m_snooker->m_highBreaks1P[1],
							GAMEAPPVIEW->m_snooker->m_highBreaks2P[0],
							GAMEAPPVIEW->m_snooker->m_highBreaks2P[1],
							bInvalidGame, bRacksStillToPlay, IWon);

		// Encode Again //
		GAMEAPPVIEW->m_snooker->m_customTable->Encode();
				

		// display nag 'register' dialogue, if this winner is a guest
		int myIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);
		if(myIdx!=-1)
		if(m_player[myIdx].bIsGuest)
		{
			GAMEAPPVIEW->m_guestRegisterDlg.DestroyWindow();
			GAMEAPPVIEW->m_guestRegisterDlg.Create(IDD_GUESTREGISTERDLG);
			GAMEAPPVIEW->m_guestRegisterDlg.ShowWindow(SW_SHOW);
		}
	}



	// game over - so save frame to file	
	int myIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);

	// If I'm a Registered Member, then Save History //
	if(m_player[myIdx].bIsGuest == FALSE)
	if(!(m_player[myIdx].state&PLRSTATE_GUESTMEMBER))
	if(myTblIdx == tblIdx)	// check i'm on this table
		{
		// Update History File (Rated Games Only) //
		int gameinfo = 0;	
		
		
	if(g_build.gameType == GAMETYPE_SNOOKER)
		{		
		// BIT 1	- Won or Lost	
		gameinfo |= IWon;
		// BITS 2-5 - Game Type (16 possible game types)
		gameinfo |= ((m_table[tblIdx].gameType) << 1);
		// BITS 6-7 - Cue Error (4 possible Values)
		gameinfo |= ((m_table[tblIdx].cueError) << 5);
		// BIT 8	- Doubles
		gameinfo |= ((m_table[tblIdx].doubles) << 7);
		// BIT 9	- Conceded
		gameinfo |= (g_conceded_game << 8);
		// BIT 10	- Miss Rule / CallShots
		gameinfo |= (m_table[tblIdx].bMissRule << 9);
		// Bits 11-16  (6 Bits Reserved for Later Use //

		GAMEAPPVIEW->m_snooker->m_customTable->Decode();
		// Bits 17-24 (Frame Score of Myself) //
		gameinfo |= ((g_gameAppView->m_snooker->m_score1P) << 16);
		// Bits 25-32 (Frame Score of Opponent) //
		gameinfo |= ((g_gameAppView->m_snooker->m_score2P) << 24);
		GAMEAPPVIEW->m_snooker->m_customTable->Encode();
		}
	else{
		// BIT 1	- Won or Lost	
		gameinfo |= IWon;
		// BITS 2-5 - Game Type (16 possible game types)
		gameinfo |= ((m_table[tblIdx].gameType) << 1);
		// BITS 6-7 - Table Type (4 possible Values: Normal, Pro, UKPool, Wild)
		gameinfo |= ( ((m_table[tblIdx].tableType)&0x03) << 5);
		// BIT 8	- Doubles
		gameinfo |= ((m_table[tblIdx].doubles) << 7);
		// BIT 9	- Conceded
		gameinfo |= (g_conceded_game << 8);
		// BIT 10	- Miss Rule / CallShots
		gameinfo |= (m_table[tblIdx].bMissRule << 9);
		// Bits 11-16  (6 Bits Reserved for Later Use //
		/*
		GAMEAPPVIEW->m_snooker->m_customTable->Decode();
		// Bits 17-24 (Frame Score of Myself) //
		gameinfo |= ((g_gameAppView->m_snooker->m_score1P) << 16);
		// Bits 25-32 (Frame Score of Opponent) //
		gameinfo |= ((g_gameAppView->m_snooker->m_score2P) << 24);
		GAMEAPPVIEW->m_snooker->m_customTable->Encode();
		*/
		}
		CTime m_time;
		m_time = g_gameAppView->GetServerTime();

		CTime t1970(0);
		//GetCurrentTime();
		CTimeSpan diff;
		diff = m_time-t1970;


		/*ss
		CTime m_time;
		m_time = g_gameAppView->GetServerTime();
		CTime t1970(1970,1,1,0,0,0);
		CTimeSpan diff;
		diff = m_time-t1970;
		*/
		
		/*
		CString timestr;
		char buff[80];
		timestr = m_time.Format("%d-%m-%Y %H:%M:%S (GMT)");
		sprintf(buff,"*** %s ***", timestr);
		GAMEAPPVIEW->Message(buff,10); // personal message

		*/
		g_gameAppView->SendUpdateHistory(m_myself.name, m_opponent.name, gameinfo, diff.GetTotalSeconds());
		}


	if(myIdx!=-1)
	if(!m_player[myIdx].bIsGuest)
	if(myTblIdx == tblIdx)	// check i'm on this table
	if(!IsTableWILD(tblIdx)) // only save non-WILD types
	if(g_gfxDetails.bAutoSaveFrames)
	{
		
		char name1P[22];
		char name2P[22];
		int plr1Idx;
		int plr2Idx;

		/*
		plr1Idx = GetPlayerIdx(m_table[tblIdx].P1StartAtTable);
		if(plr1Idx == -1) name1P[0] = 0;
			else strcpy(name1P, m_player[plr1Idx].name);

		plr2Idx = GetPlayerIdx(m_table[tblIdx].P2StartAtTable);
		if(plr2Idx == -1) name2P[0] = 0;
			else strcpy(name2P, m_player[plr2Idx].name);
		*/

		GameOverSaveFrame(m_table[tblIdx].P1StartName, m_table[tblIdx].P2StartName, FALSE);	// Don't Upload Frame //

/*
		// If we are seated (playing) then need to send our high break to the website if it was over 100 //
		BOOL seated = g_gameAppView->m_lobby->SeatedAtTableIdx(g_gameAppView->m_playerSockIdx, tblIdx);
		if(seated)
			{	
			// Only the Winner of this frame will send the High Break to the Server / Website if it's 100 or more //
			BOOL ftp = FALSE;
			if(highestBreakThisFrame >= 100 && g_ftpupload) ftp = TRUE;
			if(strcmpi(m_myself.name, m_player[plr1Idx].name) == 0)	GameOverSaveFrame(m_myself.name, m_opponent.name, ftp);
				else GameOverSaveFrame(m_opponent.name, m_myself.name, ftp);
			}
		else{
			GameOverSaveFrame(name1P, name2P, FALSE);	// Don't Upload Frame //
			}
*/
		
	}
	#endif //!MAIN_SERVER

	

	// clear some vars...
	if(!bRacksStillToPlay)
	{
		// ***for all***
		m_table[tblIdx].bGameStarted = FALSE;
		m_table[tblIdx].P1StartAtTable = -1;
		m_table[tblIdx].P2StartAtTable = -1;

		m_table[tblIdx].P1bStartAtTable = -1;
		m_table[tblIdx].P2bStartAtTable = -1;



		m_table[tblIdx].bReRackRequest = FALSE;

		m_table[tblIdx].bLockSeats = FALSE;
		GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*4+2, TRUE);
		GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*4+3, TRUE);

		m_table[tblIdx].P1StartName[0] = 0;
		m_table[tblIdx].P2StartName[0] = 0;
		m_table[tblIdx].P1bStartName[0] = 0;
		m_table[tblIdx].P2bStartName[0] = 0;
		m_table[tblIdx].m_lostConn1P = 0;
		m_table[tblIdx].m_lostConn2P = 0;

		TableLostConnectionFlag[tblIdx] = 0;


		//m_table[tblIdx].racksWon1P = 0;
		//m_table[tblIdx].racksWon2P = 0;

		// if this is a tourney table, go into toureny-state 3 to stop players starting another match/game!
		if(m_table[tblIdx].tournament == 2)
			m_table[tblIdx].tournament = 3;
	}

	
	// *NEW -  reset scores, ready for next frame
	m_table[tblIdx].score1P = m_table[tblIdx].startScore1P;
	m_table[tblIdx].score2P = m_table[tblIdx].startScore2P;

	// Alow ReRacking once for this new Frame //
	m_table[tblIdx].bCanRerack = TRUE;

	//////////////////////////////////////////
	if(m_table[tblIdx].whoBreaks == 2)
	{
		// Alternate
		if(m_table[tblIdx].bSwitchBreakTo1P) m_table[tblIdx].bSwitchBreakTo1P = FALSE;
		else m_table[tblIdx].bSwitchBreakTo1P = TRUE;
	}
	else if(m_table[tblIdx].whoBreaks == 3)
	{
		// Winner
		if(gameOver == 1) m_table[tblIdx].bSwitchBreakTo1P = FALSE;
		else if(gameOver == 2) m_table[tblIdx].bSwitchBreakTo1P = TRUE;
	}
	else if(m_table[tblIdx].whoBreaks == 4)
	{
		// Loser
		if(gameOver == 1) m_table[tblIdx].bSwitchBreakTo1P = TRUE;
		else if(gameOver == 2) m_table[tblIdx].bSwitchBreakTo1P = FALSE;
	}
	else if(m_table[tblIdx].whoBreaks == 5)
	{
		// Player Trailing
		BOOL bTrail1P = FALSE;
		if(m_table[tblIdx].racksWon1P > m_table[tblIdx].racksWon2P) bTrail1P = FALSE;
		else if(m_table[tblIdx].racksWon2P > m_table[tblIdx].racksWon1P) bTrail1P = TRUE;
		else // =
		{
			if(gameOver == 1) bTrail1P = FALSE; // 1P lost - i.e. 2P has just caight 1P and still trailing
			else if(gameOver == 2) bTrail1P = TRUE; // 1P lost - i.e. 2P has just caight 1P and still trailing
		}
		
		if(bTrail1P) m_table[tblIdx].bSwitchBreakTo1P = TRUE;
		else m_table[tblIdx].bSwitchBreakTo1P = FALSE;
	}
	//////////////////////////////////////////



	// ******************************************************************************************************
	// ------------------------------------------------------------------------------------------------------	
	// ******************************************************************************************************	
	
	// check i'm on this table before going any further
	if(myTblIdx != tblIdx)
		return;

	////////////////////////////////////////////////////////
	if(bRacksStillToPlay)
	{
		// clear game over vars
		#ifndef MAIN_SERVER
		GAMEAPPVIEW->m_snooker->m_bConcededGame = FALSE;
		GAMEAPPVIEW->m_snooker->m_gameOver = 0;
		GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;//-1;

		// ensure mouse cursor is visible (just in-case someone is mid-way thru the line-up process)
		GAMEAPPVIEW->FreeMouse();
		#endif //!MAIN_SERVER

		// *TODO - winner to rerack table for next game!!
		//ErrorMessage(m_parent, "RERACK NOW!");
		if(winnerSockIdx == GAMEAPPVIEW->m_playerSockIdx)
		{
			GAMEAPPVIEW->SendReset(); // network game only!
		}

		return;
	}
	////////////////////////////////////////////////////////


	// get ready for next frame
	ClearPlayer(&m_myself);
	ClearPlayer(&m_opponent);
	m_PLCwait = 0;

	// display "Game Over!" msg for all at table
	char _buffer[MAX_CHARS];
	
	if(bCancelGame)
	{
		sprintf(_buffer,"*** GAME ABORTED! ***");
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		sprintf(_buffer,"*** PLAYER %d LOST CONNECTION WITH HOST! ***", gameOver);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}
	else if(bTimedOut)
	{
		sprintf(_buffer,"*** LOST CONNECTION WITH PLAYER %d! ***", gameOver);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}
	/*
	else if(bTournamentMatchCancel)
	{
		sprintf(_buffer,"*** MATCH ABORTED! ***");
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		sprintf(_buffer,"*** BOTH PLAYERS HAVE FAILED TO START! ***");
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}
	*/

	//if(bTournamentMatchCancel); // dont display "Game-over!"
	//else
	//{
		sprintf(_buffer,"*** Game Over! ***");
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	//}
		
	// clear game over vars
	#ifndef MAIN_SERVER

//	if(g_build.gameType == GAMETYPE_POOL)
//		if(m_table[tblIdx].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
//			GAMEAPPVIEW->m_tableSetupDlg.EnableTableCtrls(TRUE);
	//if(g_build.gameType == GAMETYPE_POOL)
	//		if(m_table[tblIdx].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
	//			GAMEAPPVIEW->m_button[BUTTON_SETUP].EnableWindow(TRUE);
	
	GAMEAPPVIEW->m_snooker->m_bConcededGame = FALSE;
	GAMEAPPVIEW->m_snooker->m_gameOver = 0;
	GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;//-1;

	// ensure mouse cursor is visible (just in-case someone is mid-way thru the line-up process)
	GAMEAPPVIEW->FreeMouse();

	// *NEW - disable stand button during game [standdisable]
	if(!g_bTournamentLobby)
		{
		//GAMEAPPVIEW->m_standButton.EnableWindow(TRUE);
		GAMEAPPVIEW-> m_standButtonEnabled = TRUE;
		}

	#endif //!MAIN_SERVER



}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::ServerGivePlayerPts(char *name, int pts, int cash, int round, bool bAnnouce, int ladderPts)
{

#ifdef MAIN_SERVER
	
	// If we're not updating any stats.. then no point going any further.
	if(cash == 0 && pts == 0 && round > 100)
	{
		return;
	}

	Player plr;
	strcpy(plr.name, name);

	if(GAMEAPPVIEW->LoadPlayerProfile(&plr, ".txt"))
	{
		int roundval = -1;
		int old;
		int oldcash;
		int oldladderpoints;

		old = plr.rating;
		oldcash = plr.cash;
		oldladderpoints = plr.ladderPoints;
		
		// Award Points and Virtual Cash
		plr.ladderPoints += ladderPts;
		plr.rating += pts; 
		plr.cash += cash;
		
		

		
		switch(round)
		{
			case 100+0:
				plr.winner++;
				break;
			case 0:
				plr.runnerUp++;
				break;
			case 1:
				plr.semiFinals++;
				break;
			case 2:
				plr.quarterFinals++;
				break;
			case 3:
				plr.last16++;
				break;
			case 4:
				plr.last32++;
				break;
		}

		GAMEAPPVIEW->SavePlayerProfile(&plr, ".txt");
		
		// Update Player Stats (if online) //
		int UplrIdx;
		UplrIdx = FindPlayerIdx(plr.name); // get player index, from that we can determine hex
		if(UplrIdx!=-1)
		{
			// update player's rating
			m_player[UplrIdx].rating = plr.rating;

			//  update player's vcash
			m_player[UplrIdx].cash = plr.cash;

			// update player's Ladder Points
			m_player[UplrIdx].ladderPoints = plr.ladderPoints;
							
			// update player's round
			switch(round)
			{
				case 100+0:
					roundval = m_player[UplrIdx].winner = plr.winner;
					break;
				case 0:
					roundval = m_player[UplrIdx].runnerUp = plr.runnerUp;
					break;
				case 1:
					roundval = m_player[UplrIdx].semiFinals = plr.semiFinals;
					break;
				case 2:
					roundval = m_player[UplrIdx].quarterFinals = plr.quarterFinals;
					break;
				case 3:
					roundval = m_player[UplrIdx].last16 = plr.last16;
					break;
				case 4:
					roundval = m_player[UplrIdx].last32 = plr.last32;
					break;
			}				

			GAMEAPPVIEW->SendPlayerChangeData(UplrIdx, PLAYERCHANGE_RATINGANDROUND, round, roundval);

			// send this player only a text msg
			if(bAnnouce)
			{
				if(m_player[UplrIdx].playerSocketIdx != -1)
				{
					char buffer[200];
					if (pts != 0)
					{
						int cash = plr.cash - oldcash;
						sprintf(buffer, "%s's new rating %d, received $%ld.%02ld (old rating %d)", plr.name, plr.rating, cash / 100, cash % 100, old);
						GAMEAPPVIEW->SendTextMsg(buffer, 0, m_player[UplrIdx].playerSocketIdx, -1);
					}
					
					if (ladderPts != 0)
					{
						sprintf(buffer, "%s's new ladder pts %d, (old ladder pts %d)", plr.name, plr.ladderPoints, oldladderpoints);
						GAMEAPPVIEW->SendTextMsg(buffer, 0, m_player[UplrIdx].playerSocketIdx, -1);

					}
				}
			}
		}
		
		{
			char str[256];
			int cash = plr.cash-oldcash;
			
			if (pts != 0)
			{
				sprintf(str, "%s Adjusted %s's rating %d, received $%ld.%02ld (old rating %d)", g_serverHandle.GetBuffer(0), name, plr.rating, cash / 100, cash % 100, old);
				AddLog("LogModerators.txt", str);
			}

			if (ladderPts != 0)
			{
				sprintf(str, "%s Adjusted %s's ladder pts %d, (old ladder pts %d)", g_serverHandle.GetBuffer(0), name, plr.ladderPoints, oldladderpoints);
				AddLog("LogModerators.txt", str);
			}
		}
	}
	
	#endif //MAIN_SERVER

}

void PlayerCopy(Player *d, Player *s, DWORD flags)
{
	if(d == NULL) return;
	if(s == NULL) return;

	if(flags&PLAYERCOPY_SYSTEM)
	{

	}

	if(flags&PLAYERCOPY_RATINGS)
	{
		d->rating = s->rating;
		d->highestrating = s->highestrating;
		//d->wins = s->wins;
		//d->losses = s->losses;
		//d->streak = s->streak;
	}
	
	if (flags&PLAYERCOPY_LADDER)
	{
		d->ladderPoints = s->ladderPoints;
		d->ladderWinsLosses = s->ladderWinsLosses;
		d->ladderHighBreak = s->ladderHighBreak;
		d->ladderRank = s->ladderRank;
	}

	if(flags&PLAYERCOPY_GAMES)
	{
		d->wins = s->wins;
		d->losses = s->losses;
		d->streak = s->streak;
		d->reracks = s->reracks;

	}

	if(flags&PLAYERCOPY_LEVEL)
	{
		d->level = s->level;
		d->level_progress = s->level_progress;

		d->cash = s->cash;
	}

	if(flags&PLAYERCOPY_BREAKS)
	{
		d->_p50s = s->_p50s;
		d->_p60s = s->_p60s;
		d->_p70s = s->_p70s;
		d->_p80s = s->_p80s;
		d->_p90s = s->_p90s;
		d->_p100s = s->_p100s;
		d->pHighBreak = s->pHighBreak;
		d->pMax147s = s->pMax147s;
		d->_50s = s->_50s;
		d->_60s = s->_60s;
		d->_70s = s->_70s;
		d->_80s = s->_80s;
		d->_90s = s->_90s;
		d->_100s = s->_100s;
		d->HighBreak = s->HighBreak;
		d->Max147s = s->Max147s;
		
		d->bioAverageBreak = s->bioAverageBreak;
		d->pBioAverageBreak = s->pBioAverageBreak;
		//d->pBioAverageShotTime = s->pBioAverageShotTime;
		//d->pBioAveragePosition = s->pBioAveragePosition;
		//d->pBioAverageSafety = s->pBioAverageSafety;
		//d->bioAverageShotTime = s->bioAverageShotTime;
		//d->bioAveragePosition = s->bioAveragePosition;
		//d->bioAverageSafety = s->bioAverageSafety;
	}
}




//PLAYERCOPY_BREAKS
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CLobby::GameOverReceive(Player *pPlr1, Player *pPlr2, BOOL bPlr1Won, BOOL bRated, BOOL bInvalidGame, int tblIdx, int oldRating1P, int oldRating2P, BOOL bRacksStillToPlay, int EOGmsg)
{
	if(pPlr1 == NULL) return;
	if(pPlr2 == NULL) return;

	CString rPeerAddress1 = "None";
	UINT rPeerPort1 = 0;
	CString rPeerAddress2 = "None";
	UINT rPeerPort2 = 0;
	BOOL ipvoid = FALSE;

	////////////////////////////////////////////////////////////////////
	int plrCopyData = PLAYERCOPY_STATS;
	if(bRacksStillToPlay) //
		plrCopyData = PLAYERCOPY_BREAKS | PLAYERCOPY_GAMES | PLAYERCOPY_LEVEL; // if still got frames to play, only copy over 'breaks' and wins, vCash & level (level includes vCash) not ratings data
	////////////////////////////////////////////////////////////////////

	// Store old Levels for each player (used in code a little further down)
	int oldPlayerLevel[2];
	int XP_start[2];
	int XP_end[2];
	int oldVCash[2];
	int oldLadderPoints[2];

	// Set all following values to default values (from pPlr structure for safety, incase we don't find the player online //
	oldPlayerLevel[0] = pPlr1->level; 
	oldPlayerLevel[1] = pPlr2->level;

	oldVCash[0] = pPlr1->cash;
	oldVCash[1] = pPlr2->cash;

	oldLadderPoints[0] = pPlr1->ladderPoints;
	oldLadderPoints[1] = pPlr2->ladderPoints;


	XP_start[0] = pPlr1->level_progress;
	XP_start[1] = pPlr2->level_progress;

	XP_end[0] = pPlr1->level_progress;
	XP_end[1] = pPlr2->level_progress;


	// (1) update winner and loser's stats logically - If Rated Game [FOR ALL!]	
	if(bRated)
	{
		int plr1Idx = FindPlayerIdx(pPlr1->name);
		if(plr1Idx != -1)
		{
			oldLadderPoints[0] = m_player[plr1Idx].ladderPoints;
			oldVCash[0] = m_player[plr1Idx].cash;
			oldPlayerLevel[0] = m_player[plr1Idx].level;
			XP_start[0] = m_player[plr1Idx].level_progress;
			PlayerCopy(&m_player[plr1Idx], pPlr1, plrCopyData);
		}

		int plr2Idx = FindPlayerIdx(pPlr2->name);
		if(plr2Idx != -1)
		{
			oldLadderPoints[1] = m_player[plr2Idx].ladderPoints;
			oldVCash[1] = m_player[plr2Idx].cash;
			oldPlayerLevel[1] = m_player[plr2Idx].level;
			XP_start[1] = m_player[plr2Idx].level_progress;
			PlayerCopy(&m_player[plr2Idx], pPlr2, plrCopyData);

		}
	}


	// refresh Ladder Stats
	GAMEAPPVIEW->m_findMatchCtrl->UpdateLadderStats();

	// refresh all player lists
	RefreshPlayerItem();
	GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
	GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();
	GAMEAPPVIEW->m_bootDlg.RefreshPlayerItem();
	GAMEAPPVIEW->RefreshListBox();

	

	



#ifdef MAIN_SERVER




	char table_info[80];

	#ifdef THE_POOL_CLUB
		/// Get Pro or Normal Mode ////
		if(m_table[tblIdx].proTable == TRUE) strcpy(table_info, "(P)");
			else strcpy(table_info, "(S)");
		/// Get CallShots or Not
		if(m_table[tblIdx].bMissRule == TRUE) strcat(table_info, "(CS)");

		/// Get the Table Type
		switch(m_table[tblIdx].tableType)
			{
			case 0: strcat(table_info, "Std>"); break;
			case 1: strcat(table_info, "Pro>"); break;
			case 2: strcat(table_info, "UK>"); break;
			case 3: strcat(table_info, "CHole>"); break;
			case 4: strcat(table_info, "BHole>"); break;
			case 5: strcat(table_info, "RHoles3>"); break;
			case 6: strcat(table_info, "RHoles5>"); break;
			case 7: strcat(table_info, "RHoles7>");	break;
			case 8: strcat(table_info, "RHoles9>");	break;
			}
		/// Get the Game Type
		switch(m_table[tblIdx].gameType)
			{
			case 0: strcat(table_info, "8Ball"); break;
			case 1: strcat(table_info, "9Ball"); break;
			case 2: strcat(table_info, "RotBall"); break;
			case 3: strcat(table_info, "Bowls"); break;
			case 4: strcat(table_info, "6Ball"); break;
			case 5: strcat(table_info, "10Ball"); break;
			case 6: strcat(table_info, "Break"); break;
			case 7: strcat(table_info, "English Pool"); break;
			}
			
	#else
		/// Get Beginner, Semi or Pro Mode ////
		if(m_table[tblIdx].cueError == 0) strcpy(table_info, "(B)");
			else if(m_table[tblIdx].cueError == 1) strcpy(table_info, "(S)");
			else strcpy(table_info, "(P)");
		/// Get Miss Rule or Not
		if(m_table[tblIdx].bMissRule == TRUE) strcat(table_info, "(M)");
		

		/// Get the Game Type
		switch(m_table[tblIdx].gameType)
			{
			case 0: strcat(table_info, "15Reds"); break;
			case 1: strcat(table_info, "10Reds"); break;
			case 2: strcat(table_info, "6Reds"); break;
			case 3: strcat(table_info, "ReSpot"); break;
			}
	#endif


	char conceded_str[16];
	if(g_conceded_game) strcpy(conceded_str, "*C*");
		else strcpy(conceded_str, "");
#else

#endif

	g_conceded_game = FALSE;




/*
	Player plr1;
	Player plr2;
	strcpy(plr1.name, pPlr1->name);
	strcpy(plr2.name, pPlr2->name);

	int loadedP1 = 0;
	int loadedP2 = 0;

	loadedP1 = GAMEAPPVIEW->LoadPlayerProfile(&plr1, ".txt");
	loadedP2 = GAMEAPPVIEW->LoadPlayerProfile(&plr2, ".txt");

	// update winner and loser's player data:
	PlayerCopy(&plr1, pPlr1, plrCopyData);
	PlayerCopy(&plr2, pPlr2, plrCopyData);
	
	// Saving is down in ShellEx (MSGGAME_GAMEOVER)
	//if(loadedP1) GAMEAPPVIEW->SavePlayerProfile(&plr1, ".txt");
	//if(loadedP2) GAMEAPPVIEW->SavePlayerProfile(&plr2, ".txt");
*/
	// (2) save new stats to file [SERVER ONLY!]
	#ifdef MAIN_SERVER

	if(g_gamelog)
	{
	GAMEAPPVIEW->GetTime();		
	
	char buffer[256];
	BOOL properwin;
	char code[5];
	char note[24];
	code[0] = 'r';
	code[1] = 'v';
	code[2] = 'f';
	code[3] = 'l';
	code[4] = 0;
	
	note[0] = 0;

	properwin = FALSE;

	///////////////////
	// Same IP Check //
	///////////////////
	// Determine players' IPs
	int sockIdx; // ### player 1's IP ###
	sockIdx = FindPlayerSockIdxi(pPlr1->name); // Get socket index of player you wish to message.noncase sensi.
	mainShell->Server->GetPeerName(rPeerAddress1, rPeerPort1, sockIdx);

	// ### player 2's IP ###
	sockIdx = FindPlayerSockIdxi(pPlr2->name); // Get socket index of player you wish to message.noncase sensi.
	mainShell->Server->GetPeerName(rPeerAddress2, rPeerPort2, sockIdx);
		
	if(!strcmpi(rPeerAddress1,rPeerAddress2))  sprintf(note,"SAMEIP");
		

	if(bRated)
	if(!bInvalidGame)
	if(!bRacksStillToPlay) properwin = TRUE;	

	if(bRated) code[0] = 'R';	// Rated
	if(!bInvalidGame) code[1] = 'V'; // Valid
	if(!bRacksStillToPlay) code[2] = 'F'; // Finished "RVF" = good, "rvf" = bad	
	if(m_table[tblIdx].bLadderMatch == 1) code[3] = 'L'; // Ladder Match

	if(properwin)
		{
		if(!bPlr1Won)
		sprintf(buffer,"%s# %s [%s]:%s:%s BEAT %s %s <=> (%s:%d) bt. (%s:%d)", note, GAMEAPPVIEW->m_stringShortTime, GAMEAPPVIEW->m_stringShortDate,
																		table_info, pPlr2->name, pPlr1->name, conceded_str,
												rPeerAddress2,rPeerPort2,rPeerAddress1,rPeerPort1);
		else sprintf(buffer,"%s# %s [%s]:%s:%s BEAT %s %s <=> (%s:%d) bt. (%s:%d)", note, GAMEAPPVIEW->m_stringShortTime, GAMEAPPVIEW->m_stringShortDate,
																		table_info, pPlr1->name, pPlr2->name, conceded_str,
												rPeerAddress1,rPeerPort1,rPeerAddress2,rPeerPort2);		
				
		}
	else { // not so important win (unrated/invalid)
		if(!bPlr1Won)
		sprintf(buffer,"%s{%s} %s [%s]:%s:%s beat %s %s <=> (%s:%d) bt. (%s:%d)", note, code, GAMEAPPVIEW->m_stringShortTime, GAMEAPPVIEW->m_stringShortDate,
																		table_info, pPlr2->name, pPlr1->name, conceded_str,
												rPeerAddress2,rPeerPort2,rPeerAddress1,rPeerPort1);
        else sprintf(buffer,"%s{%s} %s [%s]:%s:%s beat %s %s <=> (%s:%d) bt. (%s:%d)", note, code, GAMEAPPVIEW->m_stringShortTime, GAMEAPPVIEW->m_stringShortDate,
																		table_info, pPlr1->name, pPlr2->name, conceded_str,
												rPeerAddress1,rPeerPort1,rPeerAddress2,rPeerPort2);
		
		}	

	AddLog("LogGame.txt", buffer); // ### Log 
	}
	#endif
	
	if(bRated)
	if(GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx) != tblIdx) // if not at this table...
	{
		// check for golden breaks; run-outs etc... [ANNOUNCEMENTS FOR PEOPLE IN THE LOBBY & OTHER TABLES]
		if(g_build.gameType == GAMETYPE_POOL)		
		{
			char buffer[MAX_CHARS];


			// 50 = 8Ball Std, 51 = 8Ball Pro, 52 = 8Ball UK
			// 60 = 9Ball Std, 61 = 9Ball Pro, 62 = 9Ball UK
			// 70 = English Std, 71 = English Pro, 72 = English UK

			// Get the Table Type that this Run Out was made on //
			char tblstr[16];
			if((EOGmsg%10) == 0) sprintf(tblstr, "(Std Table)");
			else if((EOGmsg%10) == 1) sprintf(tblstr, "(Pro Table)");
			else if((EOGmsg%10) == 2) sprintf(tblstr, "(UK Table)");

			switch(EOGmsg)	
			{
				case 50:
				case 51:
				case 52:
					if(bPlr1Won)
						sprintf(buffer, "ANNOUNCEMENT --> %s has just 'Run-Out' the rack on 8-Ball %s, table %d!", pPlr1->name, tblstr, tblIdx+1);
					else 
						sprintf(buffer, "ANNOUNCEMENT --> %s has just 'Run-Out' the rack on 8-Ball %s, table %d!", pPlr2->name, tblstr, tblIdx+1);

					GAMEAPPVIEW->Message(buffer, 60); // personal message
					break;
				case 60:
				case 61:
				case 62:
					if(bPlr1Won)
						sprintf(buffer, "ANNOUNCEMENT --> %s has just 'Run-Out' the rack on 9-Ball %s, table %d!", pPlr1->name, tblstr, tblIdx+1);
					else
						sprintf(buffer, "ANNOUNCEMENT --> %s has just 'Run-Out' the rack on 9-Ball %s, table %d!", pPlr2->name, tblstr, tblIdx+1);

					GAMEAPPVIEW->Message(buffer, 60); // personal message
					break;
				case 70:
				case 71:
				case 72:
					if(bPlr1Won)
						sprintf(buffer, "ANNOUNCEMENT --> %s has just 'Run-Out' the rack on English Pool %s, table %d!", pPlr1->name, tblstr, tblIdx+1);
					else
						sprintf(buffer, "ANNOUNCEMENT --> %s has just 'Run-Out' the rack on English Pool %s, table %d!", pPlr2->name, tblstr, tblIdx+1);

					GAMEAPPVIEW->Message(buffer, 60); // personal message
					break;
			}
		}
		else if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			char buffer[MAX_CHARS];
			char levelStr[40];

			int level = EOGmsg / 1000;
			int highBreak = EOGmsg - level*1000;
					
			if(level == 0)
				highBreak = 0; // Dont display an announcement if this century break is on Beginner mode
			else if(level == 1)
				sprintf(levelStr, "Semi-Pro");
			else if(level == 2)
				sprintf(levelStr, "Pro");

			if(highBreak == 147)
			{
				if(bPlr1Won)
					sprintf(buffer, "ANNOUNCEMENT --> %s has just made a '147 (Maximum Break)' on %s at table %d!", pPlr1->name, levelStr, tblIdx+1);
				else 
					sprintf(buffer, "ANNOUNCEMENT --> %s has just made a '147 (Maximum Break)' on %s at table %d!", pPlr2->name, levelStr, tblIdx+1);

				GAMEAPPVIEW->Message(buffer, 60); // personal message
			}
			else if(highBreak >= 100)
			{
				if(bPlr1Won)
					sprintf(buffer, "ANNOUNCEMENT --> %s has just made a '%d (Century Break)' on %s at table %d!", pPlr1->name, highBreak, levelStr, tblIdx+1);
				else 
					sprintf(buffer, "ANNOUNCEMENT --> %s has just made a '%d (Century Break)' on %s at table %d!", pPlr2->name, highBreak, levelStr, tblIdx+1);

				GAMEAPPVIEW->Message(buffer, 60); // personal message
			}
			else if(highBreak >= 50)
			{
				if(bPlr1Won)
					sprintf(buffer, "ANNOUNCEMENT --> %s has just made a '%d (50+ Break)' on %s at table %d!", pPlr1->name, highBreak, levelStr, tblIdx+1);
				else 
					sprintf(buffer, "ANNOUNCEMENT --> %s has just made a '%d (50+ Break)' on %s at table %d!", pPlr2->name, highBreak, levelStr, tblIdx+1);

				GAMEAPPVIEW->Message(buffer, 60); // personal message
			}
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////	


	// (3) check i'm on the same table before going any further. [ONLY PEOPLE AT TABLE!]
	int myTableIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);

	int playerlevel[2];
	char buffer[256];
	int XP;

	playerlevel[0] = pPlr1->level;
	playerlevel[1] = pPlr2->level;

	
	// For Each Player
	for(int i=0;i<2;i++)
	{

		// Get the Player Idx		
		int playerSocketIdx;
		int plrIdx;
	
		if(i==0) 
		{
			plrIdx = FindPlayerIdx(pPlr1->name);
			playerSocketIdx = m_player[plrIdx].playerSocketIdx;
		}
		else
		{
			plrIdx = FindPlayerIdx(pPlr2->name);
			playerSocketIdx = m_player[plrIdx].playerSocketIdx;
		}

		// Work out how much XP we gained

		// If we levelled, then our XP_end is the previous level_progress_max + our current level_progress
		// else, XP_end if just our level_progress


		if (playerlevel[i] > oldPlayerLevel[i])
		{
			// XP_end = prev_level_progress_max + level_progress
			XP_end[i] = (int) pow(oldPlayerLevel[i], 1.5) * 100;
			if(i==0) XP_end[i] += pPlr1->level_progress;
				else XP_end[i] += pPlr2->level_progress;
				

		}
		else
		{
			if(i==0) XP_end[i] = pPlr1->level_progress;
				else XP_end[i] = pPlr2->level_progress;
		}
				


		// If we changed Level at least 5 or we are at the Table, Give Annoucement
		if (myTableIdx == tblIdx || playerlevel[i] >= 5)
		{
			if (playerlevel[i] > oldPlayerLevel[i])
			{
				if(i == 0) sprintf(buffer, "%s has reached level %d!", pPlr1->name, pPlr1->level);
					else sprintf(buffer, "%s has reached level %d!", pPlr2->name, pPlr2->level);
				g_gameAppView->Message(buffer, 40); // personal message	


				if (playerSocketIdx == g_gameAppView->m_playerSockIdx)
				{
					

					int level;
					if(i==0) level = pPlr1->level;
						else level = pPlr2->level;

					if(level & 0x1)
					{
						sprintf(buffer, "Congratulations, you can access a new Smiley!");
						g_gameAppView->Message(buffer, 40); // personal message	

						g_gameAppView->m_SmileySelectDlg.m_bRefresh = true;
						g_gameAppView->m_SmileySelectDlg.Invalidate();
					}

					switch(level)
					{
						case 3:
								// Text Color Access
								sprintf(buffer, "Congratulations, you can now access Text Colors.");
								g_gameAppView->Message(buffer, 40); // personal message

								// Update our Text Color changer button (default color)
								g_gameAppView->m_TextColorChangerDlg.UpdateColor(14803425);
								break;
	
						case 5:
								// Cue Changer Access
								sprintf(buffer, "Congratulations, you can now access the Cue Changer.");
								g_gameAppView->Message(buffer, 40); // personal message

								break;

						case 8:
								// Chalk Changer Access
								sprintf(buffer, "Congratulations, you can now access the Chalk Changer (Pink Chalk)!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 10:sprintf(buffer, "Congratulations, you can now purchase Brown Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 12:sprintf(buffer, "Congratulations, you can now purchase Grey Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 14:sprintf(buffer, "Congratulations, you can now purchase Green Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 16:sprintf(buffer, "Congratulations, you can now purchase Orange Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 18:sprintf(buffer, "Congratulations, you can now purchase Black Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 22:sprintf(buffer, "Congratulations, you can now purchase Purple Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 26:sprintf(buffer, "Congratulations, you can now purchase Red Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 32:sprintf(buffer, "Congratulations, you can now purchase Tan Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						case 36:sprintf(buffer, "Congratulations, you can now purchase White Chalk!");
								g_gameAppView->Message(buffer, 40); // personal message
								break;
						
					}
			
		
						
				}
			}
		
		}
	}



	if(myTableIdx != tblIdx) return;

	char _buffer[MAX_CHARS];
	int cash=0;

	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	if(bRated)
	{
		
		// Rated Game, so Display Message for vCash & Level XP received
		cash = pPlr1->cash - oldVCash[0];
		sprintf(_buffer,"** %s received $%ld.%02ld vCash, %ld XP **", pPlr1->name, cash/100, cash%100, (XP_end[0]-XP_start[0]));
		GAMEAPPVIEW->Message(_buffer,12); // personal message

		cash = pPlr2->cash - oldVCash[1];
		sprintf(_buffer,"** %s received $%ld.%02ld vCash, %ld XP **", pPlr2->name, cash/100, cash%100, (XP_end[1]-XP_start[1]));
		GAMEAPPVIEW->Message(_buffer,12); // personal message

		// check for golden breaks; run-outs etc... [FOR PEOPLE AT THIS TABLE]
		if(g_build.gameType == GAMETYPE_POOL)
		{
			char buffer[MAX_CHARS];

			// DEBUG - REMOVE!
			//char buf[80];
			//sprintf(buf, "4) EOGmsg = %d", EOGmsg);
			//ErrorMessage(m_parent, buf);


			// Get the Table Type that this Run Out was made on //
			char tblstr[16];
			if((EOGmsg%10) == 0) sprintf(tblstr, "(Std Table)");
			else if((EOGmsg%10) == 1) sprintf(tblstr, "(Pro Table)");
			else if((EOGmsg%10) == 2) sprintf(tblstr, "(UK Table)");
		

			switch(EOGmsg)	
			{
				case 50:
				case 51:
				case 52:
					if(bPlr1Won) sprintf(buffer, "ANNOUNCEMENT --> 8-Ball 'Run-Out' %s by %s!", tblstr, pPlr1->name);
					else sprintf(buffer, "ANNOUNCEMENT --> 8-Ball 'Run-Out' %s by %s!", tblstr, pPlr2->name);
					GAMEAPPVIEW->Message(buffer, 60); // personal message
					break;
				case 60:
				case 61:
				case 62:
					if(bPlr1Won) sprintf(buffer, "ANNOUNCEMENT --> 9-Ball 'Run-Out' %s by %s!", tblstr, pPlr1->name);
					else sprintf(buffer, "ANNOUNCEMENT --> 9-Ball 'Run-Out' %s by %s!", tblstr, pPlr2->name);
					GAMEAPPVIEW->Message(buffer, 60); // personal message
					break;
				case 70:
				case 71:
				case 72:
					if(bPlr1Won) sprintf(buffer, "ANNOUNCEMENT --> English Pool 'Run-Out' %s by %s!", tblstr, pPlr1->name);
					else sprintf(buffer, "ANNOUNCEMENT --> English Pool 'Run-Out' %s by %s!", tblstr, pPlr2->name);
					GAMEAPPVIEW->Message(buffer, 60); // personal message
					break;

		


			}
		}
		else if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			char buffer[MAX_CHARS];
			char levelStr[40];

			int level = EOGmsg / 1000;
			int highBreak = EOGmsg - level*1000;
				
			if(level == 0)
				highBreak = 0; // Dont display an announcement if this century break is on Beginner mode
			else if(level == 1)
				sprintf(levelStr, "Semi-Pro");
			else if(level == 2)
				sprintf(levelStr, "Pro");

			if(highBreak == 147)
			{
				if(bPlr1Won)
					sprintf(buffer, "ANNOUNCEMENT --> '147 (Maximum Break)' on %s by %s!", levelStr, pPlr1->name);
				else 
					sprintf(buffer, "ANNOUNCEMENT --> '147 (Maximum Break)' on %s by %s!", levelStr, pPlr2->name);

				GAMEAPPVIEW->Message(buffer, 60); // personal message
			}
			else if(highBreak >= 100)
			{
				if(bPlr1Won)
					sprintf(buffer, "ANNOUNCEMENT --> '%d (Century Break)' on %s by %s!", highBreak, levelStr, pPlr1->name);
				else 
					sprintf(buffer, "ANNOUNCEMENT --> '%d (Century Break)' on %s by %s!", highBreak, levelStr, pPlr2->name);

				GAMEAPPVIEW->Message(buffer, 60); // personal message
			}
			else if(highBreak >= 50)
			{
				if(bPlr1Won)
					sprintf(buffer, "ANNOUNCEMENT --> '%d (50+ Break)' on %s by %s!", highBreak, levelStr, pPlr1->name);
				else 
					sprintf(buffer, "ANNOUNCEMENT --> '%d (50+ Break)' on %s by %s!", highBreak, levelStr, pPlr2->name);

				GAMEAPPVIEW->Message(buffer, 60); // personal message
			}
		}
	}
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


	// only do the following if at this particular table		

	////////////////////////////////////////////////////////////////////
	int racks1P = m_table[tblIdx].racksWon1P;
	int racks2P = m_table[tblIdx].racksWon2P;
	int rackNo = racks1P + racks2P;
	int raceTo = m_table[tblIdx].raceTo + 1;
	
	
	char gameunit[16];
	if(g_build.gameType == GAMETYPE_SNOOKER) sprintf(gameunit, "frame");
		else sprintf(gameunit, "rack");

	if(bRacksStillToPlay)
	{
		// if we still have racks/frames to play:
		if(!bPlr1Won)
			sprintf(_buffer,"*** %s wins %s #%d! (Race to %d) ***", pPlr2->name, gameunit, rackNo, raceTo);
		else
			sprintf(_buffer,"*** %s wins %s #%d! (Race to %d) ***", pPlr1->name, gameunit, rackNo, raceTo);		

		GAMEAPPVIEW->Message(_buffer,10); // personal message

		sprintf(_buffer,"*** %d %s(s) > %s ***", racks1P, gameunit, pPlr1->name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		sprintf(_buffer,"*** %d %s(s) > %s ***", racks2P, gameunit, pPlr2->name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message

		// clear game over vars [prolly not needed]
		#ifndef MAIN_SERVER
		GAMEAPPVIEW->m_snooker->m_bConcededGame = FALSE;
		GAMEAPPVIEW->m_snooker->m_gameOver = 0;
		GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;//-1;

		// ensure mouse cursor is visible (just in-case someone is mid-way thru the line-up process)
		GAMEAPPVIEW->FreeMouse();
		#endif //!MAIN_SERVER

		return;
	}
	////////////////////////////////////////////////////////////////////

	if(raceTo > 1)
	{
		if(!bPlr1Won) 
			{
			if(m_table[tblIdx].doubles)	sprintf(_buffer,"*** Team 2 wins race to %d! ***", raceTo);
				else sprintf(_buffer,"*** %s wins race to %d! ***", pPlr2->name, raceTo);
			}
		else{
			if(m_table[tblIdx].doubles) sprintf(_buffer,"*** Team 1 wins race to %d! ***", raceTo);
				else sprintf(_buffer,"*** %s wins race to %d! ***", pPlr1->name, raceTo);
			}
		
		GAMEAPPVIEW->Message(_buffer,10); // personal message


		if(m_table[tblIdx].doubles) sprintf(_buffer,"*** %d %s(s) > Team 1 ***", racks1P, gameunit);
			else sprintf(_buffer,"*** %d rack(s) > %s ***", racks1P, pPlr1->name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
		if(m_table[tblIdx].doubles) sprintf(_buffer,"*** %d %s(s) > Team 2 ***", racks2P, gameunit);
		else sprintf(_buffer,"*** %d rack(s) > %s ***", racks2P, pPlr2->name);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}
	else
	{
		if(!bPlr1Won)
			{
			if(m_table[tblIdx].doubles) sprintf(_buffer,"*** Team 2 win! ***");
				else sprintf(_buffer,"*** %s wins! ***", pPlr2->name);
			}
		else{
			if(m_table[tblIdx].doubles) sprintf(_buffer,"*** Team 1 win! ***");
				else sprintf(_buffer,"*** %s wins! ***", pPlr1->name);
			}
		
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}

	if(bRated)
	{
		if(bInvalidGame)
		{
			sprintf(_buffer,"*** Game was too brief for ratings! ***");
			GAMEAPPVIEW->Message(_buffer,10); // personal message
		}
		else
		{
			BOOL bcash = FALSE;
			if (g_build.gameType == GAMETYPE_SNOOKER)
			{
				// Only Give Virtual Cash for 15 Reds or Billiards //
				if (m_table[tblIdx].gameType == 0 || m_table[tblIdx].gameType >= 4) bcash = TRUE;
				//if(m_table[tblIdx].gameType == 0) bcash = TRUE;
			}
			else
			{
				// Only Give Virtual Cash for 8-Ball, 9-Ball & English Pool Normal / Pro //
				if (m_table[tblIdx].tableType <= 2) // Std, Pro or UK Table
				if (m_table[tblIdx].gameType <= 1 || m_table[tblIdx].gameType >= 6)  bcash = TRUE; // 8-Ball or 9Ball, English Pool or Break
			}

			

			


			sprintf(_buffer, "** %s's new rating %d (old rating %d) **", pPlr1->name, pPlr1->rating, oldRating1P);
			GAMEAPPVIEW->Message(_buffer, 11); // personal message

			// If we're a member, show the ladder stats
			//m_table[tblIdx].
				
			bool bLadderResultsShownP1 = false;
			bool bLadderResultsShownP2 = false;

			int plrIdx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);
			if (plrIdx != -1)
			{
				
				if (!(m_player[plrIdx].bIsGuest))
				if (!(m_player[plrIdx].state&PLRSTATE_GUESTMEMBER))
				// if (!(pPlr1->bIsGuest))
				// if (!(pPlr1->state&PLRSTATE_GUESTMEMBER))
				{
					// Get our Division String for Player 1
					char divisionStrP1[80];
					if (pPlr1->ladderPoints < 11000) sprintf(divisionStrP1, "Bronze division");
					else if (pPlr1->ladderPoints < 14000) sprintf(divisionStrP1, "Silver division");
					else if (pPlr1->ladderPoints < 17000) sprintf(divisionStrP1, "Gold division");
					else if (pPlr1->ladderPoints < 21000) sprintf(divisionStrP1, "Platinum division");
					else sprintf(divisionStrP1, "Diamond division");


					sprintf(_buffer, "** %s's new ladder pts %d (old ladder pts %d) - %s (Rank %d) **", pPlr1->name, pPlr1->ladderPoints, oldLadderPoints[0], divisionStrP1, pPlr1->ladderRank);
					GAMEAPPVIEW->Message(_buffer, 12); // personal message

					bLadderResultsShownP1 = true;
				}
			}


			sprintf(_buffer, "** %s's new rating %d (old rating %d) **", pPlr2->name, pPlr2->rating, oldRating2P);
			GAMEAPPVIEW->Message(_buffer, 11); // personal message

			plrIdx = GetPlayerIdx(m_table[tblIdx].player2SockIdx);
			if (plrIdx != -1)
			{

				if (!(m_player[plrIdx].bIsGuest))
				if (!(m_player[plrIdx].state&PLRSTATE_GUESTMEMBER))
					//if (!(pPlr2->bIsGuest))
					//if (!(pPlr2->state&PLRSTATE_GUESTMEMBER))
				{
					// Get our Division String for Player 2
					char divisionStrP2[80];
					if (pPlr2->ladderPoints < 11000) sprintf(divisionStrP2, "Bronze division");
					else if (pPlr2->ladderPoints < 14000) sprintf(divisionStrP2, "Silver division");
					else if (pPlr2->ladderPoints < 17000) sprintf(divisionStrP2, "Gold division");
					else if (pPlr2->ladderPoints < 21000) sprintf(divisionStrP2, "Platinum division");
					else sprintf(divisionStrP2, "Diamond division");

					sprintf(_buffer, "** %s's new ladder pts %d (old ladder pts %d) - %s (Rank %d) **", pPlr2->name, pPlr2->ladderPoints, oldLadderPoints[1], divisionStrP2, pPlr2->ladderRank);
					GAMEAPPVIEW->Message(_buffer, 12); // personal message

					bLadderResultsShownP2 = true;
				}
			}

			// If Ladder Match, show ladder points
			if (m_table[tblIdx].bLadderMatch)
			{
				// If we haven't played 5 games, then don't show stats yet..
				int gamesToPlay;
				int lWins;
				int lLosses;

				lWins = (pPlr1->ladderWinsLosses & 0xFFFF0000) >> 16;
				lLosses = (pPlr1->ladderWinsLosses & 0xFFFF);
				if ((lWins + lLosses) < 5)
				{
					gamesToPlay = 5 - (lWins + lLosses);
					if (gamesToPlay == 1) sprintf(_buffer, "** %s must complete %d more Ladder game to get this seasons's starting Ladder Points & Division **", pPlr1->name, gamesToPlay);
					else sprintf(_buffer, "** %s must complete %d more Ladder game(s) to get this seasons's starting Ladder Points & Division **", pPlr1->name, gamesToPlay);
					GAMEAPPVIEW->Message(_buffer, 11); // personal message
				}
				else
				{
					// Show Ladder Stats only if not already shown above for all games
					if (!bLadderResultsShownP1)
					{
						sprintf(_buffer, "** %s's new Ladder Points %d (old Ladder Points: %d) **", pPlr1->name, pPlr1->ladderPoints, oldLadderPoints[0]);
						GAMEAPPVIEW->Message(_buffer, 11); // personal message
					}
				}
				

				lWins = (pPlr2->ladderWinsLosses & 0xFFFF0000) >> 16;
				lLosses = (pPlr2->ladderWinsLosses & 0xFFFF);
				if ((lWins + lLosses) < 5)
				{
					gamesToPlay = 5 - (lWins + lLosses);
					if (gamesToPlay == 1) sprintf(_buffer, "** %s must complete %d more Ladder game to get this seasons's starting Ladder Points & Division **", pPlr2->name, gamesToPlay);
					else sprintf(_buffer, "** %s must complete %d more Ladder game(s) to get this seasons's starting Ladder Points & Division **", pPlr2->name, gamesToPlay);
					GAMEAPPVIEW->Message(_buffer, 11); // personal message
				}
				else
				{
					// Show Ladder Stats only if not already shown above for all games
					if (!bLadderResultsShownP2)
					{
						sprintf(_buffer, "** %s's new Ladder Points %d (old Ladder Points: %d) **", pPlr2->name, pPlr2->ladderPoints, oldLadderPoints[1]);
						GAMEAPPVIEW->Message(_buffer, 11); // personal message
					}
				}
				
			}

	

			#ifndef MAIN_SERVER
			//if(gameShell) gameShell->UpdateNameList();
			GAMEAPPVIEW->RefreshListBox();
			#endif //!MAIN_SERVER
		}			
	}

	// Give Message of Invalid Game, if Not a Member vs Member / Trialist vs Trialist //
	
/*	
	if( (m_player[plr1Idx].state&PLRSTATE_GUESTMEMBER && !(m_player[plr2Idx].state&PLRSTATE_GUESTMEMBER))  || (!(m_player[plr1Idx].state&PLRSTATE_GUESTMEMBER) && m_player[plr2Idx].state&PLRSTATE_GUESTMEMBER) ) 
	{
		sprintf(_buffer,"*** Only Member Vs Member and Trialist Vs Trialist games are Rated ***");
		GAMEAPPVIEW->Message(_buffer,40); // personal message
	}
*/


	// clear game over vars [prolly not needed]
	#ifndef MAIN_SERVER
	GAMEAPPVIEW->m_snooker->m_bConcededGame = FALSE;
	GAMEAPPVIEW->m_snooker->m_gameOver = 0;
	GAMEAPPVIEW->m_snooker->m_gameOverIWon = 0;//-1;

	// ensure mouse cursor is visible (just in-case someone is mid-way thru the line-up process)
	GAMEAPPVIEW->FreeMouse();
	#endif //!MAIN_SERVER
}



// only call for tournament lobbies only [SERVER ONLY]
void CLobby::TournamentMatchTimeUp(int tblIdx, BOOL bTimeUp1P, BOOL bTimeUp2P)
{
	int gameover;

	if(bTimeUp1P && !bTimeUp2P)
		gameover = 1; // 2P wins
	else if(!bTimeUp1P && bTimeUp2P)
		gameover = 2; // 1P wins
	else
		gameover = 5; // nobody has won, score is assumed to be 0-0 cancel game

	if (tblIdx != -1)
	{
		GAMEAPPVIEW->SendGameOverForAll(tblIdx, gameover, TRUE, TRUE);
	}
}



int CLobby::GetTableStartMessageForPlayer(int plrSockIdx)
{
	
	// notes: messages returned
	// -1 = <display no message>
	//  0 = "click 'Sit' button to join game"
	//  1 = "waiting for opponent to click 'Sit' button..."
	//  2 = "click 'Start' to begin game"
	//  3 = "waiting for opponent to click 'Start' button..."
	
	int tblIdx = GetTableIdxFromPlayer(plrSockIdx);
	if(tblIdx == -1)
		return -1;
	
	#ifndef MAIN_SERVER
	if(GAMEAPPVIEW->m_snooker->m_bConcededGame) // if someone has conceded during a break, let break continue first, before displaying for following messages
		return -1;
	#endif //!MAIN_SERVER

	#ifndef MAIN_SERVER
	// if P1 is on a break, don't show a message
	if(m_table[tblIdx].player1SockIdx == -1)
		if(m_table[tblIdx].bGameStarted)
			if(!GAMEAPPVIEW->m_snooker->m_bTurn1P)
				return -1;
	#endif //!MAIN_SERVER	

	#ifndef MAIN_SERVER	
	// if P2 is on a break, don't show a message
	if(m_table[tblIdx].player2SockIdx == -1)
		if(m_table[tblIdx].bGameStarted)
			if(GAMEAPPVIEW->m_snooker->m_bTurn1P)
				return -1;
	#endif //!MAIN_SERVER

	// determine appropriate message:
	if(m_table[tblIdx].status)
	{
		
		if(!m_table[tblIdx].doubles)
		{
			// seat 1P (me)	///////////////////////////////////////////////////////////
			if(m_table[tblIdx].player1SockIdx == plrSockIdx)
			{
				if(m_table[tblIdx].player2SockIdx == -1) return 1; // if I'm sitting in seat 1P & I'm waiting for an oppenent			
				else{
					if(m_table[tblIdx].P1StartAtTable == -1) return 2; // I have an opponent, have I clicked start?
					if(m_table[tblIdx].P2StartAtTable == -1) return 3; // has opponent click started?
					}
			}
			// seat 2P (me)	///////////////////////////////////////////////////////////
			else if(m_table[tblIdx].player2SockIdx == plrSockIdx)
			{
				if(m_table[tblIdx].player1SockIdx == -1) return 1; // if I'm sitting in seat 2P & I'm waiting for an oppenent
				else{
					if(m_table[tblIdx].P2StartAtTable == -1) return 2; // I have an opponent, have I clicked start?
					if(m_table[tblIdx].P1StartAtTable == -1) return 3; // has opponent click started?
					}
			}
			// watcher (me)	///////////////////////////////////////////////////////////
			else
			{
				if(m_table[tblIdx].player1SockIdx == -1 || m_table[tblIdx].player2SockIdx == -1) return 0; // I must be a watcher. Is there a seat available?
			}
		}
		else
		{
			// seat 1P (me)	///////////////////////////////////////////////////////////
			if(m_table[tblIdx].player1SockIdx == plrSockIdx)
			{
				if(m_table[tblIdx].player2SockIdx == -1 || m_table[tblIdx].player1bSockIdx == -1 || m_table[tblIdx].player2bSockIdx == -1) return 1; // if I'm sitting in seat 1P & I'm waiting for an oppenent			
				else{
					if(m_table[tblIdx].P1StartAtTable == -1) return 2; // All players are seated, have I clicked start?
					if(m_table[tblIdx].P2StartAtTable == -1 || m_table[tblIdx].P1bStartAtTable == -1 || m_table[tblIdx].P2bStartAtTable == -1) return 3; // has opponent click started?
					}
			}
			// seat 2P (me)	///////////////////////////////////////////////////////////
			else if(m_table[tblIdx].player2SockIdx == plrSockIdx)
			{
			if(m_table[tblIdx].player1SockIdx == -1 || m_table[tblIdx].player1bSockIdx == -1 || m_table[tblIdx].player2bSockIdx == -1) return 1; // if I'm sitting in seat 1P & I'm waiting for an oppenent			
				else{
					if(m_table[tblIdx].P2StartAtTable == -1) return 2; // All players are seated, have I clicked start?
					if(m_table[tblIdx].P1StartAtTable == -1 || m_table[tblIdx].P1bStartAtTable == -1 || m_table[tblIdx].P2bStartAtTable == -1) return 3; // has opponent click started?
					}
			}
			// seat 1Pb (me)	///////////////////////////////////////////////////////////
			else if(m_table[tblIdx].player1bSockIdx == plrSockIdx)
			{
			if(m_table[tblIdx].player1SockIdx == -1 || m_table[tblIdx].player2SockIdx == -1 || m_table[tblIdx].player2bSockIdx == -1) return 1; // if I'm sitting in seat 1P & I'm waiting for an oppenent			
				else{
					if(m_table[tblIdx].P1bStartAtTable == -1) return 2; // All players are seated, have I clicked start?
					if(m_table[tblIdx].P1StartAtTable == -1 || m_table[tblIdx].P2StartAtTable == -1 || m_table[tblIdx].P2bStartAtTable == -1) return 3; // has opponent click started?
					}
			}
			// seat 2Pb (me)	///////////////////////////////////////////////////////////
			else if(m_table[tblIdx].player2bSockIdx == plrSockIdx)
			{
			if(m_table[tblIdx].player1SockIdx == -1 || m_table[tblIdx].player2SockIdx == -1 || m_table[tblIdx].player1bSockIdx == -1) return 1; // if I'm sitting in seat 1P & I'm waiting for an oppenent			
				else{
					if(m_table[tblIdx].P2bStartAtTable == -1) return 2; // All players are seated, have I clicked start?
					if(m_table[tblIdx].P1StartAtTable == -1 || m_table[tblIdx].P2StartAtTable == -1 || m_table[tblIdx].P1bStartAtTable == -1) return 3; // has opponent click started?
					}
			}
			// watcher (me)	///////////////////////////////////////////////////////////
			else
			{
				if(m_table[tblIdx].player1SockIdx == -1 || m_table[tblIdx].player2SockIdx == -1 || m_table[tblIdx].player1bSockIdx == -1 || m_table[tblIdx].player2bSockIdx == -1) return 0; // I must be a watcher. Is there a seat available?
			}


		}
	}


	// already started - no message
	return -1;
}

void CLobby::TableHostCompleteReceive(int tblIdx)
{
	if(tblIdx == -1)
		return;

	m_table[tblIdx].bHostComplete = TRUE;
}


BOOL CLobby::JoinTableHostComplete(int tblIdx)
{
	if(tblIdx == -1)
		return FALSE;

	if(m_table[tblIdx].bHostComplete)
		return TRUE;

	return FALSE;
}


void CLobby::JoinTable(int idx, int enterType, int playerSockIdx, BOOL bIsTableHost)
{

#ifndef MAIN_SERVER

	g_iCanHandleIt = 0;


	/*
	 * Add to our TableJoinLog (local file)
	 */
	char buffer[256];

	// If this is me Joining, then need to make sure the Lobby TV isn't processing the Shot any more
	if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
		{

		// Reset Find Match Ctrl status (Server will also cancel the search for any player who is at a table)
		GAMEAPPVIEW->m_findMatchCtrl->UpdateStatus(0);

		m_bInTable = TRUE;
		GAMEAPPVIEW->m_snooker->SimEndReceive(-1);
		GAMEAPPVIEW->m_snooker->m_process = FALSE;
		GAMEAPPVIEW->m_snooker->m_firstTime=FALSE;
		if(GAMEAPPVIEW->m_snooker->m_firstFewFrames) GAMEAPPVIEW->m_snooker->m_firstFewFrames--;

		if(((CGameAppView *)m_parent)->m_elCycle&EL_BALLS)
			((CGameAppView *)m_parent)->m_elCycle^=EL_BALLS;

		#ifdef THE_POOL_CLUB
		// Make sure the sponsor text is off //
		// This is for the 2 Shot / 1 Shot msg for English Pool //
		g_gameAppView->m_sponsortxt[0] = 0;
		g_gameAppView->m_sponsoralpha = 0;
		#endif	

		// Update Camera Position (used for Demo Mode, so we get a new view //

		//float ang = rand()%628;
		//ang /= 100;
		float ang = rand()%4;
		
		

		GAMEAPPVIEW->m_TVcamera_Demo[0] = 4000.0f * cos(ang * D3DX_PI *0.5f);
		GAMEAPPVIEW->m_TVcamera_Demo[1] = 4000.0f * sin(ang * D3DX_PI *0.5f);
		GAMEAPPVIEW->m_TVcamera_Demo[2] = 1500.0f;

		GAMEAPPVIEW->Clear3DScene();

		g_tabSelected = 0;
		GAMEAPPVIEW->m_bLobbyMode = 1;
		}
	///////////////////
#endif
	
	int n = -1;


	//GAMEAPPVIEW->m_snooker->Init();

	//MessageBox(0,"$","$",MB_OK);

	// check if this player has already joined the table
	int plridx = GetPlayerIdx(playerSockIdx);
	if(plridx ==  -1)
		return;
	if(m_player[plridx].table == idx)
		return;

 	// is the table full for this player joining
	if( TableIsFull(idx, enterType) )
	{
		if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
		{
			if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx) m_bInTable = FALSE;

			// display table full message
			char _buffer[80];
			sprintf(_buffer,"Table %d is Full!", idx+1);
			GAMEAPPVIEW->Message(_buffer, 40);
		}

		return;
	}

	// if i'm the player joining
	if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
	{
		ClearPlayer(&m_myself);
		ClearPlayer(&m_opponent);
	}

	// if player is searching for a Ladder Match, make sure we cancel this and inform player. 
	// Note: we will not have to update Client (after next update) since client cancels search on it's end too
	if (m_player[plridx].searchgamestatus & 0xf)
	{
		// Make sure we've cancelled any Ladder Match search for this player, that's joining table
		m_player[plridx].searchgamestatus &= 0xfffffff0;

		// Redundand: Notify Client of cancel
		g_gameAppView->SendClientFindMatchStatus(playerSockIdx, m_player[plridx].searchgamestatus);
	}

	// join table...
	if(bIsTableHost)
	{
		// if this player is trying to host a table, check that they can host first
		m_table[idx].status = 1;
		m_table[idx].idx = idx;

		m_table[idx].bLockSeats = FALSE; // NEW: if this is an empty table, then this player is abt to host. It cant be bLocked!

		
		GAMEAPPVIEW->m_listTables.EnableItemButton(idx*4+2, TRUE);
		GAMEAPPVIEW->m_listTables.EnableItemButton(idx*4+3, TRUE);


		// find host's idx from player socket idx & set table host
		m_table[idx].hostSockIdx = playerSockIdx;
	 
		if(enterType == 1) m_table[idx].player1SockIdx = m_table[idx].hostSockIdx;
		else if(enterType == 2) m_table[idx].player2SockIdx = m_table[idx].hostSockIdx;
		else if(enterType == 3) m_table[idx].player1bSockIdx = m_table[idx].hostSockIdx;
		else if(enterType == 4) m_table[idx].player2bSockIdx = m_table[idx].hostSockIdx;
		else 
		{
			for(n=0;n<MAX_TABLE_PLAYERS;n++)
			if(m_table[idx].playersSockIdx[n] == -1)
			{
				m_table[idx].playersSockIdx[n] = m_table[idx].hostSockIdx;
				break;
			}
		}

		// add player details to table list
		AddTableItem(idx, enterType);
		
		// add table options to table list
		AddTableOptionsItem(idx);
		
		// update player details
		//int plrIdx = GetPlayerIdxFromTable(idx, enterType, n/*0*/);
		int plrIdx = GetPlayerIdx(playerSockIdx);
		if(plrIdx != -1)
		{
			m_player[plrIdx].table = idx;
			m_player[plrIdx].bInvited = TRUE;
			//m_table[idx].bReRackRequest = FALSE;

			RefreshPlayerItem();
			GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
			GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();
			GAMEAPPVIEW->m_bootDlg.RefreshPlayerItem();
			GAMEAPPVIEW->RefreshListBox();
			m_player[plrIdx].type = Player_typ::TYPE_SIMEND;
		}

		
		if (g_bTournamentLobby || m_table[idx].bLadderMatch)
		{
			bool bLadderMatch = false;
			// If not a tournament lobby check if ladder match
			if (!g_bTournamentLobby)
			{
				bLadderMatch = m_table[idx].bLadderMatch;
			}

			// if Tournament table or Ladder Match, then make this table PUBLIC, now that we have a host
			if(g_build.gameType == GAMETYPE_SNOOKER)
			{
				UpdateTableHostDetails(-1, 0, Table_typ::TYPE_PUBLIC, 0, m_player[plridx].playerSocketIdx,
													0xff, 0xff, 0xff, 0xff, 0xff,
													0xff, 0xff,
													0xff, 0xff, 0xff, 0xff, -1, 0, 0, 0, 0, bLadderMatch);
			}
			else if(g_build.gameType == GAMETYPE_POOL)
			{
				UpdateTableHostDetails(-1, 0, Table_typ::TYPE_PUBLIC, 0, m_player[plridx].playerSocketIdx,
													0xff, 0xff, 0xff, 0xff, 0xff,
													0xff, 0xff,
													0xff, 0xff, 0xff, 0xff, -1, 0, 0, 0, 0, bLadderMatch);
			}			
		}
		else
		{

			///
			// if GUEST? Restrict table settings to 6 reds:
			if(g_build.gameType == GAMETYPE_SNOOKER)
			{
				if(m_player[plridx].bIsGuest)
				{
					// default to 10 reds, for guests
					UpdateTableHostDetails(-1, 0, 0, 0, m_player[plridx].playerSocketIdx,
													20, 2, 1, 0, 0,
													0, 2,
													0, 0, 0, 0);
				}
			}
			else if(g_build.gameType == GAMETYPE_POOL)
			{
				// ^ not needed for pool, as the default is ok
			}
			///
		}

		
		// create physical table (hosts will now create a game server)
		if(playerSockIdx == GAMEAPPVIEW->m_playerSockIdx)
		{
			GAMEAPPVIEW->m_snooker->m_gameOverIWon = -1; // this player is currently the loser

			int plrIdx = GetPlayerIdx(playerSockIdx);
			if(plrIdx == -1) ErrorMessage(m_parent, "cannot find player Index 'plrIdx'");//MessageBox(0,"cannot find player Index 'plrIdx' !!Error!!","",MB_OK);
			 
			if(plrIdx != -1)
				((CGameAppView *)m_parent)->CreateGame2(TRUE, plrIdx);


			// refresh table settings...
			RefreshTableHostDetails(idx);
		}

		// Refresh seat names, only if i'm on this table
		int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
		if(myTblIdx == idx)
		{
			RefreshTableSeats(idx);
			
			char _buffer[MAX_CHARS];
			char *timestr;
			// Get (Local) time
			GAMEAPPVIEW->GetTime();
			timestr = GAMEAPPVIEW->m_stringFullTime.GetBuffer(0);
			sprintf(_buffer,"*** %s joined the table at %s ***", m_player[plrIdx].name, timestr);
			GAMEAPPVIEW->Message(_buffer,10); // personal message

			if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
			{
				sprintf(_buffer,"(Press 'F1' for game help.)");
				GAMEAPPVIEW->Message(_buffer,10); // personal message
			}
		}
	}
	else
	{
		int joinerIdx = -1;

		joinerIdx = playerSockIdx;

		if(enterType == 1)
		{
			if(m_table[idx].player1SockIdx == -1)
			{
				m_table[idx].player1SockIdx = joinerIdx;
			}
			else
			{
 				if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx) m_bInTable = FALSE;
				return;
			}
		}
		else if(enterType == 2)
		{
			if(m_table[idx].player2SockIdx == -1)
			{				
				m_table[idx].player2SockIdx = joinerIdx;
			}
			else
			{
				if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx) m_bInTable = FALSE;
				return;
			}
		}
		else if(enterType == 3)
		{
			if(m_table[idx].player1bSockIdx == -1)
			{				
				m_table[idx].player1bSockIdx = joinerIdx;
			}
			else
			{
				if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx) m_bInTable = FALSE;
				return;
			}
		}
		else if(enterType == 4)
		{
			if(m_table[idx].player2bSockIdx == -1)
			{				
				m_table[idx].player2bSockIdx = joinerIdx;
			}
			else
			{
				if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx) m_bInTable = FALSE;
				return;
			}
		}
		else 
		{
			//if(m_table[idx].playersSockIdxN == MAX_TABLE_PLAYERS)
			//	bCanJoin = FALSE;
			//else
			{
				for(n=0;n<MAX_TABLE_PLAYERS;n++)
				if(m_table[idx].playersSockIdx[n] == -1)
				{
					m_table[idx].playersSockIdx[n] = joinerIdx;
					break;
				}

				// max watches check here
				if(n == MAX_TABLE_PLAYERS)
				{
					if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx) m_bInTable = FALSE;
					return;
				}
			}
		}

		// add player details to table list
		AddTableItem(idx, enterType);

		// update player details
		//int plrIdx = GetPlayerIdxFromTable(idx, enterType, n);
		int plrIdx = GetPlayerIdx(playerSockIdx);
		if(plrIdx != -1)
		{
			m_player[plrIdx].table = idx;
			if(m_table[idx].type == Table_typ::TYPE_PUBLIC)
				m_player[plrIdx].bInvited = TRUE;
			else
				m_player[plrIdx].bInvited = FALSE; // => Table_typ::TYPE_PROTECTED
			
			//m_table[idx].bReRackRequest = FALSE;


			RefreshPlayerItem();
			GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
			GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();
			GAMEAPPVIEW->m_bootDlg.RefreshPlayerItem();
			GAMEAPPVIEW->RefreshListBox();


		
			m_player[plrIdx].type = Player_typ::TYPE_SIMEND;
		}

		

		// 'joiner' will join table, if possible?
		//if(bCanJoin)
		{
			if(playerSockIdx == GAMEAPPVIEW->m_playerSockIdx)
			{
				GAMEAPPVIEW->m_snooker->m_gameOverIWon = -1; // this player is currently the loser
							 
				int plrIdx = GetHostIdxFromTable(idx);
				if(plrIdx != -1)
					((CGameAppView *)m_parent)->CreateGame2(FALSE, plrIdx);

				// refresh table settings...
				RefreshTableHostDetails(idx);
			}		

			// if i'm the host of this table, then send over the balls [ (table)HOST -> SERVER -> (this)JOINER ]
			if(GAMEAPPVIEW->m_playerSockIdx == m_table[idx].hostSockIdx) 
			{
				GAMEAPPVIEW->SendBalls(FALSE, FALSE, playerSockIdx, TRUE); // *NEW* last param. TRUE forces the reciever to get the balls data, just in case the whiteball is selected (POSSIBLE bugfix)
			}

			// Refresh seat names, only if i'm on this table
			int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
			if(myTblIdx == idx)
			{
				RefreshTableSeats(idx);

				char _buffer[MAX_CHARS];
				char *timestr;
				// Get (Local) time
				GAMEAPPVIEW->GetTime();
				timestr = GAMEAPPVIEW->m_stringFullTime.GetBuffer(0);
				sprintf(_buffer,"*** %s joined the table at %s ***", m_player[plrIdx].name, timestr);
				GAMEAPPVIEW->Message(_buffer,10); // personal message

				if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
				{
					sprintf(_buffer,"(Press 'F1' for game help.)");
					GAMEAPPVIEW->Message(_buffer,10); // personal message
				}
			}
		}
	}			

}


void CLobby::ReJoinTable(int idx)
{
	int plrIdx;
	int tblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
	if(tblIdx==-1) return;

	// reset the ping // might not need it here as snooker init() does it also
	GAMEAPPVIEW->GamePingInit();

	// if I'm in this table && i'm not the host
	if(tblIdx == idx)
	if(m_table[tblIdx].hostSockIdx != GAMEAPPVIEW->m_playerSockIdx)
	{
		#ifndef MAIN_SERVER
		plrIdx = GetHostIdxFromTable(idx); // host of table we are 'rejoining'
		GAMEAPPVIEW->DestroyGameShell(FALSE);
		//GAMEAPPVIEW->CreateGameShell(FALSE, plrIdx);
		GAMEAPPVIEW->CreateGame(FALSE, plrIdx, TRUE);
		 /*
		// set and broadcast player profile (nick for chat msgs)
		if(gameShell)
		{
			plrIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx); // get my own player idx
			gameShell->SetNick(m_player[plrIdx].name);
			gameShell->SendProfile();
			
			if(m_table[tblIdx].player1SockIdx != -1) gameShell->SitRequest(0); // Seat #1
			else if(m_table[tblIdx].player2SockIdx != -1) gameShell->SitRequest(1); // Seat #2
		}
		else
		{
			ErrorMessage(m_parent, "ReJoiner: Unable to create GameShell!");
			//MessageBox(0,"ReJoiner: Unable to create GameShell!","",MB_OK);
		}		
		*/
		#endif //!MAIN_SERVER
	}
}


void CLobby::JoinTableFailed(int joinerSockIdx, int hostSockIdx)
{
	// 'logically' exit this table 
	ExitTable(joinerSockIdx);

	// display a system message for the host
	if(joinerSockIdx != hostSockIdx) // ensure that host has already created table
	{
		if(GAMEAPPVIEW->m_playerSockIdx == joinerSockIdx)
		{
			char buffer[MAX_CHARS];
			sprintf(buffer, "*** %s is unable to join this table [1]***", m_player[GetPlayerIdx(joinerSockIdx)].name);
			GAMEAPPVIEW->Message(buffer, 10);
			m_bInTable = FALSE;
		}
		else if(GAMEAPPVIEW->m_playerSockIdx == hostSockIdx) // if i'm(this is) the host
		{
			char buffer[MAX_CHARS];
			sprintf(buffer, "*** %s is unable to join this table [2]***", m_player[GetPlayerIdx(joinerSockIdx)].name);
			GAMEAPPVIEW->Message(buffer, 10);
			m_bInTable = FALSE; // ?? host needs this line also ??
		}
	}
	else
	{
		if(GAMEAPPVIEW->m_playerSockIdx == hostSockIdx)
		{
			// for the person trying to be the host
			char buffer[MAX_CHARS];
			sprintf(buffer, "*** %s is unable to join this table [3]***", m_player[GetPlayerIdx(joinerSockIdx)].name);
			GAMEAPPVIEW->Message(buffer, 10);
			// *TODO - player should exit table here, and do the following...
			m_bInTable = FALSE; // ?? host needs this line also ??
		}
	}
}

BOOL CLobby::TableIsFull(int tblIdx, int enterType)
{
	if(tblIdx==-1)
		return TRUE;

	int playersN = 0;
	
	for(int n=0;n<MAX_TABLE_PLAYERS;n++)
		if(m_table[tblIdx].playersSockIdx[n]!=-1)
			playersN++;

	if(enterType == 0) // if this joiner is a watcher (limit to 14 watchers)
		if(playersN >= (MAX_TABLE_PLAYERS - 2))
			return TRUE;

	if( m_table[tblIdx].player1SockIdx!=-1)
		playersN++;
	
	if( m_table[tblIdx].player2SockIdx!=-1)
		playersN++;
	
	if(playersN >= MAX_TABLE_PLAYERS)
		return TRUE;

	// table not full
	return FALSE;
}

BOOL CLobby::PlayerCanHost(int plrSockIdx)
{
	if(plrSockIdx == -1)
		return FALSE;

	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(!m_player[plrIdx].bCanHost)
		return FALSE;

	// this guy can host a table
	return TRUE;
}

void CLobby::ExitTable(int playerSockIdx, BOOL bTimedOutBoot, BOOL bPLC)
{
	int plrIdx = GetPlayerIdx(playerSockIdx);
	if(plrIdx == -1) return;
	int tblIdx = m_player[plrIdx].table;
	if(tblIdx == -1) return; // this player is in lobby (not at a table)

	
	// if i'm the player leaving
	if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
	{
		ClearPlayer(&m_myself);
		ClearPlayer(&m_opponent);
		m_bInTable = FALSE;
		m_PLCwait = 0;

		#ifdef THE_POOL_CLUB
		// Make sure the sponsor text is off //
		// This is for the 2 Shot / 1 Shot msg for English Pool //
		g_gameAppView->m_sponsortxt[0] = 0;
		g_gameAppView->m_sponsoralpha = 0;
		#endif	



		/// <-- HERE SUSPICIOUS CODE BELOW!!!!!!!!! <---
		// remove connection from gameShell - if this is a TimeOutBoot
		if(bTimedOutBoot)
		{
			int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
			if(myTblIdx == tblIdx)
			{
				GAMEAPPVIEW->ExitTable2Lobby();
				
				char _buffer[MAX_CHARS];
				sprintf(_buffer,"*** YOU LOST CONNECTION WITH TABLE %d! ***", tblIdx+1);
				GAMEAPPVIEW->Message(_buffer,10); // personal message
			}
		}

		// Display Joined Lobby Messages //

		// Display Server Time when Loggin In, so we are all consistent on GMT //
		//CString timeStr;
		
		CString timestr;
		CTime time;
		time = GAMEAPPVIEW->GetServerTime();
		timestr = time.Format("%d-%m-%Y %H:%M:%S (GMT)");

		if(timestr)
		{
			char _buffer[128];
			sprintf(_buffer,"*** %s joined the lobby at %s ***", m_player[plrIdx].name, timestr);
			GAMEAPPVIEW->Message(_buffer,10); // personal message
			g_concede_time = 0;
		}
	}


	// update player details (blank out table field)
	m_player[plrIdx].table = -1;
	m_player[plrIdx].type = Player_typ::TYPE_SIMEND;//Player_typ::TYPE_ARRIVED;



	RefreshPlayerItem();
	GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
	GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();
	GAMEAPPVIEW->m_bootDlg.RefreshPlayerItem();
	GAMEAPPVIEW->RefreshListBox();

	


	//////////////////////////////////////////////////////////////////////
	// if this player was sitting and had pressed start and was waiting for opponent to click start,
	//  then clear the table press start flags
	if(!m_table[tblIdx].bGameStarted)
	{
		if((m_table[tblIdx].player1SockIdx == playerSockIdx) || (m_table[tblIdx].player2SockIdx == playerSockIdx))
		{	
			// cancel start pressed! (THIS IS A BUFIX! - OTHERWISE WRONG PLAYER GETS/LOSES PTS)
			m_table[tblIdx].P1StartAtTable = -1;
			m_table[tblIdx].P2StartAtTable = -1;
			m_table[tblIdx].P1StartName[0] = 0;
			m_table[tblIdx].P2StartName[0] = 0;
			m_table[tblIdx].P1bStartAtTable = -1;
			m_table[tblIdx].P2bStartAtTable = -1;
			m_table[tblIdx].P1bStartName[0] = 0;
			m_table[tblIdx].P2bStartName[0] = 0;
			m_table[tblIdx].m_lostConn1P = 0;
			m_table[tblIdx].m_lostConn2P = 0;
		}
	}
	// It's possible for the game to be started for the player remaining at the table if the opponent exits the table
	// Just after start is pressed by the player (timing issue). If this happens, then we must end the Match and
	// give a message to the player why the game was aborted //
	else{
		// if i'm the player leaving
		if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
			{
			// Send a Message to the Server telling it to abort this game //
			// If Game Has Started, then 99% of the time, it should be conceded, if we are able to exit //
			// Or, the player lost connection //
			// If the Game hasn't been Conceded (timing issue), we need to make sure the match is aborted //
			#ifndef MAIN_SERVER
			if(!bPLC)	// Don't do this code if we got here due to a disconnection //
			if(GAMEAPPVIEW->m_snooker->m_bConcededGame == FALSE)
			if((m_table[tblIdx].player1SockIdx == playerSockIdx) || (m_table[tblIdx].player2SockIdx == playerSockIdx))
				{
				GAMEAPPVIEW->AbortMatch(tblIdx);
				}
			#endif
			}	
		}


	//////////////////////////////////////////////////////////////////////

	// take player off table list
	if( m_table[tblIdx].player1SockIdx == playerSockIdx)
	{
		m_table[tblIdx].player1SockIdx = -1;
		AddTableItem(tblIdx, 1);
	}	
	if( m_table[tblIdx].player2SockIdx == playerSockIdx)
	{
		m_table[tblIdx].player2SockIdx = -1;
		AddTableItem(tblIdx, 2);
	}
	if( m_table[tblIdx].player1bSockIdx == playerSockIdx)
	{
		m_table[tblIdx].player1bSockIdx = -1;
		AddTableItem(tblIdx, 3); 
	}	
	if( m_table[tblIdx].player2bSockIdx == playerSockIdx)
	{
		m_table[tblIdx].player2bSockIdx = -1;
		AddTableItem(tblIdx, 4);
	}
	

	// Remove Watchers //
	{
		for(int n=0;n<MAX_TABLE_PLAYERS;n++)
		{
			if(m_table[tblIdx].playersSockIdx[n] == playerSockIdx)
			{
				m_table[tblIdx].playersSockIdx[n] = -1;

				// refresh table in table list
				AddTableItem(tblIdx, 0);
//				break;
			}
		}

	}

	// If we are watching This Table, then Update the Lobby TV //
	// If Game hasn't started, then update Lobby TV //
	if(m_table[tblIdx].bGameStarted == 0) GAMEAPPVIEW->UpdateLobbyTableWatch(tblIdx);


	// ###### ^ WHAT IF A PLAYER SOMEHOW GETS PUT IN 'SEAT1' AND 'WATCHERS LIST' ############
	

	// Refresh seat names, only if i'm on this table
	int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
	if(myTblIdx == tblIdx)
	{
		RefreshTableSeats(tblIdx);

		char _buffer[MAX_CHARS];
		char *timestr;
		// Get (Local) time
		GAMEAPPVIEW->GetTime();
		timestr = GAMEAPPVIEW->m_stringFullTime.GetBuffer(0);
		sprintf(_buffer,"*** %s has left the table at %s ***", m_player[plrIdx].name, timestr);
		GAMEAPPVIEW->Message(_buffer,10); // personal message
	}

	
	if(!CountPlayersAtTable(tblIdx))
	{
		ClearTable(tblIdx);
	}

	
	// finally, check if host (has left)
	if( m_table[tblIdx].hostSockIdx == playerSockIdx)
	{
		// disable table type for host of table
		if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
		{
			GAMEAPPVIEW->m_tableTypeBox.EnableWindow(FALSE);
			//GAMEAPPVIEW->m_cueErrorBox.EnableWindow(FALSE);
			//if(g_build.gameType == GAMETYPE_POOL)
			//	GAMEAPPVIEW->m_button[BUTTON_SETUP].EnableWindow(FALSE);
		}

		m_table[tblIdx].hostSockIdx = -1;
//		m_table[tblIdx].bHostComplete = FALSE;

		/*if(!CountPlayersAtTable(tblIdx))
		{
			ClearTable(tblIdx);
			GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*3+0, TRUE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*3+1, TRUE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*3+2, TRUE);
			AddTableOptionsItem(tblIdx);
		}
		*/
		//else
		{
			// check to see who will be next candidate for hosting this table
			int nexthostsockidx = -1;
			int enterType = 0;
			int plrHostIdx;

			if(PlayerCanHost(m_table[tblIdx].player1SockIdx))
				nexthostsockidx = m_table[tblIdx].player1SockIdx;
			else if(PlayerCanHost(m_table[tblIdx].player2SockIdx))			
				nexthostsockidx = m_table[tblIdx].player2SockIdx;
			else if(PlayerCanHost(m_table[tblIdx].player1bSockIdx))			
				nexthostsockidx = m_table[tblIdx].player1bSockIdx;
			else if(PlayerCanHost(m_table[tblIdx].player2bSockIdx))			
				nexthostsockidx = m_table[tblIdx].player2bSockIdx;
			else {
				for(int n=0;n<MAX_TABLE_PLAYERS;n++)
					if(PlayerCanHost(m_table[tblIdx].playersSockIdx[n]))
					{
						nexthostsockidx = m_table[tblIdx].playersSockIdx[n];
						break;
					}
		
			}
			/*
			if(m_table[tblIdx].player1SockIdx != -1) 
			{
				plrHostIdx = GetPlayerIdx(m_table[tblIdx].player1SockIdx);
				if(m_player[plrHostIdx].bCanHost)
					nexthostsockidx = m_table[tblIdx].player1SockIdx;
			}
			else if(m_table[tblIdx].player2SockIdx != -1)			
			{
				nexthostsockidx = m_table[tblIdx].player2SockIdx;
			}
			else {
				for(int n=0;n<MAX_TABLE_PLAYERS;n++)
					if(m_table[tblIdx].playersSockIdx[n] != -1)
					{
					nexthostsockidx = m_table[tblIdx].playersSockIdx[n];
					break;
					}
		
			}
			*/

			
			
			if(nexthostsockidx != -1)
			{	  
				// setup new host
				m_table[tblIdx].hostSockIdx = nexthostsockidx;

				// update player lists (for '*' in table field)
				RefreshPlayerItem();
				GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
				GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();
				/*
				if(P1Idx != -1)
					UpdatePlayerItem(m_player[P1Idx].name);
				if(P2Idx != -1)
					UpdatePlayerItem(m_player[P2Idx].name);
				*/
				
				int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
				if(myTblIdx == tblIdx)
				{
					// change window title
					int hostPlrIdx = GetPlayerIdx(nexthostsockidx);
					CString title;
					if(g_build.gameType == GAMETYPE_SNOOKER)
					{
						char wildStr[8];
						wildStr[0] = 0;
						if(IsTableWILD(tblIdx)) // check for WILD tables
							sprintf(wildStr, "WILD? ");

						char buff[32];
						buff[0] = 0;
						
						if(m_table[tblIdx].gameType == 0) sprintf(buff, "Snooker");
						else if(m_table[tblIdx].gameType == 1) sprintf(buff, "Short-Snooker");
						else if(m_table[tblIdx].gameType == 2) sprintf(buff, "Mini-Snooker");
						else if(m_table[tblIdx].gameType == 3) sprintf(buff, "Re-Spot Black");
						else if(m_table[tblIdx].gameType == 4) sprintf(buff, "Billiards (50 pts)");
						else if(m_table[tblIdx].gameType == 5) sprintf(buff, "Billiards (100 pts)");
						else if(m_table[tblIdx].gameType == 6) sprintf(buff, "Billiards (200 pts)");

						char buff2[32];
						buff2[0] = 0;
						if(m_table[tblIdx].cueError == 0) sprintf(buff2, "Beginner");
						else if(m_table[tblIdx].cueError == 1) sprintf(buff2, "Semi-Pro");
						else if(m_table[tblIdx].cueError == 2) sprintf(buff2, "Pro");

						title.Format(" Table: %d - %s%s - %s -  Host: %s", m_player[hostPlrIdx].table+1, wildStr, buff, buff2, m_player[hostPlrIdx].name);
					}
					else if(g_build.gameType == GAMETYPE_POOL)
					{
						char wildStr[8];
						wildStr[0] = 0;
						if(IsTableWILD(tblIdx)) // check for WILD tables
							sprintf(wildStr, "WILD? ");
						
						char buff[32];
						buff[0] = 0;

						if(m_table[tblIdx].gameType == 0) sprintf(buff, "8-Ball");
						else if(m_table[tblIdx].gameType == 1) sprintf(buff, "9-Ball");
						else if(m_table[tblIdx].gameType == 2) sprintf(buff, "Rotation-Ball");
						else if(m_table[tblIdx].gameType == 3) sprintf(buff, "Bowls");
						else if(m_table[tblIdx].gameType == 4) sprintf(buff, "6-Ball");
						else if(m_table[tblIdx].gameType == 5) sprintf(buff, "10-Ball");
						else if(m_table[tblIdx].gameType == 6) sprintf(buff, "Break");
						else if(m_table[tblIdx].gameType == 7) sprintf(buff, "UKPool");
				
						title.Format(" Table: %d - %s%s -  Host: %s", m_player[hostPlrIdx].table+1, wildStr, buff, m_player[hostPlrIdx].name);
					}
					GAMEAPPVIEW->ChangeTitle(title);

  					char _buffer[MAX_CHARS];
					char *timestr;
					// Get (Local) time
					GAMEAPPVIEW->GetTime();
					timestr = GAMEAPPVIEW->m_stringFullTime.GetBuffer(0);
					sprintf(_buffer,"*** %s is now hosting this table at %s ***", m_player[hostPlrIdx].name, timestr);
					GAMEAPPVIEW->Message(_buffer,10); // personal message
				}



				/* // can't do the following, as people could already be playing with different settings!!!!
				// if a GUEST is the new host
				int hostPlrIdx = GetPlayerIdx(nexthostsockidx);
				if(m_player[hostPlrIdx].bIsGuest)
				{
					// clear 'rated'
					m_table[tblIdx].bRated = FALSE;
					GAMEAPPVIEW->m_ratedButton.SetCheck(0);
					
					// table type = 'PUBLIC'
					m_table[tblIdx].type = 0;
					GAMEAPPVIEW->m_tableTypeBox.SetCurSel(0);
				}
				/////////////////////////////////
				*/

				if(GAMEAPPVIEW->m_playerSockIdx == nexthostsockidx)
				{
					// ***specific to host***
////				GAMEAPPVIEW->DestroyGameShell(FALSE);
					int plrIdx = GetPlayerIdx(nexthostsockidx);
					if(plrIdx!=-1)
					{
						m_player[plrIdx].bInvited = TRUE;
						if(!m_player[plrIdx].bIsGuest)
						{
							GAMEAPPVIEW->m_tableTypeBox.EnableWindow(TRUE);
							//if(g_build.gameType == GAMETYPE_POOL)
							//	GAMEAPPVIEW->m_button[BUTTON_SETUP].EnableWindow(TRUE);

							// IS THIS OK TO REMOVE?????
							//RefreshTableHostDetails(tblIdx); // OAP removed this, cos the holes were disappearing in pool 1/4/2004
						}
						else
						{
							// if guest, then make sure this table is public
							//if(m_table[tblIdx].type != Table_typ::TYPE_PUBLIC)
							if(g_build.gameType == GAMETYPE_POOL)
								GAMEAPPVIEW->SendTableHostDetails(FALSE, -1, 0, 0, 0, GAMEAPPVIEW->m_playerSockIdx,
								20, 0, 0, 0, 0,
								0, 2,
								0, 0, 0, 0);
							else if(g_build.gameType == GAMETYPE_SNOOKER)
								GAMEAPPVIEW->SendTableHostDetails(FALSE, -1, 0, 0, 0, GAMEAPPVIEW->m_playerSockIdx,
								20, 2, 1, 0, 0,
								0, 2,
								0, 0, 0, 0);
						}
						//GAMEAPPVIEW->m_cueErrorBox.EnableWindow(TRUE);
////					GAMEAPPVIEW->CreateGameShell(TRUE, plrIdx, TRUE);
					}
					/*
					// set and broadcast player profile (nick for chat msgs)
					#ifndef MAIN_SERVER
					if(gameShell)
					{
						gameShell->SetNick(m_player[plrIdx].name);
						gameShell->SendProfile();
						
						if(m_table[tblIdx].player1SockIdx != -1) gameShell->SitRequest(0); // Seat #1
						else if(m_table[tblIdx].player2SockIdx != -1) gameShell->SitRequest(1); // Seat #2

						// if all is well, then notify others
						GAMEAPPVIEW->SendPlayerReJoin(FALSE, tblIdx);
					}
					else
					{
						ErrorMessage(m_parent, "New Host: Unable to create GameShell!");
						//MessageBox(0,"New Host: Unable to create GameShell!","",MB_OK);
					}
					#endif //!MAIN_SERVER
					*/
				}
				else
				{
					// ***non-host players***
					// idle

				}
			}
			else
			{
				// nobody could host this table
				//ClearTable(tblIdx);
				//GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*3+0, TRUE);
				//GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*3+1, TRUE);
				//GAMEAPPVIEW->m_listTables.EnableItemButton(tblIdx*3+2, TRUE);
				//AddTableOptionsItem(tblIdx);

				

				int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx); // what table am i on?
				//if(GAMEAPPVIEW->m_playerSockIdx == playerSockIdx)
				if(myTblIdx == tblIdx)
				{
					
					GAMEAPPVIEW->ExitTable2Lobby();
					// send main server message that we've left the table (destroyed the game shell)
					GAMEAPPVIEW->SendPlayerExit(FALSE, GAMEAPPVIEW->m_playerSockIdx);
					
					
				}
			}
		}
	}


}
						  
void CLobby::SitStandTable(int seat1SockIdx, int seat2SockIdx, int tableIdx, int seat1bSockIdx, int seat2bSockIdx)
{
	if(tableIdx<0 || tableIdx>=MAX_TABLEITEMS) return; // NEW 15/FEB/2004
	
	int enterType = -1;

	
	int P1Idx = GetPlayerIdx(seat1SockIdx);
	int P2Idx = GetPlayerIdx(seat2SockIdx);
	int P1bIdx = GetPlayerIdx(seat1bSockIdx);
	int P2bIdx = GetPlayerIdx(seat2bSockIdx);


	// find out whether this is a 'sit' or 'stand'
	if(seat1SockIdx == -1 || seat2SockIdx == -1 || seat1bSockIdx == -1 || seat2bSockIdx == -1)
		{ // sit message
		if(seat1SockIdx != -1)
			{
			// add player to seat #1
			if(m_table[tableIdx].player1SockIdx == -1) // if seat is empty...
				{
				m_table[tableIdx].player1SockIdx = seat1SockIdx;
				int plrIdx = GetPlayerIdx(m_table[tableIdx].player1SockIdx);
				m_player[plrIdx].bInvited = TRUE;
				enterType = 1;

				int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
				if(myTblIdx == tableIdx)
					{
					GAMEAPPVIEW->SitName(0, m_player[plrIdx].name, m_table[tableIdx].player1SockIdx); // Seat #1
					if(m_table[tableIdx].player1SockIdx == GAMEAPPVIEW->m_playerSockIdx) 
						{
						GAMEAPPVIEW->m_tableEnter = 1;
						}
					}

				// remove player1 from watch list
				for(int n=0;n<MAX_TABLE_PLAYERS;n++)
				if(m_table[tableIdx].playersSockIdx[n] == seat1SockIdx)
					{
					m_table[tableIdx].playersSockIdx[n] = -1;
					break;
					}
				}
			
			}
		else if(seat2SockIdx != -1)
			{			
			// add player to seat #2
			if(m_table[tableIdx].player2SockIdx == -1)
				{
				m_table[tableIdx].player2SockIdx = seat2SockIdx;
				int plrIdx = GetPlayerIdx(m_table[tableIdx].player2SockIdx);
				m_player[plrIdx].bInvited = TRUE;
				enterType = 2;
				
				int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
				if(myTblIdx == tableIdx)
					{
					GAMEAPPVIEW->SitName(1, m_player[plrIdx].name, m_table[tableIdx].player2SockIdx); // Seat #1

					if(m_table[tableIdx].player2SockIdx == GAMEAPPVIEW->m_playerSockIdx)
						{
						GAMEAPPVIEW->m_tableEnter = 2;
						}
					}

				// remove player2 from watch list
				for(int n=0;n<MAX_TABLE_PLAYERS;n++)
				if(m_table[tableIdx].playersSockIdx[n] == seat2SockIdx)
					{
					m_table[tableIdx].playersSockIdx[n] = -1;
					break;
					}
				}
			}
		// Doubles P1a//
		else if(seat1bSockIdx != -1)
			{			
			// add player to seat #1b
			if(m_table[tableIdx].player1bSockIdx == -1)
				{
				m_table[tableIdx].player1bSockIdx = seat1bSockIdx;
				int plrIdx = GetPlayerIdx(m_table[tableIdx].player1bSockIdx);
				m_player[plrIdx].bInvited = TRUE;
				enterType = 3;
				
				int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
				if(myTblIdx == tableIdx)
					{
					GAMEAPPVIEW->SitName(2, m_player[plrIdx].name, m_table[tableIdx].player1bSockIdx); // Seat #1

					if(m_table[tableIdx].player1bSockIdx == GAMEAPPVIEW->m_playerSockIdx)
						{
						GAMEAPPVIEW->m_tableEnter = 3;
						}
					}
				
				// remove player1b from watch list
				for(int n=0;n<MAX_TABLE_PLAYERS;n++)
				if(m_table[tableIdx].playersSockIdx[n] == seat1bSockIdx)
					{
					m_table[tableIdx].playersSockIdx[n] = -1;
					break;
					}
				
				}
			}
		// Doubles P2b //
		else if(seat2bSockIdx != -1)
			{			
			// add player to seat #1b
			if(m_table[tableIdx].player2bSockIdx == -1)
				{
				m_table[tableIdx].player2bSockIdx = seat2bSockIdx;
				int plrIdx = GetPlayerIdx(m_table[tableIdx].player2bSockIdx);
				m_player[plrIdx].bInvited = TRUE;
				enterType = 4;
				
				int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
				if(myTblIdx == tableIdx)
					{
					GAMEAPPVIEW->SitName(3, m_player[plrIdx].name, m_table[tableIdx].player2bSockIdx); // Seat #1

					if(m_table[tableIdx].player2bSockIdx == GAMEAPPVIEW->m_playerSockIdx)
						{
						GAMEAPPVIEW->m_tableEnter = 4;
						}
					}
				
				// remove player1b from watch list
				for(int n=0;n<MAX_TABLE_PLAYERS;n++)
				if(m_table[tableIdx].playersSockIdx[n] == seat2bSockIdx)
					{
					m_table[tableIdx].playersSockIdx[n] = -1;
					break;
					}
				
				}
			}
		}
	else
		{ // stand message
		
		// check if player has stood-up already
		// seat1SockIdx is actually the socket ID of the person standing.. so can be at any seat position //
		for(int n=0;n<MAX_TABLE_PLAYERS;n++)
		if(m_table[tableIdx].playersSockIdx[n] == seat1SockIdx)
			{
			// no need to go any further
			return;
			}

		// add player to watch list
		for(int n=0;n<MAX_TABLE_PLAYERS;n++)
		if(m_table[tableIdx].playersSockIdx[n] == -1)
			{
			m_table[tableIdx].playersSockIdx[n] = seat1SockIdx;
			break;
			}

		// find which seat we're sitting in, and remove
		if(m_table[tableIdx].player1SockIdx == seat1SockIdx)
			{
			int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(myTblIdx == tableIdx)
				{
				GAMEAPPVIEW->SitName(0, "", m_table[tableIdx].player1SockIdx); // Seat #1

				if(m_table[tableIdx].player1SockIdx == GAMEAPPVIEW->m_playerSockIdx)
					{
					GAMEAPPVIEW->m_tableEnter = 0;
					}
				}
			
			m_table[tableIdx].player1SockIdx = -1;
			enterType = 1;
			}
		else if(m_table[tableIdx].player2SockIdx == seat2SockIdx)
			{
			int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(myTblIdx == tableIdx)
				{
				GAMEAPPVIEW->SitName(1, "", m_table[tableIdx].player2SockIdx); // Seat #2
			
				if(m_table[tableIdx].player2SockIdx == GAMEAPPVIEW->m_playerSockIdx)
					{
					GAMEAPPVIEW->m_tableEnter = 0;
					}
				}
			 
			m_table[tableIdx].player2SockIdx = -1;
			enterType = 2;
			}
		else if(m_table[tableIdx].player1bSockIdx == seat1bSockIdx)
			{
			int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(myTblIdx == tableIdx)
				{
				GAMEAPPVIEW->SitName(2, "", m_table[tableIdx].player1bSockIdx); // Seat #1b
			
				if(m_table[tableIdx].player1bSockIdx == GAMEAPPVIEW->m_playerSockIdx)
					{
					GAMEAPPVIEW->m_tableEnter = 0;
					}
				}
			 
			m_table[tableIdx].player1bSockIdx = -1;
			enterType = 3;
			}
		else if(m_table[tableIdx].player2bSockIdx == seat2bSockIdx)
			{
			int myTblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(myTblIdx == tableIdx)
				{
				GAMEAPPVIEW->SitName(3, "", m_table[tableIdx].player2bSockIdx); // Seat #2b
			
				if(m_table[tableIdx].player2bSockIdx == GAMEAPPVIEW->m_playerSockIdx)
					{
					GAMEAPPVIEW->m_tableEnter = 0;
					}
				}
			 
			m_table[tableIdx].player2bSockIdx = -1;
			enterType = 4;
			}
		}

	// if someone has 'sat-down' OR 'stood-up'
	if(enterType != -1)
	{
		// cancel start
		if(!m_table[tableIdx].bGameStarted)
		{
			m_table[tableIdx].P1StartAtTable = -1;
			m_table[tableIdx].P2StartAtTable = -1;
			m_table[tableIdx].P1StartName[0] = 0;
			m_table[tableIdx].P2StartName[0] = 0;

			m_table[tableIdx].P1bStartAtTable = -1;
			m_table[tableIdx].P2bStartAtTable = -1;
			m_table[tableIdx].P1bStartName[0] = 0;
			m_table[tableIdx].P2bStartName[0] = 0;

			m_table[tableIdx].m_lostConn1P = 0;
			m_table[tableIdx].m_lostConn2P = 0;
		}
	}
	//
	// add player details to table list
	AddTableItem(tableIdx, 0); // update watchers list
	AddTableItem(tableIdx, enterType); // update the two seats
	
	// update player lists
	RefreshPlayerItem();
	GAMEAPPVIEW->m_inviteDlg.RefreshPlayerItem();
	GAMEAPPVIEW->m_playerListDlg.RefreshPlayerItem();

}

void CLobby::AddTable(Table *pTbl)
{
	if(pTbl == NULL) return;

	int n;
	int idx = pTbl->idx;
	
	if(idx==-1) return;
	if(idx==14 || idx==48)
		{
		int p;
		p = 0;
		}

	
	memcpy(&m_table[idx], pTbl, sizeof(Table_typ));

	// update the table buttons, depending on 'table - type'
	UpdateTableItemButtons(idx);

	// add player details to table list
	if(m_table[idx].player1SockIdx !=-1 )
		AddTableItem(idx, 1);
	if(m_table[idx].player2SockIdx !=-1 )
		AddTableItem(idx, 2);

	// For Doubles //
	if(m_table[idx].player1bSockIdx !=-1 )
		AddTableItem(idx, 3);
	if(m_table[idx].player2bSockIdx !=-1 )
		AddTableItem(idx, 4);


	for(n=0;n<MAX_TABLE_PLAYERS;n++)
		if(m_table[idx].playersSockIdx[n] !=-1)
			AddTableItem(idx, 0);

	// add options details to table list
	AddTableOptionsItem(idx);

	// Disable Join Buttons if this table has bLockSeats = TRUE //
	if(m_table[idx].bLockSeats)
		{
		GAMEAPPVIEW->m_listTables.EnableItemButton(idx*4+2, FALSE);
		GAMEAPPVIEW->m_listTables.EnableItemButton(idx*4+3, FALSE);
		}

}

void CLobby::RefreshTableSeats(int idx)
{
	if(idx == -1) return;
	
	int plr1Idx = GetPlayerIdx(m_table[idx].player1SockIdx);
	int plr2Idx = GetPlayerIdx(m_table[idx].player2SockIdx);
	
	if(plr1Idx != -1) GAMEAPPVIEW->SitName(0, m_player[plr1Idx].name, m_table[idx].player1SockIdx); // Seat #1
	else GAMEAPPVIEW->SitName(0, "", m_table[idx].player1SockIdx); // Seat #1

	if(plr2Idx != -1) GAMEAPPVIEW->SitName(1, m_player[plr2Idx].name, m_table[idx].player2SockIdx); // Seat #2
	else GAMEAPPVIEW->SitName(1, "", m_table[idx].player2SockIdx); // Seat #2


	// Doubles //
	//if(m_table[idx].doubles)
	//{
	int plr1bIdx = GetPlayerIdx(m_table[idx].player1bSockIdx);
	int plr2bIdx = GetPlayerIdx(m_table[idx].player2bSockIdx);
	
	if(plr1bIdx != -1) GAMEAPPVIEW->SitName(2, m_player[plr1bIdx].name, m_table[idx].player1bSockIdx); // Seat #1
	else GAMEAPPVIEW->SitName(2, "", m_table[idx].player1bSockIdx); // Seat #1b

	if(plr2bIdx != -1) GAMEAPPVIEW->SitName(3, m_player[plr2bIdx].name, m_table[idx].player2bSockIdx); // Seat #2
	else GAMEAPPVIEW->SitName(3, "", m_table[idx].player2bSockIdx); // Seat #2b
	//}

	GAMEAPPVIEW->Invalidate();
}

void CLobby::RefreshTableHostDetails(int idx)
{
	
	if(idx<0 || idx>=MAX_TABLEITEMS) return; // NEW 15/FEB/2004
		
	int plrIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);
	if(plrIdx==-1) return;

	
	if(m_table[idx].status)
	{
			
		// rated game:

		
		// Only account if this is not a Game of Doubles or ReSpot Black and must be a 15 Reds game with some error //
		// Assume game is Unrated //
		GAMEAPPVIEW->m_ratedButton.SetCheck(0);
		#ifndef MAIN_SERVER
		GAMEAPPVIEW->m_snooker->m_bRated = FALSE;
		#endif //!MAIN_SERVER

		// Rated Games available for Semi Pro & Pro GameTypes;
		// 15 Reds				(0)
		// 10 Reds				(1)
		// Billiards 50 pts		(4)
		// Billiards 100 pts	(5)
		// Billiards 200 pts	(6)

	if(g_build.gameType == GAMETYPE_SNOOKER)
		{
		if((m_table[idx].doubles) == 0)		// Can't be Doubles //
			{
			if((m_table[idx].cueError) !=0)	// Must be Semi Pro or Pro //
				{
				if( (m_table[idx].gameType!=2) && (m_table[idx].gameType!=3) )	// Can't be Mini Snooker (6 Reds) or ReSpot Black
					{
					if(m_table[idx].bRated)
						{
						GAMEAPPVIEW->m_ratedButton.SetCheck(1);
						#ifndef MAIN_SERVER
						GAMEAPPVIEW->m_snooker->m_bRated = TRUE;
						#endif //!MAIN_SERVER
						}
					else{
						GAMEAPPVIEW->m_ratedButton.SetCheck(0);
						#ifndef MAIN_SERVER
						GAMEAPPVIEW->m_snooker->m_bRated = FALSE;
						#endif //!MAIN_SERVER
						}
					}		
				}
			}
		}

	// Rated Game available for non 6Reds Games, Singles and Std, Pro and UK Table Types //
	else{
		if(m_table[idx].gameType != 4 && m_table[idx].doubles == 0 && m_table[idx].tableType <= 2)
			{
			if(m_table[idx].bRated)
				{
				GAMEAPPVIEW->m_ratedButton.SetCheck(1);
				#ifndef MAIN_SERVER
				GAMEAPPVIEW->m_snooker->m_bRated = TRUE;
				#endif //!MAIN_SERVER
				}
			else{
				GAMEAPPVIEW->m_ratedButton.SetCheck(0);
				#ifndef MAIN_SERVER
				GAMEAPPVIEW->m_snooker->m_bRated = FALSE;
				#endif //!MAIN_SERVER
				}
			}
		}



		// table type details
		if(m_table[idx].type>=0)
		{
			GAMEAPPVIEW->m_tableTypeBox.SetCurSel(m_table[idx].type);
			if(!m_table[idx].type || m_player[plrIdx].bInvited)
				GAMEAPPVIEW->EnableSeats(plrIdx, TRUE);
			else
				GAMEAPPVIEW->EnableSeats(plrIdx, FALSE);
		}

		// miss-rule game
		if(m_table[idx].bMissRule)
		{
			GAMEAPPVIEW->m_missRuleButton.SetCheck(1);
			#ifndef MAIN_SERVER
			GAMEAPPVIEW->m_snooker->m_bMissRule = TRUE;
			GAMEAPPVIEW->m_snooker->m_bCallShot = TRUE;
			#endif //!MAIN_SERVER
		}
		else
		{
			GAMEAPPVIEW->m_missRuleButton.SetCheck(0);
			#ifndef MAIN_SERVER
			GAMEAPPVIEW->m_snooker->m_bMissRule = FALSE;
			GAMEAPPVIEW->m_snooker->m_bCallShot = FALSE;
			#endif //!MAIN_SERVER
		}



		if(g_build.gameType == GAMETYPE_POOL)
		{

			if(m_table[idx].doubles !=0xff)
			{
				if(m_table[idx].doubles == 0)
					GAMEAPPVIEW->m_tableSetupDlg.m_doubles = FALSE;
				else if(m_table[idx].doubles == 1)
					GAMEAPPVIEW->m_tableSetupDlg.m_doubles = TRUE;
			}

			if(m_table[idx].ballSize !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg.m_ballSize = m_table[idx].ballSize;
				if(m_table[idx].ballSize == 0) // Normal
					g_ballRadiusFactor = 1.0f;
				else if(m_table[idx].ballSize == 1) // BIG
					g_ballRadiusFactor = 1.5f;					
				else if(m_table[idx].ballSize == 2) // small
					g_ballRadiusFactor = 0.75;
				g_ballRadius = 28.4625f * g_ballRadiusFactor;
				GAMEAPPVIEW->m_snooker->m_customTable->ReNormalizeBalls();
			}

	
			if(m_table[idx].tableType !=0xff || m_table[idx].ballSize !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg.m_tableType = m_table[idx].tableType;

				int tableTypeIdx = m_table[idx].tableType;
				M3d_object *M3d_table_ptr;
				M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->PoolTableGetType(tableTypeIdx);
				GAMEAPPVIEW->m_snooker->m_customTable->PoolTableChange(M3d_table_ptr, tableTypeIdx);
				GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[idx].tableColour, M3d_table_ptr);
				GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table

				if(M3d_table_ptr == &GAMEAPPVIEW->m_snooker->m_customTable->m_M3d_proTable) GAMEAPPVIEW->m_tableSetupDlg.m_proTable = TRUE;
					else GAMEAPPVIEW->m_tableSetupDlg.m_proTable = FALSE;
	
			}
			
			// table setup details
			if(m_table[idx].shotTime !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg.m_shotTime = m_table[idx].shotTime;
				//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);
			}

			
			if(m_table[idx].addTimer !=0xff)
			{
				if(m_table[idx].addTimer == 0)
					GAMEAPPVIEW->m_tableSetupDlg.m_bAddTimer = FALSE;
				else if(m_table[idx].addTimer == 1)
					GAMEAPPVIEW->m_tableSetupDlg.m_bAddTimer = TRUE;
				//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);
			}
			
			

			if(m_table[idx].tableColour !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg.m_tableColour = m_table[idx].tableColour;
				//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);

				// Change color scheme - really here we should send a packet of data to all players about the new table properties!!
				int tableTypeIdx = m_table[idx].tableType;
				M3d_object *M3d_table_ptr;
				M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->PoolTableGetType(tableTypeIdx);
				GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[idx].tableColour, M3d_table_ptr);										
				GAMEAPPVIEW->m_bUpdateTable = TRUE;
			}

			if(m_table[idx].gameType !=0xff)
				{
				GAMEAPPVIEW->m_tableSetupDlg.m_gameType = m_table[idx].gameType;
				GAMEAPPVIEW->m_snooker->m_customTable->ChangeGameType(m_table[idx].gameType);
				//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);
				GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table
				}
			
			

			
			if(m_table[idx].raceTo !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg.m_raceTo = m_table[idx].raceTo;
			}

			if(m_table[idx].whoBreaks !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg.m_whoBreaks = m_table[idx].whoBreaks;
				if(m_table[idx].whoBreaks == 0)
				{
					// 1P Always
					GAMEAPPVIEW->m_snooker->m_bTurn1P = TRUE;
				}
				else if(m_table[idx].whoBreaks == 1)
				{
					// 2P Always
					GAMEAPPVIEW->m_snooker->m_bTurn1P = FALSE;
				}
			}

			if(m_table[idx].rackType !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg.m_rackType = m_table[idx].rackType;
				GAMEAPPVIEW->m_snooker->m_wildRackType = m_table[idx].rackType;			
			}
		}
		else if(g_build.gameType == GAMETYPE_SNOOKER)
		{	
			if(m_table[idx].tableType !=0xff)
			{		
				/*
				GAMEAPPVIEW->m_tableSetupDlg2.m_tableType = m_table[idx].tableType;
				
				int tableTypeIdx = m_table[idx].tableType;
				M3d_object *M3d_table_ptr;
				M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->PoolTableGetType(tableTypeIdx);
				GAMEAPPVIEW->m_snooker->m_customTable->PoolTableChange(M3d_table_ptr, tableTypeIdx);
				GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[idx].tableColour, M3d_table_ptr);
				GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table
				*/
			}


			if(m_table[idx].doubles !=0xff)
			{
				if(m_table[idx].doubles == 0)
					GAMEAPPVIEW->m_tableSetupDlg2.m_doubles = FALSE;
				else if(m_table[idx].doubles == 1)
					GAMEAPPVIEW->m_tableSetupDlg2.m_doubles = TRUE;
			}

			if(m_table[idx].ballSize !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_ballSize = m_table[idx].ballSize;

				int ballSize = m_table[idx].ballSize;

				if(ballSize == 0) // Normal
					g_ballRadiusFactor = 1.0f;
				else if(ballSize == 1) // GIANT
					g_ballRadiusFactor = 1.5f;					
				g_ballRadius = 26.09f * g_ballRadiusFactor;
				GAMEAPPVIEW->m_snooker->m_customTable->ReNormalizeBalls();

				M3d_object *M3d_table_ptr;
				int idxType = 0; // temporary - may need to tie back into table type later? [+]
				if(ballSize == 0) // Normal
					idxType = 0;
				else if(ballSize == 1) // GIANT
					idxType = 1;

				M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->SnookerTableGetType(idxType);//tableTypeIdx);
				GAMEAPPVIEW->m_snooker->m_customTable->SnookerTableChange(M3d_table_ptr);
				GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[idx].tableColour, M3d_table_ptr);

				GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table
			}

			if(m_table[idx].rackType !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_rackType = m_table[idx].rackType;
				GAMEAPPVIEW->m_snooker->m_wildRackType = m_table[idx].rackType;			
			}
			

			if(m_table[idx].cueError !=-1)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_level = m_table[idx].cueError;
				#ifndef MAIN_SERVER
				GAMEAPPVIEW->m_snooker->m_cueError = m_table[idx].cueError;
				#endif //!MAIN_SERVER
			}

			// table setup details
			if(m_table[idx].shotTime !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_shotTime = m_table[idx].shotTime;
				//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
			}

			
			if(m_table[idx].addTimer !=0xff)
			{
				if(m_table[idx].addTimer == 0)
					GAMEAPPVIEW->m_tableSetupDlg2.m_bAddTimer = FALSE;
				else if(m_table[idx].addTimer == 1)
					GAMEAPPVIEW->m_tableSetupDlg2.m_bAddTimer = TRUE;
				//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
			}
			
			 	

			if(m_table[idx].tableColour !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_tableColour = m_table[idx].tableColour;
				//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
				
				int ballSize = m_table[idx].ballSize;
				
				int idxType = 0; // temporary - may need to tie back into table type later? [+]
				if(ballSize == 0) // Normal
					idxType = 0;
				else if(ballSize == 1) // GIANT
					idxType = 1;

				// Change color scheme - really here we should send a packet of data to all players about the new table properties!!
				int tableTypeIdx = m_table[idx].tableType;
				M3d_object *M3d_table_ptr;
				M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->SnookerTableGetType(idxType);//tableTypeIdx);
				GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[idx].tableColour, M3d_table_ptr);
				GAMEAPPVIEW->m_bUpdateTable = TRUE;
			}
			
			if(m_table[idx].gameType !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_gameType = m_table[idx].gameType;

				GAMEAPPVIEW->m_snooker->m_customTable->ChangeGameType(m_table[idx].gameType);

				//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
			}

			if(m_table[idx].raceTo !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_raceTo = m_table[idx].raceTo;
			}

			if(m_table[idx].whoBreaks !=0xff)
			{
				GAMEAPPVIEW->m_tableSetupDlg2.m_whoBreaks = m_table[idx].whoBreaks;
				if(m_table[idx].whoBreaks == 0)
				{
					// 1P Always
					GAMEAPPVIEW->m_snooker->m_bTurn1P = TRUE;
				}
				else if(m_table[idx].whoBreaks == 1)
				{
					// 2P Always
					GAMEAPPVIEW->m_snooker->m_bTurn1P = FALSE;
				}
			}
		}
	}
}


void CLobby::UpdateTableHostDetails(int cueError, BOOL bRated, char type, BOOL bMissRule, int playerSockIdx,
									BYTE shotTime, BYTE tableColour, BYTE gameType, BYTE addTimer, BYTE doubles,
									BYTE raceTo, BYTE whoBreaks,
									BYTE ballSize, BYTE rackType, BYTE tableType, BYTE proTable,
									int tournamentTable, int fp1, int fp2, int score1, int score2, bool bLadderMatch)
{
	int n;
	int state;
	BOOL bState;
	BOOL ForceUnRated = FALSE;
	//int plrIdx = GetPlayerIdx(playerSockIdx);

	int tblIdxOfHost = GetTableIdxFromPlayer(playerSockIdx);
	int plrIdxOfHost = GetPlayerIdx(playerSockIdx);
	int tblIdx = GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	int plrIdx = GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);
	
	
	

	if(tournamentTable != -1) // SHOULD BE -1 FOR NON-TOURNAMENT LOBBIES!
	{
		// override!
		tblIdxOfHost = tournamentTable;
		m_table[tblIdxOfHost].tournament = 2;
		m_table[tblIdxOfHost].racksWon1P = fp1; // thses are normally reset in PressedStartReceive() call
		m_table[tblIdxOfHost].racksWon2P = fp2;
		m_table[tblIdxOfHost].startScore1P = score1;
		m_table[tblIdxOfHost].startScore2P = score2;
		m_table[tblIdxOfHost].score1P = m_table[tblIdxOfHost].startScore1P;
		m_table[tblIdxOfHost].score2P = m_table[tblIdxOfHost].startScore2P;

		if (bLadderMatch)
		{
			// Special Case for Ladder Games, so lock the seats..
			// Lock Seats now, just in case someone tries to steal this table
			m_table[tblIdxOfHost].bLadderMatch = TRUE;
			m_table[tblIdxOfHost].bLockSeats = TRUE; // block seats, so nobody else can use them
		}

	}

	//char buff[80];
	//sprintf(buff,"tblIdxOfHost = %d, tblIdx = %d, bCueError = %d, bRated = %d",tblIdxOfHost, tblIdx, bCueError, bRated);
	//MessageBox(0,buff,"",MB_OK);
	//return;	


	

	// check if host of this table is a 'guest', if so, override settings to stop hackers ;)
	if(plrIdxOfHost != -1)
	if(m_player[plrIdxOfHost].bIsGuest)
	{
		if(g_build.gameType == GAMETYPE_SNOOKER) // iSnooker
		{
			//cueError
			//bRated
			//bmissRule
			if(type>=0) type = 0;
			
			if(shotTime!=0xff) shotTime = 20;
			if(tableColour!=0xff) tableColour = 2;
			if(gameType!=0xff) gameType = 1;		// 10 Reds //
			if(addTimer!=0xff) addTimer = 0;
			if(doubles!=0xff) doubles = 0;

			if(raceTo!=0xff) raceTo = 0;
			if(whoBreaks!=0xff) whoBreaks = 2;
			
			if(ballSize!=0xff) ballSize = 0;
			if(rackType!=0xff) rackType = 0;
			if(tableType!=0xff) tableType = 0;	
		}
		else // iPool
		{
			//cueError
			//bRated
			//bmissRule (un-used)
			if(type>=0) type = 0;
						
			if(shotTime!=0xff) shotTime = 20;
			if(tableColour!=0xff) tableColour = 0;
			if(gameType!=0xff) gameType = 0;
			if(addTimer!=0xff) addTimer = 0;
			if(doubles!=0xff) doubles = 0;

			if(raceTo!=0xff) raceTo = 0;
			if(whoBreaks!=0xff) whoBreaks = 2;
			
			if(ballSize!=0xff) ballSize = 0;
			if(rackType!=0xff) rackType = 0;
			if(tableType!=0xff) tableType = 0;	
			if(proTable!=0xff) proTable = 1;
		}
	}


	if(tblIdxOfHost != -1)
	{
		//////////////////////////////////////////////////////////////////////
		// if game hasn't started and only 1 player has pressed start, then clear the table press start flags (*NEW*)
		if(!m_table[tblIdxOfHost].bGameStarted)
		{			
			// cancel start pressed!
			m_table[tblIdxOfHost].P1StartAtTable = -1;
			m_table[tblIdxOfHost].P2StartAtTable = -1;
			m_table[tblIdxOfHost].P1StartName[0] = 0;
			m_table[tblIdxOfHost].P2StartName[0] = 0;

			m_table[tblIdxOfHost].P1bStartAtTable = -1;
			m_table[tblIdxOfHost].P2bStartAtTable = -1;
			m_table[tblIdxOfHost].P1bStartName[0] = 0;
			m_table[tblIdxOfHost].P2bStartName[0] = 0;

			m_table[tblIdxOfHost].m_lostConn1P = 0;
			m_table[tblIdxOfHost].m_lostConn2P = 0;
		}


		/////////////////////////////////////////////////////////////
		// update table data for all



		if(cueError >= 0)
		{ 
			m_table[tblIdxOfHost].cueError = cueError;
		}



		// If we are trying to change Rated when currently 6-Reds, Doubles or TableType >= 2, don't allow it //
		if(bRated)
		if(g_build.gameType == GAMETYPE_POOL)			
			{
			if(m_table[tblIdxOfHost].gameType == 4 || m_table[tblIdxOfHost].doubles || m_table[tblIdxOfHost].tableType > 2)
				{
				bRated = FALSE;

				// check whether this player is also at the same table as host
				if(tblIdx == tblIdxOfHost)
					{
					// Tell the people playing that the Table Setup have been changed //
					GAMEAPPVIEW->Message("*** This Game / Table Configuration cannot be Rated ***",10); // system message
					}
				return;
				}
			}

			


		if(bRated)
		{ // do toggle...
			char bstate = GAMEAPPVIEW->m_ratedButton.GetCheck();

			bState = m_table[tblIdxOfHost].bRated;
			if(bState)
				m_table[tblIdxOfHost].bRated = FALSE;
			else
				m_table[tblIdxOfHost].bRated = TRUE;
		}

		// table type details
		if(type>=0)
		{
			m_table[tblIdxOfHost].type = type;
			UpdateTableItemButtons(tblIdxOfHost, bLadderMatch);

			// display table type info.
			if(m_table[tblIdxOfHost].hostSockIdx != -1)
			if(m_table[tblIdxOfHost].hostSockIdx == GAMEAPPVIEW->m_playerSockIdx)
			{
				char _buffer[80];
				
				switch(type)
				{
					case Table_typ::TYPE_PUBLIC:

					if(!g_bTournamentLobby && bLadderMatch == false)
						sprintf(_buffer,"*** Anyone can play/watch ***");
					else
						sprintf(_buffer,"*** Anyone can watch ***");
					GAMEAPPVIEW->Message(_buffer,10); // personal message
					break;

					case Table_typ::TYPE_PROTECTED:
					if(!g_bTournamentLobby && bLadderMatch == false)					
						sprintf(_buffer,"*** Only invited players can play ***");
					else
						sprintf(_buffer,"*** Anyone can watch (SAME AS PUBLIC) ***");
					GAMEAPPVIEW->Message(_buffer,10); // personal message
					break;

					case Table_typ::TYPE_PRIVATE:
					if(!g_bTournamentLobby && bLadderMatch == false)
						sprintf(_buffer,"*** Only invited players can play/watch ***");
					else
						sprintf(_buffer,"*** Only invited players can watch ***");
					GAMEAPPVIEW->Message(_buffer,10); // personal message
					break;
				}	
			}
		}

		if(bMissRule)
		{ // do toggle...
			bState = m_table[tblIdxOfHost].bMissRule;
			if(bState)
				m_table[tblIdxOfHost].bMissRule = FALSE;
			else
				m_table[tblIdxOfHost].bMissRule = TRUE;
		}
		
		// table setup details
		if(shotTime !=0xff)
		{
			m_table[tblIdxOfHost].shotTime = shotTime;
		}
		

		if(doubles !=0xff)
		{
			m_table[tblIdxOfHost].doubles = doubles;
			// Hide Doubles Buttons if we are not in Doubles Mode //
			if(doubles == 0)
				{
				//g_gameAppView->m_sit1PButtonB.ShowWindow(SW_HIDE);
				//g_gameAppView->m_sit2PButtonB.ShowWindow(SW_HIDE);
				
				//Also Force players to stand up.. if seated //
				int idx1 = m_table[tblIdxOfHost].player1bSockIdx;
				int idx2 = m_table[tblIdxOfHost].player2bSockIdx;
				SitStandTable(idx1, idx1, tblIdxOfHost, idx1, idx1);
				SitStandTable(idx2, idx2, tblIdxOfHost, idx2, idx2);

				}
			else{
				//g_gameAppView->m_sit1PButtonB.ShowWindow(SW_SHOW);
				//g_gameAppView->m_sit2PButtonB.ShowWindow(SW_SHOW);
				}
		}

		if(addTimer !=0xff)
		{
			m_table[tblIdxOfHost].addTimer = addTimer;
		}

		if(tableColour !=0xff)
		{
			m_table[tblIdxOfHost].tableColour = tableColour;
		}
		
		if(gameType !=0xff)
		{
			m_table[tblIdxOfHost].gameType = gameType;
				
		}

		if(raceTo !=0xff)
		{
			m_table[tblIdxOfHost].raceTo = raceTo;
		}

		if(whoBreaks !=0xff)
		{
			m_table[tblIdxOfHost].whoBreaks = whoBreaks;
		}

		if(ballSize !=0xff)
		{
			m_table[tblIdxOfHost].ballSize = ballSize;
		}

		if(rackType !=0xff)
		{
			m_table[tblIdxOfHost].rackType = rackType;
		}

		if(tableType !=0xff)
		{
			m_table[tblIdxOfHost].tableType = tableType;
			
			
			if(g_build.gameType == GAMETYPE_POOL)			
				{
				if(tableType == 1) m_table[tblIdxOfHost].proTable = 1;
					else m_table[tblIdxOfHost].proTable = 0;

				/*
				// Only Std, Pro, UK configurations can be rated //
				if(tableType >= 3) // Non - Std, Pro or UK Table
					{
					bRated = TRUE;
					ForceUnRated = TRUE;
					}
				*/
				}
		}
		/*
		if(proTable !=0xff)
		{
			m_table[tblIdxOfHost].proTable = proTable;
		}
		*/
		AddTableOptionsItem(tblIdxOfHost);


		/////////////////////////////////////////////////////////////
		
		// check whether this player is also at the same table as host
		if(tblIdx == tblIdxOfHost)
		{
			// Tell the people playing that the Table Setup have been changed //
			GAMEAPPVIEW->Message("*** Table Setup Changed ***",10); // system message	

			if(cueError >= 0)
			{	
				//GAMEAPPVIEW->m_cueErrorBox.SetCurSel(cueError);
				
				if(g_build.gameType == GAMETYPE_SNOOKER)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_level = cueError;
				}

				#ifndef MAIN_SERVER
				GAMEAPPVIEW->m_snooker->m_cueError = cueError;
				#endif // !MAIN_SERVER

				

				// do toggle...
				char _buffer[80];
				switch(cueError)
				{
				case 0:// beginner
					{					
					sprintf(_buffer,"*** Beginner Level (No-Error) ***");
					GAMEAPPVIEW->Message(_buffer,10); // system message					
					}
					break;
				/*
				case 1:// amateur (old semi-pro)
					{
					sprintf(_buffer,"*** Amateur Level ***");
					GAMEAPPVIEW->Message(_buffer,10); // system message
					}
					break;
				*/
				case 1:// semi-pro (amateur)
					{
					sprintf(_buffer,"*** Semi-Pro Level ***");
					GAMEAPPVIEW->Message(_buffer,10); // system message											
					}
					break;				
				case 2:// pro
					{
					sprintf(_buffer,"*** Pro Level (Realistic Play) ***");
					GAMEAPPVIEW->Message(_buffer,10); // system message
					}
					break;
				}
			}


			if(bRated)
			{ // do toggle...
				state = GAMEAPPVIEW->m_ratedButton.GetCheck();
				
				// If the ForceUnRated var is on, then we must make sure the game is unrated //
				//if(ForceUnRated) state = 1;
				
				switch(state)
				{
				case 0:// toggle to ON
					{
					GAMEAPPVIEW->m_ratedButton.SetCheck(1);
					#ifndef MAIN_SERVER
					GAMEAPPVIEW->m_snooker->m_bRated = TRUE;
					char _buffer[80];
					sprintf(_buffer,"*** This game is 'Rated' ***");
					GAMEAPPVIEW->Message(_buffer,10); // system message
					#endif //!MAIN_SERVER
					}
					break;
				case 1:// toggle to OFF
					{
					GAMEAPPVIEW->m_ratedButton.SetCheck(0);
					#ifndef MAIN_SERVER
					GAMEAPPVIEW->m_snooker->m_bRated = FALSE;
					char _buffer[80];
					sprintf(_buffer,"*** This game is 'Unrated' ***");
					GAMEAPPVIEW->Message(_buffer,10); // system message
					#endif //!MAIN_SERVER
					}
					break;
				}
			}

			

			// table type details
			if(type>=0)
			{
				GAMEAPPVIEW->m_tableTypeBox.SetCurSel(type);
				if(!type || m_player[plrIdx].bInvited)
					GAMEAPPVIEW->EnableSeats(plrIdx, TRUE);
				else
					GAMEAPPVIEW->EnableSeats(plrIdx, FALSE);
			}

			if(bMissRule)
			{ // do toggle...
				state = GAMEAPPVIEW->m_missRuleButton.GetCheck();
				switch(state)
				{
				case 0:// toggle to ON
					{
					GAMEAPPVIEW->m_missRuleButton.SetCheck(1);
					#ifndef MAIN_SERVER
					
					if(g_build.gameType == GAMETYPE_SNOOKER)
					{
						GAMEAPPVIEW->m_snooker->m_bMissRule = TRUE;
						char _buffer[80];
						sprintf(_buffer,"*** 'Miss Rule' Added ***");
						GAMEAPPVIEW->Message(_buffer,10); // system message
					}
					else
					{
						GAMEAPPVIEW->m_snooker->m_bCallShot = TRUE;
						char _buffer[80];
						sprintf(_buffer,"*** 'Call Shot' Added ***");
						GAMEAPPVIEW->Message(_buffer,10); // system message
					}

					#endif //!MAIN_SERVER
					}
					break;
				case 1:// toggle to OFF
					{
					GAMEAPPVIEW->m_missRuleButton.SetCheck(0);
					#ifndef MAIN_SERVER
					
					if(g_build.gameType == GAMETYPE_SNOOKER)
					{
						GAMEAPPVIEW->m_snooker->m_bMissRule = FALSE;
						char _buffer[80];
						sprintf(_buffer,"*** 'Miss Rule' Removed ***");
						GAMEAPPVIEW->Message(_buffer,10); // system message
					}
					else
					{
						GAMEAPPVIEW->m_snooker->m_bCallShot = FALSE;
						char _buffer[80];
						sprintf(_buffer,"*** 'Call Shot' Removed ***");
						GAMEAPPVIEW->Message(_buffer,10); // system message
					}

					#endif //!MAIN_SERVER
					}
					break;
				}
			}


			if(g_build.gameType == GAMETYPE_POOL)
			{
				
			
				if(ballSize !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg.m_ballSize = m_table[tblIdxOfHost].ballSize;
					if(m_table[tblIdxOfHost].ballSize == 0) // Normal
						g_ballRadiusFactor = 1.0f;
					else if(m_table[tblIdxOfHost].ballSize == 1) // BIG
						g_ballRadiusFactor = 1.5f;					
					else if(m_table[tblIdxOfHost].ballSize == 2) // small
						g_ballRadiusFactor = 0.75;
					g_ballRadius = 28.4625f * g_ballRadiusFactor;
					GAMEAPPVIEW->m_snooker->m_customTable->ReNormalizeBalls();
				}

			
				if(tableType !=0xff || ballSize !=0xff) // allow ball size in here (important, must change table settings)
				{
					GAMEAPPVIEW->m_tableSetupDlg.m_tableType = m_table[tblIdxOfHost].tableType;
					
					int tableTypeIdx = m_table[tblIdxOfHost].tableType;
					M3d_object *M3d_table_ptr;
					M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->PoolTableGetType(tableTypeIdx);
					GAMEAPPVIEW->m_snooker->m_customTable->PoolTableChange(M3d_table_ptr, tableTypeIdx);
					GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[tblIdxOfHost].tableColour, M3d_table_ptr);
					GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table

					if(M3d_table_ptr == &GAMEAPPVIEW->m_snooker->m_customTable->m_M3d_proTable) GAMEAPPVIEW->m_tableSetupDlg.m_proTable = TRUE;
						else GAMEAPPVIEW->m_tableSetupDlg.m_proTable = FALSE;
				}

				// table setup details
				if(shotTime !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg.m_shotTime = m_table[tblIdxOfHost].shotTime;
					//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);
				}

				if(addTimer !=0xff)
				{
					if(m_table[tblIdxOfHost].addTimer == 0)
						GAMEAPPVIEW->m_tableSetupDlg.m_bAddTimer = FALSE;
					else if(m_table[tblIdxOfHost].addTimer == 1)
						GAMEAPPVIEW->m_tableSetupDlg.m_bAddTimer = TRUE;
					//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);
				}

			 
			
				if(tableColour !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg.m_tableColour = m_table[tblIdxOfHost].tableColour;
					//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);

					// Change color scheme - really here we should send a packet of data to all players about the new table properties!!
					int tableTypeIdx = m_table[tblIdxOfHost].tableType;
					M3d_object *M3d_table_ptr;
					if(m_table[tblIdxOfHost].proTable &&  m_table[tblIdxOfHost].tableType == 0)
						M3d_table_ptr = &GAMEAPPVIEW->m_snooker->m_customTable->m_M3d_proTable;
					else
						M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->PoolTableGetType(tableTypeIdx);

					GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[tblIdxOfHost].tableColour, M3d_table_ptr);
					GAMEAPPVIEW->m_bUpdateTable = TRUE;
				}
				
				if(gameType !=0xff)
					{	
					GAMEAPPVIEW->m_tableSetupDlg.m_gameType = m_table[tblIdxOfHost].gameType;

					GAMEAPPVIEW->m_snooker->m_customTable->ChangeGameType(m_table[tblIdxOfHost].gameType);

					//GAMEAPPVIEW->m_tableSetupDlg.UpdateData(TRUE);
					GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table
					GAMEAPPVIEW->m_bOnSize = TRUE;
					GAMEAPPVIEW->m_bOnSizeDrawTmpSurf = TRUE;

					switch(gameType)
						{
						case 0:	// 8-Ball
								GAMEAPPVIEW->m_snooker->Init(RERACK_8BALL);
								break;
						case 1:	// 9-Ball
								GAMEAPPVIEW->m_snooker->Init(RERACK_9BALL);
								break;
						case 2:	// Rotation-Ball
								GAMEAPPVIEW->m_snooker->Init(RERACK_ROTATIONBALL);
								break;
						case 3:	// Bowls
								GAMEAPPVIEW->m_snooker->Init(RERACK_BOWLS);
								break;
						case 4:	// 6-Ball
								GAMEAPPVIEW->m_snooker->Init(RERACK_6BALL);
								break;
						case 5:	// 10-Ball
								GAMEAPPVIEW->m_snooker->Init(RERACK_10BALL);
								break;
						case 6:	// Break
								GAMEAPPVIEW->m_snooker->Init(RERACK_BREAK);
								break;
						case 7:	// English Pool
								GAMEAPPVIEW->m_snooker->Init(RERACK_UKPOOL);
								break;
						}

					}

				
				
				
				if(raceTo !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg.m_raceTo = m_table[tblIdxOfHost].raceTo;
				}

				if(whoBreaks !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg.m_whoBreaks = m_table[tblIdxOfHost].whoBreaks;
					if(m_table[tblIdxOfHost].whoBreaks == 0)
					{
						// 1P Always
						GAMEAPPVIEW->m_snooker->m_bTurn1P = TRUE;
					}
					else if(m_table[tblIdxOfHost].whoBreaks == 1)
					{
						// 2P Always
						GAMEAPPVIEW->m_snooker->m_bTurn1P = FALSE;
					}
				}

				if(rackType !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg.m_rackType = m_table[tblIdxOfHost].rackType;
					GAMEAPPVIEW->m_snooker->m_wildRackType = m_table[tblIdxOfHost].rackType;			
				}	
 			}
			else if(g_build.gameType == GAMETYPE_SNOOKER)
			{
				if(ballSize !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_ballSize = m_table[tblIdxOfHost].ballSize;

					int ballSize = m_table[tblIdxOfHost].ballSize;

					if(ballSize == 0) // Normal
						g_ballRadiusFactor = 1.0f;
					else if(ballSize == 1) // GIANT
						g_ballRadiusFactor = 1.5f;					
					g_ballRadius = 26.09f * g_ballRadiusFactor;
					GAMEAPPVIEW->m_snooker->m_customTable->ReNormalizeBalls();

					M3d_object *M3d_table_ptr;
					int idxType = 0; // temporary - may need to tie back into table type later? [+]
					if(ballSize == 0) // Normal
						idxType = 0;
					else if(ballSize == 1) // GIANT
						idxType = 1;

					M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->SnookerTableGetType(idxType);//tableTypeIdx);
					GAMEAPPVIEW->m_snooker->m_customTable->SnookerTableChange(M3d_table_ptr);
					GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[tblIdxOfHost].tableColour, M3d_table_ptr);
					GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table
				}
				
				if(tableType !=0xff || ballSize !=0xff) // allow ball size in here (important, must change table settings)
				{		
					/*
					GAMEAPPVIEW->m_tableSetupDlg2.m_tableType = m_table[tblIdxOfHost].tableType;
					
					int tableTypeIdx = m_table[tblIdxOfHost].tableType;
					M3d_object *M3d_table_ptr;
					M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->PoolTableGetType(tableTypeIdx);
					GAMEAPPVIEW->m_snooker->m_customTable->PoolTableChange(M3d_table_ptr, tableTypeIdx);
					GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[tblIdxOfHost].tableColour, M3d_table_ptr);
					GAMEAPPVIEW->m_bUpdateTable = TRUE; // force a redraw of table
					*/
				}
				
				if(rackType !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_rackType = m_table[tblIdxOfHost].rackType;
					GAMEAPPVIEW->m_snooker->m_wildRackType = m_table[tblIdxOfHost].rackType;			
				}
				
				
				// table setup details
				if(shotTime !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_shotTime = m_table[tblIdxOfHost].shotTime;
					//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
				}

				if(addTimer !=0xff)
				{
					if(m_table[tblIdxOfHost].addTimer == 0)
						GAMEAPPVIEW->m_tableSetupDlg2.m_bAddTimer = FALSE;
					else if(m_table[tblIdxOfHost].addTimer == 1)
						GAMEAPPVIEW->m_tableSetupDlg2.m_bAddTimer = TRUE;
					//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
				}

				if(doubles !=0xff)
				{
					if(m_table[tblIdxOfHost].doubles == 0)
						GAMEAPPVIEW->m_tableSetupDlg2.m_doubles = FALSE;
					else if(m_table[tblIdxOfHost].doubles == 1)
						GAMEAPPVIEW->m_tableSetupDlg2.m_doubles = TRUE;
					//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
				}
			 				
				if(tableColour !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_tableColour = m_table[tblIdxOfHost].tableColour;
					//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);
					
					int ballSize = m_table[tblIdxOfHost].ballSize;
					
					int idxType = 0; // temporary - may need to tie back into table type later? [+]
					if(ballSize == 0) // Normal
						idxType = 0;
					else if(ballSize == 1) // GIANT
						idxType = 1;

					// Change color scheme - really here we should send a packet of data to all players about the new table properties!!
					int tableTypeIdx = m_table[tblIdxOfHost].tableType;
					M3d_object *M3d_table_ptr;
					M3d_table_ptr = GAMEAPPVIEW->m_snooker->m_customTable->SnookerTableGetType(idxType);//tableTypeIdx);
					GAMEAPPVIEW->m_snooker->m_customTable->ChangeColorScheme(m_table[tblIdxOfHost].tableColour, M3d_table_ptr);
					GAMEAPPVIEW->m_bUpdateTable = TRUE;
				}
				
				if(gameType !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_gameType = m_table[tblIdxOfHost].gameType;

					GAMEAPPVIEW->m_snooker->m_customTable->ChangeGameType(m_table[tblIdxOfHost].gameType);

					//GAMEAPPVIEW->m_tableSetupDlg2.UpdateData(TRUE);

				}

				if(raceTo !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_raceTo = m_table[tblIdxOfHost].raceTo;
				}

				if(whoBreaks !=0xff)
				{
					GAMEAPPVIEW->m_tableSetupDlg2.m_whoBreaks = m_table[tblIdxOfHost].whoBreaks;
					if(m_table[tblIdxOfHost].whoBreaks == 0)
					{
						// 1P Always
						GAMEAPPVIEW->m_snooker->m_bTurn1P = TRUE;
					}
					else if(m_table[tblIdxOfHost].whoBreaks == 1)
					{
						// 2P Always
						GAMEAPPVIEW->m_snooker->m_bTurn1P = FALSE;
					}
				}
 			}


			if(g_build.gameType == GAMETYPE_POOL)
			{
				// refresh table window title 
				CString title;
				
				char wildStr[8];
				wildStr[0] = 0;
				if(IsTableWILD(tblIdx)) // check for WILD tables
					sprintf(wildStr, "WILD? ");

				char buff[32];
				buff[0] = 0;
				
				if(m_table[tblIdxOfHost].gameType == 0) sprintf(buff, "8-Ball");
				else if(m_table[tblIdxOfHost].gameType == 1) sprintf(buff, "9-Ball");
				else if(m_table[tblIdxOfHost].gameType == 2) sprintf(buff, "Rotation-Ball");
				else if(m_table[tblIdxOfHost].gameType == 3) sprintf(buff, "Bowls");
				else if(m_table[tblIdxOfHost].gameType == 4) sprintf(buff, "6-Ball");
				else if(m_table[tblIdxOfHost].gameType == 5) sprintf(buff, "10-Ball");
				else if(m_table[tblIdxOfHost].gameType == 6) sprintf(buff, "UKPool");
				else if(m_table[tblIdxOfHost].gameType == 7) sprintf(buff, "Break");

				if(plrIdxOfHost != -1)
				{
					title.Format(" Table: %d - %s%s -  Host: %s", m_player[plrIdxOfHost].table+1, wildStr, buff, m_player[plrIdxOfHost].name);
					GAMEAPPVIEW->ChangeTitle(title);
				}
			}
			else if(g_build.gameType == GAMETYPE_SNOOKER)
			{
				// refresh table window title 
				CString title;

				char wildStr[8];
				wildStr[0] = 0;
				if(IsTableWILD(tblIdx)) // check for WILD tables
					sprintf(wildStr, "WILD? ");

				char buff[32];
				buff[0] = 0;
				
				if(m_table[tblIdxOfHost].gameType == 0) sprintf(buff, "Snooker");
				else if(m_table[tblIdxOfHost].gameType == 1) sprintf(buff, "Short-Snooker");
				else if(m_table[tblIdxOfHost].gameType == 2) sprintf(buff, "Mini-Snooker");
				else if(m_table[tblIdxOfHost].gameType == 3) sprintf(buff, "Re-Spot Black");
				else if(m_table[tblIdxOfHost].gameType == 4) sprintf(buff, "Billiards (50 pts)");
				else if(m_table[tblIdxOfHost].gameType == 5) sprintf(buff, "Billiards (100 pts)");
				else if(m_table[tblIdxOfHost].gameType == 6) sprintf(buff, "Billiards (200 pts)");

				char buff2[32];
				buff2[0] = 0;
				if(m_table[tblIdxOfHost].cueError == 0) sprintf(buff2, "Beginner");
				//else if(m_table[tblIdxOfHost].cueError == 1) sprintf(buff2, "Amateur");
				else if(m_table[tblIdxOfHost].cueError == 1) sprintf(buff2, "Semi-Pro");
				else if(m_table[tblIdxOfHost].cueError == 2) sprintf(buff2, "Pro");

				if(plrIdxOfHost != -1)
				{
					title.Format(" Table: %d - %s%s - %s -  Host: %s", m_player[plrIdxOfHost].table+1, wildStr, buff, buff2, m_player[plrIdxOfHost].name);
					GAMEAPPVIEW->ChangeTitle(title);
				}
			}
			
			//GAMEAPPVIEW->UpdateWindowTitle(tblIdxOfHost);
		}
		
		/////////////////////////////////////////////////////////////
	}	
}

int CLobby::GetPlayerIdxFromTable(int tableIdx, int enterType, int watcher)
{
	if(tableIdx<0 || tableIdx>=MAX_TABLEITEMS) return -1; // NEW 15/FEB/2004

	// find host's idx from player socket idx & set table host
	if(m_table[tableIdx].status)
	{							 
		int playerSockIdx = -1;
		switch(enterType)
		{
			case 0:
				if(watcher == -1 || watcher >=MAX_TABLE_PLAYERS)
					return -1;
				
				playerSockIdx = m_table[tableIdx].playersSockIdx[watcher];
				//if(!m_table[tableIdx].playersSockIdxN) return -1;
				//playerSockIdx = m_table[tableIdx].playersSockIdx[m_table[tableIdx].playersSockIdxN-1];
			break;

			case 1:
			playerSockIdx = m_table[tableIdx].player1SockIdx;
			break;

			case 2:
			playerSockIdx = m_table[tableIdx].player2SockIdx;
			break;
			
			case 3:
			playerSockIdx = m_table[tableIdx].player1bSockIdx;
			break;
			
			case 4:
			playerSockIdx = m_table[tableIdx].player2bSockIdx;
			break;
		}
			
		for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
		if(m_player[n].status)
		if(m_player[n].playerSocketIdx == playerSockIdx)
		{
			return n;
		}
	}

	return -1;
}

int CLobby::GetHostIdxFromTable(int tableIdx)
{
	if(tableIdx<0 || tableIdx>=MAX_TABLEITEMS) return -1; // NEW 15/FEB/2004

	// find host's idx from player socket idx & set table host
	if(m_table[tableIdx].status)
	{							 
		int hostSockIdx = m_table[tableIdx].hostSockIdx;
		if(hostSockIdx == -1) return -1;
					
		for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
		if(m_player[n].status)
		if(m_player[n].playerSocketIdx == hostSockIdx)
		{
			return n;
		}
	}

	return -1;
}

BOOL CLobby::HostOfTable(int plrSockIdx)
{
	if(plrSockIdx == -1)
		return FALSE;

	int n;
	for(n=0;n<MAX_TABLEITEMS;n++)
	if(m_table[n].status)
	if(m_table[n].hostSockIdx == plrSockIdx)
		return TRUE;

	return FALSE;
}

int CLobby::GetTableIdxFromPlayer(int plrSockIdx)
{
	int plrIdx = GetPlayerIdx(plrSockIdx);
	if(plrIdx == -1) return -1;
	else return m_player[plrIdx].table;
}


void CLobby::ProcessTables(void)
{

}


									  
// Table Items functions
void CLobby::UpdateTableItemButtons(int nItem, bool bLadderMatch)
{
	//if(!g_oncreatecomplete) return;

	if(nItem<0 || nItem>=MAX_TABLEITEMS) return; // NEW 15/FEB/2004

	// table type details
	int type = m_table[nItem].type;

	bLadderMatch = m_table[nItem].bLadderMatch;

	// secial case for 'TOURNAMENT' Lobby...
	
	if(g_bTournamentLobby)
	{		
		// control table buttons (enable/disable)
		switch(type)
		{
			case Table_typ::TYPE_PUBLIC:
			case Table_typ::TYPE_PROTECTED:
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+0, TRUE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+1, TRUE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+2, FALSE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+3, FALSE);
			break;			

			case Table_typ::TYPE_PRIVATE:
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+0, TRUE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+1, FALSE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+2, FALSE);
			GAMEAPPVIEW->m_listTables.EnableItemButton(nItem*4+3, FALSE);
			break;			
		}		
	}
	else
	{ // default type Lobby...		
		// control table buttons (enable/disable)

		if (bLadderMatch)
		{
			switch (type)
			{
			case Table_typ::TYPE_PUBLIC:
			case Table_typ::TYPE_PROTECTED:
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 0, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 1, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 2, FALSE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 3, FALSE);
				break;

			case Table_typ::TYPE_PRIVATE:
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 0, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 1, FALSE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 2, FALSE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 3, FALSE);
				break;
			}
		}
		else
		{
			switch (type)
			{
			case Table_typ::TYPE_PUBLIC:
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 0, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 1, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 2, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 3, TRUE);
				break;

			case Table_typ::TYPE_PROTECTED:
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 0, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 1, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 2, FALSE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 3, FALSE);
				break;

			case Table_typ::TYPE_PRIVATE:
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 0, TRUE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 1, FALSE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 2, FALSE);
				GAMEAPPVIEW->m_listTables.EnableItemButton(nItem * 4 + 3, FALSE);
				break;
			}
		}
	}

}



void CLobby::AddTableItem(int nItem, int enterType)
{
	
	if(nItem<0 || nItem>=MAX_TABLEITEMS) return; // NEW 15/FEB/2004

	int plrIdx = -1;
	int plrIdxb = -1;


	// Get Partner's Idx if one exists //
	if(m_table[nItem].doubles) 
		{
		switch(enterType)
			{
			case 0:	// watcher //
					plrIdx	= GetPlayerIdxFromTable(nItem, 0);
					break;

			case 1:	// Team 1	
			case 3:	// Team 1
					plrIdx	= GetPlayerIdxFromTable(nItem, 1);
					plrIdxb = GetPlayerIdxFromTable(nItem, 3);
					break;

			case 2:	// Team 2	
			case 4:	// Team 2
					plrIdx	= GetPlayerIdxFromTable(nItem, 2);
					plrIdxb = GetPlayerIdxFromTable(nItem, 4);
					break;
			}
		}
	else plrIdx = GetPlayerIdxFromTable(nItem, enterType);

	CString item;
	// add or remove sub-item
	if(enterType == 1 || enterType == 3) // player 1 seat
		{
			if(m_table[nItem].doubles)
				{
				if(plrIdx != -1) item.Format("%s", m_player[plrIdx].name);
					else if(plrIdxb != -1) item.Format("%s", m_player[plrIdxb].name);
					else item.Format("");
				}
			else{ 
				if(plrIdx != -1) item.Format("%s", m_player[plrIdx].name);
					else item.Format("");
				}
		/*
			((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 3, &item);
			((CGameAppView *)m_parent)->m_listTables.HideItemButton(nItem*4+(3-1));
		*/
		if(plrIdx != -1 || plrIdxb != -1)
			{
			((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 3, &item);
			((CGameAppView *)m_parent)->m_listTables.HideItemButton(nItem*4+(3-1));
			}
		else{
			((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 3, &item);
			if(m_table[nItem].type == Table_typ::TYPE_PUBLIC && !m_table[nItem].tournament)
				((CGameAppView *)m_parent)->m_listTables.ShowItemButton(nItem*4+(3-1));
			else
				((CGameAppView *)m_parent)->m_listTables.ShowItemButton(nItem*4+(3-1), FALSE);
			}


		}
	else if(enterType == 2 || enterType == 4) // player 2 seat
		{
			if(m_table[nItem].doubles)
				{
				if(plrIdx != -1) item.Format("%s", m_player[plrIdx].name);
					else if(plrIdxb != -1) item.Format("%s", m_player[plrIdxb].name);
					else item.Format("");
				}
			else{ 
				if(plrIdx != -1) item.Format("%s", m_player[plrIdx].name);
					else item.Format("");
				}
			/*
			((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 4, &item);
			((CGameAppView *)m_parent)->m_listTables.HideItemButton(nItem*4+(4-1));
			*/
		if(plrIdx != -1 || plrIdxb != -1)
			{
			((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 4, &item);
			((CGameAppView *)m_parent)->m_listTables.HideItemButton(nItem*4+(4-1));
			}
		else{
			((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 4, &item);
			if(m_table[nItem].type == Table_typ::TYPE_PUBLIC && !m_table[nItem].tournament)
				((CGameAppView *)m_parent)->m_listTables.ShowItemButton(nItem*4+(4-1));
			else
				((CGameAppView *)m_parent)->m_listTables.ShowItemButton(nItem*4+(4-1), FALSE);
			}
		}
	else if(enterType == 0) // players watching
		{
			CString item;
			CString listStr = _T(" ");
			
			for(int n=0;n<MAX_TABLE_PLAYERS;n++)//m_table[nItem].playersSockIdxN // NOTE: same as above** [-]
			{
				if(m_table[nItem].playersSockIdx[n] != -1)
				{
					int watchIdx = GetPlayerIdx(m_table[nItem].playersSockIdx[n]);
					if(watchIdx!=-1)
					//if(watchIdx == m_table[nItem].player1SockIdx);
					//else if(watchIdx == m_table[nItem].player2SockIdx);
					//else
					{
						listStr += m_player[watchIdx].name;
						listStr += _T(", ");
					}
				}
			}

				
			if(!listStr.GetLength())
				listStr = listStr.Left( listStr.GetLength()-1 );
						 
			item = listStr;
			((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 5, &item);
		}
	
}


CString CLobby::GetTableOptionsItemStr(int nItem)
{
	if(nItem<0 || nItem>=MAX_TABLEITEMS) return _T("NULL"); // NEW 15/FEB/2004

	BOOL bTourneyTableUsed = FALSE;	
	if(m_table[nItem].tournament >= 2)
		//if(g_tournament.bIsTableUsed(nItem))
			bTourneyTableUsed = TRUE;

	CString item;

	item.Format("Options:"); 
						  
	if(m_table[nItem].status || bTourneyTableUsed)
	{
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			if(IsTableWILD(nItem)) // check for WILD tables
				item = item + _T(" WILD?,");

			if(m_table[nItem].gameType == 0) // Snooker
				item = item + _T(" Snooker,");
			else if(m_table[nItem].gameType == 1) // Short-Snooker
				item = item + _T(" Short-Snooker,");
			else if(m_table[nItem].gameType == 2) // Mini-Snooker
				item = item + _T(" Mini-Snooker,");
			else if(m_table[nItem].gameType == 3) // Re-Spot Black
				item = item + _T(" Re-Spot Black,");
			else if(m_table[nItem].gameType == 4) // Billiards (50 pts)
				item = item + _T(" Billiards,");
			else if(m_table[nItem].gameType == 5) // Billiards (100 pts)
				item = item + _T(" Billiards,");
			else if(m_table[nItem].gameType == 6) // Billiards (200 pts)
				item = item + _T(" Billiards,");

			if(m_table[nItem].cueError == 2)
				item = item + _T(" Pro Level,");//Cue-Error,");
			else if(m_table[nItem].cueError == 1)
				item = item + _T(" Semi-Pro Level,");
			else if (m_table[nItem].cueError == 0)
				item = item + _T(" Beginner Level,");
						
			if(m_table[nItem].addTimer)
			{
				CString str;
				str.Format(" Shot-Time:%dseconds,", m_table[nItem].shotTime);
				item = item + str;
			}

			if(m_table[nItem].bRated)
				item = item + _T(" Rated,");

			if(m_table[nItem].type == Table_typ::TYPE_PUBLIC)
				item = item + _T(" Public");
			else if(m_table[nItem].type == Table_typ::TYPE_PROTECTED)
				item = item + _T(" Protected");
			else if(m_table[nItem].type == Table_typ::TYPE_PRIVATE)
				item = item + _T(" Private");
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			
			if(IsTableWILD(nItem)) // check for WILD tables
				item = item + _T(" WILD?,");
			else{
				if(m_table[nItem].tableType == 0) // Standard Table
					item = item + _T(" *Std*,");
				if(m_table[nItem].tableType == 1) // Pro Table
					item = item + _T(" *Pro*,");
				if(m_table[nItem].tableType == 2) // UK Table
					item = item + _T(" *UK*,");
				}
			
			/*
			if(m_table[nItem].proTable == 1) // *Pro*
				item = item + _T(" *Pro*,");
			*/
			if(m_table[nItem].gameType == 0) // 8-Ball
				item = item + _T(" 8-Ball,");
			else if(m_table[nItem].gameType == 1) // 9-Ball
				item = item + _T(" 9-Ball,");
			else if(m_table[nItem].gameType == 2) // Rotation-Ball
				item = item + _T(" Rotation-Ball,");
			else if(m_table[nItem].gameType == 3) // Bowls
				item = item + _T(" Bowls,");
			else if(m_table[nItem].gameType == 4) // 6-Ball
				item = item + _T(" 6-Ball,");
			else if(m_table[nItem].gameType == 5) // 10-Ball
				item = item + _T(" 10-Ball,");
			else if(m_table[nItem].gameType == 6) // Break
				item = item + _T(" Break,");
			else if(m_table[nItem].gameType == 7) // English Pool
				item = item + _T(" English Pool,");
			
			if(m_table[nItem].addTimer)
			{
				CString str;
				str.Format(" Shot-Time:%dseconds,", m_table[nItem].shotTime);
				item = item + str;
			}

			if(m_table[nItem].bRated)
				item = item + _T(" Rated,");

			if(m_table[nItem].type == Table_typ::TYPE_PUBLIC)
				item = item + _T(" Public");
			else if(m_table[nItem].type == Table_typ::TYPE_PROTECTED)
				item = item + _T(" Protected");
			else if(m_table[nItem].type == Table_typ::TYPE_PRIVATE)
				item = item + _T(" Private");
		}
	}	

	return item;
}

void CLobby::AddTableOptionsItem(int nItem)
{
	if(nItem<0 || nItem>=MAX_TABLEITEMS) return; // NEW 15/FEB/2004
	CString item;
	item = GetTableOptionsItemStr(nItem);

	
	//((CGameAppView *)m_parent)->m_listTables.get
	((CGameAppView *)m_parent)->m_listTables.AddSplitText(nItem, item.GetBuffer(0));
	
	((CGameAppView *)m_parent)->m_listTables.RedrawItems(nItem, nItem);
		
	
	//BAP_FIX // ((CGameAppView *)m_parent)->m_listTables.RedrawItems(nItem, nItem);
	
}

void CLobby::ClearTableItem(int nItem)
{
	if(nItem<0 || nItem>=MAX_TABLEITEMS) return; // NEW 15/FEB/2004

	// remove sub-item
	{
		CString item;
		item.Format("");
		((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 3, &item);
		((CGameAppView *)m_parent)->m_listTables.ShowItemButton(nItem*4+(1));
	}
	
	{
		CString item;
		item.Format("");
		((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 4, &item);
		((CGameAppView *)m_parent)->m_listTables.ShowItemButton(nItem*4+(2));
	}
	
	{
		CString item;
		CString listStr = _T("");
		item = listStr;
		((CGameAppView *)m_parent)->m_listTables.AddSubItemText(nItem, 5, &item);
		((CGameAppView *)m_parent)->m_listTables.ShowItemButton(nItem*4+(3));		
	}



	// clear table options
	CString item;

	item.Format("Options:"); 
	
	((CGameAppView *)m_parent)->m_listTables.AddSplitText(nItem, item.GetBuffer(0));
	//((CGameAppView *)m_parent)->m_listTables.RedrawItems(nItem, nItem);
	
}

void CLobby::ClearAllTableItems(void)
{
	for(int n=0;n<MAX_TABLEITEMS;n++)
		ClearTableItem(n);
}

BOOL CLobby::DeleteTableItem(int nItem)
{
	if(nItem<0 || nItem>=MAX_TABLEITEMS) return 0; // NEW 15/FEB/2004	

	return ((CGameAppView *)m_parent)->m_listTables.DeleteItem(nItem);
}

BOOL CLobby::DeleteAllTableItems(void)
{	
	return ((CGameAppView *)m_parent)->m_listTables.DeleteAllItems();
}


BOOL CLobby::IsTableWILD(int tblIdx)
{
	// check for WILD tables
	if(tblIdx<0 || tblIdx>=MAX_TABLEITEMS) return FALSE; // NEW 15/FEB/2004
	
	BOOL bWild = FALSE;
	//if(m_table[tblIdx].ballSize!=0 || m_table[tblIdx].rackType!=0 || (m_table[tblIdx].tableType!=0 && m_table[tblIdx].tableType!=6) ) bWild = TRUE;
	if(m_table[tblIdx].ballSize!=0 || m_table[tblIdx].rackType!=0 || m_table[tblIdx].tableType>2 ) bWild = TRUE;
	
	return bWild;
}


// Player Items functions
void CLobby::AddPlayerItem(CString name, int rating, int tableNo, int level)
{
	CString items[4];
	items[0] = name;
	items[1].Format("%d", rating);
	if(!tableNo) items[2].Format("");
	else items[2].Format("%d", tableNo);
	
	items[3].Format("%d", level);

	((CGameAppView *)m_parent)->m_listPlayers.CreateItem(
								((CGameAppView *)m_parent)->m_listPlayers.m_totalItems, 
								items );

}

BOOL CLobby::IsPlayerInGroup(int plrIdx, int group)
{
	// check if we should should order trial-members (for Admin/moderators only)
	BOOL bShowTrialMembers = FALSE;
	int myPlrIdx = GetPlayerIdx( ((CGameAppView *)m_parent)->m_playerSockIdx );
	if(myPlrIdx!=-1)
	{
		if( m_player[myPlrIdx].state&PLRSTATE_MODERATOR || m_player[myPlrIdx].state&PLRSTATE_ADMIN )
			bShowTrialMembers = TRUE;
	}

	// sort players into their correct groups
	if(group==0) // group 0 -> myself
	{
		if(plrIdx!=0) return FALSE;
	}
	
	if(plrIdx!=0) // ignore player idx in slot 0
	{
		if(group==1) // group 1 -> Admin
		{
			if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK) != PLRSTATE_ADMIN ) return FALSE;
			// If in Stealth Mode, return FALSE too //
			if(m_player[plrIdx].state2&PLRSTATE2_STEALTH) return FALSE;
			
		}

		else if(group==2) // group 2 -> Moderators
		{
			if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK) != PLRSTATE_MODERATOR &&
				(m_player[plrIdx].state&PLRSTATE_AUTHMASK) != PLRSTATE_LEADMODERATOR) return FALSE;
		}

		else if(group==3) // group 3 -> Members
		{
			if(m_player[plrIdx].state2&PLRSTATE2_STEALTH) return TRUE;  // Make Stealthed People appear in this Group //

			if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK)) return FALSE;
			
			if(bShowTrialMembers)
			{
				if( (m_player[plrIdx].state&PLRSTATE_GUESTMEMBER) ) return FALSE;
			}

			if(m_player[plrIdx].bIsGuest) return FALSE;
		}
		else if(group==4) // group 4 -> Trial-Members
		{
			if(bShowTrialMembers)
			{
				if( (m_player[plrIdx].state&PLRSTATE_AUTHMASK) ) return FALSE;				
				if( !(m_player[plrIdx].state&PLRSTATE_GUESTMEMBER) ) return FALSE;
				if(m_player[plrIdx].bIsGuest) return FALSE;
			}
			else return FALSE;
		}
		else if(group==5) // group 5 -> guests
		{
			if(!m_player[plrIdx].bIsGuest) return FALSE;
		}
	}
	else // ignore all idx = 0 for all non-zero groups
	{
		if(group!=0) return FALSE;
	}
		
	return TRUE;
}

void CLobby::SortPlayerRatings(void)
{
	// inserts all current online players
	int n;
	int m;
	int bestRatingLimit = -1;
	int bestRating = 0;
	int bestIdx = -1;
	
	
	// should be <= to this size of scan-loop (Because some players may have same ratings)
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	{
		// find best rating
		for(m=0;m<MAX_LOBBY_PLAYERS;m++)
		if(m_player[m].status)
		{		
			if(m_player[m].rating < bestRatingLimit || bestRatingLimit == -1)
			if(bestRating <= m_player[m].rating)			
			{
				bestRating = m_player[m].rating;
				bestIdx = m;				
			}
		}

		// add all player(s) with this rating to lobby list
		if(bestIdx != -1)
		{
		for(m=0;m<=bestIdx;m++)
		if(m_player[m].status)
		if(m_player[m].rating == bestRating)
		{
			AddPlayerItem(m_player[m].name, m_player[m].rating, m_player[m].table+1, m_player[m].level);
		}

		bestRatingLimit = bestRating;
		bestRating = 0;
		bestIdx = -1;
		}
		else break;
	}
}

void CLobby::SortPlayerTables(void)
{
	// inserts all current online players
	int n;
	int m;
	int bestTableLimit = -2;
	int bestTable = 50;
	int bestIdx = -1;
	
	
	// should be <= to this size of scan-loop (Because some players may have same ratings)
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	{
		// find best table
		for(m=0;m<MAX_LOBBY_PLAYERS;m++)
		if(m_player[m].status)
		{		
			if(m_player[m].table > bestTableLimit || bestTableLimit == -2)
			if(bestTable >= m_player[m].table)
			{
				bestTable = m_player[m].table;
				bestIdx = m;				
			}
		}

		// add all player(s) with this rating to lobby list
		if(bestIdx != -1)
		{
		for(m=0;m<=bestIdx;m++)
		if(m_player[m].status)
		if(m_player[m].table == bestTable)
		{
			AddPlayerItem(m_player[m].name, m_player[m].rating, m_player[m].table+1, m_player[m].level);
		}

		bestTableLimit = bestTable;
		bestTable = 50;
		bestIdx = -1;
		}
		else break;
	}
}

void CLobby::SortPlayerNames(void)
{
	// inserts all current online players
	int n;
	int m;
	//int bestNameLimit = -1;
	//int bestName = 0;
	char bestNameLimit[22];
	char bestName[22];
	bestNameLimit[0] = 0;
	bestName[0] = 0;
	int bestIdx = -1;
	BOOL bFirstTime = TRUE;
	
	// should be <= to this size of scan-loop (Because some players may have same ratings)
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	{
		// find best name
		for(m=0;m<MAX_LOBBY_PLAYERS;m++)
		if(m_player[m].status)
		{		
			if( strcmpi(m_player[m].name, bestNameLimit) > 0 || bestNameLimit[0] == 0)
			if( strcmpi(bestName, m_player[m].name) > 0 || bFirstTime)
			{		
				bFirstTime = FALSE;
				strcpy(bestName, m_player[m].name);
				bestIdx = m;				
			}
		}

		// add all player(s) with this rating to lobby list
		if(bestIdx != -1)
		{
		for(m=0;m<=bestIdx;m++)
		if(m_player[m].status)
		if( strcmpi(m_player[m].name, bestName) == 0)
		{
			AddPlayerItem(m_player[m].name, m_player[m].rating, m_player[m].table+1, m_player[m].level);
			break;
		}

		strcpy(bestNameLimit, bestName);
		bestName[0] = 0;
		bestIdx = -1;
		bFirstTime = TRUE;
		}
		else break;
	}
}

void CLobby::RefreshPlayerItem(void)
{
	
	// TODO: add this function to listctrlex (like createItem(...) i.e. DeleteAllItems(...
	((CGameAppView *)m_parent)->m_listPlayers.m_totalItems = 0;


	//char buffer[256];
	//sprintf(buffer, "N = %d, R = %d, T = %d", m_bToggleName, m_bToggleRating, m_bToggleTable);
	//ErrorMessage(m_parent, buffer);
	if(!((CGameAppView *)m_parent)->m_listPlayers.m_bToggleName && 
	   !((CGameAppView *)m_parent)->m_listPlayers.m_bToggleRating &&
	   !((CGameAppView *)m_parent)->m_listPlayers.m_bToggleTable) // default
	{		
		// inserts all current online players
		for(int g=0;g<6;g++)
		for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
		{		
			if( !IsPlayerInGroup(n, g) ) continue;

			if(m_player[n].status)
			{
				AddPlayerItem(m_player[n].name, m_player[n].rating, m_player[n].table+1, m_player[n].level);
			}
		}
		//ErrorMessage(m_parent, "Default");
	}	
	else if(((CGameAppView *)m_parent)->m_listPlayers.m_bToggleName) // Name
	{
		SortPlayerNames();
		//ErrorMessage(m_parent, "Name");
	}
	else if(((CGameAppView *)m_parent)->m_listPlayers.m_bToggleRating) // Rating
	{
		SortPlayerRatings();
	}
	else if(((CGameAppView *)m_parent)->m_listPlayers.m_bToggleTable) // Table
	{
		SortPlayerTables();
	}


	
	//((CGameAppView *)m_parent)->m_listPlayers.RedrawItems(0, ((CGameAppView *)m_parent)->m_listPlayers.m_totalItems);

//	CScrollBar *vert = ((CGameAppView *)m_parent)->m_listPlayers.GetScrollBarCtrl( SB_VERT ),
	// clear redraw flag - while we add items to list
	((CGameAppView *)m_parent)->m_listPlayers.SetRedraw(FALSE);

	// clear dead players from end of list
	for(int n=((CGameAppView *)m_parent)->m_listPlayers.m_totalItems;n<MAX_LOBBY_PLAYERS;n++)
	{
		((CGameAppView *)m_parent)->m_listPlayers.DeleteItem(n);
	}

	
	// Set redraw flag - since we've finished adding items to list
	((CGameAppView *)m_parent)->m_listPlayers.SetRedraw(TRUE);
	
	


	// search list for previous selection name (not the player list!)
	for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
	{
		CString str;
			
		str = ((CGameAppView *)m_parent)->m_listPlayers.GetItemText(n, 0);
		if( strcmpi(str, ((CGameAppView *)m_parent)->m_listPlayers.m_lastSelectedName) == 0 )
		{
			((CGameAppView *)m_parent)->m_listPlayers.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			break;
		}		
	}

	// we must force the scrollbar to be on, since SetRedraw screws with this //
	((CGameAppView *)m_parent)->m_listPlayers.ShowScrollBar(SB_VERT);


	// set redraw flag back for updating
//	((CGameAppView *)m_parent)->m_listPlayers.SetRedraw(TRUE);
//	((CGameAppView *)m_parent)->m_listPlayers.Invalidate(); ??
//	((CGameAppView *)m_parent)->m_listPlayers.UpdateWindow(); ??
}

void CLobby::UpdatePlayerItem(CString name)
{
	// TODO: add this function to listctrlex (like createItem(...) i.e. DeleteAllItems(...
	//((CGameAppView *)m_parent)->m_listPlayers.DeleteAllItems();
	//((CGameAppView *)m_parent)->m_listPlayers.m_totalItems = 0;

	CString tmpName;

	int n;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	{
		tmpName = m_player[n].name;
		//if(tmpName == name)
		if(tmpName.CompareNoCase(name) == 0)
		{
			//((CGameAppView *)m_parent)->m_listPlayers.DeleteItem(n); // causes a crash
			AddPlayerItem(m_player[n].name, m_player[n].rating, m_player[n].table+1, m_player[n].level);
			break;
		}
	}
}


BOOL CLobby::RemovePlayerItem(CString name)
{

	
	return TRUE;//((CGameAppView *)m_parent)->m_listTables.DeleteItem(nItem);
}

BOOL CLobby::RemoveAllPlayerItems(void)
{
	
	return TRUE;//((CGameAppView *)m_parent)->m_listTables.DeleteAllItems();
}

void InitPlayer(Player *pPlr) // Use this for laying the foundations of a player structure.
{
	if(pPlr == NULL) return;

	pPlr->status = 0;
//	pPlr->idx = -1;
	pPlr->name[0] = 0;
	// Dont write over the internet stuff!!! hence it's remmed out
//	pPlr->playerPort = -1;
//	pPlr->playerIP[0] = 0;
//	pPlr->playerLocalIP[0] = 0;
//	pPlr->playerSocketIdx = -1;
	pPlr->rating = 0;
	pPlr->highestrating = 1200;
	pPlr->ladderPoints = 12000;
	pPlr->ladderWinsLosses = 0;
	pPlr->ladderHighBreak = 0;
	pPlr->cash = 0;
	pPlr->realcash = 0;
	pPlr->wins = 0;
	pPlr->losses = 0;
	pPlr->streak = 0;
	pPlr->reracks = 0;

	
	pPlr->_p50s = 0;
	pPlr->_p60s = 0;
	pPlr->_p70s = 0;
	pPlr->_p80s = 0;
	pPlr->_p90s = 0;
	pPlr->_p100s = 0;
	pPlr->pHighBreak = 0;
	pPlr->pMax147s = 0;
	///////////
	pPlr->_50s = 0;
	pPlr->_60s = 0;
	pPlr->_70s = 0;
	pPlr->_80s = 0;
	pPlr->_90s = 0;
	pPlr->_100s = 0;
	pPlr->HighBreak = 0;
	pPlr->Max147s = 0;
	//pPlr->centuries = 0;
	//pPlr->max147s = 0;

	pPlr->pBioAverageBreak = 0;
	pPlr->pBioAverageShotTime = 0;
	pPlr->pBioAveragePosition = 0;
	pPlr->pBioAverageSafety = 0;
	//////////////
	pPlr->bioAverageBreak = 0;
	pPlr->bioAverageShotTime = 0;
	pPlr->bioAveragePosition = 0;
	pPlr->bioAverageSafety = 0;

	pPlr->winner = 0;
	pPlr->runnerUp = 0;
	pPlr->semiFinals = 0;
	pPlr->quarterFinals = 0;
	pPlr->last16 = 0;
	pPlr->last32 = 0;

	pPlr->trialsysid = 0;
	pPlr->trialcpuid = 0;
	pPlr->inventory = 0;
	pPlr->inventory |= (1<<16);
	pPlr->cuetype = 0;
	pPlr->chalktype = 0;
	pPlr->control = 0; //
		
	pPlr->table = -1;
	pPlr->getAllPlayers = -1;
	pPlr->getAllTables = -1;
	pPlr->type = Player_typ::TYPE_SIMEND;//Player_typ::TYPE_ARRIVED;
	pPlr->bInvited = FALSE;
	pPlr->bIsGuest = FALSE;
	pPlr->bCanHost = FALSE;
	pPlr->bCanHostKnown = FALSE;
	pPlr->bIgnore = FALSE;
	pPlr->textFormat = TEXTFORMAT_DEFAULT;
	pPlr->playertype = 6;	// Guest Status 
	pPlr->bandata = 0;		// Defaultly we have no History of Bans
	pPlr->activated = 0;	// Defaultly not activated
	pPlr->searchgamestatus = 0; // Defaultly not searching for a game
	pPlr->searchgametime = 0;
	pPlr->searchgametableinvite = -1;	// Don't invite to a table
	pPlr->searchgameseatinvite = -1;	// no seat

#ifdef FORCE_HIGHID
	pPlr->bCanHost = TRUE;
	pPlr->bCanHostKnown = TRUE;
#endif


	pPlr->state = PLRSTATE_AVAILABLE;
	pPlr->state2 = 0;
	pPlr->lobbytablewatch = -1;		// Defaultly Not Watching any Table //

	pPlr->level = 1;
	pPlr->level_progress = 10;
	
}


void NewAccountPlayer(Player *pPlr, int type)
{ // type: 0 = Guest, n = type of account created i.e. privileges
	if(pPlr == NULL) return;

	InitPlayer(pPlr);

	// Set expiry date to current date, then add more time on based on account purchased!
	//CTime m_time = CTime::GetCurrentTime();
	CTime m_time;
	m_time = m_time.GetCurrentTime();

	pPlr->ExpDay = m_time.GetDay();
	pPlr->ExpMonth = m_time.GetMonth();
	pPlr->ExpYear = m_time.GetYear()-2000;

	//TopUpAccountPlayer(pPlr, type);
}

void TopUpAccountPlayerDays(Player *pPlr, int days, BOOL bSet)
{ // type: 0 = Guest, n = type of account created i.e. privileges
	if(pPlr == NULL) return;

	if(TimeToExpire(pPlr)<1) // If account is already expired, set it to present date, then add more.
		{
		// Set expiry date to current date, then add more time on based on account purchased!
		//CTime m_time = CTime::GetCurrentTime();
		CTime m_time;
		m_time = m_time.GetCurrentTime();

		pPlr->ExpDay = m_time.GetDay();
		pPlr->ExpMonth = m_time.GetMonth();
		pPlr->ExpYear = m_time.GetYear()-2000;
		}
	
	if(bSet==FALSE) // Allow it
	{
	
	}
	else { // Force (Set expiry to (days) from current date) - do this to pool members2 people
		//CTime m_time = CTime::GetCurrentTime();
		CTime m_time;
		m_time = m_time.GetCurrentTime();

		pPlr->ExpDay = m_time.GetDay();
		pPlr->ExpMonth = m_time.GetMonth();
		pPlr->ExpYear = m_time.GetYear()-2000;
		}

	{ // Add 7 days to the account expiry.
						 int y,m,d;
						 y = pPlr->ExpYear;
						 m = pPlr->ExpMonth;
						 d = pPlr->ExpDay;

						 CTime t(2000+y,m,d, 0,0,0);
						 t+= CTimeSpan(days, 0, 0, 0); // 7 Days

						 pPlr->ExpDay = t.GetDay();
						 pPlr->ExpMonth = t.GetMonth();
						 pPlr->ExpYear = t.GetYear()-2000;
	}
}



void TopUpAccountPlayer(Player *pPlr, int type)
{ // type: 0 = Guest, n = type of account created i.e. privileges
	if(pPlr == NULL) return;

	if(TimeToExpire(pPlr)<1) // If account is already expired, set it to present date, then add more.
	{
	// Set expiry date to current date, then add more time on based on account purchased!
	//CTime m_time = CTime::GetCurrentTime();
	CTime m_time;
	m_time = m_time.GetCurrentTime();

	pPlr->ExpDay = m_time.GetDay();
	pPlr->ExpMonth = m_time.GetMonth();
	pPlr->ExpYear = m_time.GetYear()-2000;
	}
	switch(type)
		{
		// Special = Beta Tester's account.
		case MCODE_SPECIAL:pPlr->ExpDay = g_config.m_day;//SPECIAL_DAY; // 1st
						   pPlr->ExpMonth = g_config.m_month;//SPECIAL_MONTH; // September
						   pPlr->ExpYear = g_config.m_year;//SPECIAL_YEAR; // 2003
						   break;
		case MCODE_1YEAR:pPlr->ExpYear++;
						 break;
		case MCODE_6MONTHS:pPlr->ExpMonth+=6;
						   if(pPlr->ExpMonth>12) 
							{
							pPlr->ExpMonth-=12;
							pPlr->ExpYear++;
						    }
						   break;
		case MCODE_3MONTHS:pPlr->ExpMonth+=3;
						   if(pPlr->ExpMonth>12) 
							{
							pPlr->ExpMonth-=12;
							pPlr->ExpYear++;
						    }
						   break;
		case MCODE_1MONTH:pPlr->ExpMonth++;
						   if(pPlr->ExpMonth>12) 
							{
							pPlr->ExpMonth-=12;
							pPlr->ExpYear++;
						    }
						   break;	
		case MCODE_TRIAL:{ // Add 14 days to the account expiry.
						 int y,m,d;
						 y = pPlr->ExpYear;
						 m = pPlr->ExpMonth;
						 d = pPlr->ExpDay;

						 CTime t(2000+y,m,d, 0,0,0);
						 t+= CTimeSpan(g_daysFreeTrial, 0, 0, 0); // 14 Days

						 pPlr->ExpDay = t.GetDay();
						 pPlr->ExpMonth = t.GetMonth();
						 pPlr->ExpYear = t.GetYear()-2000;
						 break;
						 }
		case MCODE_TRIAL2:{ // Add 14 days to the account expiry (plus 1 extra).
						 int y,m,d;
						 y = pPlr->ExpYear;
						 m = pPlr->ExpMonth;
						 d = pPlr->ExpDay;

						 CTime t(2000+y,m,d, 0,0,0);
						 t+= CTimeSpan(g_daysFreeTrial+1, 0, 0, 0); // 15 Days

						 pPlr->ExpDay = t.GetDay();
						 pPlr->ExpMonth = t.GetMonth();
						 pPlr->ExpYear = t.GetYear()-2000;
						 break;
						 }
		case MCODE_GUESTMEMBER:{ // Sub 1 days from account expiry. so it's already expired.
						 int y,m,d;
						 y = pPlr->ExpYear;
						 m = pPlr->ExpMonth;
						 d = pPlr->ExpDay;

						 CTime t(2000+y,m,d, 0,0,0);
						 t-= CTimeSpan(1, 0, 0, 0); // 1 Day ago (i.e. expired)

						 pPlr->ExpDay = t.GetDay();
						 pPlr->ExpMonth = t.GetMonth();
						 pPlr->ExpYear = t.GetYear()-2000;
						 break;
						 }
		} // end switch
}

int TimeToExpire(Player *pPlr)
{ // returns number of days until this guy's account will expire!
	if(pPlr == NULL) return 0; // 0 days (invalid player)

	int d,m,y;
	d = pPlr->ExpDay;
	m = pPlr->ExpMonth;
	y = pPlr->ExpYear;
	y+=2000;

	//CTime m_time = CTime::GetCurrentTime();
	CTime m_time;
	m_time = m_time.GetCurrentTime();

	return DaysLeft(m_time.GetDay(),m_time.GetMonth(),m_time.GetYear(),
				    d,m,y);
}

void ModifyMember(char *fname)
{
	if(fname == NULL) return;

	Player plr;
	char fname_txt[MAX_CHARS];
	char fname_bak[MAX_CHARS];

	sprintf(fname_txt,"%sMembers/%s",g_basedir, fname);
	sprintf(fname_bak,"%sMembers/%s.bak",g_basedir, fname);

	LoadPlayerFile(fname_txt, &plr);
	//TopUpAccountPlayer(&plr, MCODE_SPECIAL); // Set account to expire on date of standard specials.
	TopUpAccountPlayerDays(&plr, 21);
	SavePlayerFile(fname_bak, &plr);
	SavePlayerFile(fname_txt, &plr);
	DeleteFile(fname_bak);
}

void ModifyMember2(char *fname)
{
	if(fname == NULL) return;

	Player plr;
	char fname_txt[MAX_CHARS];
	char fname_bak[MAX_CHARS];

	sprintf(fname_txt,"%sMembers2/%s",g_basedir, fname);
	sprintf(fname_bak,"%sMembers2/%s.bak",g_basedir, fname);

	LoadPlayerFile(fname_txt, &plr);
	/*if(TimeToExpire(&plr)<-30)
	{
	FILE *fptr;
	fptr = fopen("memberlog.txt","a+b");
	fprintf(fptr,"%s\r\n", fname);
	fclose(fptr);
	DeleteFile(fname_txt);
	return;
	}*/


	//TopUpAccountPlayerDays(&plr, 21, TRUE);
	plr.rating = 1200;
	SavePlayerFile(fname_bak, &plr);
	SavePlayerFile(fname_txt, &plr);
	DeleteFile(fname_bak);
}

void ModifyAllMembers(void)
{
	
	/*{
	_finddata_t fileinfo;
	int fhandle;
	int t;
	int safe;
	char path[256];

	sprintf(path,"%sMembers/*.txt",g_basedir);
	// Go thru all members //

	t = 0;

	safe = 4096; // safety mech. prevents loading too many (causing delays and stuff)

	while(safe)
	{
	if(!t) fhandle = _findfirst(path, &fileinfo), t = 1;
	else {
		 if(_findnext(fhandle, &fileinfo)==-1) break; //t = 2;
		 }
	ModifyMember(fileinfo.name);
	safe--;
	}

	_findclose(fhandle);
	}*/

	{
	_finddata_t fileinfo;
	int fhandle;
	int t;
	int safe;
	char path[256];

	sprintf(path,"%sMembers2/*.txt",g_basedir);
	// Go thru all members //

	t = 0;

	safe = 8192; // safety mech. prevents loading too many (causing delays and stuff)

	while(safe)
	{
	if(!t) fhandle = _findfirst(path, &fileinfo), t = 1;
	else {
		 if(_findnext(fhandle, &fileinfo)==-1) break; //t = 2;
		 }
	ModifyMember2(fileinfo.name);
	safe--;
	}

	_findclose(fhandle);
	}

	{
	_finddata_t fileinfo;
	int fhandle;
	int t;
	int safe;
	char path[256];

	sprintf(path,"%sMembers/*.txt",g_basedir);
	// Go thru all members //

	t = 0;

	safe = 8192; // safety mech. prevents loading too many (causing delays and stuff)

	while(safe)
	{
	if(!t) fhandle = _findfirst(path, &fileinfo), t = 1;
	else {
		 if(_findnext(fhandle, &fileinfo)==-1) break; //t = 2;
		 }
	ModifyMember2(fileinfo.name);
	safe--;
	}

	_findclose(fhandle);
	}

}


void TOMYSQLMember(char *fname)
{
	if(fname == NULL) return;

	Player plr;
//	char fname_txt[MAX_CHARS];

//	sprintf(fname_txt,"%sMembers2/%s",g_basedir, fname);

//	LoadPlayerFile(fname_txt, &plr);
	if(LoadPlayerFile(fname, &plr))
	{
	db_savememberfile(&plr);
	}

	/*
	//TopUpAccountPlayerDays(&plr, 21, TRUE);
	plr.rating = 1200;
	SavePlayerFile(fname_bak, &plr);
	SavePlayerFile(fname_txt, &plr);
	DeleteFile(fname_bak);*/
}

void TOMYSQLAllMembers(void)
{
	char tpath[256];
	char sql[4096];
	g_sql_banned = FALSE;
	
	{
	_finddata_t fileinfo;
	int fhandle;
	int t;
	int safe;
	char path[256];	
	

	sprintf(path,"%sMembers2/*.txt",g_basedir);
	// Go thru all members //

	t = 0;

	safe = 8192; // safety mech. prevents loading too many (causing delays and stuff)

	while(safe)
	{
	if(!t) fhandle = _findfirst(path, &fileinfo), t = 1;
	else {
		 if(_findnext(fhandle, &fileinfo)==-1) break; //t = 2;
		 }
	sprintf(tpath,"%sMembers2/%s",g_basedir, fileinfo.name);
	g_sql_trial = TRUE;
	TOMYSQLMember(tpath);//fileinfo.name);
	safe--;
	}

	_findclose(fhandle);
	}


	{
	_finddata_t fileinfo;
	int fhandle;
	int t;
	int safe;
	char path[256];
	sprintf(path,"%sMembers/*.txt",g_basedir);
	// Go thru all members //

	t = 0;

	safe = 8192; // safety mech. prevents loading too many (causing delays and stuff)

	while(safe)
	{
	if(!t) fhandle = _findfirst(path, &fileinfo), t = 1;
	else {
		 if(_findnext(fhandle, &fileinfo)==-1) break; //t = 2;
		 }
	sprintf(tpath,"%sMembers/%s",g_basedir, fileinfo.name);
	g_sql_trial = FALSE;
	TOMYSQLMember(tpath);//fileinfo.name);
	safe--;
	}

	_findclose(fhandle);
	}


/////////// Handle the Banned Players ////////
	
	
	{
	_finddata_t fileinfo;
	int fhandle;
	int t;
	int safe;
	char path[256];	
	
	sprintf(path,"%sMembers2/*.ban",g_basedir);
	// Go thru all members //

	t = 0;

	safe = 8192; // safety mech. prevents loading too many (causing delays and stuff)

	while(safe)
	{
	if(!t) fhandle = _findfirst(path, &fileinfo), t = 1;
	else {
		 if(_findnext(fhandle, &fileinfo)==-1) break; //t = 2;
		 }

	sprintf(tpath,"%sMembers2/%s",g_basedir, fileinfo.name);
	g_sql_trial = TRUE;
	g_sql_banned = TRUE;
	TOMYSQLMember(tpath);//fileinfo.name);
	safe--;
	}

	_findclose(fhandle);
	}


	{
	_finddata_t fileinfo;
	int fhandle;
	int t;
	int safe;
	char path[256];
	sprintf(path,"%sMembers/*.ban",g_basedir);
	// Go thru all members //

	t = 0;

	safe = 8192; // safety mech. prevents loading too many (causing delays and stuff)

	while(safe)
	{
	if(!t) fhandle = _findfirst(path, &fileinfo), t = 1;
	else {
		 if(_findnext(fhandle, &fileinfo)==-1) break; //t = 2;
		 }
	sprintf(tpath,"%sMembers/%s",g_basedir, fileinfo.name);
	g_sql_trial = FALSE;
	TOMYSQLMember(tpath);//fileinfo.name);
	safe--;
	}

	_findclose(fhandle);
	}


	g_sql_banned = FALSE;
}









int PlayerFileExists(char *fname)
{
	return LoadPlayerFile(fname, NULL);
}
/*
int LoadPlayerFileExFT(char *fname, Player *plr, char *filetype)
{
	int t;
	char nameFile[256];
	sprintf(nameFile,"%sMembers/%s%s",g_basedir,fname,filetype);
	if(LoadPlayerFile(nameFile, plr)) return 1;

	sprintf(nameFile,"%sMembers2/%s%s",g_basedir,fname,filetype);
	t = LoadPlayerFile(nameFile, plr);  // try loading memberfile
	plr->state|=PLRSTATE_GUESTMEMBER;
	return t;
}

int LoadPlayerFileEx(char *fname, Player *plr)
{
	int t;
	char nameFile[256];
	sprintf(nameFile,"%sMembers/%s.txt",g_basedir,fname);
	if(LoadPlayerFile(nameFile, plr)) return 1;

	sprintf(nameFile,"%sMembers2/%s.txt",g_basedir,fname);
	t = LoadPlayerFile(nameFile, plr);  // try loading memberfile
	plr->state|=PLRSTATE_GUESTMEMBER;
	return t;
}*/


void TextChanged(char *name, int textformat, int textcolor)
{
	db_textchanged(name, textformat, textcolor);
}


void CueChanged(char *name, int cuetype)
{
	db_cuechanged(name, cuetype);
}

void ChalkChanged(char *name, int chalktype)
{
	db_chalkchanged(name, chalktype);
}

void UpdateSysMacIpEx(char *name, char *sysid, char *macid, char *ip)
{
	db_sysmacipmemberfile(name, sysid, macid, ip);
}

void BanPlayerFileEx(char *name)
{
	db_banmemberfile(name, 1);
}

void UnBanPlayerFileEx(char *name)
{
	db_banmemberfile(name, 0);
}

//////////////
//// Wall ////
//////////////
int CheckWalledEx(char *sysid)
{
	return db_checkwalled(sysid);
}
void WallSysIDEx(char *sysid)
{
	db_wall(sysid, 1);
}
void UnWallSysIDEx(char *sysid)
{
	db_wall(sysid, 0);
}
//////////////
//// Deny ////
//////////////
int CheckDeniedEx(char *macid)
{
	return db_checkdenied(macid);
}
void DenyMacIDEx(char *macid)
{
	db_deny(macid, 1);
}
void UnDenyMacIDEx(char *macid)
{
	db_deny(macid, 0);
}

//////////////
//// Block ////
//////////////
int CheckBlockedEx(char *ip)
{
	return db_checkblocked(ip);
}
void BlockIPEx(char *ip)
{
	db_block(ip, 1);
}
void UnBlockIPEx(char *ip)
{
	db_block(ip, 0);
}



BOOL CheckPasswordEx(char *name, char *pass)
{
	return db_checkpassword(name, pass);
}


int GetLadderRank(char *name, int ladderPoints)
{
	return db_getLadderRank(name, ladderPoints);
}

std::array<ladderObject, 20> GetLadderRankings()
{
	return db_getLadderRankings();
}

void ResetLadderRankings()
{
	db_resetLadderRankings();
}


void NewAccountLogEx(char *name, char *atype, char *mcode, char *email, char *date)
{
	db_newaccountlog(name, atype, mcode, email, date);
}

int GetMemberIDEx(char *name)
{
return db_getidmemberfile(name);
}

int GetPlayerType(char *name)
{
return db_getplayertype(name); // Find the PlayerType
}


void UpdateCash(char *name, int cash)
{
	db_updateCash(name, cash);
}



int LoadPlayerFileExFT(char *fname, Player *plr, char *filetype)
{
#ifdef MYSQL_SYSTEM
	return LoadPlayerFileEx(fname, plr); // filetype is always .txt in this case anyways.
#else
	int t;
	char nameFile[256];
	
	sprintf(nameFile,"%sMembers/%s%s",g_basedir,fname,filetype);
	if(LoadPlayerFile(nameFile, plr)) return 1;

	sprintf(nameFile,"%sMembers2/%s%s",g_basedir,fname,filetype);
	t = LoadPlayerFile(nameFile, plr);  // try loading memberfile
	plr->state|=PLRSTATE_GUESTMEMBER;
	return t;
#endif
}

int LoadPlayerFileEx(char *fname, Player *plr)
{
#ifdef MYSQL_SYSTEM
	BOOL flag;
	Player tmp;
	flag = FALSE;
	if(plr==NULL) // just checking //
	{
	strcpy(tmp.name, fname);
	flag = TRUE;
	return db_loadmemberfile(&tmp, flag);
	}
	strcpy(plr->name, fname); // set the player name field prior to entry.//
	return db_loadmemberfile(plr, flag);
#else
	int t;
	char nameFile[256];
	sprintf(nameFile,"%sMembers/%s.txt",g_basedir,fname);
	if(LoadPlayerFile(nameFile, plr)) return 1;

	sprintf(nameFile,"%sMembers2/%s.txt",g_basedir,fname);
	t = LoadPlayerFile(nameFile, plr);  // try loading memberfile
	plr->state|=PLRSTATE_GUESTMEMBER;

	if(!t) // check if it's a ban file ([+] upgrade all checks to this for case 2)
	{
	sprintf(nameFile,"%sMembers/%s.ban",g_basedir,fname);
	if(LoadPlayerFile(nameFile, NULL)) return 2;
	sprintf(nameFile,"%sMembers2/%s.ban",g_basedir,fname);
	if(LoadPlayerFile(nameFile, NULL)) return 2;
	}
	return t;
#endif
}


int LoadPlayerFile(char *fname, Player *plr)
{
	// Not In Use //


	return 1;

}

void SavePlayerFileExFT(char *fname, Player *plr, char *filetype)
{
	if(fname == NULL) return;
	if(plr == NULL) return;
	if(filetype == NULL) return;
#ifdef MYSQL_SYSTEM
	if(!strcmpi(filetype,".bak")) return; // allow the .bak saves we dont need any safety here.
	SavePlayerFileEx(fname, plr);
#else
	char nameFile[256];

	if(plr->state&PLRSTATE_GUESTMEMBER)
	{
	sprintf(nameFile,"%sMembers2/%s%s",g_basedir,fname,filetype);
	SavePlayerFile(nameFile, plr);
	}
	else {
	sprintf(nameFile,"%sMembers/%s%s",g_basedir,fname,filetype);
	SavePlayerFile(nameFile, plr);
	}
#endif
}

void SavePlayerFileEx(char *fname, Player *plr)
{
	if(fname == NULL) return;
	if(plr == NULL) return;
#ifdef MYSQL_SYSTEM
	//strcpy(plr->name, fname); // set the player name field prior to entry.//
	// Update SQL Globals: sysid, macid, ip //
	db_savememberfile(plr);
#else
	char nameFile[256];

	if(plr->state&PLRSTATE_GUESTMEMBER)
	{
	sprintf(nameFile,"%sMembers2/%s.txt",g_basedir,fname);
	SavePlayerFile(nameFile, plr);
	}
	else {
	sprintf(nameFile,"%sMembers/%s.txt",g_basedir,fname);
	SavePlayerFile(nameFile, plr);
	}
#endif
}

void SavePlayerFile(char *fname, Player *plr)
{
	/*
	DWORD v32;
	if(fname == NULL) return;
	if(plr == NULL) return;

	
	FILE *fp;

	if((fp = fopen(fname, "w")) == NULL)
		{
			// can't open file for 'write'
		}
	else
		{
			// save member's details:
			CString str;

			//0
			//str = ((CGameAppView *)m_pParentWnd)->m_detailsDlg.m_name + "\n";
			str = CString(plr->name) + "\n";
			fwrite(str, str.GetLength(), 1, fp);
			
			//1
			//str = ((CGameAppView *)m_pParentWnd)->m_detailsDlg.m_password + "\n";
			str = CString(plr->password) + "\n";
			fwrite(str, str.GetLength(), 1, fp);
			
			//2
			//str = ((CGameAppView *)m_pParentWnd)->m_detailsDlg.m_email + "\n";
			str = "\n";
			fwrite(str, str.GetLength(), 1, fp);

			//3
			//str = ((CGameAppView *)m_pParentWnd)->m_detailsDlg.m_town + "\n";
			str = "\n";
			fwrite(str, str.GetLength(), 1, fp);
			
			//4
			//str = ((CGameAppView *)m_pParentWnd)->m_detailsDlg.m_comments + "\n";
			str = "\n";
			fwrite(str, str.GetLength(), 1, fp);

			///////////////////////
			//int rating;
			//int wins;
			//int losses;
			//int streak;
			//int centuries;
			//int max147s;						
			char buffer[80];						
			// 5
			//plr.rating = 1200-300 + rand()%(2400 - 900); //should be 1200;
			sprintf(buffer, "%d\n", (int) plr->rating);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 6
			//plr.wins = 0;
			sprintf(buffer, "%d\n", (int) plr->wins);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 7
			//plr.losses = 0;
			sprintf(buffer, "%d\n", (int) plr->losses);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 8
			//plr.streak = 0;
			sprintf(buffer, "%d\n", (int) plr->streak);
			fwrite(buffer, strlen(buffer), 1, fp);

			
			// 9
			//plr._p50s = 0;
			sprintf(buffer, "%d\n", (int) plr->_50s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 10
			//plr._p60s = 0;
			sprintf(buffer, "%d\n", (int) plr->_60s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 11
			//plr._p70s = 0;
			sprintf(buffer, "%d\n", (int) plr->_70s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 12
			//plr._p80s = 0;
			sprintf(buffer, "%d\n", (int) plr->_80s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 13
			//plr._p90s = 0;
			sprintf(buffer, "%d\n", (int) plr->_90s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 14
			//plr._p100s = 0;
			sprintf(buffer, "%d\n", (int) plr->_100s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 15
			//plr.pHighBreak = 0;
			sprintf(buffer, "%d\n", (int) plr->HighBreak);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 16
			//plr.pMax147s = 0;
			sprintf(buffer, "%d\n", (int) plr->Max147s);
			fwrite(buffer, strlen(buffer), 1, fp);
			
			////////////////////////////

			// 17
			//plr._50s = 0;
			sprintf(buffer, "%d\n", (int) plr->_p50s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 18
			//plr._60s = 0;
			sprintf(buffer, "%d\n", (int) plr->_p60s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 19
			//plr._70s = 0;
			sprintf(buffer, "%d\n", (int) plr->_p70s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 20
			//plr._80s = 0;
			sprintf(buffer, "%d\n", (int) plr->_p80s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 21
			//plr._90s = 0;
			sprintf(buffer, "%d\n", (int) plr->_p90s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 22
			//plr._100s = 0;
			sprintf(buffer, "%d\n", (int) plr->_p100s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 23
			//plr.HighBreak = 0;
			sprintf(buffer, "%d\n", (int) plr->pHighBreak);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 24
			//plr.Max147s = 0;
			sprintf(buffer, "%d\n", (int) plr->pMax147s);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 25
			{
			char str[80];
			int d,m,y;
			d = plr->ExpDay;
			m = plr->ExpMonth;
			y = plr->ExpYear;
			y+=2000;
			DateToStr(str, d, m, y);
			sprintf(buffer, "%s\n", str);
			fwrite(buffer, strlen(buffer), 1, fp);
			}


			// 26
			//plr.WildHighBreak = 0;
			sprintf(buffer, "%d\n", (int) plr->WildHighBreak);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 27
			//plr.pWildHighBreak = 0;
			sprintf(buffer, "%d\n", (int) plr->pWildHighBreak);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 28
			//plr.reracks = 0;
			sprintf(buffer, "%d\n", (int) plr->reracks);
			fwrite(buffer, strlen(buffer), 1, fp);


//char buff[128];
//sprintf(buff, "plr->bioAverageShotTime=%f, plr->bioAveragePosition=%f, plr->bioAverageSafety=%f", plr->bioAverageShotTime, plr->bioAveragePosition, plr->bioAverageSafety);
//ErrorMessage(g_wnd, buff);


			// 29
			//plr.bioAverageShotTime = 0;
			sprintf(buffer, "%f\n", plr->bioAverageShotTime);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 30
			//plr.bioAveragePosition = 0;
			sprintf(buffer, "%f\n", plr->bioAveragePosition);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 31
			//plr.bioAverageSafety = 0;
			sprintf(buffer, "%f\n", plr->bioAverageSafety);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 32
			//plr.pBioAverageShotTime = 0;
			sprintf(buffer, "%f\n", plr->pBioAverageShotTime);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 33
			//plr.pBioAveragePosition = 0;
			sprintf(buffer, "%f\n", plr->pBioAveragePosition);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 34
			//plr.pBioAverageSafety = 0;
			sprintf(buffer, "%f\n", plr->pBioAverageSafety);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 35
			//plr.bioAverageBreak = 0;
			sprintf(buffer, "%f\n", plr->bioAverageBreak);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 36
			//plr.pBioAverageBreak = 0;
			sprintf(buffer, "%f\n", plr->pBioAverageBreak);
			fwrite(buffer, strlen(buffer), 1, fp);
			
			// 37
			//plr->tournamentRating = 0;
			//sprintf(buffer, "%d\n", plr->tournamentRating);
			sprintf(buffer, "0\n");
			fwrite(buffer, strlen(buffer), 1, fp);

			// 38
			//plr->winner = 0;
			sprintf(buffer, "%d\n", plr->winner);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 39
			//plr->runnerUp = 0;
			sprintf(buffer, "%d\n", plr->runnerUp);
			fwrite(buffer, strlen(buffer), 1, fp);

			// 40
			//plr->semiFinals = 0;
			sprintf(buffer, "%d\n", plr->semiFinals);
			fwrite(buffer, strlen(buffer), 1, fp);

  			// 41
			//plr->quarterFinals = 0;
			sprintf(buffer, "%d\n", plr->quarterFinals);
			fwrite(buffer, strlen(buffer), 1, fp);

  			// 42
			//plr->last16 = 0;
			sprintf(buffer, "%d\n", plr->last16);
			fwrite(buffer, strlen(buffer), 1, fp);

  			// 43
			//plr->last32 = 0;
			sprintf(buffer, "%d\n", plr->last32);
			fwrite(buffer, strlen(buffer), 1, fp); 


  			// 44
			//plr->trialsysid = 0;
			sprintf(buffer, "%lx\n", plr->trialsysid);
			fwrite(buffer, strlen(buffer), 1, fp); 

  			// 45
			//plr->textFormat = 0;
			v32 = plr->textFormat;
			sprintf(buffer, "%lx\n", v32);
			fwrite(buffer, strlen(buffer), 1, fp); 

  			// 46
			//plr->state2 = 0;
			sprintf(buffer, "%lx\n", plr->state2);
			fwrite(buffer, strlen(buffer), 1, fp); 

			// 47
			//plr->trailmacid_u = 0;
			sprintf(buffer, "%lx\n", plr->trialmacid_u);
			fwrite(buffer, strlen(buffer), 1, fp); 
		
			// 48
			//plr->trailmacid_l = 0;
			sprintf(buffer, "%x\n", plr->trialmacid_l);
			fwrite(buffer, strlen(buffer), 1, fp); 
			
			// 49
			//plr->trailcpu = 0;
			sprintf(buffer, "%04x\n", plr->trialcpuid);
			fwrite(buffer, strlen(buffer), 1, fp); 

			// 50
			//plr->trailcpu = 0;
			sprintf(buffer, "%lx\n", plr->inventory);
			fwrite(buffer, strlen(buffer), 1, fp); 

			// 51
			//plr->highestrating = 0;
			sprintf(buffer, "%d\n", plr->highestrating);
			fwrite(buffer, strlen(buffer), 1, fp); 
			
			// 52
			//plr->cash = 0;
			sprintf(buffer, "%d\n", plr->cash);
			fwrite(buffer, strlen(buffer), 1, fp); 

			// 53
			//plr->cuetype = 0;
			sprintf(buffer, "%lx\n", plr->cuetype);
			fwrite(buffer, strlen(buffer), 1, fp); 

			
			fclose(fp);
		}
	*/
}

void LoadDetailsFile(char *fname, Details *dtls)
{
	if(fname == NULL) return;
	if(dtls == NULL) return;

	// load details from file
	FILE *fp;

	
	char fname_full[128];

	// Try the savepath first, if nothing exists, check installed path
	sprintf(fname_full, "%s%s", g_build.savepath, fname);
	fp = fopen(fname_full, "r");

	if(fp == NULL)
	{
		fp = fopen(fname, "r");
	}

	if(fp == NULL)
	{
		dtls->m_name = _T("");//"guest";
		dtls->m_password = _T("");
		dtls->m_email = _T("");
		dtls->m_town = _T("");
		dtls->m_comments = _T("");
		dtls->m_realName = _T("");
		dtls->m_sex = _T("");
		dtls->m_age = _T("");
		dtls->m_version = _T("");
		dtls->m_bpass = _T("");
		return;
	}

	
	
	// load my details:
	int n, m;
	char str[256];
	BOOL bFoundEOF = FALSE;
	
	for(m=0; m<10; m++)
	{
		for(n=0; n<256; n++)
		{
			if( feof(fp) )
			{
				bFoundEOF = TRUE;
				break; 
			}

			fread(&str[n], 1, 1, fp);
			if(str[n] == '\n') {
				str[n] = NULL;				
				break;
			}
		}
		
		if(bFoundEOF) break; 
		
		switch(m)
		{
			case 0:
				dtls->m_name = CString(str);
				break;
			case 1:
				dtls->m_password = CString(str);
				break;
			case 2:				
				dtls->m_email = CString(str);				
				break;			
			case 3:
				//if(!str[0]) strcpy(str, "N/A");
				dtls->m_town = CString(str);
				break;
			case 4:
				//if(!str[0]) strcpy(str, "N/A");
				dtls->m_comments = CString(str);
				break;
			case 5:
				//if(!str[0]) strcpy(str, "N/A");
				dtls->m_realName = CString(str);
				break;
			case 6:
				//if(!str[0]) strcpy(str, "N/A");
				dtls->m_sex = CString(str);
				break;
			case 7:
				//if(!str[0]) strcpy(str, "N/A");
				dtls->m_age = CString(str);
				break;
			case 8:
				//if(!str[0]) strcpy(str, "N/A");
				dtls->m_version = CString(str);
				break;
			case 9:
				//if(!str[0]) strcpy(str, "N/A");
				dtls->m_bpass = CString(str);

				if(!strcmpi(dtls->m_bpass, "SAVE")) g_RememberPassword = TRUE;
					else g_RememberPassword = FALSE;
				break;
		}
	}

	fclose(fp);
}

void SaveDetailsFile(char *fname, Details *dtls)
{
	if(fname == NULL) return;
	if(dtls == NULL) return;

	// save details to file
	FILE *fp;


	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, fname);

	if((fp = fopen(fname_full, "w")) == NULL)
	{
		//can't open file for 'write'
		return;
	}

	//char buffer[256];
	//encodepass(buffer,dtls->m_password); 
	// save my details:
	CString str;

	str = dtls->m_name + "\n";		fwrite(str, str.GetLength(), 1, fp);
	str = dtls->m_password + "\n";	fwrite(str, str.GetLength(), 1, fp);
	str = dtls->m_email + "\n";		fwrite(str, str.GetLength(), 1, fp);
	
	str = dtls->m_town + "\n";		fwrite(str, str.GetLength(), 1, fp);
	str = dtls->m_comments + "\n";	fwrite(str, str.GetLength(), 1, fp);
	
	str = dtls->m_realName + "\n";	fwrite(str, str.GetLength(), 1, fp);
	str = dtls->m_sex + "\n";	fwrite(str, str.GetLength(), 1, fp);
	str = dtls->m_age + "\n";	fwrite(str, str.GetLength(), 1, fp);
	str = dtls->m_version + "\n";	fwrite(str, str.GetLength(), 1, fp);
	str = dtls->m_bpass + "\n";	fwrite(str, str.GetLength(), 1, fp);

	fclose(fp);
}

SerialStruct_typ g_serialStruct;

int CheckSerialV2(char *mCode)
{
	int type_idx;

#ifdef MYSQL_SYSTEM
	char email[256];
	type_idx = CheckSerialdb(mCode, email);
	sprintf(g_serialStruct.email, email);
	sprintf(g_serialStruct.order_no, "");

#else
	if(mCode == NULL) return -1; // serial/file not found. (Invalid)

	FILE *fptr;
	char path[256];
	int idx,n;
	char str[81];
	char type[32];
	BOOL ok;
	for(n=0;n<64;n++) // make sure it's alphanumeric only!
	{
	if(!mCode[n]) break;
	ok = FALSE;
	if(mCode[n]>='0' && mCode[n]<='9') ok = TRUE;
	else if(mCode[n]>='a' && mCode[n]<='z') ok = TRUE;
	else if(mCode[n]>='A' && mCode[n]<='Z') ok = TRUE;
	if(!ok) return -1;
	}

	sprintf(path,"%sSerials/%s.txt", g_basedir, mCode);
	fptr = fopen(path,"rb");
	if(fptr==NULL) return -1; // serial/file not found.

	type[0] = 0;

	for(idx=0;idx<3;idx++)
	{
	for(n=0;n<80;n++)
	{
	fread(&str[n],1,1,fptr);
	if(str[n]==0x0d) 
		{
		str[n] = 0;
		switch(idx)
			{
				case 0:strcpy(type,str);
					break;
				case 1:strcpy(g_serialStruct.email,str);
					break;
				case 2://sscanf(str,"%d",&g_serialStruct.order_no);
					   strcpy(g_serialStruct.order_no,str);
					break;
			} // end switch
		}
	else if(str[n]==0x0a) break;
	} // end for
	} // end for

	int t; // Find Type Index from List.
	type_idx = MCODE_UNDEFINED;
	for(t=0;t<256;t++)
	{
	if(!g_mcode_type_lst[t]) break;
	if(!strcmpi(type,g_mcode_type_lst[t])) {
											type_idx = t;
											break;
											}
	}

	/*if(!strcmpi(type,"1Year")) type_idx = MCODE_1YEAR;
	else if(!strcmpi(type,"6Months")) type_idx = MCODE_6MONTHS;
	else if(!strcmpi(type,"3Months")) type_idx = MCODE_3MONTHS;
	else if(!strcmpi(type,"Special")) type_idx = MCODE_SPECIAL;
	else type_idx = MCODE_UNDEFINED;*/
	
	fclose(fptr);
#endif
	return type_idx;
}

void DeleteSerialV2(char *mCode)
{
#ifdef MYSQL_SYSTEM
	DeleteSerialdb(mCode);
#else
	if(mCode == NULL) return;

	char path[256];
	sprintf(path,"%sSerials/%s.txt", g_basedir, mCode);
	unlink(path);
#endif
}

int MembershipCodeValid(char *mCode)
{ 
	if(mCode == NULL) return 0; // Invalid code. (Invalid)


	int mysql_injection = 0;
	// Don't allow any ' or "" symbols in the code - mysql injections //
	int n;
	for(n=0;n<strlen(mCode);n++)
	if(mCode[n] == 34 || mCode[n]== 39) 
		{
		mysql_injection = 1;
		break;
		}
		
	if(mysql_injection) 
		{
		return 0; // Invalid code. (Invalid)
		}


	//if(!g_config.m_accountSettingsTick) return 0; // if checkbox aint ticked, then no membership
												  // code shall work

	g_serialStruct.email[0] = 0;
	g_serialStruct.order_no[0] = 0;

	// If Account settings is checkboxed, then check for serial code match/allow all mode.
	if(g_config.m_accountSettingsTick)
		{
		if(g_config.m_accountSettings.IsEmpty());// return MCODE_SPECIAL; // if field is empty then allow any membership code (free for all)

		if(!strcmpi(mCode,g_config.m_accountSettings.GetBuffer(0))) return MCODE_SPECIAL;
		}

	// Free Trial Code //
	if(g_daysFreeTrial) // only give free trial if enabled.
	if(!mCode[0]) return MCODE_TRIAL;
	//if(!strcmpi(mCode,"trial")) return MCODE_TRIAL;

	//if(g_guestMember)
	//if(!mCode[0]) return MCODE_GUESTMEMBER; // An Expired Account GuestMember	

	// If it don't match with master code, then check with serial code list
	int serial_idx;
	int type_idx;

	// For now ThePoolClub will use the modern system, and snookerclub the old system.
	// both using the new system.
	type_idx = CheckSerialV2(mCode);
	if(type_idx!=-1)
	{
	DeleteSerialV2(mCode);
	return type_idx;
	}
//	if(g_freeaccounts) return MCODE_SPECIAL; // give away free accounts.
	if(g_config.m_freeaccounts) return MCODE_SPECIAL;
	return 0; // Invalid code.
}

int CrcCodeValid(DWORD crc)
{
	if(crc==0x73577357) return 1; // Magic Code
	// Scan against a list of valid CRCs

	if(g_config.m_crcvalid.IsEmpty()) return 1; // if theres no crc code in the box then accept application code.
		{
		char buffer[80];
		sprintf(buffer,"0x%lX",crc); // turn crc code into a hex expression. [+] change this to a proper numerical way
		if(!strcmpi(buffer,g_config.m_crcvalid.GetBuffer(0)))
			{
			return 1;
			}
		}	
	return 0;
}

	

void AddLog(char *fname, char *str) // general purpose logfile creator
{
	
	FILE *fptr;
	

#ifdef MAIN_SERVER
	
	
	if( (fptr = fopen(fname,"a")) != NULL)
	{
		fprintf(fptr,"%s\r\n",str);
		fclose(fptr);
	}
#else
	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, fname);
	if( (fptr = fopen(fname_full,"a")) != NULL)
	{
		fprintf(fptr,"%s\r\n",str);
		fclose(fptr);
	}
#endif

}


void AddSubLog(char *fname, char *str) // general purpose logfile creator
{
#ifdef MAIN_SERVER
	FILE *fptr;
	if( (fptr = fopen(fname,"a")) != NULL)
	{
		fprintf(fptr,"%s",str);
		fclose(fptr);
	}
#else
	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, fname);

	FILE *fptr;
	if( (fptr = fopen(fname_full,"a")) != NULL)
	{
		fprintf(fptr,"%s",str);
		fclose(fptr);
	}
#endif
}


// [CModerator]
CModerator::CModerator(int max)
{
	m_name_lst_n = max;
	m_name_lst = new char *[m_name_lst_n]; // allocate enuf pointers.
	m_type_lst = new BYTE [m_name_lst_n]; // allocate enuf bytes
	m_name_lst_idx = 0;
}

CModerator::~CModerator()
{
	Reset();
	delete m_name_lst;
	delete m_type_lst;
}


void CModerator::Reset(void)
{
	while(m_name_lst_idx)
	{
	m_name_lst_idx--;
	delete m_name_lst[m_name_lst_idx];
	}
}

BOOL CModerator::AddModerator(char *_str)
{
	char *str;
	BYTE type;
	if(m_name_lst_idx==m_name_lst_n) return FALSE; // can't add no more!!

	type = 0;	// Adminstrator
	str = _str;
	if(_str[0]=='+') str = &_str[1], type = 1; // Appointed Moderator
	else if(_str[0]=='*') str = &_str[1], type = 2; // Appointed Lead-Moderator (between Admin & Moderator)
	else if(_str[0]=='^') str = &_str[1], type = 3; // Special Moderator (Jang)
	
	m_name_lst[m_name_lst_idx] = new char[strlen(str)+1];
	strcpy(m_name_lst[m_name_lst_idx], str);
	m_type_lst[m_name_lst_idx] = type;
	m_name_lst_idx++;
	return TRUE; // added
}

BOOL CModerator::DeleteModerator(char *str)
{
	int n;
	int i;
	n = CheckName(str);
	if(n==-1) return FALSE; // didn't exist


	delete m_name_lst[n]; // take off the deleted entry.
	
	for(i=n;i<m_name_lst_idx-1;i++)
	{
	m_name_lst[i] = m_name_lst[i+1]; // move entries up by 1.
	m_type_lst[i] = m_type_lst[i+1];
	}
	m_name_lst_idx--;

	return TRUE;
}

int CModerator::CheckName(char *str)
{
	int n;
	for(n=0;n<m_name_lst_idx;n++)
	{
	if(!strcmpi(m_name_lst[n], str)) return n;
	}
	return -1; // couldn't find the mod.
}

int CModerator::GetType(char *str)
{
	return GetType(CheckName(str));
}

int CModerator::GetType(int idx)
{
	if(idx==-1) return -1;
	if(idx>=m_name_lst_idx) return -1;
	return m_type_lst[idx];
}

void CModerator::LoadModerators(char *fname)
{
	FILE *fp;
	int eof;

	Reset();

	if((fp = fopen(fname, "r")) == NULL)
	{
		//can't open file for 'read'
		return;
	}
	
	// load my details:
	int n, m;
	unsigned char str[80];
	
	//for(m=0; m<5; m++)
	//{
	eof = 0;
	while(!eof)
	{
		str[0] = 0;
		for(n=0; n<80; n++)
		{
			if(fread(&str[n], 1, 1, fp)<=0) eof = 1;

			if(str[n] == '\n') {
				str[n] = NULL;
				break;
			}
		}

		if(str[0]<=32); // obviously aint a name if it's a space or something at the start
		else AddModerator((char *) str);
	}

	fclose(fp);
}

void CModerator::SaveModerators(char *fname)
{
	FILE *fp;

	if((fp = fopen(fname, "w")) == NULL)
	{
		//can't open file for 'read'
		return;
	}
	int n;
	for(n=0;n<m_name_lst_idx;n++)
	{
	if(m_type_lst[n]==1) fprintf(fp,"+");
	else if(m_type_lst[n]==2) fprintf(fp,"*");
	else if(m_type_lst[n]==3) fprintf(fp,"^");
	fprintf(fp,"%s\n",m_name_lst[n]);
	}
	fclose(fp);
}

// [CSerialCode]
CSerialCode::CSerialCode(int max)
{
	m_code_lst = new char*[max];
	m_code_lst_n = max;
	m_code_lst_idx = 0;
	m_serialsAdded = new char[16384]; // 16k (16*1024 ish i.e. 1024 new codes)
	m_serialsAdded_n = 16384;
	m_serialsAdded_ptr = m_serialsAdded;
}

CSerialCode::~CSerialCode()
{
	Reset();
	delete m_code_lst;
	delete m_serialsAdded;
}

void CSerialCode::Reset(void)
{
	while(m_code_lst_idx)
	{
	m_code_lst_idx--;
	delete m_code_lst[m_code_lst_idx];
	}
}

void CSerialCode::AddSerial(char *str)
{
	char buffer[80];

	m_code_lst[m_code_lst_idx] = new char[strlen(str)+1];
	strcpy(m_code_lst[m_code_lst_idx],str);
	m_code_lst_idx++;
}

int CSerialCode::SerialCodecmpi(char *str, char *code)
{
		int n;
		char c;
		char d;
		for(n=0;n<80;n++)
		{
		c = str[n];
		d = code[n];
		if(c<=32 && d==':') return 0; // match (one string has endered, and the other has reached ':')
		// Toupper
		if(c>='a' && c<='z') c-=32;
		if(d>='a' && d<='z') d-=32;
		if(c!=d) break; // failed to match.
		}
		return 1; // failed
}

char *CSerialCode::GetSerialType(char *str)
{ // gets code after XXXX;Type
		int n;
		char c;
		char d;
		for(n=0;n<80;n++)
		{
		if(str[n]==':') return &str[n+1];
		if(!str[n]) break;
		}
		return str;
}

char *CSerialCode::GetSerialTypeFromIdx(int idx)
{
	return GetSerialType(m_code_lst[idx]);
}

int CSerialCode::FindSerial(char *str)
{
	int n;
	for(n=0;n<m_code_lst_idx;n++)
	if(m_code_lst[n]!=NULL)
	if(!SerialCodecmpi(str,m_code_lst[n])) {
//	if(!strcmpi(str,m_code_lst[n])) {
									// All codes must be atleast 8 digits or they'll fail!
									if(strlen(str)<8) return -1; // just incase theres some kinda
																 // mistake-hack e.g. a blank serial code "" 
									return n; // return index of serial #
									}

	return -1; // failed to find serial # i.e. doesn't exist!!!
}

void CSerialCode::DeleteSerialIdx(int idx)
{
	// SerialsDelete: is a debug to find which code should be deleted if the server may have crashed!
	FILE *fptr;
	fptr = fopen("SerialsDelete.txt","a");
	if(fptr == NULL) return;
	fprintf(fptr,"%s",m_code_lst[idx]);
	fclose(fptr);

	delete m_code_lst[idx];
	m_code_lst[idx] = NULL;
}

void CSerialCode::ShowAll(void)
{
	int n;
	for(n=0;n<m_code_lst_idx;n++)
	if(m_code_lst[n]!=NULL) // only save non-deleted ones.
	{
	char buffer[80];
	sprintf(buffer,"%d:%s",n,m_code_lst[n]);
	}
}

void CSerialCode::SaveSerials(char *fname)
{
	// SerialsDelete: is a debug to find which code should be deleted if the server may have crashed!
	int n;
	FILE *fptr;
	fptr = fopen(fname,"w");
	if(fptr == NULL) return;
	for(n=0;n<m_code_lst_idx;n++)
	if(m_code_lst[n]!=NULL) // only save non-deleted ones.
	{
	fprintf(fptr,"%s\n",m_code_lst[n]);
	}
	fclose(fptr);

	remove("SerialsDelete.txt");
}

void CSerialCode::LoadSerials(char *fname)
{
	FILE *fp;
	int eof;

	if((fp = fopen(fname, "r")) == NULL)
	{
		//can't open file for 'read'
		return;
	}
	
	// load my details:
	int n, m;
	char str[80];
	
	//for(m=0; m<5; m++)
	//{
	eof = 0;
	while(!eof)
	{

		str[0] = 0;
		for(n=0; n<80; n++)
		{
			if(fread(&str[n], 1, 1, fp)<=0) eof = 1;

			if(str[n] == '\n') {
				str[n] = NULL;
				break;
			}
		}

		if(strlen(str)>8)
		{
		if(str[0]<=32); // obviously aint a name if it's a space or something at the start
		else AddSerial(str);
		}
	}

	fclose(fp);
}

void CSerialCode::AddSerialsToFile(FILE *dptr, char *fname, int amount, char *codetype)
{
	FILE *fp;
	int eof;

	if((fp = fopen(fname, "r")) == NULL)
	{
		//can't open file for 'read'
		return;
	}
	
	// load my details:
	int n, m;
	char str[80];
	
	//for(m=0; m<5; m++)
	//{
	eof = 0;
	while(!eof)
	{

		str[0] = 0;
		for(n=0; n<80; n++)
		{
			if(fread(&str[n], 1, 1, fp)<=0) eof = 1;

			if(str[n] == '\n') {
				str[n] = NULL;
				break;
			}
		}

		if(strlen(str)>8)
		{
		if(str[0]<=32); // obviously aint a name if it's a space or something at the start
		else {
			    if(codetype==NULL) fprintf(dptr,"%s\n",str);
				else fprintf(dptr,"%s:%s\n",str,codetype);
				if(amount==0); // amount = 0 means add all!
				else {
					 amount--;
					 if(!amount) goto end;
					 }
			 }
			//AddSerial(str);
		}
	}
	end:

	fclose(fp);
}

void CSerialCode::AddSerialsToFileNEG(FILE *dptr, char *fname, int amount, char *codetype)
{
	FILE *fp;
	int eof;

	if((fp = fopen(fname, "r")) == NULL)
	{
		//can't open file for 'read'
		return;
	}
	
	// load my details:
	int n, m;
	char str[80];
	
	//for(m=0; m<5; m++)
	//{
	eof = 0;
	while(!eof)
	{

		str[0] = 0;
		for(n=0; n<80; n++)
		{
			if(fread(&str[n], 1, 1, fp)<=0) eof = 1;

			if(str[n] == '\n') {
				str[n] = NULL;
				break;
			}
		}

		if(strlen(str)>8)
		{
		if(str[0]<=32); // obviously aint a name if it's a space or something at the start
		else {
				if(amount) amount--; // ignore first few records
				else {
			    if(codetype==NULL) fprintf(dptr,"%s\n",str);
				else fprintf(dptr,"%s:%s\n",str,codetype);
				}
			 }
			//AddSerial(str);
		}
	}
	end:

	fclose(fp);
}

void CSerialCode::MoveSerials(char *fname, char *addfname, char *codetype, int amount)
{
	FILE *fp;
	int eof;

	FILE *dptr;

	// Make temp file.
	if((dptr = fopen("Serials.tmp", "w")) == NULL)
	{
		//can't open file for 'read'
		return;
	}

	AddSerialsToFile(dptr, addfname, amount, codetype); // only add a selected amount of addfname
	AddSerialsToFile(dptr, fname); // add all of current serials

	fclose(dptr);

	Reset();
	LoadSerials("Serials.tmp");
	SaveSerials(fname);

	remove("Serials.tmp");



	// Deduct from Add file.


	// Make temp file.
	if((dptr = fopen("SerialsAdd.tmp", "w")) == NULL)
	{
		//can't open file for 'read'
		return;
	}

	AddSerialsToFileNEG(dptr, addfname, amount); // only add a selected amount of addfname

	fclose(dptr);

	// now copy back to addfname


	// Make temp file.
	if((dptr = fopen(addfname, "w")) == NULL)
	{
		//can't open file for 'read'
		return;
	}

	AddSerialsToFile(dptr, "SerialsAdd.tmp"); // only add a selected amount of addfname

	fclose(dptr);




	remove("SerialsAdd.tmp");

}

void CSerialCode::AddSerialsToFileDAT(FILE *dptr, char *fname, int amount, char *codetype)
{
	DWORD current;
	DWORD max;
	char codestr[17];
	FILE *fptr;
	// Make temp file.
	if((fptr = fopen(fname, "rb+")) == NULL)
	{
		char buffer[80];
		sprintf(buffer,"Can't Open File %s for read",fname);
		return;
	}

	fread(&current,4,1,fptr);
	fread(&max,4,1,fptr);

	fseek(fptr, 16+current*16,SEEK_SET);
	while(amount)
	{
	fread(codestr,16,1,fptr);
	codestr[16] = 0;

	strcpy(m_serialsAdded_ptr,codestr); // copy 16 character code str to added thing.
	m_serialsAdded_ptr+=16;
	*m_serialsAdded_ptr++='\r';
	*m_serialsAdded_ptr++='\n';

    if(codetype==NULL) fprintf(dptr,"%s\n",codestr);
	else fprintf(dptr,"%s:%s\n",codestr,codetype);

	amount--;
	current++;
	}
	// [+] Alter the current pos!
	fseek(fptr, 0, SEEK_SET); // go back to start to file
	fwrite(&current,4,1,fptr);

	fclose(fptr);
}			

char *CSerialCode::MoveSerialsDAT(char *fname, char *addfname, char *codetype, int amount)
{
	FILE *fp;
	int eof;

	FILE *dptr;

	m_serialsAdded_ptr = m_serialsAdded; // RESET

	// Make temp file.
	if((dptr = fopen("Serials.tmp", "w")) == NULL)
	{
		//can't open file for 'read'
		return "Can't Open File";
	}

	AddSerialsToFileDAT(dptr, addfname, amount, codetype); // only add a selected amount of addfname
	AddSerialsToFile(dptr, fname); // add all of current serials

	fclose(dptr);

	Reset();
	LoadSerials("Serials.tmp");
	SaveSerials(fname);

	remove("Serials.tmp");

	*m_serialsAdded_ptr++=0;

	return m_serialsAdded;
}

// CIPConnectCoolDown

CIPConnectCoolDown::CIPConnectCoolDown()
{
	int n;
	m_pList_n = 256;
	m_pList = new char*[m_pList_n];
	m_pListLast = new CTime[m_pList_n];
	m_pListSecs = new int[m_pList_n];
	for(n=0;n<m_pList_n;n++)
	m_pList[n] = NULL;
}

CIPConnectCoolDown::~CIPConnectCoolDown()
{
	int n;
	for(n=0;n<m_pList_n;n++) // delete all connections.
	if(m_pList[n]) delete m_pList[n];

	delete m_pList;
	delete m_pListLast;
	delete m_pListSecs;
}

void CIPConnectCoolDown::CleanUp(void)
{

	int n;
	for(n=0;n<m_pList_n;n++) // delete all connections.
	if(m_pList[n])
	{
	CTimeSpan s;
	//CTime timeCurrent = CTime::GetCurrentTime();
	CTime timeCurrent;
	timeCurrent = timeCurrent.GetCurrentTime();
	s = timeCurrent - m_pListLast[n];
	if(s>60) // 60 seconds expire - get rid of record.
		{
		delete m_pList[n];
		m_pList[n] = NULL;
		}
	}
}

int CIPConnectCoolDown::Check(char *ip, int secs)
{
	int n;
	int x;
	CleanUp();
	for(n=0;n<m_pList_n;n++) // delete all connections.
	if(m_pList[n])
	{
	if(!strcmpi(m_pList[n], ip)) 
		{
		// Check time. it could've expired already.
		CTimeSpan s;
		//CTime timeCurrent = CTime::GetCurrentTime();
		CTime timeCurrent;
		timeCurrent = timeCurrent.GetCurrentTime();
		s = timeCurrent - m_pListLast[n];
		x = s.GetTotalSeconds();

		if(x>=0 && x<=m_pListSecs[n]) {
						if(x<m_pListSecs[n]) // if they connected within 5 seconds then they're likely doing a hack we'll reset the thing as punishment :)
						{
						m_pListLast[n] = timeCurrent; // update listlast counter
						if(secs>m_pListSecs[n]) m_pListSecs[n] = secs; // use most serious offense
						return -2; // exists on our list but no point talking about such a frequent connection just dismiss it!
						}

						return (int) x; // exists on our list - that's bad!!! - can't connect within 10 seconds.
						}
		m_pListLast[n] = timeCurrent; // update listlast counter
		if(secs>m_pListSecs[n]) m_pListSecs[n] = secs; // use most serious offense
		return -1;
		}
	}
	// not on list //

	for(n=0;n<m_pList_n;n++) // find vacancy.
	if(!m_pList[n])
	{
	m_pList[n] = new char[strlen(ip)+1];
	strcpy(m_pList[n], ip);
	//m_pListLast[n] = CTime::GetCurrentTime();
	m_pListLast[n] = m_pListLast[n].GetCurrentTime();
	m_pListSecs[n] = secs;
	}
	return -1;
}






void SendToQueueTable(CComm *pComm, void *lpBuf, int nBufLen, int tblIdx)
{
	if(tblIdx == -1) return;

	int n;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	if(m_player[n].table == tblIdx)	 // -1 = main lobby, (n) = table
	{
		pComm->SendToQueue(lpBuf, nBufLen, m_player[n].playerSocketIdx);
	}
/*
	#ifdef MAIN_SERVER
	if(tblIdx == -1)
	{
		// send this message to itself (assuming this is always a green chat message)
		pComm->SendToQueue(lpBuf, nBufLen, 0);
	}
	#endif // MAINSERVER
*/
}

void SendToQueueTableX(CComm *pComm, void *lpBuf, int nBufLen, int tblIdx)
{
	int n;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	if(m_player[n].table == tblIdx)	 // -1 = main lobby, (n) = table
	{
		pComm->SendToQueue(lpBuf, nBufLen, m_player[n].playerSocketIdx);
	}

	#ifdef MAIN_SERVER
	if(tblIdx == -1)
	{
		// send this message to itself (assuming this is always a green chat message)
		pComm->SendToQueue(lpBuf, nBufLen, 0);
	}
	#endif // MAINSERVER
}

void SendToQueueTableExceptOne(CComm *pComm, void *lpBuf, int nBufLen, int tblIdx, int plrIdx)
{
	if(tblIdx == -1) return;

	int n;
	for(n=0;n<MAX_LOBBY_PLAYERS;n++)
	if(m_player[n].status)
	if(m_player[n].table == tblIdx)	 // -1 = main lobby, (n) = table
	if(n!=plrIdx) // ignore plr index!
	{
		pComm->SendToQueue(lpBuf, nBufLen, m_player[n].playerSocketIdx);
	}

/*
	#ifdef MAIN_SERVER
	if(tblIdx == -1)
	{
		// send this message to itself (assuming this is always a green chat message)
		pComm->SendToQueue(lpBuf, nBufLen, 0);
	}
	#endif // MAINSERVER
*/
}

/* leave this for now it wont take effect until the next server revision anyways
class CProfanityFilter {
char **m_word;
int m_word_n;
public:
CProfanityFilter(int max);
~CProfanityFilter();

void AddWord(char *str);
void DeleteWord(char *str);
char *SimilarizeString(char *str);
BOOL CheckString(char *str);
};

CProfanityFilter::CProfanityFilter(int max)
{	
	int n;
	m_word_n = max;
	m_word = new char *[m_word_n];
	for(n=0;n<m_word_n;n++)
	m_word[n] = NULL;
}

CProfanityFilter::~CProfanityFilter()
{
	int n;
	for(n=0;n<m_word_n;n++)
	if(m_word[n]) delete m_word[n];
}

void CProfanityFilter::AddWord(char *str)
{
	int n;
	for(n=0;n<m_word_n;n++)
	if(!m_word[n])
	{
	//m_word[n] = new char[strlen(str)+1];
	//strcpy(m_word[n],str);
	m_word[n] = SimilarizeString(str); // convert string to similar (already allocates the memory, we take respons.)
	return;
	}
}

void CProfanityFilter::DeleteWord(char *str)
{
	int n;
	char *sim;
	sim = SimilarizeString(str); // convert string to similar (already allocates the memory, we take respons.)

	for(n=0;n<m_word_n;n++)
	if(m_word[n])
	{
	if(!strcmpi(m_word[n],sim))
		{
		delete m_word[n];
		m_word[n] = NULL;
		return;
		}
	}
}

char *CProfanityFilter::SimilarizeString(char *str)
{
	char *buf;
	int n;
	int len;
	// [1] Convert String to basic characters
	len = strlen(str);
	buf = new char[len+1];
	for(n=0;n<len+1;n++) // copy over chars + terminator.
	{
	buf[n] = str[n]; // do a 256 mapping i.e. 0->o, 1->i etc. so m0th3r is recognized as mother
	}
	return buf; // delete it once you're finished!
}

BOOL CProfanityFilter::CheckString(char *str)
{
	char *buf;
	int n;
	int len;
	// [1] Convert String to basic characters
	len = strlen(str);
	buf = new char[len+1];
	for(n=0;n<len;n++)
	{

	}
}
*/

CIPCoolDown::CIPCoolDown(int max, int limit)
{
	int n;
	m_limit = limit;
	m_ip_n = max;
	m_ip = new char*[m_ip_n];
	for(n=0;n<m_ip_n;n++)
	m_ip[n] = 0;
	m_ip_cnt = new int[m_ip_n];
	m_ip_coolDown = new int[m_ip_n];
}

CIPCoolDown::~CIPCoolDown()
{
	int n;
	for(n=0;n<m_ip_n;n++)
	if(m_ip[n])
	{
	delete m_ip[n];
	}
	delete m_ip;
	delete m_ip_cnt;
	delete m_ip_coolDown;
}

void CIPCoolDown::Process(void) // Performed every 30 mins
{
	int n;
	for(n=0;n<m_ip_n;n++)
	if(m_ip[n])
	{
	if(m_ip_coolDown[n]) m_ip_coolDown[n]--;
	if(!m_ip_coolDown[n])
		{
		delete m_ip[n];
		m_ip[n] = NULL;
		}
	}
}

BOOL CIPCoolDown::CheckIP(char *str)
{
	int n;
	for(n=0;n<m_ip_n;n++)
	if(m_ip[n])
	{
	if(!strcmpi(str,m_ip[n])) // Found a Match!
		{
		/*	{
		char buffer[256];
		sprintf(buffer,"[%s:%d,%d]",str,m_ip_cnt[n],m_ip_coolDown[n]);
		AddLog("LogDebug.txt", buffer); // general purpose logfile creator
			}*/

		if(m_ip_cnt[n]>=3) return FALSE; // don't add it - we've reached the limit.
		}
	}
	return TRUE;
}

void CIPCoolDown::AddIP(char *str)
{
	int n;
	for(n=0;n<m_ip_n;n++)
	if(m_ip[n])
	{
	if(!strcmpi(str,m_ip[n])) // Found a Match!
		{
		if(m_ip_cnt[n]>=3) return; // don't add it - we've reached the limit.

		m_ip_cnt[n]++;
		//m_ip_coolDown[n] = 16; // 16*30mins = 8 Hours
		return;
		}
	}
	// Didn't exist. i.e. Fresh IP (not used in the past 8 hours to make a free account)
	for(n=0;n<m_ip_n;n++)
	if(!m_ip[n])
	{
	m_ip[n] = new char[strlen(str)+1];
	strcpy(m_ip[n], str);
	m_ip_cnt[n] = 1;
	m_ip_coolDown[n] = 16; // 16*30mins = 8 Hours
	return;
	}
}

// [CSysList]

CSysList::CSysList(int max, char *fname)
{
	m_list_idx = 0;
	m_list_n = max;
	m_list_u = new DWORD[m_list_n];
	m_list_l = new WORD[m_list_n];
	strcpy(m_fname, fname);
}

CSysList::~CSysList()
{
	delete m_list_u;
	delete m_list_l;
}

void CSysList::Blank(void)
{
	memset(m_list_u, 0, m_list_n*4); // set entire map to 0.
	memset(m_list_l, 0, m_list_n*2); // set entire map to 0.
	m_list_idx = 0;
}

BOOL CSysList::Load(void) // loads a 256k mash
{
	FILE *fptr;
	int len;
	len = Mmain_filelen(m_fname);
	if(len==-1) {
				Blank();
				return FALSE;
				}
	fptr = fopen(m_fname,"rb");
	if(!fptr)
	{
	Blank();
	return FALSE;
	}
	fread(&m_list_idx, 4, 1,fptr);
	fread(m_list_u, m_list_n*4, 1,fptr);
	fread(m_list_l, m_list_n*2, 1,fptr);
	fclose(fptr);
	if(m_list_idx>=m_list_n) m_list_idx=0; // incase data loaded was bigger than this buffer, we start pointer closer to the beginning.
	return TRUE;
}

BOOL CSysList::Save(void) // saves a 256k mash
{
	FILE *fptr;
	
	fptr = fopen(m_fname,"wb");
	if(!fptr) return FALSE;
	fwrite(&m_list_idx, 4, 1,fptr);
	fwrite(m_list_u, m_list_n*4, 1,fptr);
	fwrite(m_list_l, m_list_n*2, 1,fptr);
	fclose(fptr);
	return TRUE;
}

void CSysList::AddID(DWORD id, WORD cpuid) // has a m_list_n long memory.
{
	m_list_u[m_list_idx] = id;
	m_list_l[m_list_idx] = cpuid;
	m_list_idx++;
	if(m_list_idx>=m_list_n) m_list_idx=0;
}

BOOL CSysList::CheckID(DWORD id, WORD cpuid) // has a m_list_n long memory.
{
	int n;
	for(n=0;n<m_list_n;n++)
	if(id==m_list_u[n] && cpuid==m_list_l[n]) return FALSE;

	return TRUE;
}


BOOL CSysList::SYSmacLog(char *name, char *pass, DWORD sysid, WORD cpuid, DWORD mac_u, WORD mac_l)
{
	// Find How Many Times this MAC ID is Already Present in Memory Bank of MAC Addresses //
	int n, freq;
	freq=0;
	for(n=0;n<m_list_n;n++)
	if(sysid==m_list_u[n] && cpuid==m_list_l[n]) freq++;

	
	FILE *fptr;
	fptr = fopen("MultiTLog.txt", "a+b");
	if(fptr == NULL) return 0;
	fprintf(fptr, "Name:%s, Pass:%s, syscpuID:%lx%x [Freq:%d], (macID:%lx%x)\r\n", name, pass, sysid, cpuid, freq, mac_u, mac_l);
	fclose(fptr);
	return 1;
}



// [CMACList]

CMACList::CMACList(int max, char *fname)
{
	m_maclist_idx = 0;
	m_maclist_n = max;
	m_maclist_u = new DWORD[m_maclist_n];
	m_maclist_l = new WORD[m_maclist_n];
	strcpy(m_fname, fname);
}

CMACList::~CMACList()
{
	delete m_maclist_u;
	delete m_maclist_l;
}

void CMACList::Blank(void)
{
	memset(m_maclist_u, 0, m_maclist_n*4); // set entire map to 0.
	memset(m_maclist_l, 0, m_maclist_n*2); // set entire map to 0.
	m_maclist_idx = 0;
}

BOOL CMACList::Load(void) // loads a 256k +128k mash
{
	FILE *fptr;
	int len;
	len = Mmain_filelen(m_fname);
	if(len==-1) {
				Blank();
				return FALSE;
				}
	fptr = fopen(m_fname,"rb");
	if(!fptr)
	{
	Blank();
	return FALSE;
	}
	fread(&m_maclist_idx, 4, 1,fptr);
	fread(m_maclist_u, m_maclist_n*4, 1,fptr);
	fread(m_maclist_l, m_maclist_n*2, 1,fptr);
	fclose(fptr);
	if(m_maclist_idx>=m_maclist_n) m_maclist_idx=0; // incase data loaded was bigger than this buffer, we start pointer closer to the beginning.
	return TRUE;
}

BOOL CMACList::Save(void) // saves a 256k mash
{
	FILE *fptr;
	
	fptr = fopen(m_fname,"wb");
	if(!fptr) return FALSE;
	fwrite(&m_maclist_idx, 4, 1,fptr);
	fwrite(m_maclist_u, m_maclist_n*4, 1,fptr);
	fwrite(m_maclist_l, m_maclist_n*2, 1,fptr);
	fclose(fptr);
	return TRUE;
}

void CMACList::AddMAC(DWORD mac_u, WORD mac_l) // has a m_list_n long memory.
{
	m_maclist_u[m_maclist_idx] = mac_u;
	m_maclist_l[m_maclist_idx] = mac_l;
	m_maclist_idx++;
	if(m_maclist_idx>=m_maclist_n) m_maclist_idx=0;
}

BOOL CMACList::CheckMAC(DWORD mac_u, WORD mac_l) // has a m_list_n long memory.
{
	int n;
	for(n=0;n<m_maclist_n;n++)
	if(mac_u==m_maclist_u[n] && mac_l==m_maclist_l[n]) return FALSE;

	return TRUE;
}

BOOL CMACList::MACsysLog(char *name, char *pass, DWORD mac_u, WORD mac_l, DWORD sysid, WORD cpuid)
{
	// Find How Many Times this MAC ID is Already Present in Memory Bank of MAC Addresses //
	int n, freq;
	freq=0;
	for(n=0;n<m_maclist_n;n++)
	if(mac_u==m_maclist_u[n] && mac_l==m_maclist_l[n]) freq++;

	
	FILE *fptr;
	fptr = fopen("MultiTLog.txt", "a+b");
	if(fptr == NULL) return 0;
	fprintf(fptr, "Name:%s, Pass:%s, macID:%lx%x [Freq:%d], (syscpuID:%lx)\r\n", name, pass, mac_u, mac_l, freq, sysid, cpuid);
	fclose(fptr);
	return 1;
}




// [CServerAllPlayersStats]
CServerAllPlayersStats::CServerAllPlayersStats(int max)
{
	int n;
	m_plrList_n = max;
	m_plrList = new CServerPlayerStats[m_plrList_n];
	
	for(n=0;n<m_plrList_n;n++)
	{
		m_plrList[n].Clear();
		//m_plrList[n]->name[0] = 0;
		//m_plrList[n]->runOuts8ball = 0;
		//m_plrList[n]->runOuts9ball = 0;
	}
	
	m_plrList_idx = 0;
}

CServerAllPlayersStats::~CServerAllPlayersStats()
{
	delete m_plrList;
}

CServerPlayerStats CServerAllPlayersStats::AddPlrStats(CServerPlayerStats plr)
{
	int n;

		
	// see if this player is already in the list, then update details.
	for(n=0;n<m_plrList_n;n++)	
	if(m_plrList[n].name[0] != 0)
	if(strcmpi(m_plrList[n].name, plr.name) == 0)
	{
		m_plrList[n].runOuts8ballStd += plr.runOuts8ballStd;
		m_plrList[n].runOuts9ballStd += plr.runOuts9ballStd;
		m_plrList[n].runOutsEnglishStd += plr.runOutsEnglishStd;
		
		m_plrList[n].runOuts8ballPro += plr.runOuts8ballPro;
		m_plrList[n].runOuts9ballPro += plr.runOuts9ballPro;
		m_plrList[n].runOutsEnglishPro += plr.runOutsEnglishPro;
		
		m_plrList[n].runOuts8ballUK += plr.runOuts8ballUK;
		m_plrList[n].runOuts9ballUK += plr.runOuts9ballUK;
		m_plrList[n].runOutsEnglishUK += plr.runOutsEnglishUK;

		return m_plrList[n];
	}

	// ...this player is a new entry
	for(n=0;n<m_plrList_n;n++)
	if(m_plrList[n].name[0]==0)
	{
	
		strcpy(m_plrList[n].name, plr.name);
		m_plrList[n].runOuts8ballStd += plr.runOuts8ballStd;
		m_plrList[n].runOuts9ballStd += plr.runOuts9ballStd;
		m_plrList[n].runOutsEnglishStd += plr.runOutsEnglishStd;
		
		m_plrList[n].runOuts8ballPro += plr.runOuts8ballPro;
		m_plrList[n].runOuts9ballPro += plr.runOuts9ballPro;
		m_plrList[n].runOutsEnglishPro += plr.runOutsEnglishPro;
		
		m_plrList[n].runOuts8ballUK += plr.runOuts8ballUK;
		m_plrList[n].runOuts9ballUK += plr.runOuts9ballUK;
		m_plrList[n].runOutsEnglishUK += plr.runOutsEnglishUK;

		m_plrList_idx++;
		return m_plrList[n];
	}

	// failed to add player to list. i.e. we've reached the end of our 'max' list
	plr.Clear();
	return plr;
}

CServerPlayerStats CServerAllPlayersStats::GetPlrStats(char name[22])
{
	int n;	
	CServerPlayerStats plr;
	plr.Clear();
	//plr.name[0] = 0;	
	//plr.runOuts8ball = 0;
	//plr.runOuts9ball = 0;

	// see if this player is already in the list, then update details.
	for(n=0;n<m_plrList_n;n++)
	if(m_plrList[n].name[0] != 0)
	if(strcmpi(m_plrList[n].name, name) == 0)
	{		
		strcpy(plr.name, m_plrList[n].name);
		plr.runOuts8ballStd = m_plrList[n].runOuts8ballStd;
		plr.runOuts9ballStd = m_plrList[n].runOuts9ballStd;
		plr.runOutsEnglishStd = m_plrList[n].runOutsEnglishStd;

		plr.runOuts8ballPro = m_plrList[n].runOuts8ballPro;
		plr.runOuts9ballPro = m_plrList[n].runOuts9ballPro;
		plr.runOutsEnglishPro = m_plrList[n].runOutsEnglishPro;

		plr.runOuts8ballUK = m_plrList[n].runOuts8ballUK;
		plr.runOuts9ballUK = m_plrList[n].runOuts9ballUK;
		plr.runOutsEnglishUK = m_plrList[n].runOutsEnglishUK;

		break;
	}

	return plr;
}


void CServerAllPlayersStats::DeleteAll(void)
{
	int n;
		
	for(n=0;n<m_plrList_n;n++)
	{
		m_plrList[n].Clear();
	}
	
	m_plrList_idx = 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////


// [CServerAllPlayersBreaks]
CServerAllPlayersBreaks::CServerAllPlayersBreaks(int max)
{
	int n;
	m_plrList_n = max;
	m_plrList = new CServerPlayerBreak[m_plrList_n];
	
	for(n=0;n<m_plrList_n;n++)
		{
		m_plrList[n].Clear();
		}
}

CServerAllPlayersBreaks::~CServerAllPlayersBreaks()
{
	delete m_plrList;
}


CServerPlayerBreak CServerAllPlayersBreaks::AddPlrBreak(CServerPlayerBreak plr)
{
	int n;
	// If there is a 0 break in the 64 top breaks, then add us here //
	for(n=0;n<m_plrList_n;n++)	
	if(m_plrList[n].highbreak == 0)
		{
		strcpy(m_plrList[n].name, plr.name);
		m_plrList[n].highbreak = plr.highbreak;
		return m_plrList[n];
		}

	// If all 64 Entries are full, then we need to check if there's any breaks less than ours. If so, we replace this
	// Search from lowest to highest, so we replace the worst break first //
	for(n=(m_plrList_n-1);n>=0;n--)	
	if(m_plrList[n].highbreak < plr.highbreak)
		{
		strcpy(m_plrList[n].name, plr.name);
		m_plrList[n].highbreak = plr.highbreak;
		return m_plrList[n];
		}


	// failed to add player to list. i.e. This break didn't break any of the 64 current breaks //
	plr.Clear();
	return plr;
}

CServerPlayerBreak CServerAllPlayersBreaks::GetPlrBreak(char name[22])
{
	int n;	
	CServerPlayerBreak plr;
	plr.Clear();

	// see if this player is already in the list, then update details.
	for(n=0;n<m_plrList_n;n++)
	if(m_plrList[n].name[0] != 0)
	if(strcmpi(m_plrList[n].name, name) == 0)
	{		
		strcpy(plr.name, m_plrList[n].name);
		plr.highbreak = m_plrList[n].highbreak;
		break;
	}

	return plr;
}

// This function orders the breaks, highest to lowest //
void CServerAllPlayersBreaks::OrderBreaks(void)
{
	char **t_name;
	int *t_highbreak;
	BOOL *t_checked;
	
	t_name = new char *[m_plrList_n];
	t_highbreak = new int[m_plrList_n];
	t_checked = new BOOL[m_plrList_n];


	int n,i;	
	int idx;
	int highest;
	int highest_idx;

	
	for(n=0;n<m_plrList_n;n++) t_checked[n] = 0;
		

	idx = 0;

	for(n=0;n<m_plrList_n;n++)
		{
		highest = 0;
		highest_idx = -1;

		// Find the current highest break //
		for(i=0;i<m_plrList_n;i++)
			{
			if(t_checked[i] == 0)
			if(m_plrList[i].highbreak > highest) 
				{
				highest = m_plrList[i].highbreak;
				highest_idx = i;
				}
			}
		
		// If we have a high break (i.e. highest_idx != -1) //
		if(highest_idx != -1)
			{
			t_checked[highest_idx] = 1;
			t_name[idx] = new char[strlen(m_plrList[highest_idx].name)+1];
			strcpy(t_name[idx], m_plrList[highest_idx].name);
			t_highbreak[idx] = m_plrList[highest_idx].highbreak;
			idx++;
			}
		}

	// Now copy the temp list back to the main list //
	for(n=0;n<idx;n++)
		{
		strcpy(m_plrList[n].name, t_name[n]);
		m_plrList[n].highbreak = t_highbreak[n];
		}

	for(n=0;n<idx;n++) delete t_name[n];
	delete t_highbreak;
	delete t_checked;

		
}


void CServerAllPlayersBreaks::DeleteAll(void)
{
	int n;
		
	for(n=0;n<m_plrList_n;n++)
	{
		m_plrList[n].Clear();
	}
}


void CServerAllPlayersBreaks::UpdateHTML(char *fname)
{
	if(fname == NULL) return;
	char *timestr;
	// Get (Local) time
if(g_gameAppView)
	{
	g_gameAppView->GetTime();
	timestr = g_gameAppView->m_stringFullTime.GetBuffer(0);
	}
	char buf[64];
	int n;
	FILE *fptr;
	if(fptr = fopen(fname, "wb"))
	{
		// Write the High Breaks in HTML Format //
		fprintf(fptr, "<html>\n");
		fprintf(fptr, "<meta http-equiv=refresh content=60; URL='leaderboarda.html'>\n");
		fprintf(fptr, "<body bgcolor='#FFFFFF' scroll='yes' topmargin='0'>\n");
		if(g_gameAppView) fprintf(fptr, "<br><br> This page was last updated: %s<br>\n", timestr);
		fprintf(fptr, "<font size=2>This page will auto refresh in 60 seconds.</font><br><br><br>\n");
		fprintf(fptr, "<table cellspacing=0 cellpadding=0>\n");



		for(n=0;n<m_plrList_n;n++)
		if(m_plrList[n].name[0] != 0)
			{
			// Limit Length of Username to 8 Characters //
			strcpy(buf, m_plrList[n].name);
			if(strlen(buf) > 8)
				{
				buf[9] = '.';
				buf[10] = '.';
				buf[11] = 0;
				}
			fprintf(fptr, "<tr><td width=128><font size=2 color='#000000'><b>%s</b></font size=2></td><td width=16></td><td><font size=2 color='#000000'><b>%d</b></td></tr>\n", m_plrList[n].name, m_plrList[n].highbreak);
			}

		fprintf(fptr, "</table>\n");
		fprintf(fptr, "</font size=2>\n");
		fprintf(fptr, "</body>\n");
		fprintf(fptr, "</html>\n");

		fclose(fptr);
	}

}










///////////////////////////////////////////////////////////////////////////////////////////////

// [CTMatch]
void CTMatch::Clear(void)
{
	m_status = 0;
	m_bNewMatch = TRUE;
	m_bLadderMatch = FALSE;
	m_bMatchOver = FALSE;
	m_bMatchConceded = FALSE;
	m_matchOverCount = MATCH_TIMETOCLEARTABLE;
	m_table = -1;
	m_RaceTo = 1;
	m_name1P[0] = 0;
	m_name2P[0] = 0;
	m_fp1 = 0;
	m_fp2 = 0;
	m_wpts = 0;
	m_lpts = 0;
	m_fee1 = 0;
	m_fee2 = 0;
	m_score1 = 0;
	m_score2 = 0;
	m_round = -1;
	m_waccext = 0;
	m_laccext = 0;
	m_penalty = 0;
	//m_sponsor[0] = 0;
	m_timer1P = 0;
	m_timer2P = 0;
	m_bStopTiming = FALSE;
	m_lostConn1P = 0;
	m_lostConn2P = 0;
	m_bTheFinal = FALSE;	
	m_tid = 0;
	m_tourneyidx = 0;
}

void CTMatch::Add(char *stid, char *sRaceTo, char *name1P, char *name2P, char *sfp1, char *sfp2, char *swpts, char *slpts, char *swcash, char *slcash, char *sfee1, char *sfee2, char *sscore1, char *sscore2, char *sround, char *sWACCEXT, char *sLACCEXT, char *sPen, int idx, bool bLadderMatch)
{

	m_bLadderMatch = bLadderMatch;

	m_tourneyidx = idx;

	sscanf(stid, "%d", &m_tid);

	sscanf(sRaceTo, "%d", &m_RaceTo);
	if(m_RaceTo <= 0) m_RaceTo = 1; // safety
	
	strcpy(m_name1P, name1P);
	strcpy(m_name2P, name2P);
	
	sscanf(sfp1, "%d", &m_fp1);
	if(m_fp1 < 0) m_fp1 = 0; // safety
	sscanf(sfp2, "%d", &m_fp2);
	if(m_fp2 < 0) m_fp2 = 0; // safety

	sscanf(swpts, "%d", &m_wpts);
	if(m_wpts < 0) m_wpts = 0; // safety
	if(m_wpts > 300) m_wpts = 300; // safety
	sscanf(slpts, "%d", &m_lpts);
	if(m_lpts < 0) m_lpts = 0; // safety
	if(m_lpts > 300) m_lpts = 300; // safety

	sscanf(swcash, "%d", &m_wcash);
	if(m_wcash < 0) m_wcash = 0; // safety
	sscanf(slcash, "%d", &m_lcash);
	if(m_lcash < 0) m_lcash = 0; // safety


	sscanf(sfee1, "%d", &m_fee1);
	if(m_fee1 < 0) m_fee1 = 0; // safety
	if(m_fee1 > 50) m_fee1 = 50; // safety
	sscanf(sfee2, "%d", &m_fee2);
	if(m_fee2 < 0) m_fee2 = 0; // safety
	if(m_fee2 > 50) m_fee2 = 50; // safety

	sscanf(sround, "%d", &m_round);

	if(m_round == 0)
		m_bTheFinal = TRUE;
	else
		m_bTheFinal = FALSE;

	/*
	if(m_wpts > 0)
		m_bTheFinal = TRUE;
	else
		m_bTheFinal = FALSE;
	*/

	sscanf(sscore1, "%d", &m_score1);
	if(m_score1 < 0) m_score1 = 0; // safety
	if(m_score1 > 200) m_score1 = 200; // safety
	sscanf(sscore2, "%d", &m_score2);
	if(m_score2 < 0) m_score2 = 0; // safety
	if(m_score2 > 200) m_score2 = 200; // safety	

	

	m_waccext = 0;
	if(sWACCEXT != NULL)
	{
		sscanf(sWACCEXT, "%d", &m_waccext);
		if(m_waccext < 0) m_waccext = 0;
		if(m_waccext > 14) m_waccext = 14;
	}

	m_laccext = 0;
	if(sLACCEXT != NULL)
	{
		sscanf(sLACCEXT, "%d", &m_laccext);
		if(m_laccext < 0) m_laccext = 0;
		if(m_laccext > 14) m_laccext = 14;
	}

	m_penalty = 0;
	if(sPen != NULL)
	{
		sscanf(sPen, "%d", &m_penalty);
		if(m_penalty < 0) m_penalty = 0;
		if(m_penalty > 100) m_penalty = 100;
	}

}

void CTMatch::NotSeatedInvite(bool bAccepted)
{	
	
	// Only Invite Players If they aren't already seated //
	/*
	char buf[256];
	sprintf(buf, "TID: %d -> Inviting: %s vs %s to Table# %d.", m_tid, m_name1P, m_name2P, m_table+1);
	ErrorMessage(((CGameAppView *)g_wnd), buf);
	*/
	
	int n;
	bool bAlreadyInGame;



	// 1P
	int plr1SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_name1P);
	if(plr1SockIdx != -1)
	{
		// If Player 1 is already involved in another game, skip this invite //
		bAlreadyInGame = false;
		for(n=0;n<MAX_LOBBY_TABLES;n++)
		{
			if(g_gameAppView->m_lobby->SeatedAtTableIdx(plr1SockIdx, n))
			if(g_gameAppView->m_lobby->PressedStartAtTable(plr1SockIdx))
			{
				bAlreadyInGame = true;
				break;
			}
		}

		if(!bAlreadyInGame)
		if(!g_gameAppView->m_lobby->SeatedAtTableIdx(plr1SockIdx, m_table))
		{
			// ErrorMessage(((CGameAppView *)g_wnd), "1P Invited!");
			// Auto-Invite player-1 to table
			g_gameAppView->SendInvitePressed(0, plr1SockIdx, g_serverHandle.GetBuffer(0), m_name1P, bAccepted, NULL, m_table, m_table, 1); // network game only!
		}
	}

	// 2P
	int plr2SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_name2P);
	if(plr2SockIdx != -1)
	{
		// If Player 2 is already involved in another game, skip this invite //
		bAlreadyInGame = false;
		for(n=0;n<MAX_LOBBY_TABLES;n++)
		{
			if(g_gameAppView->m_lobby->SeatedAtTableIdx(plr2SockIdx, n))
			if(g_gameAppView->m_lobby->PressedStartAtTable(plr2SockIdx))
			{
				bAlreadyInGame = true;
				break;
			}
		}

		if(!bAlreadyInGame)
		if(!g_gameAppView->m_lobby->SeatedAtTableIdx(plr2SockIdx, m_table))
		{
			// ErrorMessage(((CGameAppView *)g_wnd), "2P Invited!");
			// Auto-Invite player-2 to table
			g_gameAppView->SendInvitePressed(0, plr2SockIdx, g_serverHandle.GetBuffer(0), m_name2P, bAccepted, NULL, m_table, m_table, 2); // network game only!
		}
	}
}



// [CTournament]
CTournament::CTournament(void)
{

	m_name[0] = 0;
	m_desc[0] = 0;

	for(int i=0;i<MAX_TOURNEYS;i++)
	{
		m_table[i].cueError = -1;
		m_table[i].bRated = FALSE; // (note: this is a toggle variable)
		m_table[i].type = Table_typ::TYPE_PRIVATE;
		m_table[i].bMissRule = FALSE; // or callshot(note: this is a toggle variable)
		m_table[i].shotTime = 0xff;
		m_table[i].tableColour = 0xff;
		m_table[i].gameType = 0xff;
		m_table[i].addTimer = 0;//0xff;
		m_table[i].raceTo = 0xff;
		m_table[i].whoBreaks = 0xff;
		m_table[i].ballSize = 0xff;
		m_table[i].rackType = 0xff;
		m_table[i].tableType = 0xff;
		m_table[i].proTable = 0;//0xff;
		m_table[i].doubles = 0;//0xff;
	}

	m_highB = 0;

	m_autoInviteCounter = 0;
	m_autoInviteGroup = 0;

	m_bFinalMatch = FALSE;

	int n;
	for(n=0; n<MAX_TMATCHES; n++)
		m_match[n].Clear();
}

CTournament::~CTournament()
{
	/*
	if(m_name!=NULL)
		delete m_name;
	m_name = NULL;

	if(m_desc!=NULL);
		delete m_desc;
	m_desc = NULL;
	*/
}

void CTournament::AddName(char *name, int idx)
{
	strcpy(m_name, name);
	m_name[255] = 0;
}

void CTournament::AddDesc(char *desc, int idx)
{
	strcpy(m_desc, desc);
	m_desc[1023] = 0;
}

void CTournament::AddSponsor(char *sponsor, int idx)
{	
	strcpy(g_gameAppView->m_sponsortxt, sponsor);
	g_gameAppView->UpdateSponsor();
}

void CTournament::AddSponsorURL(char *sponsorURL, int idx)
{	
	strcpy(g_gameAppView->m_sponsorurl, sponsorURL);
	g_gameAppView->UpdateSponsor();
}

void CTournament::AddOptionsLevel(char *sLevel, int idx)
{	
	#ifndef THE_POOL_CLUB

	if(strcmpi(sLevel, "Beginner") == 0)
		m_table[idx].cueError = 0;
	else if(strcmpi(sLevel, "SemiPro") == 0)
		m_table[idx].cueError = 1;
	else if(strcmpi(sLevel, "Pro") == 0)
		m_table[idx].cueError = 2;
	else
		m_table[idx].cueError = 1;

	#endif //!THE_POOL_CLUB
}

void CTournament::AddOptionsGameType(char *sGameType, int idx)
{
	#ifdef THE_POOL_CLUB

	if(strcmpi(sGameType, "8Ball") == 0)
		m_table[idx].gameType = 0;
	else if(strcmpi(sGameType, "9Ball") == 0)
		m_table[idx].gameType = 1;
	else if(strcmpi(sGameType, "RotationBall") == 0)
		m_table[idx].gameType = 2;
	else if(strcmpi(sGameType, "Bowls") == 0)
		m_table[idx].gameType = 3;
	else if(strcmpi(sGameType, "6Ball") == 0)
		m_table[idx].gameType = 4;
	else if(strcmpi(sGameType, "10Ball") == 0)
		m_table[idx].gameType = 5;
	else if(strcmpi(sGameType, "Break") == 0)
		m_table[idx].gameType = 6;
	else if(strcmpi(sGameType, "UKPool") == 0)
		m_table[idx].gameType = 7;
	
	
	else
		m_table[idx].gameType = 0;

	#else

	if(strcmpi(sGameType, "Snooker") == 0)
		m_table[idx].gameType = 0;
	else if(strcmpi(sGameType, "ShortSnooker") == 0)
		m_table[idx].gameType = 1;
	else if(strcmpi(sGameType, "MiniSnooker") == 0)
		m_table[idx].gameType = 2;
	else if(strcmpi(sGameType, "ReSpotBlack") == 0)
		m_table[idx].gameType = 3;
	else if(strcmpi(sGameType, "Billiards50") == 0)
		m_table[idx].gameType = 4;
	else if(strcmpi(sGameType, "Billiards100") == 0)
		m_table[idx].gameType = 5;
	else if(strcmpi(sGameType, "Billiards200") == 0)
		m_table[idx].gameType = 6;
	else
		m_table[idx].gameType = 0;

	#endif
}

void CTournament::AddOptionsWhoBreaks(char *sWhoBreaks, int idx)
{
	#ifdef THE_POOL_CLUB

	if(strcmpi(sWhoBreaks, "1PAlways") == 0)
		m_table[idx].whoBreaks = 0;
	else if(strcmpi(sWhoBreaks, "2PAlways") == 0)
		m_table[idx].whoBreaks = 1;
	else if(strcmpi(sWhoBreaks, "Alternate") == 0)
		m_table[idx].whoBreaks = 2;
	else if(strcmpi(sWhoBreaks, "Winner") == 0)
		m_table[idx].whoBreaks = 3;
	else if(strcmpi(sWhoBreaks, "Loser") == 0)
		m_table[idx].whoBreaks = 4;
	else if(strcmpi(sWhoBreaks, "PlayerTrailing") == 0)
		m_table[idx].whoBreaks = 5;
	else
		m_table[idx].whoBreaks = 2;

	#else

	if(strcmpi(sWhoBreaks, "1PAlways") == 0)
		m_table[idx].whoBreaks = 0;
	else if(strcmpi(sWhoBreaks, "2PAlways") == 0)
		m_table[idx].whoBreaks = 1;
	else if(strcmpi(sWhoBreaks, "Alternate") == 0)
		m_table[idx].whoBreaks = 2;
	else if(strcmpi(sWhoBreaks, "Winner") == 0)
		m_table[idx].whoBreaks = 3;
	else if(strcmpi(sWhoBreaks, "Loser") == 0)
		m_table[idx].whoBreaks = 4;
	else if(strcmpi(sWhoBreaks, "PlayerTrailing") == 0)
		m_table[idx].whoBreaks = 5;
	else
		m_table[idx].whoBreaks = 2;

	#endif
}

void CTournament::AddOptionsTableColour(char *sTableColour, int idx)
{
	#ifdef THE_POOL_CLUB

	if(strcmpi(sTableColour, "Blue") == 0)
		m_table[idx].tableColour = 0;
	else if(strcmpi(sTableColour, "Red") == 0)
		m_table[idx].tableColour = 1;
	else if(strcmpi(sTableColour, "Green") == 0)
		m_table[idx].tableColour = 2;
	else if(strcmpi(sTableColour, "Black") == 0)
		m_table[idx].tableColour = 3;
	else if(strcmpi(sTableColour, "Yellow") == 0)
		m_table[idx].tableColour = 4;
	else if(strcmpi(sTableColour, "Purple") == 0)
		m_table[idx].tableColour = 5;
	else if(strcmpi(sTableColour, "Teal") == 0)
		m_table[idx].tableColour = 6;
	else if(strcmpi(sTableColour, "Brown") == 0)
		m_table[idx].tableColour = 7;
	else if(strcmpi(sTableColour, "White") == 0)
		m_table[idx].tableColour = 8;
	else if(strcmpi(sTableColour, "BlueBlk") == 0)
		m_table[idx].tableColour = 9;
	else if(strcmpi(sTableColour, "RedBlk") == 0)
		m_table[idx].tableColour = 10;
	else if(strcmpi(sTableColour, "PurpleBlk") == 0)
		m_table[idx].tableColour = 11;
	else
		m_table[idx].tableColour = 1;

	#else

	if(strcmpi(sTableColour, "Blue") == 0)
		m_table[idx].tableColour = 0;
	else if(strcmpi(sTableColour, "Red") == 0)
		m_table[idx].tableColour = 1;
	else if(strcmpi(sTableColour, "Green") == 0)
		m_table[idx].tableColour = 2;
	else if(strcmpi(sTableColour, "Black") == 0)
		m_table[idx].tableColour = 3;
	else if(strcmpi(sTableColour, "Yellow") == 0)
		m_table[idx].tableColour = 4;
	else if(strcmpi(sTableColour, "Purple") == 0)
		m_table[idx].tableColour = 5;
	else if(strcmpi(sTableColour, "Teal") == 0)
		m_table[idx].tableColour = 6;
	else if(strcmpi(sTableColour, "DarkGreen") == 0)
		m_table[idx].tableColour = 7;
	else
		m_table[idx].tableColour = 0;

	#endif
}

void CTournament::AddOptionsTableType(char *sTableType, int idx)
{
	#ifdef THE_POOL_CLUB

	if(strcmpi(sTableType, "Normal") == 0)
		m_table[idx].tableType = 0;
	else if(strcmpi(sTableType, "Pro") == 0)
		m_table[idx].tableType = 1;
	else if(strcmpi(sTableType, "UKPool") == 0)
		m_table[idx].tableType = 2;
	else if(strcmpi(sTableType, "CenterHole") == 0)
		m_table[idx].tableType = 3;
	else if(strcmpi(sTableType, "BlackHole") == 0)
		m_table[idx].tableType = 4;
	else if(strcmpi(sTableType, "Random3Holes") == 0)
		m_table[idx].tableType = 5;
	else if(strcmpi(sTableType, "Random5Holes") == 0)
		m_table[idx].tableType = 6;
	else if(strcmpi(sTableType, "Random7Holes") == 0)
		m_table[idx].tableType = 7;
	else if(strcmpi(sTableType, "Random9Holes") == 0)
		m_table[idx].tableType = 8;
	else 
		m_table[idx].tableType = 0;

	#else

	if(strcmpi(sTableType, "Normal") == 0)
		m_table[idx].tableType = 0;
	else 
		m_table[idx].tableType = 0;

	#endif
}

void CTournament::AddOptionsBallSize(char *sBallSize, int idx)
{
	#ifdef THE_POOL_CLUB

	if(strcmpi(sBallSize, "Normal") == 0)
		m_table[idx].ballSize = 0;
	else if(strcmpi(sBallSize, "Big") == 0)
		m_table[idx].ballSize = 1;
	else if(strcmpi(sBallSize, "Small") == 0)
		m_table[idx].ballSize = 2;
	else
		m_table[idx].ballSize = 0;

	#else

	if(strcmpi(sBallSize, "Normal") == 0)
		m_table[idx].ballSize = 0;
	else if(strcmpi(sBallSize, "Giant") == 0)
		m_table[idx].ballSize = 1;
	else
		m_table[idx].ballSize = 0;

	#endif
}

void CTournament::AddOptionsRackType(char *sRackType, int idx)
{
	#ifdef THE_POOL_CLUB

	if(strcmpi(sRackType, "Normal") == 0)
		m_table[idx].rackType = 0;
	else if(strcmpi(sRackType, "Groups") == 0)
		m_table[idx].rackType = 1;
	else if(strcmpi(sRackType, "Random") == 0)
		m_table[idx].rackType = 2;
	else
		m_table[idx].rackType = 0;

	#else

	if(strcmpi(sRackType, "Normal") == 0)
		m_table[idx].rackType = 0;
	else if(strcmpi(sRackType, "RandomlySpottedColours") == 0)
		m_table[idx].rackType = 1;
	else if(strcmpi(sRackType, "RandomlyPlaced olours") == 0)
		m_table[idx].rackType = 2;
	else if(strcmpi(sRackType, "TriangleMix") == 0)
		m_table[idx].rackType = 3;
	else if(strcmpi(sRackType, "RedsBehindBlue") == 0)
		m_table[idx].rackType = 4;
	else if(strcmpi(sRackType, "RedsBehindBrown") == 0)
		m_table[idx].rackType = 5;
	else if(strcmpi(sRackType, "21Reds") == 0)
		m_table[idx].rackType = 6;
	else if(strcmpi(sRackType, "28Reds") == 0)
		m_table[idx].rackType = 7;
	else if(strcmpi(sRackType, "AllBlacks") == 0)
		m_table[idx].rackType = 8;
	else if(strcmpi(sRackType, "ScrapHeap") == 0)
		m_table[idx].rackType = 9;
	else
		m_table[idx].rackType = 0;

	#endif
}

void CTournament::AddOptionsAddTimer(char *sAddTimer, int idx)
{
	if(strcmpi(sAddTimer, "Yes") == 0)
		m_table[idx].addTimer = 1;
	else if(strcmpi(sAddTimer, "No") == 0)
		m_table[idx].addTimer = 0;
	else
		m_table[idx].addTimer = 0;
//m_table.addTimer = 0xff;
}

void CTournament::AddOptionsProTable(char *sProTable, int idx) // note: for iPool only
{
	// This Function is no longer needed as pro table is stored in Table Type now for iPool //
	/*
	if(strcmpi(sProTable, "Yes") == 0)
		m_table.proTable = 1;
	else if(strcmpi(sProTable, "No") == 0)
		m_table.proTable = 0;
	else
		m_table.proTable = 0;
	*/
}

void CTournament::AddOptionsDoubles(char *sDoubles, int idx) // note: for iPool only
{
	//char buffer[80];
	//sprintf(buffer, "%s", sProTable);
	//ErrorMessage(g_wnd, buffer);

	if(strcmpi(sDoubles, "Yes") == 0)
		m_table[idx].doubles = 1;
	else if(strcmpi(sDoubles, "No") == 0)
		m_table[idx].doubles = 0;
	else
		m_table[idx].doubles = 0;
//m_table.addTimer = 0xff;
}


void CTournament::AddOptionsShotTime(char *sShotTime, int idx)
{
	sscanf(sShotTime, "%d", &m_table[idx].shotTime);
	if(m_table[idx].shotTime < 3) m_table[idx].shotTime = 3; // safety
	if(m_table[idx].shotTime > 60) m_table[idx].shotTime = 60; // safety
//	m_table.shotTime = 0xff;
}

void CTournament::AddOptionsRated(char *sRated, int idx)
{
	if(strcmpi(sRated, "Yes") == 0)
		m_table[idx].bRated = FALSE;
	else if(strcmpi(sRated, "No") == 0)
		m_table[idx].bRated = TRUE;
	else
		m_table[idx].bRated = FALSE;
}

void CTournament::AddOptionsCallShots(char *sCallShots, int idx)
{
	if(strcmpi(sCallShots, "Yes") == 0)
		m_table[idx].bMissRule = TRUE;
	else if(strcmpi(sCallShots, "No") == 0)
		m_table[idx].bMissRule = FALSE;
	else
		m_table[idx].bMissRule = FALSE;
}

void CTournament::AddOptionsMissRule(char *sMissRule, int idx)
{ // note: this is the same as above	
	if(strcmpi(sMissRule, "Yes") == 0)
		m_table[idx].bMissRule = TRUE;
	else if(strcmpi(sMissRule, "No") == 0)
		m_table[idx].bMissRule = FALSE;
	else
		m_table[idx].bMissRule = FALSE;
}

void CTournament::AddOptionsHighB(char *sHighB, int idx)
{
	sscanf(sHighB, "%d", &m_highB);
	if(m_highB < 0) m_highB = 0; // safety
	if(m_highB > 300) m_highB = 300; // safety
}

void CTournament::AddOptionsHighBCash(char *sHighBCash, int idx)
{
	sscanf(sHighBCash, "%d", &m_highBCash);
	if(m_highBCash < 0) m_highBCash = 0; // safety
}

/*
 * Returns true if name belongs to any match
 */
bool CTournament::InMatch(char *name)
{
	int n;
	for (n = 0; n < MAX_TMATCHES; n++)
	if (m_match[n].m_status)
	if (!m_match[n].m_bMatchOver)
	if (!m_match[n].m_bMatchConceded)
	{
		if (strcmpi(m_match[n].m_name1P, name) == 0) return true;
		else if (strcmpi(m_match[n].m_name2P, name) == 0) return true;
		else;
	}

	return false;
}

void CTournament::AddMatch(char *stid, char *sRaceTo, char *name1P, char *name2P, char *sfp1, char *sfp2, char *swpts, char *slpts, char *swcash, char *slcash, char *sfee1, char *sfee2, char *sscore1, char *sscore2, char *sround, char *sWACCEXT, char *sLACCEXT, char *sPen, char *sGameType, int idx, bool bLadderMatch)
{	
	int n;
	int tid = -1;

	sscanf(stid, "%d", &tid);

	if(tid == -1) return;

	if (bLadderMatch)
	{
		// Only Compare matches that aren't over (if Ladder Match)
		// (1) see if name1P&name2P together is in our matches list
		for (n = 0; n<MAX_TMATCHES; n++)
		if (m_match[n].m_tid == tid)		// Make sure we are comparing Matches in the Same Tournament
		if(!m_match[n].m_bMatchOver)
			//	if(!m_match[n].m_bStopTiming)
		if (strcmpi(m_match[n].m_name1P, name1P) == 0)
		if (strcmpi(m_match[n].m_name2P, name2P) == 0)
		{
			m_match[n].Add(stid, sRaceTo, name1P, name2P, sfp1, sfp2, swpts, slpts, swcash, slcash, sfee1, sfee2, sscore1, sscore2, sround, sWACCEXT, sLACCEXT, sPen, idx, bLadderMatch);
			AddOptionsGameType(sGameType, idx); // new
			FinalMatchProcess(n);
			// etc...
			return;
		}
	}
	else
	{

		// (1) see if name1P&name2P together is in our matches list
		for (n = 0; n < MAX_TMATCHES; n++)
		if (m_match[n].m_tid == tid)		// Make sure we are comparing Matches in the Same Tournament
			//	if(!m_match[n].m_bMatchOver)
			//	if(!m_match[n].m_bStopTiming)
		if (strcmpi(m_match[n].m_name1P, name1P) == 0)
		if (strcmpi(m_match[n].m_name2P, name2P) == 0)
		{
			m_match[n].Add(stid, sRaceTo, name1P, name2P, sfp1, sfp2, swpts, slpts, swcash, slcash, sfee1, sfee2, sscore1, sscore2, sround, sWACCEXT, sLACCEXT, sPen, idx, bLadderMatch);
			AddOptionsGameType(sGameType, idx); // new
			FinalMatchProcess(n);
			// etc...
			return;
		}
	}

	// (2) ...if got this far, must be a new match entry
	for(n=0; n<MAX_TMATCHES; n++)
	if(!m_match[n].m_status)
	{
		m_match[n].m_bLadderMatch = bLadderMatch;
		m_match[n].m_bNewMatch = TRUE;
		m_match[n].m_status = 1;
		m_match[n].Add(stid, sRaceTo, name1P, name2P, sfp1, sfp2, swpts, slpts, swcash, slcash, sfee1, sfee2, sscore1, sscore2, sround, sWACCEXT, sLACCEXT, sPen, idx, bLadderMatch);
		AddOptionsGameType(sGameType, idx); // new
		FinalMatchProcess(n);
		return;
	}
}

void CTournament::FinalMatchProcess(int n)
{	
	if(m_match[n].m_bTheFinal)
		m_bFinalMatch = TRUE;
	else
	{
		if(m_bFinalMatch)
		{
			// clear the highscore's table in lobby (since this is the first match after the final)...
			m_bFinalMatch = FALSE;
			// Don't Reset Breaks for Sit n Go Lobby //
			if(g_tourney_system!=4)
			{
				g_gameAppView->SendAllHighBreaksReset();
			}
		}
	}
}

BOOL CTournament::bIsEmptyTable(int tbl)
{
	int n;

	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(m_match[n].m_table == tbl)
		return FALSE;

	return TRUE;
}

void CTournament::ClearMatchTableNo(int tbl)
{
	int n;

	if(tbl == -1) return;

	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(m_match[n].m_table == tbl)
	{
		m_match[n].m_table = -1;
	}
}

void CTournament::ClearMatchTable(int tbl)
{
	int n;

	if(tbl == -1) return;

	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(m_match[n].m_table == tbl)
	{
		m_match[n].Clear();
	}
}

BOOL CTournament::bIsTableUsed(int tbl)
{
	int n;

	if(tbl == -1) return FALSE;;

	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(m_match[n].m_table == tbl)
	{
		return TRUE;
	}

	return FALSE;
}

int CTournament::GetMatchFromTableNo(int tbl)
{
	int n;

	if(tbl == -1) return FALSE;;

	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(m_match[n].m_table == tbl)
	{
		return n;
	}

	return -1;
}
//
void CTournament::ProcessAutoInvites(void) // called every 2secs!
{
	int n;				
	int tbl;
	BOOL b1stNewMatch = TRUE; // for all new groups of matches, only allow 1 NEW Match every time we get in here

	// Scan thru all matches, seeing who is still to be invited...
	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(!m_match[n].m_bMatchOver)
	{
		if(!b1stNewMatch)
			if(m_match[n].m_bNewMatch)
				continue; // as we've already had a new Match this time, skip this NEW Match until the next time

		// If this is a Conceded Match (that is now over), don't bother inviting players anymore...
		if (m_match[n].m_bMatchConceded) continue;

		// (1) if this match doesn't have a table assigned to it...
		if(m_match[n].m_table == -1)
		{
			// find vacant table
			for(tbl=0; tbl<MAX_TABLEITEMS; tbl++)
			if(g_gameAppView->m_lobby->EmptyTable(tbl))
			if(bIsEmptyTable(tbl))
			{
				// found a vacant table!
				break;
			}
			if(tbl == MAX_TABLEITEMS) continue; //return; // no spare table

			// take table!
			m_match[n].m_table = tbl;
			
			// update this table in the lobby
			int tidx = m_match[n].m_tourneyidx;
			
				
			g_gameAppView->SendTableHostDetails(FALSE, g_tournament.m_table[tidx].cueError, g_tournament.m_table[tidx].bRated, g_tournament.m_table[tidx].type, g_tournament.m_table[tidx].bMissRule, -1,
													g_tournament.m_table[tidx].shotTime, g_tournament.m_table[tidx].tableColour, g_tournament.m_table[tidx].gameType, g_tournament.m_table[tidx].addTimer,
													g_tournament.m_table[tidx].doubles, m_match[n].m_RaceTo-1/*g_tournament.m_table.raceTo*/, g_tournament.m_table[tidx].whoBreaks,
													g_tournament.m_table[tidx].ballSize, g_tournament.m_table[tidx].rackType, g_tournament.m_table[tidx].tableType, g_tournament.m_table[tidx].proTable,
													tbl, m_match[n].m_fp1, m_match[n].m_fp2, m_match[n].m_score1, m_match[n].m_score2, m_match[n].m_bLadderMatch);
			// tick off new match!
			b1stNewMatch = FALSE;
			m_match[n].m_bNewMatch = FALSE;
		}

		// (2) invite players to their match table...
		if(m_autoInviteCounter == 0)
		{	
			if (m_match[n].m_bLadderMatch)
			{
				// Don't do group invites for ladder matches.
				// Sends invite dialog, giving player option to accept or decline
				m_match[n].NotSeatedInvite(false);
			}
			else
			{
				if (m_autoInviteGroup == (n % 4))
				{
					// Sends invite dialog, giving player option to accept or decline
					m_match[n].NotSeatedInvite(false);
				}
			}
				
		}
	}

	m_autoInviteCounter++;
	if(m_autoInviteCounter >= 2)  // (called every 2secs)*2 = 4secs // invite someone every 2 seconds
	{			
		m_autoInviteCounter = 0;
		m_autoInviteGroup++;
		if(m_autoInviteGroup >= 4) // 4 groups interleaved (mod4) (0),1,2,3,(4),5,6,7,(8),9,10,11,(12),... etc So 16secs per person
			m_autoInviteGroup = 0;
	}

	// called every 2 secs...

	/// BP_FIX 13Nov05//
/// I have Moved this function to be called at the end of Every Tourney Match, just after
/// Winner's Prize is awarded ///
	CheckGiveHighBreakPrize();
}
//
void CTournament::ProcessMatchTimers(void)
{
	int n;
	int tbl;
	int plr1SockIdx;
	int plr2SockIdx;
	BOOL bTimeUp1P;
	BOOL bTimeUp2P;
	BOOL bSeated1P;
	BOOL bSeated2P;
	BOOL bPressedStart1P;
	BOOL bPressedStart2P;


	// New Code, Check if we have any matches with unseated player.. if so, make sure time is on



	// Scan thru all matches for unseated players...
	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(!m_match[n].m_bMatchOver)
	if(m_match[n].m_table != -1) // we have an active match?
	if(!m_match[n].m_bStopTiming)
	{
		bTimeUp1P = FALSE;
		bTimeUp2P = FALSE;

		int plr1SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_match[n].m_name1P);
		int plr2SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_match[n].m_name2P);

		bSeated1P = g_gameAppView->m_lobby->SeatedAtTableIdx(plr1SockIdx, m_match[n].m_table);
		bSeated2P = g_gameAppView->m_lobby->SeatedAtTableIdx(plr2SockIdx, m_match[n].m_table);

		//
		bPressedStart1P = g_gameAppView->m_lobby->PressedStartAtTable(plr1SockIdx);
		bPressedStart2P = g_gameAppView->m_lobby->PressedStartAtTable(plr2SockIdx);

		
		// if both players are seated, check if they've both pressed start
		if(bSeated1P && bSeated2P)
		{
			DoBroadcastMatchTimers(n, 1);

			if(bPressedStart1P)
				m_match[n].m_timer1P = 0;
			else
			{
				m_match[n].m_timer1P++;

				if (m_match[n].m_bLadderMatch)
				{
					if (m_match[n].m_timer1P >= LADDERMATCH_TIMETOSITSTART)
						bTimeUp1P = TRUE;
				}
				else
				{
					if (m_match[n].m_timer1P >= MATCH_TIMETOSITSTART)
						bTimeUp1P = TRUE;
				}

//				{
//				char buf[256];
//				sprintf(buf, "(*)1P has not Pressed Start: %d", m_match[n].m_timer1P);
//				ErrorMessage(((CGameAppView *)g_wnd), buf);
//				}
			}

			if(bPressedStart2P)
				m_match[n].m_timer2P = 0;
			else
			{
				m_match[n].m_timer2P++;


				if (m_match[n].m_bLadderMatch)
				{
					if (m_match[n].m_timer2P >= LADDERMATCH_TIMETOSITSTART)
						bTimeUp2P = TRUE;
				}
				else
				{
					if (m_match[n].m_timer2P >= MATCH_TIMETOSITSTART)
						bTimeUp2P = TRUE;
				}


//				{
//				char buf[256];
//				sprintf(buf, "(*)2P has not Pressed Start: %d", m_match[n].m_timer2P);
//				ErrorMessage(((CGameAppView *)g_wnd), buf);
//				}
			}

			if(bTimeUp1P || bTimeUp2P)
			{
//				{
//				char buf[256];
//				if(bTimeUp1P && bTimeUp2P)
//					sprintf(buf, "1P && 2P TimeUp!");
//				else
//					sprintf(buf, "1P || 2P TimeUp!");
//				ErrorMessage(((CGameAppView *)g_wnd), buf);
//				}

				// Cancel our search
				g_gameAppView->SendClientFindMatchStatus(plr1SockIdx, 0);
				// Cancel our search
				g_gameAppView->SendClientFindMatchStatus(plr2SockIdx, 0);

				//m_match[n].m_bMatchOver = TRUE; //
				//m_match[n].m_matchOverCount = MATCH_TIMETOCLEARTABLE; //
				m_match[n].m_bStopTiming = TRUE;
				g_gameAppView->m_lobby->TournamentMatchTimeUp(m_match[n].m_table, bTimeUp1P, bTimeUp2P);
			}
		}
		else
		{ // 1 or more players aren't seated

			DoBroadcastMatchTimers(n, 0);

			if(bSeated1P)
				m_match[n].m_timer1P = 0;
			else
			{
				m_match[n].m_timer1P++;

				if (m_match[n].m_bLadderMatch)
				{
					if (m_match[n].m_timer1P >= LADDERMATCH_TIMETOSITSTART)
						bTimeUp1P = TRUE;
				}
				else
				{
					if (m_match[n].m_timer1P >= MATCH_TIMETOSITSTART)
						bTimeUp1P = TRUE;
				}


//				{
//				char buf[256];
//				sprintf(buf, "( )1P has not Sat: %d", m_match[n].m_timer1P);
//				ErrorMessage(((CGameAppView *)g_wnd), buf);
//				}
			}

			if(bSeated2P)
				m_match[n].m_timer2P = 0;
			else
			{
				m_match[n].m_timer2P++;
			
				if (m_match[n].m_bLadderMatch)
				{
					if (m_match[n].m_timer2P >= LADDERMATCH_TIMETOSITSTART)
						bTimeUp2P = TRUE;
				}
				else
				{
					if (m_match[n].m_timer2P >= MATCH_TIMETOSITSTART)
						bTimeUp2P = TRUE;
				}


//				{
//				char buf[256];
//				sprintf(buf, "( )2P has not Sat: %d", m_match[n].m_timer2P);
//				ErrorMessage(((CGameAppView *)g_wnd), buf);
//				}
			}

			if(bTimeUp1P || bTimeUp2P)
			{
//				{
//				char buf[256];
//				if(bTimeUp1P && bTimeUp2P)
//					sprintf(buf, "1P && 2P TimeUp!");
//				else
//					sprintf(buf, "1P || 2P TimeUp!");
//				ErrorMessage(((CGameAppView *)g_wnd), buf);
//				}

				//m_match[n].m_bMatchOver = TRUE;
				//m_match[n].m_matchOverCount = MATCH_TIMETOCLEARTABLE;

				// We also better tell clients to reset Find Match Ctrl

				// Cancel our search
				g_gameAppView->SendClientFindMatchStatus(plr1SockIdx, 0);
				// Cancel our search
				g_gameAppView->SendClientFindMatchStatus(plr2SockIdx, 0);

				m_match[n].m_bStopTiming = TRUE;
				g_gameAppView->m_lobby->TournamentMatchTimeUp(m_match[n].m_table, bTimeUp1P, bTimeUp2P);



			}
		}
	} // end for n
}

void CTournament::ProcessMatchOvers(void)
{
	int n;


	// Insetad of clearing the table after 5 minutes (i.e. when m_matchOverCount is 0)
	// We allow the player to continue playing provided they make a shot within 2 minutes 
	// If a player exceeds 2 minutes, then clear the table


	
	
	// Scan thru all matches for unseated players...
	for(n=0; n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(m_match[n].m_bMatchOver)
	{
		DoBroadcastMatchOvers(n, 10);
		m_match[n].m_matchOverCount--;
		if(m_match[n].m_matchOverCount <= 0)
		{
			// server to send 'cleartable' msg
			if(m_match[n].m_table != -1)
				g_gameAppView->SendClearTable(m_match[n].m_table);

			// clear match logically from server's list
			m_match[n].Clear();
		}
	}
	
}

void CTournament::DoBroadcastMatchTimers(int matchIdx, int type)
{
	int n = matchIdx;
	int timePeriod = 60;
	if (m_match[n].m_bLadderMatch) timePeriod = 30;


	// tell players how long they have left (1P)...
	if (m_match[n].m_timer1P > 0)
	{
		int _timeRemaining1P;
		
		if (m_match[n].m_bLadderMatch) _timeRemaining1P = LADDERMATCH_TIMETOSITSTART - m_match[n].m_timer1P;
		else _timeRemaining1P = MATCH_TIMETOSITSTART - m_match[n].m_timer1P;

		//if(m_match[n].m_timer1P == 1) _timeRemaining1P += 1; // TRICK! - so that we get into the following code block one extra time, at the start
		if (_timeRemaining1P != 0)
		{
			if (!(_timeRemaining1P % timePeriod)) // on every timePeriod, send time-left msg to this individual player
			{
				int plr1SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_match[n].m_name1P);
				if (plr1SockIdx != -1)
				{
					g_gameAppView->SendTMatchTimerLeft(_timeRemaining1P, type, m_match[n].m_name1P, plr1SockIdx);
				}

				int plr2SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_match[n].m_name2P);
				if (plr2SockIdx != -1)
				{
					g_gameAppView->SendTMatchTimerLeft(_timeRemaining1P, type, m_match[n].m_name1P, plr2SockIdx);
				}
			}
		}
		

	}

	// tell players how long they have left (2P)...
	if (m_match[n].m_timer2P > 0)
	{
		int _timeRemaining2P;
		if (m_match[n].m_bLadderMatch) _timeRemaining2P = LADDERMATCH_TIMETOSITSTART - m_match[n].m_timer2P;
		else _timeRemaining2P = MATCH_TIMETOSITSTART - m_match[n].m_timer2P;

		//if(m_match[n].m_timer2P == 1) _timeRemaining2P += 1; // TRICK! - so that we get into the following code block one extra time, at the start
		if (_timeRemaining2P != 0 )
		if (!(_timeRemaining2P % timePeriod)) // on every timePeriod, send time-left msg to this individual player
		{
			int plr2SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_match[n].m_name2P);
			if(plr2SockIdx != -1)
			{
				g_gameAppView->SendTMatchTimerLeft(_timeRemaining2P, type, m_match[n].m_name2P, plr2SockIdx);
			}

			int plr1SockIdx = g_gameAppView->m_lobby->FindPlayerSockIdx(m_match[n].m_name1P);
			if(plr1SockIdx != -1)
			{
				g_gameAppView->SendTMatchTimerLeft(_timeRemaining2P, type, m_match[n].m_name2P, plr1SockIdx);
			}
		}
	}
}

void CTournament::DoBroadcastMatchOvers(int matchIdx, int type)
{
	int n = matchIdx;
	int timePeriod = 60;

	if (m_match[n].m_bLadderMatch) timePeriod = 30;

	// tell all players at table how long they have left before this table is cleared!...
	if(m_match[n].m_matchOverCount > 0)
	{
		int _timeRemaining = m_match[n].m_matchOverCount;
		if (!(_timeRemaining % timePeriod)) // on every minute, send time-left msg to all players at table...
		{
			if(m_match[n].m_table != -1)
				g_gameAppView->SendTMatchTimerLeft(_timeRemaining, type, "", -1, m_match[n].m_table);
		}
	}
}


/// BP_FIX 13Nov05//
/// I have Moved this function to be called at the end of Every Tourney Match, just after
/// Winner's Prize is awarded ///
void CTournament::CheckGiveHighBreakPrize(void)
{
	if(m_highB <= 0) return;
	if(m_doubles == 1) return; // Special Flag, so Most RunOut Prize isn't awarded for every KO Match in Doubles 

	// We know final is finished, so just skip to the prize awarding code

	// Set the Final to False, since it's now Over //
	int n;
	for(n=0;n<MAX_TMATCHES; n++)
	if(m_match[n].m_status)
	if(m_match[n].m_bTheFinal)
	if(m_match[n].m_bMatchOver)
	if(m_match[n].m_matchOverCount < (MATCH_TIMETOCLEARTABLE - 10)) // 10 seconds after match is over
		{
		m_match[n].m_bTheFinal = FALSE;
		goto _doitnow;
		}
	
	return;

// if this was the final match, give out HIGHBREAK prize!
_doitnow:;


	// Get Tourney Active High Breaks / Most Runouts //
	int HighBreak[3];
	CString HighBreakName[3];

	HighBreak[0] = 0;
	HighBreak[1] = 0;
	HighBreak[2] = 0;

	HighBreakName[0] = "...";
	HighBreakName[1] = "...";
	HighBreakName[2] = "...";


	
	if(g_gameAppView->m_sLobbyHighBreak[0]>0) // Semi Pro or 8-Ball 
		{
		HighBreak[0] = g_gameAppView->m_sLobbyHighBreak[0];
		HighBreak[1] = g_gameAppView->m_sLobbyHighBreak[1];
		HighBreak[2] = g_gameAppView->m_sLobbyHighBreak[2];
		HighBreakName[0] = g_gameAppView->m_sLobbyHighBreakName[0];
		HighBreakName[1] = g_gameAppView->m_sLobbyHighBreakName[1];
		HighBreakName[2] = g_gameAppView->m_sLobbyHighBreakName[2];
		}
	else if(g_gameAppView->m_sLobbyPHighBreak[0]>0) // Pro or 9-Ball 
		{
		HighBreak[0] = g_gameAppView->m_sLobbyPHighBreak[0];
		HighBreak[1] = g_gameAppView->m_sLobbyPHighBreak[1];
		HighBreak[2] = g_gameAppView->m_sLobbyPHighBreak[2];
		HighBreakName[0] = g_gameAppView->m_sLobbyPHighBreakName[0];
		HighBreakName[1] = g_gameAppView->m_sLobbyPHighBreakName[1];
		HighBreakName[2] = g_gameAppView->m_sLobbyPHighBreakName[2];
		}
	else if(g_gameAppView->m_sLobbyUKHighBreak[0]>0) // Billiards or English Pool 
		{
		HighBreak[0] = g_gameAppView->m_sLobbyUKHighBreak[0];
		HighBreak[1] = g_gameAppView->m_sLobbyUKHighBreak[1];
		HighBreak[2] = g_gameAppView->m_sLobbyUKHighBreak[2];
		HighBreakName[0] = g_gameAppView->m_sLobbyUKHighBreakName[0];
		HighBreakName[1] = g_gameAppView->m_sLobbyUKHighBreakName[1];
		HighBreakName[2] = g_gameAppView->m_sLobbyUKHighBreakName[2];
		}
	

	if(HighBreak[0] > 0) // only if top break is > 0...
	{
		char buf[200];
		
		FILE *fptr = NULL;
		fptr = fopen("TourneyHighBreaks.txt","a");


		///////////////////////////////////////////////////////
		CTime _time = CTime::GetCurrentTime();
		int _ExpDay = _time.GetDay();
		int _ExpMonth = _time.GetMonth();
		int _ExpYear = _time.GetYear()-2000;
//		if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [score]%d\n", _ExpDay, _ExpMonth, _ExpYear, g_gameAppView->m_sLobbyHighBreakName[0].GetBuffer(0), g_gameAppView->m_sLobbyHighBreak[0]);
		//if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [score]%d\n", _ExpDay, _ExpMonth, _ExpYear, GAMEAPPVIEW->m_sLobbyHighBreakName[0].GetBuffer(0), GAMEAPPVIEW->m_sLobbyHighBreak[0]);
		//if(fptr) fclose(fptr);
		 ///////////////////////////////////////////////////////



		#ifdef THE_POOL_CLUB
		//////////////////////
		if((HighBreak[0]==HighBreak[1]) && (HighBreak[0]==HighBreak[2]) ) // top 3 runouts are the same, so split prize in 3
			{	
			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' most tournament runouts of %d)", HighBreakName[0].GetBuffer(0) , m_highB/3, (m_highBCash/3)/100, (m_highBCash/3)%100, HighBreak[0]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[0].GetBuffer(0), m_highB/3, m_highBCash/3);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [runouts]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[0].GetBuffer(0), HighBreak[0]);

			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' most tournament runouts of %d)", HighBreakName[1].GetBuffer(0) , m_highB/3, (m_highBCash/3)/100, (m_highBCash/3)%100, HighBreak[1]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[1].GetBuffer(0), m_highB/3, m_highBCash/3);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [runouts]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[1].GetBuffer(0), HighBreak[1]);

			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' most tournament runouts of %d)", HighBreakName[2].GetBuffer(0) , m_highB/3, (m_highBCash/3)/100, (m_highBCash/3)%100, HighBreak[2]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[2].GetBuffer(0), m_highB/3, m_highBCash/3);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [runouts]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[2].GetBuffer(0), HighBreak[2]);
		}
		else if(HighBreak[0] == HighBreak[1])
		{	// top 2 runouts are the same, so split prize in 2
			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' most tournament runouts of %d)", HighBreakName[0].GetBuffer(0) , m_highB/2, (m_highBCash/2)/100, (m_highBCash/2)%100, HighBreak[0]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[0].GetBuffer(0), m_highB/2, m_highBCash/2);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [runouts]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[0].GetBuffer(0), HighBreak[0]);

			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' most tournament runouts of %d)", HighBreakName[1].GetBuffer(0) , m_highB/2, (m_highBCash/2)/100, (m_highBCash/2)%100, HighBreak[1]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[1].GetBuffer(0), m_highB/2, m_highBCash/2);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [runouts]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[1].GetBuffer(0), HighBreak[1]);
		}
		else
		{	// give prize to single top runout player (normal case)						
			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the most tournament runouts of %d)", HighBreakName[0].GetBuffer(0) , m_highB, m_highBCash/100, m_highBCash%100, HighBreak[0]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[0].GetBuffer(0), m_highB, m_highBCash);
			
			// append new high-score to file...			
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [runouts]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[0].GetBuffer(0), HighBreak[0]);
		}
		/////////////////////
		#else
		/////////////////////
		// snookerclub...

	
		if((HighBreak[0] == HighBreak[1]) && (HighBreak[0] == HighBreak[2]))			
		{	// top 3 breaks are the same, so split prize in 3
			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' highest tournament break of %d)", HighBreakName[0].GetBuffer(0) , m_highB/3, (m_highBCash/3)/100, (m_highBCash/3)%100, HighBreak[0]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[0].GetBuffer(0), m_highB/3, m_highBCash/3);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [break]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[0].GetBuffer(0), HighBreak[0]);

			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' highest tournament break of %d)", HighBreakName[1].GetBuffer(0) , m_highB/3, (m_highBCash/3)/100, (m_highBCash/3)%100, HighBreak[1]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[1].GetBuffer(0), m_highB/3, m_highBCash/3);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [break]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[1].GetBuffer(0), HighBreak[1]);

			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' highest tournament break of %d)", HighBreakName[2].GetBuffer(0) , m_highB/3, (m_highBCash/3)/100, (m_highBCash/3)%100, HighBreak[2]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[2].GetBuffer(0), m_highB/3, m_highBCash/3);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [break]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[2].GetBuffer(0), HighBreak[2]);
		}
		else if(HighBreak[0] == HighBreak[1])
		{	// top 2 breaks are the same, so split prize in 2
			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' highest tournament break of %d)", HighBreakName[0].GetBuffer(0) , m_highB/2, (m_highBCash/2)/100, (m_highBCash/2)%100, HighBreak[0]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[0].GetBuffer(0), m_highB/2, m_highBCash/2);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [break]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[0].GetBuffer(0), HighBreak[0]);

			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the 'equal' highest tournament break of %d)", HighBreakName[1].GetBuffer(0) , m_highB/2, (m_highBCash/2)/100, (m_highBCash/2)%100, HighBreak[1]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[1].GetBuffer(0), m_highB/2, m_highBCash/2);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [break]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[1].GetBuffer(0), HighBreak[1]);
		}
		else 
		{	// give prize to single top break player (normal case)		
			sprintf(buf, "('%s' is awarded %d pts, $%ld.%ld vCash for the highest tournament break of %d)", HighBreakName[0].GetBuffer(0) , m_highB, m_highBCash/100, m_highBCash%100, HighBreak[0]);
			g_gameAppView->SendTextMsg(buf, 1, -1, -1);
			g_gameAppView->m_lobby->ServerGivePlayerPts(HighBreakName[0].GetBuffer(0), m_highB, m_highBCash);
			if(fptr) fprintf(fptr, "[dd/mm/yy]%d/%d/%d  [name]%s  [break]%d\n", _ExpDay, _ExpMonth, _ExpYear, HighBreakName[0].GetBuffer(0), HighBreak[0]);
		}
		/////////////////////
		#endif

		if(fptr) fclose(fptr);

	}		
	
	
}





unsigned char encrypt_tbl[] = {6,3,1,9,5,7,8,3,2,6};

CString ENCRYPT_STR(CString pass)
{
int i, idx, len;
unsigned char result[128];

idx=0;
len = strlen(pass);
for(i=0;i<len;i++)
	{
	result[i] = pass[i] + (encrypt_tbl[idx] + len);
	idx++;
	if(idx==10) idx=0;
	}
result[strlen(pass)] = 0;
return CString(result);
}


CString DECRYPT_STR(CString pass)
{
int i, idx, len;
unsigned char result[128];

idx=0;
len = strlen(pass);
for(i=0;i<len;i++)
	{
	result[i] = pass[i] - (encrypt_tbl[idx] + len);
	idx++;
	if(idx==10) idx=0;
	}
result[strlen(pass)] = 0;
return CString(result);
}

/*
class CIgnoreLst {
public:
char **m_name;
int m_total;
CIgnoreLst(int max);
~CIgnoreLst();
CIgnoreLst AddName(char *name);
CIgnoreLst CheckName(char *name);
CIgnoreLst DeleteName(char *name);
};
*/


CIgnoreLst::CIgnoreLst(int max)
{
	
	int n;
	m_max = max;
	m_total = 0;
	m_name = new char *[m_max];
	
	for(n=0;n<m_max;n++) 
		{
		m_name[n] = new char[22];
		m_name[n][0] = 0;
		}


}

CIgnoreLst::~CIgnoreLst()
{
	int n;
	for(n=0;n<m_max;n++)
	if(m_name[n]) delete m_name[n];
}


void CIgnoreLst::Load(char *fname)		// fname is always the name of the player + ilst.txt
{
	// Check if there's any ignorelst.txt files to load //
	FILE *fptr;
	char str[128];
	
	// Update our member filename //
	// char test[80];
	strcpy(m_fname, fname);
	
	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, m_fname);

	if(fptr = fopen(fname_full, "a+b") )
		{
		while(fscanf(fptr, "%s", &str) != EOF)
			{
			m_name[m_total] = new char[strlen(str)+1];
			strcpy(m_name[m_total], str);
			m_total++;
			if(m_total >= 128) break;
			}
		fclose(fptr);
		}
}


void CIgnoreLst::Save(void)
{
	
	// Check if there's any ignorelst.txt files to load //
	FILE *fptr;
	int n;
	char str[128];
	
	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, m_fname);

	if(fptr = fopen(fname_full, "w"))
		{
		for(n=0;n<m_total;n++)
			{
			// Save the ignore names with carriage returns at the end of each //
			if(m_name[n]) fprintf(fptr, "%s\r\n", m_name[n]);		
			}
		fclose(fptr);
		}
}



void CIgnoreLst::AddName(char *name)
{
	if(m_total >= 64) return;
	
	int n;
	for(n=0;n<m_max;n++)
	if(m_name[n][0] == 0)
		{
		m_name[n] = new char[strlen(name)+1];
		strcpy(m_name[n], name);
		m_total++;
		break;
		}
}

void CIgnoreLst::DeleteName(char *name)
{
	int n;
	for(n=0;n<m_total;n++)
	if(!strcmpi(name, m_name[n]))
		{
		m_name[n][0] = 0;
		m_total--;
		break;
		}

}


BOOL CIgnoreLst::CheckName(char *name)
{
	int n;
	for(n=0;n<m_total;n++)
	if(!strcmpi(name, m_name[n]))
		{
		// Found this name in our Ignore List so return TRUE;
		return TRUE;
		}
	// We didn't find this name in our Ignore List so return FALSE //	
	return FALSE;
}

/*
// Get a new copy of Advertisement File from server //
// This tells us which rooms will have what type of Advertisements (on the Table and Around the Room) //
BOOL CLobby::GetAdvertisementsFile(void)
{



}
*/



/*
	// Only Upload to Website if the break is 100 or more //
	if(ftp)
	{
		CInternetSession sess(_T("MyProgram/1.0"));
		CFtpConnection* pConnect = NULL;
		pConnect = sess.GetFtpConnection("ftp.isnooker.net", "isnooker.net", "s21678", 21, TRUE);
		if(pConnect->PutFile(str, fname, FTP_TRANSFER_TYPE_BINARY)) ;//AfxMessageBox("Great Success2!");
		// if the connection is open, close it
		if (pConnect != NULL)
		  pConnect->Close();
		delete pConnect;
	}

*/