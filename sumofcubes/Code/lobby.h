// Lobby.h
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __LOBBY_H__
#define __LOBBY_H__

#include "Common.h"
#include <array>



typedef struct {
	char name[32];
	int ladderPoints;
	int ladderRank;
	int ladderHighBreak;
	int ladderWinsLosses;
} ladderObject;


#define PLRSTATE_AVAILABLE		0x00
#define PLRSTATE_BUSY			0x01
#define PLRSTATE_BRB			0x02	// Be right back.
#define PLRSTATE_AWAY			0x03
#define PLRSTATE_PHONE			0x04
#define PLRSTATE_LUNCH			0x05
#define PLRSTATE_DND			0x06	// Do not disturb.
#define PLRSTATE_AWAY2			0x07	// special - for idle time longer than 5 mins (temp away)
#define PLRSTATE_STATUSMASK		0x0f

#define PLRSTATE_MODERATOR		0x10
#define PLRSTATE_ADMIN			0x20
#define PLRSTATE_LEADMODERATOR	0x30

#define PLRSTATE_AUTHMASK		0x30

#define PLRSTATE_GUESTMEMBER	0x40
#define PLRSTATE_EXTMASK		0xc0

#define PLRSTATE_MASK			0xff

#define PLRSTATE2_MUTE			0x01
#define PLRSTATE2_STEALTH		0x02

#define PLRSTATE2_LOGTIME		0x20	// If this is set, then whenever the user logs in or out, 
										// the time is record ing the isnk_memberslogtime table //
										// Note: Moderators & Admins have their times logged always //

///////
#define PLAYERCOPY_SYSTEM	0x01
#define PLAYERCOPY_RATINGS	0x02
#define PLAYERCOPY_BREAKS	0x04
#define PLAYERCOPY_GAMES	0x08
#define PLAYERCOPY_LEVEL	0x10
#define PLAYERCOPY_LADDER	0x20


#define PLAYERCOPY_STATS	(PLAYERCOPY_RATINGS | PLAYERCOPY_BREAKS | PLAYERCOPY_GAMES | PLAYERCOPY_LEVEL | PLAYERCOPY_LADDER)
///////

#define EPD_PERSONALLAYOUT		0

// textFormat of player
#define TEXTFORMAT_DEFAULT		0x00
#define TEXTFORMAT_ITALIC		0x01
//#define TEXTFORMAT_UNDERLINE	0x02
#define TEXTFORMAT_BOLD			0x02
#define TEXTFORMAT_COMIC		0x04

// 04 can still be used

// Colors //
#define TEXTFORMAT_COLOR		0xf8		// 32 Colors Mask
#define TEXTFORMAT_NCOLOR		(0xf8^0xff)	// 8 Colors Not Mask (lower 3 bits 0x07)

#define TEXTFORMAT_NORMAL		0x00		// like default

#define TEXTFORMAT_BLUE			0x10
#define TEXTFORMAT_PEACH		0x20
#define TEXTFORMAT_PINK			0x30
#define TEXTFORMAT_MUD			0x40
#define TEXTFORMAT_RED			0x50
#define TEXTFORMAT_YELLOW		0x60
#define TEXTFORMAT_LTBLUE		0x70

#define TEXTFORMAT_GREEN		0x80
#define TEXTFORMAT_TAN			0x90
#define TEXTFORMAT_JADE			0xa0
#define TEXTFORMAT_ROSE			0xb0
#define TEXTFORMAT_SOFTGREEN	0xc0
#define TEXTFORMAT_WHITE		0xd0

#define TEXTFORMAT_ABLUE		0x08
#define TEXTFORMAT_APURPLE		0x18
#define TEXTFORMAT_AORANGE		0x28
#define TEXTFORMAT_ATEAL		0x38
#define TEXTFORMAT_MRED			0x48
#define TEXTFORMAT_MBLUE		0x58
#define TEXTFORMAT_MYELLOW		0x68
#define TEXTFORMAT_MGREEN		0x78
#define TEXTFORMAT_MGRAY		0x88


char *tfmt_color_lst[];
DWORD tfmt_colorrgb_lsta[];
DWORD tfmt_colorrgb_lstb[];
DWORD tfmt_colorrgbadjust_lst[];

//#define TEXTFORMAT_BLUE		0x04
//#define TEXTFORMAT_GREEN		0x08
//#define TEXTFORMAT_ORANGE		0x10

typedef struct CheatAlert_typ {
	char buffer[4];
	char process[128];
} CheatAlert, *CheatAlert_ptr;
	


typedef struct RequestFile_typ {
	char buffer[4];
	char fname[64];
} RequestFile, *RequestFile_ptr;
	

typedef struct ReceiveFile_typ {
	char buffer[4];
	char fname[64];
	int fsize;
	char data[65535];
} ReceiveFile, *ReceiveFile_ptr;
	

typedef struct TextColorChanged_typ {
	char buffer[4];
	int textcolor;
} TextColorChanged, *TextColorChanged_ptr;
	

char *g_status_strlst[];


typedef struct HistoryData_typ {// 
	int history_id[10];			// 40 Bytes
	int time[10];			// 40 Bytes
	char winner[10][22];	// 220 Bytes		// We only keep the first 16 Characters of Usernames, to save Bandwidth //
	char loser[10][22];		// 220 Bytes
	int gameinfo[10];		// 40 Bytes
} HistoryData, *HistoryData_Ptr; // 520 bytes 

typedef struct HistoryReportData_typ {// 
	char buffer[4];		// 4 Bytes
	int time;			// 4 Bytes
	char myself[22];	// 20 Bytes 
	char opponent[22];	// 20 Bytes
	int gameinfo;		// 4 Bytes
	int update_hid;			// 4 Bytes
} HistoryReportData, *HistoryReportData_Ptr; // 72 bytes

typedef struct Player2_typ { // Used for initial connection only [as it's a minimal sized packet]
	char name[22];			// uses
	char password[20];		// uses
	int playerPort;			// uses
	char playerLocalIP[20];	// uses
} Player2, *Player2_Ptr; // 74 bytes






typedef struct Player_typ {
	char status;	// on/off 0,1
	int idx;		// idx in server player list
	char name[22];
	char password[20];
	int playerPort;
	char playerIP[20];
	char playerLocalIP[20];
	int playerSocketIdx;
	DWORD sysID;
	DWORD macID_U;
	WORD macID_L;
	WORD cpuID;
	short rating; // short...
	short wins;
	short losses;
	short streak;
	short reracks;

	short _p50s;		// also Used for 8-Ball Runouts on iPool STD
	short _p60s;		// also Used for 9-
	short _p70s;
	short _p80s;
	short _p90s;
	short _p100s;
	short pHighBreak;
	short pMax147s;
	//////////////
	short _50s;
	short _60s;
	short _70s;
	short _80s;
	short _90s;
	short _100s;
	short HighBreak;
	short Max147s; // short ^
	
	float pBioAverageBreak;
	float pBioAverageShotTime;
	float pBioAveragePosition;
	float pBioAverageSafety;
	//////////////
	float bioAverageBreak;
	float bioAverageShotTime;
	float bioAveragePosition;
	float bioAverageSafety;

	int tournamentRating;
	int winner;
	int runnerUp;
	int semiFinals;
	int quarterFinals;
	int last16;
	int last32;
		
	int table;					// -1 = lobby, 0..255 tables (-128 to +127)
	int getAllPlayers;			// Server use only (-1 = inactive, 0 - n = sending of player details)
	int getAllTables;			// Server use only (-1 = inactive, 0 - n = sending of table details)
	BYTE state;					// Available = 0, AFK/Eating etc.
	char type;
	unsigned char ExpDay;		// 1 - 31
	unsigned char ExpMonth;		// 1 - 12
	unsigned char ExpYear;		// Relative to year 2000 i.e. range 2000 to 2255
	DWORD id;					// unique id (use for sending to players)
	enum { TYPE_ARRIVED, TYPE_SIM, TYPE_SIMEND, MAX_TYPES };
	BOOL bInvited;
	BOOL bIsGuest;
	BOOL bCanHost;
	BOOL bCanHostKnown;			// defaultly you don't know!
	BOOL bIgnore;	
	DWORD textFormat;
	DWORD state2;
	DWORD trialsysid;
	WORD  trialcpuid;
	DWORD trialmacid_u;
	WORD  trialmacid_l;
	DWORD inventory;			// Bits 0-11 are used for Text Colours that are purchased, Bits 16-24 are used for Cue Types that are purchased
	int highestrating;
	int cash;					// stores Virtual Cash
	DWORD cuetype;				// Holds the Current Cue
	char playertype;			// 0=Admin, 1=SeniorMod, 2=Mod2, 3=Mod, 4=Members, 5=Trialist, 6=Guest 
	char ssysid[16];			// Current System ID + CPU ID (String Format)
	char smacid[16];			// Current MAC ID (String Format)
	char sip[16];				// Current IP Address (String Format)
	char control;				// XXXX8421  - 1: Banned, 2: Walled, 3: Denied, 4: Blocked
	char lobbytablewatch;		// This ranges from 1-50, and tells us which table we are watching. If this is value of 0, then we're not watching any table
	int bandata;
	int level;
	int level_progress;
	int textColor;
	int chalktype;				// Hold the Current Chalk Type
	int activated;
	int searchgamestatus;		// Bit 0: Search Game State, Bit 1: League Match State, Bit 2: Pro Match Status
	long searchgametime;		// Time that we started searching for a Game
	char searchgametableinvite;	// Table to Invite to
	char searchgameseatinvite;	// Seat to sit at
	int ladderPoints;
	int ladderWinsLosses;		// Top 16 bits for Wins, Lower 16 Bits for Losses
	int ladderHighBreak;		// Top 16 bits for pro High Break, Lower 16 bits for Semi Pro high break
	int ladderRank;
	int highstreak;
	int realcash;				// Stores real money..

} Player, *Player_Ptr;

typedef struct MuteList_typ {
	char name[22];
} MuteList, *MuteList_Ptr;

class CExtendedPlayerData {
public:
CExtendedPlayerData *m_next; // linked list.
// Data contained in struct.
char *m_data;
int m_data_n;
DWORD m_tag;
public:
CExtendedPlayerData();
~CExtendedPlayerData();
int AddPackage(void *pData, int size, DWORD tag, int ref=0); // returns how nested the data was added.
void *GetPackageData(DWORD tag);
CExtendedPlayerData *GetPackage(DWORD tag);
int GetNumberOfPackages(int ref=0);	// returns how nested this is, from this point in the chain.
};

#define VIA_MAINSERVER	0
#define VIA_GAMESERVER	1
#define VIA_LOCALHOST	2

class CLobby;

class CExtendedPlayerManager {
public:
CExtendedPlayerData *m_sendpackage;
DWORD m_offset;
int m_mode;
DWORD m_targetid;
DWORD m_sourceid;
CLobby *m_lobbyParent;
public:
CExtendedPlayerManager(CLobby *m_parent);
~CExtendedPlayerManager();

BOOL SendPackage(CExtendedPlayerData *ptr, DWORD tag, char *name, int mode = VIA_MAINSERVER);
BOOL SendPackage(CExtendedPlayerData *ptr, DWORD tag, DWORD targetid, DWORD sourceid, int mode = VIA_MAINSERVER);
void ReceivePackage(void *pData);
void Process(void);
};


// (EPD - ExtendedPlayerData)
typedef struct EPDPersonalLayout_typ {
int avatar_idx; // -1 = custom avatar, (n) = generic avatar
} EPDPersonalLayout, *EPDPersonalLayout_Ptr;

typedef struct Details_typ {
CString m_name;
CString m_password;
CString m_email;
CString m_town;
CString m_comments;
CString	m_realName;
CString	m_sex;
CString	m_age;
CString m_version;
CString m_bpass;
} Details, *Details_Ptr;

typedef struct Table_typ {
	char status;	// on/off 0,1
	int idx;		// idx in server table list
	int hostSockIdx;
	int player1SockIdx;
	int player2SockIdx;
	int player1bSockIdx;
	int player2bSockIdx;
	
	int P1StartAtTable;
	int P2StartAtTable;
	int P1bStartAtTable;
	int P2bStartAtTable;
	BOOL bGameStarted;
	BOOL bReRackRequest;
	BOOL bSwitchBreakTo1P;
	BYTE racksWon1P;
	BYTE racksWon2P;

	// *TODO - lobby frames scores
	int startScore1P;
	int startScore2P;
	int score1P;
	int score2P;
	int break1P;
	int break2P;
	//BOOL bTurn1P;
	//int bestScore;

	//BOOL cueError; ?? POTENTIAL BUG: should be int (3/2/2004 OAP)
	int cueError; // ^
	BOOL bRated;
	char type;
	enum { TYPE_PUBLIC, TYPE_PROTECTED, TYPE_PRIVATE , MAX_TYPES };
	BOOL bMissRule; // or callShot

	BYTE shotTime;
	BYTE tableColour;
	BYTE gameType;
	BYTE addTimer;
	BYTE raceTo;
	BYTE whoBreaks;
	BYTE ballSize;
	BYTE rackType;
	BYTE tableType;
	BYTE proTable;
	BYTE doubles;

	BOOL bHostComplete;

	int tournament;	// >= 1 (true, if this is a tournament table)

	//BOOL bSimEndMsgBalls;	// Server use only (FALSE => MsgBall's for current strike HAST NOT! been received yet by SERVER)
	//						//                 (TRUE => MsgBall's for current strike HAS! been received yet by SERVER)
	BYTE nSimEndMsgBalls;	// sendStrike-sendBalls tag marker, so that server can disregard old sendballs msgs - BUGFIX for playing shot twice

	BOOL bLockSeats;
	BOOL bLadderMatch;

	char P1StartName[22];		// These 4 names take 4,000 BYTES
	char P2StartName[22];
	char P1bStartName[22];
	char P2bStartName[22];
	
	BOOL bCanRerack;	// We will only allow 1 Re-rack per game //

	BYTE playersSockIdxN;		// This is important,and will tell us how much of playersSockIdx to Send for MSGGAME_TABLEDETAILS
	int playersSockIdx[MAX_TABLE_PLAYERS];

	int m_lostConn1P;	// Tracks how many times Player 1 has disconnected in the Frame
	int m_lostConn2P;	// Tracks how many times Player 2 has disconnected in the Frame

} Table, *Table_ptr;





extern char TableLostConnectionFlag[50];	// 0: ok, 1: Waiting for a Player to Rejoin Table //


#define MODTYPE_NONE			-1	// Anyone who ain't on the moderator list.
#define MODTYPE_ADMIN			0	// 
#define MODTYPE_LEADMODERATOR	1	// *
#define MODTYPE_SMODERATOR		2	// ^
#define MODTYPE_MODERATOR		3	// +


class CModerator {
public:
char **m_name_lst;
BYTE *m_type_lst;
int m_name_lst_n;
int m_name_lst_idx;
public:
CModerator(int max);
~CModerator();
void Reset(void);
BOOL AddModerator(char *_str);
int CheckName(char *str);
int CheckNameSQL(char *str);
int GetType(char *str);
int GetType(int idx);
void LoadModerators(char *fname);
void SaveModerators(char *fname);
BOOL DeleteModerator(char *str);
};

#define MCODE_UNDEFINED		0
#define MCODE_SPECIAL		1	// Special account make using special membership code password
#define MCODE_1YEAR			2
#define MCODE_6MONTHS		3
#define MCODE_3MONTHS		4
#define MCODE_1MONTH		5
#define MCODE_TRIAL			6
#define MCODE_GUESTMEMBER	7
#define MCODE_TRIAL2		8
	
class CSerialCode {
char **m_code_lst;
int m_code_lst_n;
int m_code_lst_idx;
char *m_serialsAdded;
int m_serialsAdded_n;
char *m_serialsAdded_ptr;
public:
CSerialCode(int max);
~CSerialCode();
void Reset(void);
void AddSerial(char *str);
int SerialCodecmpi(char *str, char *code);
char *GetSerialType(char *str);
char *GetSerialTypeFromIdx(int idx);
int FindSerial(char *str);
void DeleteSerialIdx(int idx);
void ShowAll(void);
void SaveSerials(char *fname);
void LoadSerials(char *fname);
void AddSerialsToFile(FILE *dptr, char *fname, int amount = 0, char *codetype = NULL);
void AddSerialsToFileNEG(FILE *dptr, char *fname, int amount = 0, char *codetype = NULL);
void MoveSerials(char *fname, char *addfname, char *codetype, int amount = 0);
void AddSerialsToFileDAT(FILE *dptr, char *fname, int amount, char *codetype = NULL);
char *MoveSerialsDAT(char *fname, char *addfname, char *codetype, int amount = 0);
};

class CIPConnectCoolDown {
char **m_pList;
int m_pList_n;
CTime *m_pListLast;
int *m_pListSecs;
public:
CIPConnectCoolDown();
~CIPConnectCoolDown();
void CleanUp(void);
int Check(char *ip, int secs = 2); // returns heat of socket (-1 is ok)
};

class CIPCoolDown {
char **m_ip;
int m_ip_n;
int *m_ip_cnt; // number of times this ip has been used (once it reaches 3, cool down comes into effect)
int *m_ip_coolDown;
int m_limit;
public:
CIPCoolDown(int max, int limit);
~CIPCoolDown();
void Process(void); // Performed every 30 mins
BOOL CheckIP(char *str);
void AddIP(char *str);
};


class CServerPlayerStats {
public:
char name[22];
BYTE runOuts8ballStd;
BYTE runOuts9ballStd;
BYTE runOutsEnglishStd;
BYTE runOuts8ballPro;
BYTE runOuts9ballPro;
BYTE runOutsEnglishPro;
BYTE runOuts8ballUK;
BYTE runOuts9ballUK;
BYTE runOutsEnglishUK;


void Clear(void)
{
	name[0] = 0;
	runOuts8ballStd = 0;
	runOuts9ballStd = 0;
	runOutsEnglishStd = 0;
	runOuts8ballPro = 0;
	runOuts9ballPro = 0;
	runOutsEnglishPro = 0;	
	runOuts8ballUK = 0;
	runOuts9ballUK = 0;
	runOutsEnglishUK = 0;
}

};

class CServerAllPlayersStats {
public:
CServerPlayerStats *m_plrList;
int m_plrList_n;
int m_plrList_idx;
CServerAllPlayersStats(int max);
~CServerAllPlayersStats();
CServerPlayerStats AddPlrStats(CServerPlayerStats plr);
CServerPlayerStats GetPlrStats(char name[22]);
void DeleteAll(void);
};


class CServerPlayerBreak {
public:
char name[22];
BYTE highbreak;

void Clear(void)
	{
	name[0] = 0;
	highbreak = 0;
	}
};

class CServerAllPlayersBreaks {
public:
CServerPlayerBreak *m_plrList;
int m_plrList_n;
//int m_plrList_idx;
CServerAllPlayersBreaks(int max);
~CServerAllPlayersBreaks();
CServerPlayerBreak AddPlrBreak(CServerPlayerBreak plr);
CServerPlayerBreak GetPlrBreak(char name[22]);
void OrderBreaks(void);
void UpdateHTML(char *fname);
void DeleteAll(void);
};


class CIgnoreLst {
public:
char **m_name;
int m_total;
int m_max;
char m_fname[80];
CIgnoreLst(int max);
~CIgnoreLst();
void Save(void);
void Load(char *fname);
void AddName(char *name);
BOOL CheckName(char *name);
void DeleteName(char *name);
};


class CTMatch {
public:
	int m_status;
	BOOL m_bLadderMatch;
	BOOL m_bNewMatch;
	BOOL m_bMatchOver;
	BOOL m_bMatchConceded;
	int m_matchOverCount;
	int m_table;
	int m_RaceTo;
	char m_name1P[22];
	char m_name2P[22];
	int m_fp1;
	int m_fp2;
	int m_wpts;
	int m_lpts;
	int m_wcash;
	int m_lcash;
	int m_fee1;
	int m_fee2;
	int m_score1;
	int m_score2;
	int m_round;
	int m_waccext;
	int m_laccext;
	int m_penalty;
	//char m_sponsor[256];
	int m_timer1P;
	int m_timer2P;
	BOOL m_bStopTiming;
	int m_lostConn1P;
	int m_lostConn2P;
	BOOL m_bTheFinal;
	int m_tid;		// This is the unique tourney identifier that Tourney System uses 
	int m_tourneyidx;		// This is the Index of the Tournament we are running (0-15 value)
	void Clear(void);
	void Add(char *stid, char *sRaceTo, char *name1P, char *name2P, char *sfp1, char *sfp2, char *swpts, char *slpts, char *swcash, char *slcash, char *sfee1, char *sfee2, char *sscore1, char *sscore2, char *sround, char *sWACCEXT, char *sLACCEXT, char *sPen, int idx, bool bLadderMatch);
	void NotSeatedInvite(bool bAccepted);
};

#define MAX_TOURNEYS			50
#define MAX_TMATCHES			100		// should be up to 32 matches on 32 tables, if we're only doing last 64 players
#define MATCH_TIMETOSITSTART	5*60	// 5mins
#define MATCH_TIMETOCLEARTABLE	3*60	// 3mins
#define LADDERMATCH_TIMETOCLEARTABLE	1*60	// 1min
#define LADDERMATCH_TIMETOSITSTART		3*60	// 3min

class CTournament {
public:
	char m_name[256];						// name of tournament
	char m_desc[1024];						// tournament description
	Table m_table[MAX_TOURNEYS];			// tournament table options
	CTMatch m_match[MAX_TMATCHES];			// list of tournament matches
	int m_autoInviteCounter;
	int m_autoInviteGroup;
	BOOL m_bFinalMatch;

	int m_highB;
	int m_highBCash;
	int m_doubles;

	

	CTournament(void);
	~CTournament();

	void AddName(char *name, int idx);
	void AddDesc(char *desc, int idx);
	void AddSponsor(char *sponsor, int idx);
	void AddSponsorURL(char *sponsorURL, int idx);
	void AddOptionsLevel(char *sLevel, int idx);
	void AddOptionsGameType(char *sGameType, int idx);
	void AddOptionsWhoBreaks(char *sWhoBreaks, int idx);
	void AddOptionsTableColour(char *sTableColour, int idx);
	void AddOptionsTableType(char *sTableType, int idx);
	void AddOptionsBallSize(char *sBallSize, int idx);
	void AddOptionsRackType(char *sRackType, int idx);
	void AddOptionsAddTimer(char *sTimer, int idx);
	void AddOptionsProTable(char *sProTable, int idx);
	void AddOptionsDoubles(char *sDoubles, int idx);
	void AddOptionsShotTime(char *sShotTime, int idx);
	void AddOptionsRated(char *sRated, int idx);
	void AddOptionsCallShots(char *sCallShots, int idx);
	void AddOptionsMissRule(char *sMissRule, int idx);
	void AddOptionsHighB(char *sHighB, int idx);
	void AddOptionsHighBCash(char *sHighB, int idx);
	void AddMatch(char *stid, char *sRaceTo, char *name1P, char *name2P, char *sfp1, char *sfp2, char *swpts, char *slpts, char *swcash, char *slcash, char *sfee1, char *sfee2, char *sscore1, char *sscore2, char *sround, char *sWACCEXT, char *sLACCEXT, char *sPen, char *sGameType, int idx, bool bLadderMatch);
	bool InMatch(char *name);
	void FinalMatchProcess(int n);
	BOOL bIsEmptyTable(int tbl);
	void ClearMatchTableNo(int tbl);
	void ClearMatchTable(int tbl);
	BOOL bIsTableUsed(int tbl);
	int GetMatchFromTableNo(int tbl);
	void ProcessAutoInvites(void);
	void ProcessMatchTimers(void);
	void ProcessMatchOvers(void);
	void DoBroadcastMatchTimers(int matchIdx, int type);
	void DoBroadcastMatchOvers(int matchIdx, int type);
	void CheckGiveHighBreakPrize(void);
};


extern Player m_player[];
extern Player m_myself;
extern Player m_opponent;
extern Table m_table[];
extern Details m_details;
extern CSerialCode *g_serialcode;
extern char *g_mcode_type_lst[];
extern char *g_db_type_lst[];
extern MuteList m_mutelist[];

extern CExtendedPlayerData *g_extendedPlayerData[];
extern CExtendedPlayerData *g_extendedPlayerDataLocal;
extern CExtendedPlayerManager *g_extendedPlayerManagerLocal;


class CLobby
{
public:
	CLobby(CWnd *pParent); // constructor
	~CLobby(); // destructor

	CWnd *m_parent;
	
	BOOL m_bInLobby;
	BOOL m_bLobbyReady;
	BOOL m_bInTable;
	int m_PLCwait;
	char m_PLCname[22];
	
	// window screen size width / height
	int m_cx;
	int m_cy;

	BOOL m_bDeclineAllGuests;
	BOOL m_bDeclineAll;

	// HUD
	//CPicture m_pictureBackground;
		
	void Init(void);
	void Deinit(void);
	void Process(void);
	void AddMute(char *name);
	void RemoveMute(char *name);
	BOOL IsMuted(char *name);
	void CheckTableHogger(void);

	void ProcessFindMatchSetup(void);
	void ProcessFindMatchInvites(void);

	void LostConnectionAutoInvites(void);
	void LostConnectionMessageCantSit(void);
	BOOL LostConnectionReturned(BOOL bSendReturnMsg = FALSE);
	void LostConnectionWaitMore(void);
	void LostConnectionGameOver(void);
	void MaxTimeUpWaitMore(void);
	void MaxTimeUpGameOver(void);
	void LobbyJoinMessage(int plrSockIdx);
	BOOL GetAdvertisementsFile(void);
	void ServerDataReceive(BOOL bTournament, BOOL bOriginalVersion, char handle[22]);
	void LobbyReady(void);
	void Draw(void);

	void ReceiveTMatchTimerLeft(int timeRemaining, int type, char *str, int plrSockIdx, int tblIdx);
	void ReceiveTextMsg(char *str, int type, int plrSockIdx, int tblIdx, int lobbyAndTblIdx);

	void SetupLocalEPD(void); // Avatars/Personal Details/homepage url/etc. etc.
	void ReRouteEPD(unsigned char *buffer, int plrSockIdx); // Servers reroute EPD messages.
	void ReceiveEPD(unsigned char *buffer);

	BOOL LoggedInAlready(char *plrname);
	BOOL LoggedInAlreadySYSIDMACID(char *name, DWORD sysID, char *ip, DWORD macID_U, DWORD macID_L);
	
	void HostSimEndInit(void);
	void HostSimStartReceive(int playerSockIdx);
	void HostSimEndReceive(int playerSockIdx);
	BOOL HostSimEndComplete(void);
	void SimEndWaiting(void);
	
	void ClearPlayer(int idx);
	void ClearPlayer(Player *pPlr);
	void ClearPlayerNonRating(Player *pPlr);
	void AddPlayerNonRating(Player *pDPlr, Player *pSPlr);
	void AddPlayer(Player *plr);
	void AddPlayerData(int n, Player *plr);
	void RemovePlayer(int playerSocketIdx);
	int GetPlayerIdx(int playerSocketIdx);
	int FindPlayerIdxFromId(DWORD id);
	char *FindPlayerNameFromId(DWORD id);

	int FindPlayerIdx(CString name);
	int FindPlayerSockIdx(CString name);
	int FindPlayerSockIdxi(CString name); // non case sensi version.

	int FindPlayerSockIdx_Ignore(CString name, int IgnoreIdx);
	int FindPlayerSockIdxi_Ignore(CString name, int IgnoreIdx); // non case sensi version.
	
	int GetPlayerLevel(int idx);
	int GetPlayerLevelProgress(int idx);
	void SetPlayerLevel(int idx, int level);
	void SetPlayerLevelProgress(int idx, int level_progress);


	int GetPlayerRating(int idx);
	int GetPlayerSeat(int plrSockIdx);
	void ProcessPlayers(void);
	int CountPlayers(void);
	
	void ClearTable(int idx);
	void ClearTableReceive(int tblIdx);
	int CountPlayersAtTable(int idx);
	BOOL EmptyTable(int idx);
	void TakeTable(int idx);
	BOOL SeatedAtTable(int plrSockIdx);
	BOOL SeatedAtTableIdx(int plrSockIdx, int tblIdx);
	BOOL PressedStartAtTable(int plrSockIdx);
	BOOL BothSeatsTaken(int plrSockIdx);
	BOOL PressedStartReceive(int plrSockIdx);
	BOOL PlayingAGame(int plrSockIdx);
	BOOL IsPlayerWinner(int plrSockIdx);
	void ReceivePlayerLostConnection(int playerSockIdx, char name[22], int tblIdx);
	void ReceivePlayerConnection(int playerSockIdx, int gameOver, int gameOverIWon, int playerSockIdx2);
	void ReceivePlayerTextFormat(int playerSockIdx, char name[22], int textFormat);
	BOOL PressedConcedeReceive(int plrSockIdx, BOOL bAndExit = FALSE, int tblIdx = -1);
	void PressedReRackReceive(int plrSockIdx, BOOL bAbortRaceTo);
	void ReRackAccpetedReceive(int plrSockIdx, int acceptType, BOOL bAbortRaceTo);
	void EndGameReceive(int tblIdx);
	void TableClearReRackRequest(int plrSockIdx);
	void PressedInviteReceive(int plrSockIdx, int invitedPlrSockIdx, char *hostName, int initialHostTbl, int table, int enterType, int reracks = 0, int totalGames = 0);
	void InvitationAcceptedReceive(int plrSockIdx, int invitedPlrSockIdx, char *str, int initialHostTbl, int table, int enterType);
	void UpdateMyselfOpponent(int tblIdx);
	void PressedBootReceive(int plrSockIdx, int bootedPlrSockIdx);
	void GamePingReceive(int plrSockIdx);
	BOOL GameStarted(int plrSockIdx);
	void PlayerWin(Player *pPlr);
	void PlayerLose(Player *pPlr);
	void PlayerDraw(Player *pPlr);

	void PlayerResultLadder(Player *pPlr, bool bWin, int highBreak, bool bPro);
	

	void CalcNewELORatings(int gameOver, Player *pPlr1, Player *pPlr2);	
	void CalcNewELORatingsLobby(int gameOver, Player *pPlr1, Player *pPlr2);	
	void CalcNewELORatingsTourney(int gameOver, Player *pPlr1, Player *pPlr2);	
	void CalcNewELOLadderPoints(int gameOver, int highBreakP1, int highBreakP2, bool bPro, Player *pPlr1, Player *pPlr2, bool bLadderMatch);
	
	//void CalcNewELORatings(int gameOver, Player *pPlr1, Player *pPlr2, Player *pPlr1b, Player *pPlr2b);	
	//void CalcNewELORatingsLobby(int gameOver, Player *pPlr1, Player *pPlr2, Player *pPlr1b, Player *pPlr2b);	
	//void CalcNewELORatingsTourney(int gameOver, Player *pPlr1, Player *pPlr2, Player *pPlr1b, Player *pPlr2b);

	void GetHighBreak(Player *pPlr, Player *pPlrOpponent, int cueError, int highBreak, int high4, int tblIdx);
	BOOL HighBreakReceive(int plrSockIdx, int highBreak, int cueError, int high4 = -1);	
	void WinnerDoGameOver(int plrSockIdx, int gameOver, int highBreaks1P0, int highBreaks1P1, int highBreaks2P0, int highBreaks2P1, BOOL bInvalidGame, BOOL bRacksStillToPlay, BOOL IWon);
	void GameOverSaveFrame(char *name1P, char *name2P, BOOL ftp = FALSE);
	void GameOverForAllReceive(int tblIdx, int gameOver, BOOL bForceRacksEnd = FALSE, BOOL bMatchTimeUp = FALSE);
	void ServerGivePlayerPts(char *name, int pts, int cash, int round = -1, bool bAnnouce = true, int ladderPts = 0);
	void GameOverReceive(Player *pPlr1, Player *pPlr2, BOOL bPlr1Won, BOOL bRated, BOOL bInvalidGame, int tblIdx, int oldRating1P, int oldRating2P, BOOL bRacksStillToPlay, int EOGmsg = 0);
	void TournamentMatchTimeUp(int tblIdx, BOOL bTimeUp1P, BOOL bTimeUp2P);
	int GetTableStartMessageForPlayer(int plrSockIdx);
	void TableHostCompleteReceive(int tblIdx);
	BOOL JoinTableHostComplete(int tblIdx);
	void JoinTable(int idx, int enterType, int playerSockIdx, BOOL bIsTableHost);
	void ReJoinTable(int idx);
	void JoinTableFailed(int joinerSockIdx, int hostSockIdx);
	BOOL TableIsFull(int tblIdx, int enterType);
	BOOL PlayerCanHost(int plrSockIdx);
	void ExitTable(int playerSockIdx, BOOL bTimedOutBoot = FALSE, BOOL bPLC = FALSE);
	void SitStandTable(int seat1SockIdx, int seat2SockIdx, int tableIdx, int seat1bSockIdx, int seat2bSockIdx);
	void AddTable(Table *pTbl);
	void RefreshTableSeats(int idx);
	void RefreshTableHostDetails(int idx);
	void UpdateTableHostDetails(int cueError, BOOL bRated, char type, BOOL bMissRule, int playerSockIdx,
								BYTE shotTime, BYTE tableColour, BYTE gameType, BYTE addTimer, BYTE doubles, 
								BYTE raceTo, BYTE whoBreaks,
								BYTE ballSize, BYTE rackType, BYTE tableType, BYTE proTable,
								int tournamentTable = -1, int fp1 = 0, int fp2 = 0, int score1 = 0, int score2 = 0, bool bLadderMatch = false);
	int GetPlayerIdxFromTable(int tableIdx, int enterType, int watcher = -1);
	int GetHostIdxFromTable(int tableIdx);
	BOOL HostOfTable(int plrSockIdx);
	int GetTableIdxFromPlayer(int plrSockIdx);
	void ProcessTables(void);

	void UpdateTableItemButtons(int nItem, bool bLadderMatch = false);
	void AddDoublesButtons(char state);
	void AddTableItem(int nItem, int enterType);
	CString GetTableOptionsItemStr(int nItem);
	void AddTableOptionsItem(int nItem);
	void ClearTableItem(int nItem);
	void ClearAllTableItems(void);
	BOOL DeleteTableItem(int nItem);
	BOOL DeleteAllTableItems(void);
	BOOL IsTableWILD(int tblIdx);

	void AddPlayerItem(CString name, int rating, int tableNo, int level);
	BOOL IsPlayerInGroup(int plrIdx, int group);
	void SortPlayerRatings(void);
	void SortPlayerTables(void);
	void SortPlayerNames(void);
	void RefreshPlayerItem(void);
	void CLobby::UpdatePlayerItem(CString name);
	BOOL RemovePlayerItem(CString name);
	BOOL RemoveAllPlayerItems(void);
};


int TimeToExpire(Player *pPlr);
int PlayerFileExists(char *fname);

void TextChanged(char *name, int textformat, int textcolor);
void CueChanged(char *name, int cuetype);
void ChalkChanged(char *name, int chalktype);

void UpdateSysMacIpEx(char *name, char *sysid, char *macid, char *ip);

void BanPlayerFileEx(char *name);
void UnBanPlayerFileEx(char *name);

int CheckWalledEx(char *sysid);
void WallSysIDEx(char *sysid);
void UnWallSysIDEx(char *sysid);

int CheckDeniedEx(char *macid);
void DenyMacIDEx(char *macid);
void UnDenyMacIDEx(char *macid);

int CheckBlockedEx(char *ip);
void BlockIPEx(char *ip);
void UnBlockIPEx(char *ip);



BOOL CheckPasswordEx(char *name, char *pass);

int GetLadderRank(char *name, int ladderPoints);
std::array<ladderObject, 20> GetLadderRankings();
void ResetLadderRankings();

void NewAccountLogEx(char *name, char *atype, char *mcode, char *email, char *date);
int GetMemberIDEx(char *name);
int GetPlayerType(char *name);

void ModifyMember(char *fname);
void ModifyMember2(char *fname);
void ModifyAllMembers(void);
int LoadPlayerFileExFT(char *fname, Player *plr, char *filetype);
int LoadPlayerFileEx(char *fname, Player *plr);
int LoadPlayerFile(char *fname, Player *plr);
void InitPlayer(Player *pPlr);
void NewAccountPlayer(Player *pPlr, int type = 0);
void TopUpAccountPlayerDays(Player *pPlr, int days = 0, BOOL bSet = FALSE);
void TopUpAccountPlayer(Player *pPlr, int type = 0);
void SavePlayerFileExFT(char *fname, Player *plr, char *filetype);
void SavePlayerFileEx(char *fname, Player *plr);
void SavePlayerFile(char *fname, Player *plr);
void LoadDetailsFile(char *fname, Details *dtls);
void SaveDetailsFile(char *fname, Details *dtls);
int MembershipCodeValid(char *mCode);
int CrcCodeValid(DWORD crc);
void AddLog(char *fname, char *str);
void AddSubLog(char *fname, char *str);
void SendToQueueTable(CComm *pComm, void *lpBuf, int nBufLen, int tblIdx = -1);
void SendToQueueTableX(CComm *pComm, void *lpBuf, int nBufLen, int tblIdx = -1);
void SendToQueueTableExceptOne(CComm *pComm, void *lpBuf, int nBufLen, int tblIdx, int plrIdx);
void UpdateCash(char *name, int cash);


typedef struct {
char email[256];
char order_no[256];
} SerialStruct_typ;

extern SerialStruct_typ g_serialStruct;

// [CSysList]

class CSysList {
public:
DWORD *m_list_u;
WORD *m_list_l;
int m_list_n;
DWORD m_list_idx;
char m_fname[80];
CSysList(int max, char *fname);
~CSysList();
void Blank(void);
BOOL Load(void); // loads a 256k mash
BOOL Save(void); // saves a 256k mash
void AddID(DWORD id, WORD cpuid); // has a m_list_n long memory.
BOOL CheckID(DWORD id, WORD cpuid); // has a m_list_n long memory.
BOOL SYSmacLog(char *name, char *pass, DWORD sysid, WORD cpuid, DWORD mac_u, WORD mac_l);
};

class CMACList {
public:
DWORD *m_maclist_u;
WORD *m_maclist_l;
int m_maclist_n;
DWORD m_maclist_idx;
char m_fname[80];
CMACList(int max, char *fname);
~CMACList();
void Blank(void);
BOOL Load(void); // loads a 256k mash
BOOL Save(void); // saves a 256k mash
void AddMAC(DWORD mac_u, WORD mac_l); // has a m_list_n long memory.
BOOL CheckMAC(DWORD mac_u, WORD mac_l); // has a m_list_n long memory.
BOOL MACsysLog(char *name, char *pass, DWORD mac_u, WORD mac_l, DWORD sysid, WORD cpuid);
};

void TOMYSQLAllMembers(void);
void TOMYSQLMember(char *fname);

extern BOOL g_sql_banned;
extern BOOL g_sql_trial;



CString ENCRYPT_STR(CString pass);
CString DECRYPT_STR(CString pass);

#endif // __LOBBY_H__
