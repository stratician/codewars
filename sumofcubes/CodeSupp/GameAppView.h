// GameAppView.h : interface of the CGameAppView class	
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __GAMEAPPVIEW_H__
#define __GAMEAPPVIEW_H__

// for OnTimer(... calls
#define GAMEAPP_FPS						50.0f//60.0f // for Main Server App.
#define GAMEAPPCLIENT_FPS				16.0f		 // for Client App.	   

//#include "Picture.h"

#include "AboutDlg.h"
#include "DetailsDlg.h"
#include "DetailsSettingsDlg.h"
#include "LoginWizardDlg.h"
#include "statsdlg.h"
#include "TableTabCtrlDlg.h"
#include "StatsTabCtrlDlg.h"
#include "statsChatdlg.h"
#include "statsdlg2.h"
#include "PersonalDetailsDlg.h"
#include "TournamentsDlg.h"
// #include "ShopDlg.h"
#include "concededlg.h"
#include "ReRackDlg.h"
#include "ReRackAccept2Dlg.h"
#include "InviteDlg.h"
#include "InvitationDlg.h"
#include "ExitDlg.h"
#include "BootDlg.h"
#include "ReplayDlg.h"
#include "AlertDlg.h"
#include "NewAccountDlg.h"
#include "ServerConfigDlg.h"
#include "AccountConfigDlg.h"
#include "PlayerListDlg.h"
#include "PracticeEditDlg.h"
#include "TableSetupDlg.h"
#include "TableSetupDlg2.h"
#include "ForfeitDlg.h"
#include "NotePadDlg.h"
#include "GuestRegisterDlg.h"
#include "LobbySelect.h"
#include "ButtonOwnerEx.h"
#include "BitmapEx.h"
#include "SplitterWndEx.h"
#include "EditEx.h"
#include "ListBoxEx.h"
#include "ButtonEx.h"
#include "ListCtrlEx.h"
#include "TabCtrlEx.h"
#include "SendEdit.h"
#include "SmileySelectDlg.h"
#include "TextColorChangerDlg.h"
#include "winthreadex.h"
#include "GameAppDoc.h"
#include "MyButton.h"
#include "Achievements.h"
#include "LadderRankings.h"
#include "MySliderCtrl.h"

#include "Replay.h"
#include "../TermsDlg.h"
#include "../NewsDlg.h"

#include <dwmapi.h>

#include "../Code/common.h"
#include "../Code/Snooker.h"
#include "../Code/Lobby.h"
#include "../Code/Bot.h"

#include "../Codedx9/Camera.h"
#include "../Codedx9/advert.h"


#include "../Codedx9/font.h"
#include "Level.h"
#include "CueChanger.h"
#include "FineTuner.h"
#include "ChalkChanger.h"

#include "Sprite.h"
#include "LobbyButtons.h"
#include "FindMatchCtrl.h"
#include "StatsWindow.h"
#include <vector>
#include <ctime>


#define MAX_PROFILENAMECHARS	16 // TODO: [+]

#define MAX_LOBBYCHATLINES		1000 // # of lines of chat text for lobby - then clears all text 

#define TAG_MUSIC			0		// Perform Operation to ALL SFX
#define TAG_SFX				1

#define MAX_INVITATIONDLGS	8

#define MAX_MENU_ITEMS		11

#define BUTTON_START		0
#define BUTTON_RERACK		1
#define BUTTON_CONCEDE		2
#define BUTTON_EXIT			3
#define BUTTON_UNDO			4
#define BUTTON_INVITE		5
#define BUTTON_BOOT			6
#define BUTTON_SETUP		7
#define BUTTON_REPORT		8
#define BUTTON_STANDUP		9
#define MAX_BUTTONS			10
extern char *g_button_name[MAX_BUTTONS];

#define EL_BALLS			1
#define EL_REST				2


extern IDirect3DSurface9* pRenderSurface;
extern IDirect3DSurface9* pRenderSurfaceMiniCam;
extern IDirect3DSurface9* pRenderSurfaceShop;


extern char *g_RecentMessengerName;

extern int g_iDesktopWidth;
extern int g_iDesktopHeight;

extern int g_iMaxWidthWS;
extern int g_iMaxHeightWS;

extern Camera* g_pCamera;
extern Camera* g_pCameraMiniCam;
extern Camera* g_pCameraFree;

extern Advert **g_pAdvert;
extern float FPS;
extern int g_iCanHandleIt;

extern int g_pCameraMode;		// 0: Game Mode (Default), 1: Camera Free Mode, lets us pan around ourselves 
extern bool g_bCameraModeJustChanged;

extern DWORD g_dwBackBufferWidth;
extern DWORD g_dwBackBufferHeight;
extern DWORD g_dwChatHeight;

extern DWORD g_dwMainViewWidth;
extern DWORD g_dwMainViewHeight;

					
extern HBITMAP smiley_empty;
extern char *smiley_lst[33];


extern float g_fProbe;
extern ID3DXMesh* Sphere;

extern float g_fPower;

typedef struct smiley_data{
int smiley_spos;
int	smiley_epos;
int smiley_offset;
int smiley_type;
int smiley_adjust;
} smiley_data_typ;
	
extern smiley_data_typ smiley_data;

extern char *gstr;
extern void LOAD_SKIN(void);
extern void LOAD_SMILEYS(void);
extern void LOAD_ABUSE(void);
extern BOOL GET_SMILEYS(smiley_data_typ *smiley_data, int level);
extern BOOL GET_ABUSE(smiley_data_typ *smiley_data);


extern HWND g_hwndDX;			// just a global for hwnd
extern HWND g_hwndDX2;			// just a global for hwnd
extern HWND g_hwndDXStats;		// gloabl for hwnd for stats

extern HBITMAP Skin_ButtonSayOn;
extern HBITMAP Skin_ButtonSayOff;
extern HBITMAP Skin_ButtonSmiley;
extern HBITMAP Skin_ButtonSmiley2;

extern HBITMAP g_playerlistbgbmp;

#define MAX_SMILIES			32
extern HBITMAP smiley[MAX_SMILIES+1];
//extern IRichEditOle	*m_pRichEditOle;

// SFX defines
enum {
SFX_INTRO = 0,
SFX_CUE2BALL,
SFX_BALL2BALL,
SFX_BALL2POCKET,
//SFX_COUGH1,
//SFX_CUE2BALLSOFT,
//SFX_BALL2BALLSOFT,
//SFX_BALL2POCKETMIDDLE45,
//SFX_TAP,
SFX_20PLUSBREAK,
SFX_CENTURY,
SFX_GOODSHOT,
SFX_BALLAROUNDPOCKET,
SFX_MAX,
};

#define ASYNCSOCK_TYPE_MAIN					0 // for Client
#define ASYNCSOCK_TYPE_GAME					1 // for Client
#define ASYNCSOCK_TYPE_GAME_REJOIN			2 // for Client
#define ASYNCSOCK_TYPE_CHECK				3
#define ASYNCSOCK_TYPE_MAINSERVER			4 // for Server
#define ASYNCSOCK_TYPE_GAMESERVER			5 // for Server
#define ASYNCSOCK_TYPE_GAMESERVER_REJOIN	6 // for Server

#define ASYNCSOCK_TYPE_CHECKLISTEN	12

#define PLAYER_INVALID		0
#define PLAYER_EXISTS		1
#define PLAYER_BANNED		2
#define PLAYER_WALLED		4
#define PLAYER_DENIED		8
#define PLAYER_BLOCKED		16


// Menu 'WideScreen'
#define WIDESCREEN_MENU						11

#define WIDESCREEN_DISABLED					0
#define WIDESCREEN_ENABLED					1



// Menu 'File'
#define FILE_MENU							0

#define FILE_LOADFRAMES						0
#define FILE_SAVEFRAMES						1
#define FILE_AUTOSAVEFRAMES					2

// Menu 'Edit'
#define EDIT_MENU							1

#define EDIT_CUT							0
#define EDIT_COPY							1
#define EDIT_PASTE							2
#define EDIT_SELECTALL						3

// Menu 'Game'
#define GAME_MENU							2

#define GAME_START							0
#define GAME_RERACK							1
#define GAME_CONCEDE						2
#define GAME_STANDUP						3
#define GAME_INVITE							4
#define GAME_BOOT							5
#define GAME_SETUP							6
#define GAME_UNDO							7
#define GAME_EXIT							8

#define GAME_PLAYERLIST						10
#define GAME_INVITATIONS					11

// Menu 'Game->Sound'
#define GAME_INVITATIONS_DECLINEALLGUESTS	0
#define GAME_INVITATIONS_DECLINEALL			1

// Menu 'Players'
#define PLAYERS_MENU					5

#define PLAYERS_1PVSCPU					0
#define PLAYERS_CPUVS1P					1
#define PLAYERS_1PVS2P					2
#define PLAYERS_CPUVSCPU				3

#define PLAYERS_CPUSTRENGTH				5

// Menu 'Practice'
#define PRACTICE_MENU					6

#define PRACTICE_STARTINGPOSITION		0
#define PRACTICE_LINEUP					1
#define PRACTICE_COLOURS				2
#define PRACTICE_RESPOTTEDBLACK			3
#define PRACTICE_REDSSCATTERED1			4
#define PRACTICE_REDSSCATTERED2			5
#define PRACTICE_BILLIARDS				6

#define PRACTICE_BALLINHANDALWAYS		8

#define PRACTICE_EDITPOSITION			10

#define PRACTICE_LOAD					12
#define PRACTICE_SAVE					13

// POOL equivalents
#define POOL_PRACTICE_8BALLSETUP		0
#define POOL_PRACTICE_9BALLSETUP		1
#define POOL_PRACTICE_8BALLSCATTERED	2
#define POOL_PRACTICE_9BALLSCATTERED	3
#define POOL_PRACTICE_ROTATIONBALL		4
#define POOL_PRACTICE_BOWLS				5
#define POOL_PRACTICE_6BALLSETUP		6
#define POOL_PRACTICE_6BALLSCATTERED	7
#define POOL_PRACTICE_10BALLSETUP		8
#define POOL_PRACTICE_10BALLSCATTERED	9
#define POOL_PRACTICE_BREAKSETUP		10
#define POOL_PRACTICE_ENGLISHPOOLSETUP	11

#define POOL_PRACTICE_NORMALTABLE		12
#define POOL_PRACTICE_PROTABLE			13
#define POOL_PRACTICE_UKTABLE			14

#define POOL_PRACTICE_BALLINHANDALWAYS	13

#define POOL_PRACTICE_EDITPOSITION		14

#define POOL_PRACTICE_LOAD				16
#define POOL_PRACTICE_SAVE				17


// Menu 'Options'
#define OPTIONS_MENU					3

#define OPTIONS_HIGH_PERFORMANCE		0
#define OPTIONS_SET_REFRESH_RATE		1
#define OPTIONS_LOW_GRAPHICS			2
#define OPTIONS_MEDIUM_GRAPHICS			3
#define OPTIONS_HIGH_GRAPHICS			4
#define OPTIONS_MOUSE_SENSITIVITY		5
#define OPTIONS_SHOWFPS					6

#define OPTIONS_2D_MODE					8
#define OPTIONS_2D3D_MODE				9
#define OPTIONS_2DTVAUTO_MODE			10
#define OPTIONS_2D3DFREE_MODE			11
//#define OPTIONS_TVM_MODE				12


#define OPTIONS_SHOW_MINICAM			14
#define OPTIONS_COMPLEX_SHADOWS			15
#define OPTIONS_COLOUR_BLIND			16
#define OPTIONS_SOUND					17
#define OPTIONS_DISPLAY_FRAME_RATE		18
#define OPTIONS_DRAW_EYE_CAM_CUE		19
#define OPTIONS_DRAW_COLOUR_SL			20
#define OPTIONS_DRAW_ERROR_SL			21
#define OPTIONS_DRAW_GHOST_CUE_BALL		22
#define OPTIONS_DRAW_CUEBALL_LE			23
#define OPTIONS_DRAW_STATIC_POWER		24

#define OPTIONS_REPLAY_PANEL			26

#define OPTIONS_SHOW_LOGIN_WIZARD		28
#define OPTIONS_DEFAULT_SCHEME			29



// #define OPTIONS_DEBUG_MODE			31

#define OPTIONS_DOTTEDCUEBALL			31
#define OPTIONS_SHOW_LOBBYTV			32
#define OPTIONS_MINICAM_SHOWHELPERS		33
#define OPTIONS_MINICAM_LOCKMOUSE		34


// Menu 'Options->Set Refresh Rate'
#define OPTIONS_SET_REFRESH_RATE_DEFAULT	0
#define OPTIONS_SET_REFRESH_RATE_25			1
#define OPTIONS_SET_REFRESH_RATE_30			2
#define OPTIONS_SET_REFRESH_RATE_50			3
#define OPTIONS_SET_REFRESH_RATE_60			4
#define OPTIONS_SET_REFRESH_RATE_70			5
#define OPTIONS_SET_REFRESH_RATE_72			6
#define OPTIONS_SET_REFRESH_RATE_75			7
#define OPTIONS_SET_REFRESH_RATE_85			8
#define OPTIONS_SET_REFRESH_RATE_100		9
#define OPTIONS_SET_REFRESH_RATE_120		10
#define OPTIONS_SET_REFRESH_RATE_140		11
#define OPTIONS_SET_REFRESH_RATE_150		12

// Menu 'Options->Sound'
#define OPTIONS_SOUND_BALL_SFX			0
#define OPTIONS_SOUND_CLAPPING_SFX		1
#define OPTIONS_SOUND_VOICES			2

// Menu 'Options->Draw Eye-Cam cue'
#define OPTIONS_DRAW_EYE_CAM_CUE_TIP			0
#define OPTIONS_DRAW_EYE_CAM_CUE_TRANSPARENT	1
#define OPTIONS_DRAW_EYE_CAM_CUE_SOLID			2

// Menu 'Options->Debug Mode (Don't Use!)
#define OPTIONS_DEBUG_MODE_ACTIVATE				0
#define OPTIONS_DEBUG_MODE_DEACTIVATE			1

// PlayerChange Codes //
#define PLAYERCHANGE_STATUS			0
#define PLAYERCHANGE_RATING			1
#define PLAYERCHANGE_RATINGANDROUND	2
#define PLAYERCHANGE_STATUS2		3
#define PLAYERCHANGE_CUE			4
#define PLAYERCHANGE_BIO			5		// Just need to update pBioAverageBreak & BioAverageBreak
#define PLAYERCHANGE_VCASHCUE		6
#define PLAYERCHANGE_CHALK			7
#define PLAYERCHANGE_VCASHCHALK		8
#define PLAYERCHANGE_REALCASH		9

// TableChange Codes //
#define TABLECHANGE_SPONSOR			0

extern BOOL g_laddersave;
extern BOOL g_leader;
extern BOOL g_freeaccounts;
extern BOOL g_disabletriallock;
extern BOOL g_guestonly;
extern BOOL g_membersonly;
extern BOOL g_memberstrialsonly;
extern BOOL g_tourney_system;
extern BOOL g_sponsoron;
extern BOOL g_sponsorback;
extern BOOL g_defensemac;
extern BOOL g_defensesys;
extern BOOL g_conceded_game;
extern DWORD g_concede_time;
extern BOOL g_ftpupload;

extern float g_GoldThreshold8Ball;
extern float g_GoldThreshold9Ball;
extern float g_GoldThresholdEng;

extern float g_GoldDecay8Ball;
extern float g_GoldDecay9Ball;
extern float g_GoldDecayEng;


#define SERVERSENDCONFIG_FLAGS_SEERT		1

typedef struct ServerSendConfig_typ {
	char buffer[4];
	DWORD flags;
} ServerSendConfig, *ServerSendConfig_ptr;

extern ServerSendConfig g_serverConfig;
extern BOOL g_bOriginalVersion;
extern BOOL g_bTournamentLobby;
extern BOOL g_hacked;
extern char g_tabSelected;
extern BOOL g_bLastTurn;
extern float g_ACamAngle;
extern HistoryData g_history_data;


extern CString g_serverHandle;

extern CString g_lobbyMessageBoard;
extern CString g_lobbyMessageBoard2;
extern CString g_lobbyTournament;
extern CString g_lobbyLeague;
extern CString g_lobbyRegister;

extern CString g_URLlobbyLeaderBoardA;
extern CString g_URLlobbyLeaderBoardB;
extern CString g_FNamelobbyLeaderBoardA;
extern CString g_FNamelobbyLeaderBoardB;

extern CString g_URLlobbyTickerBoard;

#ifdef THE_POOL_CLUB // iPool
extern CString g_URLBanner;
extern CString g_URLMessageBoard;
extern CString g_URLTournament;
extern CString g_URLBanner2;
extern CString g_URLMessageBoard2;
#else // iSnooker
extern CString g_URLBanner;
extern CString g_URLMessageBoard;
extern CString g_URLTournament;
extern CString g_URLBanner2;
extern CString g_URLMessageBoard2;
#endif

// User did not click the Mouse //
// Used to make SmileyDialog remain open during the game //
extern BOOL g_UserMouseClick; 
extern BOOL g_WideScreenEnabled;
extern BOOL g_RememberPassword;
extern BOOL g_Menu_Refresh;
extern BOOL g_WindowsVista;

typedef struct gfxDetails_typ {
BOOL bHighPerformance;
int optionsSetRefreshRate;
BOOL bOptionsHighPerformance;
BOOL bOptionsLowgfx;
BOOL bOptionsMediumgfx;
BOOL bOptionsHighgfx;
BOOL bOptionsAntiAliasing;
BOOL bOptionsWireframe;
BOOL bFrameRateDisplay;
BOOL bOptions2DMode;
BOOL bOptions2D3DMode;
BOOL bOptions2DTVAutoMode;
BOOL bOptions2D3DFreeMode;
BOOL bOptionsShowMiniCam;
BOOL bOptionsColourBlind;
BOOL bOptionsDottedCueBall;
BOOL bOptionsMiniCamShowHelpers;
BOOL bOptionsMiniCamLockMouse;
BOOL bOptionsComplexShadows;
BOOL bSoundBalls;
BOOL bSoundClapping;
BOOL bSoundVoices;
BOOL bEyeCamDrawCueTip;
BOOL bEyeCamDrawCueTransparent;
BOOL bEyeCamDrawCueSolid;
BOOL bShowLoginWizard;
BOOL bOptionsDrawColourSL;
BOOL bOptionsDrawErrorSL;
BOOL bOptionsDrawGhostCueBall;
BOOL bOptionsDrawCueBallLE;
BOOL bOptionsDrawStaticPower;
int iOptionsSensitivity;


// extra vars - not related to 'Options' menu

//-----//
// screen coords.
int x;
int y;
int cx;
int cy;
//-----//
// 'Game' -> 'Invitations'
BOOL bDeclineAllGuests;
BOOL bDeclineAll;
// 'File' -> 'Auto Save Frames'
BOOL bAutoSaveFrames;
BOOL bDefaultColorScheme;
BOOL bShowShotInLobbyTV;
} gfxDetails, *gfxDetails_ptr;

extern BOOL g_bWireframeChanged;
extern BOOL g_bAntiAliasChanged;
extern BOOL g_bPerformanceChanged;

extern gfxDetails g_gfxDetails;
extern BOOL LoadGfxDetails(void);
extern BOOL SaveGfxDetails(void);

typedef struct setupDetails_typ {
BOOL bGfxQuality;
char appNameToExecute[128];
} setupDetails, *setupDetails_ptr;

extern setupDetails g_setupDetails;
extern BOOL LoadSetupDetails(void);


// Menu 'Practice ' settings
typedef struct practiceDetails_typ {
BOOL bOptionsBallInHandAlways;
} practiceDetails, *practiceDetails_ptr;
extern practiceDetails g_practiceDetails;


typedef struct LobbyTVInfo_typ{
char lobbytablewatch;
char name1P[22];
char name2P[22];
int score1P;
int score2P;
int break1P;
int break2P;
char bTurn1P;
BOOL bShowShot;
} LobbyTVInfo, *LobbyTVInfo_ptr;
extern LobbyTVInfo g_LobbyTVInfo;

typedef struct LobbyWatchSendData_typ{
char buffer[4]; // general send data
char tblIdx;
int plrSockIdx;
} LobbyWatchSendData, *LobbyWatchSendData_ptr;



extern DWORD g_id_count;

#define CONNECTSTYLE_NORMAL		0
#define CONNECTSTYLE_NEWACC		1
#define CONNECTSTYLE_TOPUP		2


typedef struct AbuseLstSendData_typ {
	char buffer[4];
	int abuse_lst_n;
	char abuse_lst[64][16];			// 64 Words, maximum length 16 characters each 1024 BYTES + 4 BYTES
} AbuseLstSendData, *AbuseLstSendData_ptr;
	
extern AbuseLstSendData g_AbuseLstData;

typedef struct serverTimeSendData_typ {
	char buffer[4];
	int year;
	char month;
	char day;
	char hour;
	char minute;
	char second;
	char g_GoldThreshold8Ball, g_GoldThreshold9Ball, g_GoldThresholdEng;
	char g_GoldDecay8Ball, g_GoldDecay9Ball, g_GoldDecayEng;

} serverTimeSendData, *serverTimeSendData_ptr;

typedef struct ServerSendData_typ {
	char buffer[4];
	BOOL m_bTournament;
	BOOL m_bOriginalVersion;
	char m_handle[22];
} ServerSendData, *ServerSendData_ptr;

typedef struct playerDetailsSendDataConnect2_typ { // More efficient connection
	char buffer[4];
	Player2 m_player2;
	DWORD m_crc;
	char m_versionstr[10];
	DWORD m_sysID;
	WORD m_cpuID;
	DWORD m_macID_U;
	WORD  m_macID_L;
	
} playerDetailsSendDataConnect2, *playerDetailsSendDataConnect2_ptr;

// client-server lobby-send-data
typedef struct playerDetailsSendData_typ {
	char buffer[4];
	Player m_player;
	HistoryData m_history;	// 400 Bytes
} playerDetailsSendData, *playerDetailsSendData_ptr;

typedef struct playerDetailsSendDataC_typ {
	BYTE buffer[4]; // 3rd byte is for state, 4th byte is a mash of flags
	short rating; // short...	
	//////////////
	// for isnooker (GOLD NAME)
	short Max147s;
	short pMax147s;
	short _100s;
	short _p100s;
	// for ipool (GOLD NAME)
	short _50s;
	short _60s;
	short _70s;

	short _80s;
	short _90s;
	short _p50s;

	short _p60s;
	short _p70s;
	short _p80s;

	//////////////
	char playerSocketIdx;
	char table; // -1 = lobby, 0..255 tables (-128 to +127)	
	//BYTE state; // Available = 0, AFK/Eating etc.	
	DWORD id;				// unique id (use for sending to players)	
	//BOOL bInvited;
	//BOOL bIsGuest;
	char name[22]; // 12 bytes + strlen(name) + 1
	BYTE textFormat;
	DWORD state2;
	DWORD cuetype; // For cue type 
	char playertype;	// 0=Admin, 1=SeniorMod, 2=Mod2, 3=Mod, 4=Members, 5=Trialist, 6=Guest 
	int bandata;
	float BioAverageBreak;	// Pot Average for iPool .. used to tell use when we display the name in Gold (Player on Fire)
	float pBioAverageBreak;	// Pro Pot Average for iPool .. used to tell use when we display the name in Gold (Player on Fire)
	int level;
	int level_progress;
	int textColor;
	int cash;
	int chalktype;
	int ladderPoints;
	int ladderWinsLosses;
	int ladderHighBreak;
} playerDetailsSendDataC, *playerDetailsSendDataC_ptr;


typedef struct tableDetailsSendData_typ {
	char buffer[4];
	Table m_table;	// This version of Table is optimized for sending - less data
	/*
	// We will have to be smart here //
	// playersSockIdxN tells us how many playerSockIdx's we are sending //
	BYTE playersSockIdxN;
	int *playersSockIdx;
	*/
	
} tableDetailsSendData, *tableDetailsSendData_ptr;

typedef struct playerJoinSendData_typ {
	char buffer[4];
	int m_tableIdx;
	int m_enterType;
	int m_playerSockIdx;
	BOOL m_bIsTableHost;
	DWORD m_tableseatsA; // Required for Doubles Seating: 0-15 Bits 1SockIdx, 15-31 Bits 1bSockIdx
	DWORD m_tableseatsB; // Required for Doubles Seating: 0-15 Bits 2SockIdx, 15-31 Bits 2bSockIdx
} playerJoinSendData, *playerJoinSendData_ptr;

typedef struct playerReJoinSendData_typ {
	char buffer[4];
	int m_tableIdx;
} playerReJoinSendData, *playerReJoinSendData_ptr;


typedef struct playerJoinFailedSendData_typ {
	char buffer[4];
	int m_joinerSockIdx;
	int m_hostSockIdx;
} playerJoinFailedSendData, *playerJoinFailedSendData_ptr;


typedef struct playerExitSendData_typ {
	char buffer[4];
	int m_playerSockIdx;
	BOOL m_bTimedOutBoot;
	BOOL m_bPLC;
} playerExitSendData, *playerExitSendData_ptr;

typedef struct playerSitSendData_typ {
	char buffer[4];
	int m_seat1SockIdx;
	int m_seat2SockIdx;
	int m_tableIdx;
	int m_seat1bSockIdx;
	int m_seat2bSockIdx;
} playerSitSendData, *playerSitSendData_ptr;

typedef struct tableHostDetailsSendData_typ {
	char buffer[4];
	//BOOL m_bCueError;
	int m_cueError;
	BOOL m_bRated;
	char m_type;
	BOOL m_bMissRule;
	
	// table setup vars.
	BYTE m_shotTime;
	BYTE m_tableColour;
	BYTE m_gameType;
	BYTE m_addTimer;
	BYTE m_raceTo;
	BYTE m_whoBreaks;
	BYTE m_ballSize;
	BYTE m_rackType;
	BYTE m_tableType;
	BYTE m_proTable;
	BYTE m_doubles;

	int m_playerSockIdx;
	int m_tournamentTable;
	int m_fp1;
	int m_fp2;
	int m_score1;
	int m_score2;
	bool m_bLadderMatch;
} tableHostDetailsSendData, *tableHostDetailsSendData_ptr;

typedef struct newAccountSendData_typ {
	char buffer[4];
	char m_name[22];
	char m_password[20];
	char m_mCode[20];
	char m_email[64];
	char m_mReferer[22];
	char m_mHearAbout[80];
	DWORD m_sysID;
	WORD m_cpuID;
	DWORD m_macID_U;
	WORD  m_macID_L;
	char MultiTrialist;
} newAccountSendData, *newAccountSendData_Ptr;
  



// client-server game-send-data
typedef struct strikeBallSendData_typ {
char buffer[4]; // general send data
M3d_vtx m_sPoint;// specific send data
M3d_vtx m_tableMousePos;
M3d_vtx m_cuetip;
int m_selectBall;
float m_strikeRnd;
float m_strikeMaxRnd;

BOOL m_ballMouseOn;
M3d_vtx m_ballMousePos;
int m_nominateBall;

int m_testBallColour;

BYTE m_nSimEndMsgBalls;

DWORD m_crc;
} strikeBallSendData, *strikeBallSendData_ptr;

typedef struct ballPosSendData_typ {
char buffer[4]; // general send data
CVector m_P;// specific send data
int m_selectBall;
} ballPosSendData, *ballPosSendData_ptr;

typedef struct selectBallSendData_typ {
char buffer[4]; // general send data
int m_cueBallSelect;// specific send data
CPoint m_point;
int w;
int h;
int m_selectBall;
} selectBallSendData, *selectBallSendData_ptr;

typedef struct mouseMoveSendData_typ {
char buffer[4]; // general send data
float x;
float y;
int w;
int h;
float strikeLine;
float strikeDist;
} mouseMoveSendData, *mouseMoveSendData_ptr;

typedef struct CueBallTipSendData_typ {
char buffer[4]; // general send data
float m_ex;
float m_ey;
float m_ez;
float m_sx;
float m_sy;
float m_sz;
BOOL m_ballMouseOn;
M3d_vtx m_ballMousePos;
M3d_vtx m_cueTip;
int m_nominateBall;
int m_RaiseButt;
int m_RaiseButtOld;
float m_cueAngle;
} CueBallTipSendData, *CueBallTipData_ptr;

typedef struct CueBallTipSendDataC_typ {
BYTE buffer[4]; // general send data
BYTE selectBall;
BYTE power;
//BYTE angle;
WORD angle;
BYTE raise;
} CueBallTipSendDataC, *CueBallTipDataC_ptr;

typedef struct SimEndSendData_typ {
char buffer[4]; // general send data
//int m_plrSockIdx;
//BOOL m_bShotEnd;
} SimEndSendData, *SimEndSendData_ptr;

typedef struct SimStartSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
} SimStartSendData, *SimStartSendData_ptr;

typedef struct BallsSendData_typ {	
char buffer[4]; // general send data			// 4
float m_ballx[35]; // MAX_TOTALBALLS = 22		// 88 // TODO: 22+(6+7)
float m_bally[35]; // MAX_TOTALBALLS = 22		// 88 // TODO: 22+(6+7)
int m_score1P;									// 4
int m_score2P;									// 4
int m_break1P;									// 4
int m_break2P;									// 4
int m_bestScore;								// 4
int m_gameOver;									// 4
BOOL m_bConcededGame;							// 1
BOOL m_bTurn1P;									// 1


Flags m_foul;		            				// 4
BOOL m_bMiss;		                     		// 1
BOOL m_rednext;									// 1
int m_colourseq;								// 4
BOOL m_bBallInHand;								// 1
int m_nominateBall;								// 4

int m_dominoHitLast;							// 4
int m_dominoHit1st;								// 4
																										
int m_testBallColour;							// 4

BOOL m_bSimEnd;									// 1

BOOL m_bForceReceive;							// 1

int m_reRackRndVal;
int m_foul3Times1P;
int m_foul3Times2P;

int m_touchBall[MAX_TOUCHING_BALLS];			// 6 * 4

int m_shotTime;

int m_poolPots1P;
int m_poolPots2P;

BYTE m_nSimEndMsgBalls;
												// ---

int m_lifes;									// 4
BOOL m_bTurnChanged;							// 1
												// ~230+4 bytes
} BallsSendData, *BallsSendData_ptr;


typedef struct StartPressedSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
int m_rndval;
} StartPressedSendData, *StartPressedSendData_ptr;

typedef struct GameOverForAllSendData_typ {
char buffer[4]; // general send data			// 4
int m_tblIdx;
int m_gameOver;
BOOL m_bForceRacksEnd;
BOOL m_bMatchTimeUp;
} GameOverForAllSendData, *GameOverForAllSendData_ptr;

typedef struct GameOverSendData_typ {
char buffer[4]; // general send data			// 4

char m_nameP1[22];								// 30
char m_nameP2[22];								// 30

int m_ratingP1;									// 4
int m_winsP1;									// 4
int m_lossesP1;									// 4
int m_streakP1;									// 4
int m_p50sP1;									// 4
int m_p60sP1;									// 4
int m_p70sP1;									// 4
int m_p80sP1;									// 4
int m_p90sP1;									// 4
int m_p100sP1;									// 4
int m_pHighBreakP1;								// 4
int m_pMax147sP1;								// 4
int m_50sP1;									// 4
int m_60sP1;									// 4
int m_70sP1;									// 4
int m_80sP1;									// 4
int m_90sP1;									// 4
int m_100sP1;									// 4
int m_HighBreakP1;								// 4
int m_Max147sP1;								// 4
float m_pBioAverageBreakP1;						// 4
float m_pBioAverageShotTimeP1;					// 4
float m_pBioAveragePositionP1;					// 4
float m_pBioAverageSafetyP1;					// 4
float m_bioAverageBreakP1;						// 4
float m_bioAverageShotTimeP1;					// 4
float m_bioAveragePositionP1;					// 4
float m_bioAverageSafetyP1;						// 4
int m_highestratingP1;							// 4
int m_cashP1;									// 4
int m_reracksP1;								// 4
int m_levelP1;									// 4
int m_levelProgressP1;							// 4
int m_ladderPointsP1;							// 4
int m_ladderWinsLossesP1;						// 4
int m_ladderHighBreakP1;						// 4
int m_ladderRankP1;								// 4

int m_ratingP2;									// 4
int m_winsP2;									// 4
int m_lossesP2;									// 4
int m_streakP2;									// 4
int m_p50sP2;									// 4
int m_p60sP2;									// 4
int m_p70sP2;									// 4
int m_p80sP2;									// 4
int m_p90sP2;									// 4
int m_p100sP2;									// 4
int m_pHighBreakP2;								// 4
int m_pMax147sP2;								// 4
int m_50sP2;									// 4
int m_60sP2;									// 4
int m_70sP2;									// 4
int m_80sP2;									// 4
int m_90sP2;									// 4
int m_100sP2;									// 4
int m_HighBreakP2;								// 4
int m_Max147sP2;								// 4
float m_pBioAverageBreakP2;						// 4
float m_pBioAverageShotTimeP2;					// 4
float m_pBioAveragePositionP2;					// 4
float m_pBioAverageSafetyP2;					// 4
float m_bioAverageBreakP2;						// 4
float m_bioAverageShotTimeP2;					// 4
float m_bioAveragePositionP2;					// 4
float m_bioAverageSafetyP2;						// 4
int m_highestratingP2;							// 4
int m_cashP2;									// 4
int m_reracksP2;								// 4
int m_levelP2;									// 4
int m_levelProgressP2;							// 4
int m_ladderPointsP2;							// 4
int m_ladderWinsLossesP2;						// 4
int m_ladderHighBreakP2;						// 4
int m_ladderRankP2;								// 4

unsigned m_bPlr1Won				:1;				// 1
unsigned m_bRated				:1;				// 1
unsigned m_bInvalidGame			:1;				// 1
unsigned m_bRacksStillToPlay	:1;				// 1

int m_tblIdx;									// 4
int m_oldRating1P;								// 4
int m_oldRating2P;								// 4


int m_EOGmsg;									// 4

int level;										// 4
int level_progress;								// 4
int text_color;									// 4


												// ---
												// 239+1 + 4 bytes + 16bytes [~260]

/*
int m_plrSockIdx;
int m_gameOver;
int m_highBreaks1P0;
int m_highBreaks1P1;
int m_highBreaks2P0;
int m_highBreaks2P1;
BOOL m_bInvalidGame;
*/
} GameOverSendData, *GameOverSendData_ptr;

typedef struct PlayerBioSendData_typ {
char buffer[4]; // general send data
int m_cueError;
float m_bioAverageShotTime;
float m_bioAveragePosition;
float m_bioAverageSafety;
float m_bioAverageBreak;
} PlayerBioSendData, *PlayerBioSendData_ptr;

typedef struct RequestPlayerBioSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
int m_returnPlrSockIdx;
} RequestPlayerBioSendData, *RequestPlayerBioSendData_ptr;

typedef struct ReturnPlayerBioSendData_typ {
char buffer[4]; // general send data
int m_returnPlrSockIdx;
char m_comments[128+1];
char m_town[28+1];
char m_age[3+1];
char m_name[28+1];
char m_sex[1+1];
} ReturnPlayerBioSendData, *ReturnPlayerBioSendData_ptr;

typedef struct ConcedePressedSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
int m_tableIdx;
BOOL m_bAndExit;
BOOL m_bAndClose;
} ConcedePressedSendData, *ConcedePressedSendData_ptr;

typedef struct ReRackPressedSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
BOOL m_bAccepted;
int m_acceptType;
BOOL m_bAbortRaceTo;
} ReRackPressedSendData, *ReRackPressedSendData_ptr;

typedef struct InvitePressedSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
int m_invitedPlrSockIdx;
BOOL m_bAccepted;
char m_str[84];
int m_initialHostTbl;
int m_table;
int m_enterType;
char m_name[22];
char m_invitedName[22];

// server to set these on sent invitations.
int m_reracks;	
int m_totalGames; //wins + losses
} InvitePressedSendData, *InvitePressedSendData_ptr;

typedef struct AutoInvitePressedSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
int m_invitedPlrSockIdx;
BOOL m_bAccepted;
char m_str[84];
} AutoInvitePressedSendData, *AutoInvitePressedSendData_ptr;

typedef struct BootPressedSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
int m_bootedPlrSockIdx;
} BootPressedSendData, *BootPressedSendData_ptr;

typedef struct ClearTableSendData_typ {
char buffer[4]; // general send data
int m_tblIdx;
} ClearTableSendData, *ClearTableSendData_ptr;

typedef struct UpdateVCashSendData_typ {
char buffer[4]; // general send data
int playerSockIdx;
int cash;
} UpdateVCashSendData, *UpdateVCashSendData_ptr;

typedef struct FindMatchSendData_typ {
char buffer[4]; // general send data
char optionRaceToTwo;
char optionPro;
char optionExactMatch;
int status;
int playerSockIdx;
} FindMatchSendData, *FindMatchSendData_ptr;


typedef struct FindMatchCancelSendData_typ {
	char buffer[4]; // general send data
} FindMatchCancelSendData, *FindMatchCancelSendData_ptr;


typedef struct GetLadderRankingsData_typ {
	char buffer[4]; // general send data
} GetLadderRankingsData, *GetLadderRankingsData_ptr;


typedef struct LadderRankingsData_typ {
	char buffer[4]; // general send data
	std::array<ladderObject, 20> rankData;
	int playerSockIdx;
} LadderRankingsData, *LadderRankingsData_ptr;


typedef struct TextColorSendData_typ {
char buffer[4]; // general send data
int textcolor;
} TextColorSendData, *TextColorSendData_ptr;



typedef struct TextSendData_typ {
char buffer[4]; // general send data
int textcolor;
char msg[256];
} TextSendData, *TextSendData_ptr;


typedef struct TextFormatSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
char m_name[22];
int m_textFormat;
} TextFormatSendData, *TextFormatSendData_ptr;

typedef struct HighBreakSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
int m_highBreak;
int m_cueError;
int m_high4;
} HighBreakSendData, *HighBreakSendData_ptr;


typedef struct AllHighBreaksSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
char m_highBreakName0[22];
char m_highBreakName1[22];
char m_highBreakName2[22];
int m_highBreak0;
int m_highBreak1;
int m_highBreak2;

char m_highPBreakName0[22];
char m_highPBreakName1[22];
char m_highPBreakName2[22];
int m_highPBreak0;
int m_highPBreak1;
int m_highPBreak2;

char m_highUKBreakName0[22];
char m_highUKBreakName1[22];
char m_highUKBreakName2[22];
int m_highUKBreak0;
int m_highUKBreak1;
int m_highUKBreak2;

int m_high4;
int m_cueError;
} AllHighBreaksSendData, *AllHighBreaksSendData_ptr;

typedef struct TableHostCompleteSendData_typ {
char buffer[4]; // general send data
int m_tblIdx;
} TableHostCompleteSendData, *TableHostCompleteSendData_ptr;

typedef struct TimeUpSendData_typ {
char buffer[4]; // general send data
int m_shotTime;
BOOL m_bTimeUp;
} TimeUpSendData, *TimeUpSendData_ptr;

typedef struct MaxTimeUpSendData_typ {
char buffer[4]; // general send data
int m_playerSockIdx;
} MaxTimeUpSendData, *MaxTimeUpSendData_ptr;

typedef struct PLCSendData_typ {
	char buffer[4];
	int m_playerSockIdx;
	char m_name[22];
	int m_tblIdx;
} PLCSendData, *PLCSendData_ptr;


typedef struct PCSendData_typ {
	char buffer[4];
	int m_playerSockIdx;
	int m_gameOver;
	int m_gameOverIWon;
	int m_playerSockIdx2;
	int m_tblIdx;
	int m_cueBallSelect;		// need to Tell the player connecting if we have selected a ball or not also //
} PCSendData, *PCSendData_ptr;



typedef struct PlayerTargetSendData_typ {
	char buffer[4];
	int m_targetPocket;
	int m_targetBall;
	BOOL m_bTargetSuccess;
} PlayerTargetSendData, *PlayerTargetSendData_ptr;

typedef struct GamePingSendData_typ {
char buffer[4]; // general send data
int m_plrSockIdx;
} GamePingSendData, *GamePingSendData_ptr;

typedef struct TMatchTimerLeftSendData_typ {
	char buffer[4]; // general send data
	int m_timeRemaining;
	int m_type;
	char m_str[128];
	int m_plrSockIdx;
	int m_tblIdx;
} TMatchTimerLeftSendData, *TMatchTimerLeftSendData_ptr;

typedef struct TextMsgSendData_typ {
	char buffer[4]; // general send data
	char m_str[200];
	int m_type;
	int m_plrSockIdx;
	int m_tblIdx;
	int m_lobbyAndTblIdx; // send to all in lobby and in tbl Idx
} TextMsgSendData, *TextMsgSendData_ptr;

typedef struct TemperSendData_typ {
	char buffer[4]; // general send data
	int m_plrSockIdx;
} TemperSendData, *TemperSendData_ptr;


// [CEventTimer] //

class CEventTimer {
protected:
CTime m_timeEvent; // Time for next event to occur.
CTimeSpan m_timeFrequency; // 100 seconds
public:
CEventTimer();
~CEventTimer();
void Process(void);
virtual void OnEvent(void);
};


// -------------- //

class CEventTimerMidnightWeekly : public CEventTimer {
private:

public:
	CEventTimerMidnightWeekly();
	void OnEvent(void);
};


// -------------- //

class CEventTimerMIDNIGHT : public CEventTimer {
private:

public:
CEventTimerMIDNIGHT();
void OnEvent(void);
};

// -------------- //

class CEventTimerOFTEN : public CEventTimer {
private:

public:
CEventTimerOFTEN();
void OnEvent(void);
};

// -------------- //

class CEventTimerMINUTE : public CEventTimer {
public: //was prviate: OAP
int m_tick1;
int m_tick2;
int m_tick3;
int m_tick4;
int m_tick5;
public:
CEventTimerMINUTE();
void OnEvent(void);
};



struct NextTournamentData {
	long long timestamp;		// -1 means no event. anything else is the timestamp of the next tournament
	char name[256];				// Name of tournament event
	char url[256];				// URL for Tournament system (i.e. "208.117.81.46/mtourney/mtourney.php")
	char lobbyDesc[128];
	int tourneyid;				// tourney ID (from this we can generate Entry Page URL and Standings)
	long long nextBroadcastCountDown;
	long long nextBroadcastPeriod;
	int expirationCycles;		// number of cycles to wait before removing event. I.e. if this event isn't updated the next time we get events, remove it
	int entrants;

	NextTournamentData() {
		timestamp = -1;
		nextBroadcastCountDown = 0;
		nextBroadcastPeriod = INT_MAX;
		expirationCycles = 1;
		name[0] = 0;
		tourneyid = 0;
		entrants = 0;
	}

	NextTournamentData(const char* name, int tourneyid, long long timestamp, int entrants, const char *url, const char *lobbyDesc) {
		Update(name, tourneyid, timestamp, entrants, url, lobbyDesc);
	}

	void Update(const char* name, int tourneyid, long long timestamp, int entrants, const char *url, const char *lobbyDesc) {
		strcpy(this->name, name);
		strcpy(this->url, url);
		this->tourneyid = tourneyid;
		this->timestamp = timestamp;
		this->entrants = entrants;
		strcpy(this->lobbyDesc, lobbyDesc);
		ResetExpiration();
	}

	// This should be set to the total number of events we'll be pinging. So we get an updates every Event Count, if event if running
	void ResetExpiration();


	void Delete() {
		timestamp = -1;
		name[0] = 0;
		tourneyid = 0;
	}

	char* getInfo() const {
		char* buf = new char[256];
		sprintf(buf, "NextEventData = TourneyName: %s, TourneyId: %d, timestamp: %ld", name, tourneyid, timestamp);
		return buf;
	}

	void UpdateBroadcastTime();

};			



// -------------- //

class CEventTimerSECOND : public CEventTimer {
private:
int m_tick1;
int m_tick2;
int m_tick3;
int m_tick4;
int m_tick5;
public:
CEventTimerSECOND();
void OnEvent(void);
};




// -------------- //

class CGatewayLst {
public:
char **m_ip;
char **m_addr;
int *m_port;
int *m_players;
int m_ip_n;
public:
CGatewayLst(int n);
~CGatewayLst();
int AddLobby(char *ip, char *ext);
char *GetLobby(int idx);
char *GetLobbyAddr(int idx);
int GetLobbyPort(int idx);
};

// -------------- //

#define CLOBBYSETTINGSF_MEMBERS		1
#define CLOBBYSETTINGSF_GUESTS		2
#define CLOBBYSETTINGSF_ALL			3

class CLobbySettings {
public:
CWnd *m_pParent;
char m_name[80];
char m_pass[80];
char m_desc[80];
char m_gatewayIP[80];
int m_gatewayPort;
char m_tourneyIP[80];
int m_tourneyPort;
char m_lobbyIP[80];
int m_lobbyPort;
int m_maxGuests;
int m_maxMembers;
char m_tag[80];
int m_fps;
char m_tourneybcUrl[3][80];
int m_tourneybcUrlCount;
int m_tourneybcPingSeconds;
int m_tourneybcIntervalSeconds;


DWORD m_flags;
public:
CLobbySettings();
~CLobbySettings();
void GetParent(CWnd *pParent);
void Load(char *fname);
};

void ServerLoadAbuse(void);
void ClientLoadAbuse(void);

// -------------- //

class CEnvironment;
class CHTTPSocketEx;
class CLobbySettings;

extern BOOL g_bGfxQuality;
extern int g_cyc;
extern int g_cycOld;
extern float g_seq;
//extern int g_bounce255;
//extern BOOL g_bBounce255;
extern int g_ballmouseoncyc;
extern DWORD g_playerIdleTime;

extern BOOL g_oncreatecomplete;

extern CTournament g_tournament;

// [CRequestManager] //

class CRequestManager {
CHTTPSocketEx *m_httpSocketEx[8];
public:
CRequestManager(CWnd* pParent);
~CRequestManager();
int SlotsUsed(void);
void ClearSlots(void);
void StartRequest(char *ip, int port, char *reqstr);
};

// [CRecentLog] //

class CRecentLog {
public:
char **m_strList;
int m_strList_n;
int m_strList_idx;
int m_strList_tot;
int m_heat;
public:
CRecentLog(int max);
~CRecentLog();
void AddStr(char *str);
void Debug(void);
void Report(void);
};

// [CGatewayLst] //
class CGatewayEXELst {
public:
char **m_gatewaylst;
int *m_port;
int m_gatewaylst_n;
int m_gatewaylst_i;
int m_gatewaylst_s;
public:
CGatewayEXELst();
~CGatewayEXELst();
void ResetGateway(void);
void AddGateway(char *addr, int port);
};

class CGameAppView : public CView
{
protected: // create from serialization only
	CGameAppView();
	DECLARE_DYNCREATE(CGameAppView)

	std::vector<NextTournamentData> nextTournamentData;
	void InitNextTournamentData();
	


// Attributes
public:
//-#ifndef MAIN_SERVER
	CSnooker *m_snooker;
//-#endif //!MAIN_SERVER
	CLobby *m_lobby;
	CBot *m_bot;
	CReplay *m_replay;
	CModerator *m_moderator;
	CEnvironment *m_environment;

	CIPConnectCoolDown *m_IPConnectCoolDown;
	CIPCoolDown *m_ipCoolDown;
	BOOL m_bGateopen;
	CSysList *m_syslist;
	CMACList *m_maclist;
	CGatewayEXELst *m_gatewayEXELst;

	CServerAllPlayersStats *m_serverAllPlayersStats;
	CServerAllPlayersBreaks *m_serverAllPlayersSBreaks;
	CServerAllPlayersBreaks *m_serverAllPlayersPBreaks;
	CServerAllPlayersBreaks *m_serverAllPlayersTempBreaks;

	// Ignore List //
	CIgnoreLst *m_ignorelst;

	//CRanking *m_ranking;
	CRequestManager *m_reqManager;
	CRecentLog *m_recentLog;

	CHTTPSocketEx *m_httpSocketEx;
	CHTTPSocketEx *m_httpSocketEx2;
	CLobbySettings *m_lobbySettings;

	CWinThreadEx m_mainThreadEx;
	
	//----------------//
	servent	   m_pservent;
	//char       m_lpszMessage[100];
	char       m_lpszPort[6];
	HANDLE	   m_hGetServByName;
	char       m_lpszServEntryBuf[10/*MAXGETHOSTSTRUCT*/];
	//----------------//

	// important network flags
	int m_socketConnected;
	enum {LOBBYROOM = 1, GAMEROOM = 2};
	//BOOL m_bIsMainServer;
	//BOOL m_bIsGameServer;
	
	CGameAppDoc* GetDocument();
	CString m_strText;
	//char m_message[81];
	CString m_nick;
	
	// child windows
	CWnd m_wnd;
	CSendEdit m_sendEdit;
	CSmileySelectDlg m_SmileySelectDlg;
	CTextColorChangerDlg m_TextColorChangerDlg;

	CEditEx m_editExChat;
	//CListBoxEx m_listBox;
	CTableTabCtrlDlg m_tableTabCtrlDlg;
	CLobbySelect m_lobbySelectTabCtrlDlg;
	CTabCtrlEx m_tabLobbyCtrlEx;
	
	
	CButton m_viewtablesButton;
	CButton m_exitlobbyButton;
	CButton m_changelobbyButton;
	CButton m_messageBoardFullButton;
	
	CButton m_homeButton;
	CButton m_registerButton;
	BOOL m_registerButtonEnabled;
	CButton m_tourneysButton;
	CButton m_leaguesButton;
	CButton m_refreshButton;
	BOOL m_refreshButtonEnabled;
	
	
	CButton m_EnterLobbyButton; // note: this is for in lobby - lobby select
	CListCtrlEx m_listBox;
	CListCtrlEx m_listPlayers;
	CListCtrlEx m_listTables;
	CButton m_button[8 + 2];
	
	CButtonEx m_standButton;
	BOOL m_standButtonEnabled;


	CButtonEx m_sit1PButton;
	CButtonEx m_sit2PButton;
	CButtonEx m_sit1PButtonB;
	CButtonEx m_sit2PButtonB;
	CButton m_EndPracticeButton;
	//CButton m_cueErrorButton;
	//CComboBox m_cueErrorBox;
	CButton m_ratedButton;
	//CButton m_sayButton;
	MyButton m_sayButton;

	CButton m_smileyselectButton;
	CComboBox m_tableTypeBox;
	CButton m_missRuleButton;
	CAboutDlg m_aboutDlg;
	CMySliderCtrl m_sliderRaiseButt;
	BOOL m_bMySliderRaiseButt;
	CDetailsDlg m_detailsDlg;
	CDetailsSettingsDlg m_detailsSettingsDlg;
	CLoginWizardDlg m_loginWizardDlg;
	CStatsDlg m_statsDlg;
	CStatsTabCtrlDlg m_statsTabCtrlDlg;
	CStatsChatDlg m_statsChatDlg;
	CStatsDlg2 m_statsDlg2;
	CPersonalDetailsDlg m_personalDetailsDlg;
	CTournamentsDlg m_tournamentsDlg;
	// CShopDlg m_shopDlg;
	CConcedeDlg m_concedeDlg;
	CReRackDlg m_rerackDlg;
	CReRackAccept2Dlg m_rerackAccept2Dlg;
	CExitDlg m_exitDlg;
	CInviteDlg m_inviteDlg;
	CInvitationDlg m_invitationDlg[MAX_INVITATIONDLGS];
	int m_invitationN;
	CBootDlg m_bootDlg;
	CAlertDlg m_alertDlg;
	CReplayDlg m_replayDlg;
	CPlayerListDlg m_playerListDlg;
	CPracticeEditDlg m_practiceEditDlg;
	CTableSetupDlg m_tableSetupDlg;
	CTableSetupDlg2 m_tableSetupDlg2;
	CLobbySelect m_lobbySelectDlg;
	CTermsDlg m_TermsDlg;
	CNewsDlg m_NewsDlg;
	CForfeitDlg m_forfeitDlg;
	CNotePadDlg m_notePadDlg;
	CGuestRegisterDlg m_guestRegisterDlg;

	
//-	#ifdef MAIN_SERVER
	CServerConfigDlg m_serverConfigDlg;
	CButton m_serverConfigButton;
	CAccountConfigDlg m_accountConfigDlg;
	CButton m_accountConfigButton;
	CButton m_lobbyStatButton;
//-	#endif //MAIN_SERVER
	CMenu *m_pMenuPopup;
	
	fntData *m_pFntData;
	fntData *m_pFntData_A;
	CFont m_font;
	CFont m_checkBoxFont;
	CFont m_listCtrlFont;
	CFont m_listCtrlFontSmall;
	CBitmapEx m_bitmap;
	Mgfx_spr m_sprite;
	CBitmapEx m_bitmapLobby;
	Mgfx_spr m_spriteLobby;

	Font *g_pFont1;
	Font *g_pFont2;
	Font *g_pFont3;
	Font *g_pFontSnk1; 
	Font *g_pFontSnk2; 

	

#ifndef MAIN_SERVER
	CHtmlViewEx m_htmlViewBanner;
	CHtmlViewEx m_htmlViewLeaderBoardA;
	CHtmlViewEx m_htmlViewLeaderBoardB;
	CHtmlViewEx m_htmlViewTickerBoard;
	CHtmlViewEx m_htmlViewMB;
	CHtmlViewEx *m_htmltest;
#endif

	BOOL m_bFullMode;
	BOOL m_bLobbyMode;				// 0: Default Mode, Message-Board is On, 1: Playing Mode, Tables is On //
	BOOL m_first_time;
//	Mgfx_spr *m_spriteTable[45]; // 8-ball*9 + 9-ball*9 + r-ball*9 + bowls*9 + WILD*9 = 45
	HBITMAP m_bitmapTable[81]; // 8-ball*9 + 9-ball*9 + r-ball*9 + bowls*9 + 6-ball*9 + 10-ball*9 + WILD*9 +UKPool + Break = 65
//	Mgfx_spr *m_spriteTable2[42]; // 15reds*8 + 10 reds*8 + 6 reds*8 + black*8 + WILD*8 + practice + off
	HBITMAP m_bitmapTable2[10];

	BOOL m_bReplayMode;
	BOOL m_bPlayOffline;
	BOOL m_bExtraGameLoop;
//	BOOL m_bFocus;
	BOOL m_bSetFocus;
	BOOL m_bFlashWindow;
	int m_knockknock;
	
	BOOL m_bMainFramePlayingClose;

	// window screen size width / height
	BOOL m_bOnSize;
	BOOL m_bOnSizeDrawTmpSurf;
	CSize m_SizeDesktop;
	int m_cx;
	int m_cy;
	float m_fX;
	float m_fY;
	
	// click coordinates
	CPoint m_mouseMovePoint;
	CPoint m_point;
	CPoint m_mousePoint;
	BOOL m_bLButtonDown;
	BOOL m_bRButtonDown;
	BOOL m_bMButtonDown;
	BOOL m_bLButtonJustUp;
	BOOL m_bRButtonJustUp;
	BOOL m_bMButtonJustUp;
	float m_cameraAngle;
	float m_cameraMag;
	float m_panX;
	float m_panY;
	float m_cameraLift;

	// Camera Position for TV Mode //
	float m_TVcameraX_pos[2], m_TVcameraY_pos[2], m_TVcameraZ_pos[2];
	float m_TVcameraX_at[2], m_TVcameraY_at[2], m_TVcameraZ_at[2];
	float m_TVcamera_Demo[3];
	int m_TVcameraChange;
	char m_TVClipType;

	CRect m_rectCursor;

	CDC *m_dblDC;

	HCURSOR m_hCursor;
	HCURSOR m_hCursorHand;
	BOOL m_bUpdateCursor;
	BOOL m_bShowCursor;
	BOOL m_bDisableOnDraw;
	BOOL m_bJustExitedLobby;
	BOOL m_bFoulDlg;
	
	CRect m_origin;
	CRect m_newClientRect;
	CRect m_oldClientRect;
	CRect m_newClientSRect;
	CRect m_oldClientSRect;
	CTime m_time; // use him for getting the time!
	CString m_stringFullDate;
	CString m_stringFullTime;
	CString m_stringShortDate;
	CString m_stringShortTime;

	CString m_stringTimeStamp;	// Includes Time & Date (used for LogChat)

	int m_ExpDateDay;
	int m_ExpDateMonth;
	int m_ExpDateYear;


	CEventTimerMidnightWeekly *m_eventTimerMidnightWeekly;
	CEventTimerMIDNIGHT *m_eventTimerMidnight;
	CEventTimerOFTEN *m_eventTimerOften;
	CEventTimerMINUTE *m_eventTimerMinute;
	CEventTimerSECOND *m_eventTimerSecond;

	// data to remember!
	newAccountSendData m_newAccountDataRemember;
	int m_rememberPlayerSocketIdx;

	// for debug purposes!!!
	char bufferNum[81];
	char bufferNum2[81];
	char bufferNum3[81];
	
	Msfx_smp m_game_smp[96];
	int m_game_smp_n;
	
	strikeBallSendData m_strikeBallSendTmp;

	BOOL m_bOnTimerBusy;
	BOOL m_bIdleAway;
	BOOL m_justEndedPractice;
	BOOL m_bPracticeEditPlay;
	int m_practiceEditColour;
	int m_kicked;
	char m_kreason[256]; // Reason for being kicked!
	char m_greeting[256];

	int m_hostcheckactive; // (n) socket index which is undergoing hostchecking. -1 = no socket.
	int m_hostchecktimeout; // if this times out and socket is still !=-1 then it puts socket back to -1
	int m_hostchecktimeoutK;	// time out K, can be set with /hostchecktimeout (moderators only)

	int m_oldCBS; // keep track of old(previous) CueBallSelect value

	int m_tableNo;
	int m_tableEnter; // 0 - Watch / 1 - 1P Join / 2 - 2P Join
	int m_playerSockIdx; // [server] servers sent player Idx
	DWORD m_playerId;	// [global]-unique, universal player id (like your name, but is more convenient-4 bytes)
	int m_playerIdx;	// [local]
		
	int m_checkPlrSockIdx; // current plrsockidx for the checking!
	
	int m_createGamePlayerIdx;

	int m_sendGamePingCount;
	int m_receiveGamePingCount;
	float m_hostPingTime;

	int m_GFXMode; // GFX view mode
	float m_frameRate;
	
	CString m_sLobbyHighBreakName[3];
	int m_sLobbyHighBreak[3];
	CString m_sLobbyPHighBreakName[3];
	int m_sLobbyPHighBreak[3];
	CString m_sLobbyUKHighBreakName[3];
	int m_sLobbyUKHighBreak[3];

	BOOL m_bUpdateHighBreaks;

	int m_cyc;
	float m_seq;
	int m_connectStyle;
	int m_lobbyRefreshDelay;
	int m_elCycle;

	float m_viewportWidth, m_viewportHeight;
	BOOL m_bUpdateTable;
	
	BOOL m_bDrawSeats;
	int m_drawSeatsDelay;

	CGatewayLst *m_gatewaylst;
	BOOL m_lobbySelectSuccess;
	BOOL m_bServerState;
	BOOL m_bDoneADraw;
	
	int m_functionKey;
	int m_functionKeyGFXDelay;

	int m_ltime2stime;

	float m_sponsoralpha;
	char m_sponsortxt[256];
	char m_sponsorurl[256];


	CCueChanger *m_cueChanger;
	CFineTuner *m_fineTuner;
	CChalkChanger *m_chalkChanger;
	CAchievements *achievements = NULL;
	CLadderRankings *ladderRankings = NULL;

	// 2D Sprites (pDC)
	
	CSprite *m_testSprite;
	CLobbyButtons *m_lobbyButtons;

	// Find Match Control
	CFindMatchCtrl *m_findMatchCtrl;

	// Stats Window
	CStatsWindow *m_statsWindow;

	// This is our global variable for the current Mini Cam Height (note.. the Mini Cam is square, so Height is the Width also)
	int m_miniCamHeight;
	char *m_cheatProcess;
	int m_timeToNextCheck;
	int m_timeToCrash;
	
	// We have mouse down on power bar, so setcapture (for mouse), so when we release it, we get this message no matter where we released capture
	bool g_bOnPowerBarMouseDown = false;
		
// Operations
public:

	void CreateAchievements(char *playerName);
	void CreateLadderRankings(char *playerName, std::array<ladderObject, 20> rankingsData);
	void DestroyAchievements();

	void GameLoop(float timeDelta);
		
	void Init(void);
	void Deinit(void);
	void Process(float timeDelta);
	void Draw(float timeDelta);
	
	void ProcessSnooker(float timeDelta);
	void ProcessSnookerChildWindows(void);
	void ProcessSnookerMiniCamExtra(void);
	void DrawSnooker(float timeDelta);

	void DrawBitmap(CDC* pDC,int aX, int aY,  HBITMAP hBitmap);
	void DrawBitmapRegion(CDC* pDC, int x1, int y1, int x2, int y2, HBITMAP hBitmap);
	void DrawBitmapScale(CDC* pDC, int x1, int y1, int width, int height, HBITMAP hBitmap);

	void Render3DSetupCamera(void); // 3D = DirectX Stuff on Table/MiniCam
	void Render3DSetupStates(void);
	void Render3DBalls(BOOL bMainView = TRUE, BOOL alpha_on = TRUE);
	
	void Render3DBeatle(void);
	void Render3DCueTip(void);
	void Render3DCueTipCentre(void);
	void Render3DCueTipDC(void);
	void Render3DSnookerTable(char type = 0, float alpha = 1.00f);
	void Render3DCue(float timeDelta, float alpha = 1.00f);
	void UpdatePower(CPoint point);
	void UpdatePower(float percent);
	void CalcPowerOfShot(void);
	void Render3DWhiteSightLine(void);
	void Render3DObjectSightLine(void);
	void Render3DGhostBall(void);
	void Render3DWhiteTouchingBalls(void);

	void Render2DClockProbe(void);	// for debugging!
	void Render2DTableCuePercentage();	// 2D = DirectX Stuff on Table
	void Render2DTableHint();
	void Render2DDebug(void);		// 2D = DirectX Stuff elsewhere
	void Render2DTurn(void);
	void Render2DNextBall(void);
	void Render2DSitButtons(void);
	void Render2DCameraButtons(void);
	void Render2DOverLayPower(void);
	void Render2DOverLaySeating(void);
	
	void Render3DOverLayHelper(void);
	void Render3DOverLayTable(void);
	void Render3DOverLayTargetPocket(void);
	void Render3DOverLayTargetBall(void);
	void Render3DWhiteTouchingBallsMiniCam(void);
	void Render3DOverLayHelperMini(void);
	
	void Render3DOverLay3D2DToggle(void);
	void Render3DScene(float alpha = 1.00f, float lighting = 1.00f);		// 1.00f is default lighting
	void Render3DOverlayMiniCam(DWORD width, DWORD height);

	void InitSponsor(void);
	void ProcessSponsor(void);

	int m_sSyncStart;
	void VSyncStart(void);
	void Clear3DScene(void);
	void Render3D(float timeDelta);
	void Render3DMiniCam(float timeDelta);
	void Render2DText(float timeDelta);
	void CalcFPS(float timeDelta);
	void Draw3D(float timeDelta);
	void DrawLobbyTV(int tv_x, int tv_y, int tv_w, int tv_h);
	void DrawMini3D(void);
	void Draw2D(float timeDelta);
	void Draw2DRender(CDC *dc, BOOL bNoDbl = FALSE);
	void Draw2DLobby(void);
	void Draw2DLobbyRender(CDC *dc);
	void OnDrawSnooker(CDC *pDC);
	void DrawSeatsOnly(int x, int y);
	void DrawSeats(int x, int y);
	void SitName(int idx, char *name, int plrSockIdx);
	void SitClicked(int id);
	void RefreshListBox(void);
	void ResizeScreenToSaved(void);
	
	void ProcessLobby(void);
	void DrawLobby(void);
	void OnDrawBanners(CDC* pDC);
	void OnDrawLobby(CDC *pDC);


	void TableView_MOUSEMOVE(UINT Msg, WPARAM wParam, LPARAM lParam);
	void TableView_LBUTTONDOWN(UINT Msg, WPARAM wParam, LPARAM lParam);
	void TableView_LBUTTONUP(UINT Msg, WPARAM wParam, LPARAM lParam);
	void TableView_RBUTTONDOWN(UINT Msg, WPARAM wParam, LPARAM lParam);
	void TableView_RBUTTONUP(UINT Msg, WPARAM wParam, LPARAM lParam);
	void TableView_MBUTTONDOWN(UINT Msg, WPARAM wParam, LPARAM lParam);
	void TableView_MBUTTONUP(UINT Msg, WPARAM wParam, LPARAM lParam);
	void TableView_MOUSEWHEEL(UINT msg, WPARAM wParam, LPARAM lParam);

	void ButtonPressedSend(char *str);
	void Message(LPCTSTR lpszMessage, int messageID = 0, int plrSockIdx = -1, COLORREF col = 14803425, int senderLevel =-1);	// RGB(225, 225, 225) by default
	void MessageSend(CString msg);
	void Text(CDC* pDC, int x, int y, LPCTSTR lpszString);
	void DisplayFoulUpdate(void);

	BOOL OnLobbySelect(int n);
	void EnterLobby(char *addr, int port);
	void PostLobby(void);
	void ConnectToLobby(BOOL bIsTabCtrlWindow = FALSE);
	bool ShowTerms(BOOL bIsTablCtrlWindow = FALSE);
	bool ShowNews(void);
	void OnConnect(CAsyncSocketEx *pSocket, int nErrorCode);
	void OnConnectGAMESERVER(CAsyncSocketEx *pSocket, int nErrorCode);
	void OnConnectGAMESERVER_REJOIN(CAsyncSocketEx *pSocket, int nErrorCode);
	void OnConnectGAME(CAsyncSocketEx *pSocket, int nErrorCode);
	void OnConnectGAME_REJOIN(CAsyncSocketEx *pSocket, int nErrorCode);

	void SendListenCheckListening(int port, int i);
	
	void SetCheckSockIdx(int plrSockIdx);
	void SetCheckID(BOOL id);
	void SendServerMessage(char *str, int sockIdx);

	void LoginFailed(char failType);
	void AccountCreated(char failType);
	void LoginNewAccount(int timetoexp, BYTE ext);

	void ExitFromLobby(int disc = 0, BOOL bIsTabCtrlWindow = FALSE); // disconnect flag (either u were disconnected or you did it urself!)

	int IsCompositionEnabled(void);
	bool InitD3D(CRect rect, CREATESTRUCT cs, bool type = 0);
	bool DeinitD3D(void);
	bool ResetDevice(void);
	bool InitDevice(bool bM3D = true);

	bool CreateStatsWindow(void);


	void UpdateStatsDialogs(void);

	// lobby sends
	void SendServerData(int i);
	void SendServerConfig(int i);
	void ServerConfigReceive(ServerSendConfig_typ *sd); // [receive]
	void SendLobbyReady(BOOL bIsHost, int i = -1);
	void SendLoginFailed(BOOL bIsHost, char type, int i);
	void SendLoginNewAccount(BOOL bIsHost, int i, int timetoexp = 1);
	void SendAccountCreated(BOOL bIsHost, int type, int i);
	void SendUpdateHistory(char *myself_name, char *opponent_name, int gameinfo, int time);
	//void SendPlayerDetailsConnect(BOOL bIsHost, Player *pPlr, int i = -1, int blank_pass = 1);
	void SendPlayerDetailsConnect2(BOOL bIsHost, Player *pPlr, int i = -1, int blank_pass = 1);
	void SendPlayerDetailsCompact(BOOL bIsHost, Player *pPlr, int i = -1, int blank_pass = 1);

	void SendTableDetails(BOOL bIsHost, Table *pTbl, int i = -1);
	void SendPlayerJoin(BOOL bIsHost, int tableIdx, int enterType, int playerSockIdx, BOOL bIsTableHost, int i = -1, DWORD tableseatsA = 0xFFFFFFFF, DWORD tableseatsB = 0xFFFFFFFF);
	void SendPlayerReJoin(BOOL bIsHost, int tableIdx, int i = -1);
	void SendPlayerJoinFailed(BOOL bIsHost, int joinerSockIdx, int hostSockIdx, int i = -1);
	void SendPlayerExit(BOOL bIsHost, int playerSockIdx, BOOL bTimedOutBoot = FALSE, BOOL bPLC = FALSE);
	void SendPlayerSit(BOOL bIsHost, int seat1SockIdx, int seat2SockIdx, int tableIdx, int seat1bSockIdx, int seat2bSockIdx);
	void SendTableHostDetails(BOOL bIsHost, int cueError, BOOL bRated, char type, BOOL bmissRule, int playerSockIdx,
							  BYTE shotTime = 0xff, BYTE tableColour = 0xff, BYTE gameType = 0xff, BYTE addTimer = 0xff, BYTE doubles = 0xff, 
							  BYTE raceTo = 0xff, BYTE whoBreaks = 0xff,
							  BYTE ballSize = 0xff, BYTE rackType = 0xff, BYTE tableType = 0xff, BYTE proTable = 0xff,
							  int tournamentTable = -1, int fp1 = 0, int fp2 = 0, int score1 = 0, int score2 = 0, bool bLadderMatch = false);
	
	// game/table sends
	void SendPlayerShot(float sx,float sy,float sz, float ex,float ey,float ez, M3d_vtx cuetip, int m_selectBall, float m_strikeRnd, float m_strikeMaxRnd,
								  BOOL m_ballMouseOn, M3d_vtx m_ballMousePos, int m_nominateBall, int m_testBallColour, DWORD crc);

	void SendPlayerBallPos(CVector pos, int m_selectBall);
	void SendPlayerSelectBall(int select, CPoint point, int w, int h, int selectBall);
	//void SendPlayerMouseMove(float x, float y, int w, int h);
	void SendPlayerMouseMove(float x, float y, int w, int h, float strikeLine, float strikeDist);
	void SendPlayerCueBallTip(float sx, float sy, float sz, float ex, float ey, float ez,
							  BOOL ballMouseOn, M3d_vtx ballMousePos, M3d_vtx cueTip,
							  int nominateBall,
							  int RaiseButt, int RaiseButtOld,
							  float cueAngle);
	void SendPlayerCueBallTipCompact(float sx, float sy, float sz, float ex, float ey, float ez,
										BOOL ballMouseOn, M3d_vtx ballMousePos, M3d_vtx cueTip,
										int nominateBall,
										int RaiseButt, int RaiseButtOld,
										float cueAngle);


	void SendServerCheatAlert(char *cheatProcess);
	void ClientRequestFile(char *fname);
	void ServerSendFile(char *fname, int plrSockIdx);
	void SendServerFindMatch();
	void SendServerFindMatchCancel();
	void SendClientFindMatchStatus(int playerSockIdx, int status);
	
	void SendServerGetLadderRankings();
	void SendClientLadderRankings(std::array<ladderObject, 20> rankData, int playerSockIdx);


	void SendUpdateGame(void);
	void SendPlayerPlayAgain(void);
	void SendPlayerFoulAlert(char n = 0);
	void SendPlayerMissReset(void);
	void SendPot8BallOnBreakRerack(char type = 0);
	void SendBadBreakRerack(BOOL bOpponentToBreak = FALSE);
	void SendReset(char playerTurn = 0);
	void SendSimEnd(int plrSockIdx, BOOL bShotEnd = TRUE);
	void SendSimStart(int plrSockIdx);
	void SendBalls(BOOL bIsHost, BOOL bSimEnd, int i = -1, BOOL bForceReceive = FALSE);
	void SendBallsReceive(BallsSendData *sd);
	void SendStartPressed(int plrSockIdx);
	void SendGameOverForAll(int tblIdx, int gameOver, BOOL bForceRacksEnd = FALSE, BOOL bMatchTimeUp = FALSE);
	void SendGameOver(Player *pPlr1, Player *pPlr2, BOOL bPlr1Won, BOOL bRated, BOOL bInvalidGame, int tblIdx, int oldRating1P, int oldRating2P, BOOL bRacksStillToPlay, int EOGmsg = 0);
	void SendPlayerBioData(int cueError);
	void SendRequestPlayerBio(int plrIdx);
	void ReceiveRequestPlayerBio(int returnPlrSockIdx);
	void SendReturnPlayerBio(int returnPlrSockIdx);
	void ReceiveReturnPlayerBio(ReturnPlayerBioSendData *sd);
	void ReceivePlayerBioData(int plrIdx, int cueError, float bioAverageShotTime, float bioAveragePosition, float bioAverageSafety, float bioAverageBreak);
	void SendConcedePressed(BOOL bIsHost, int plrSockIdx, BOOL bAndExit = FALSE, BOOL bAndClose = FALSE, int tableIdx = -1);
	void SendReRackPressed(int plrSockIdx, BOOL bAccepted, int acceptType, BOOL bAbortRaceTo = FALSE);
	void ReRackAcceptDlg(BOOL bAbortRaceTo = FALSE);
	void SendInvitePressed(int plrSockIdx, int invitedPlrSockIdx, char *name, char *invitedName, BOOL bAccepted, char *str = NULL, int initialHostTbl = -1, int table = -1, int enterType = 0);
	void InvitationDlg(int HostSockIdx, char *hostName, int initialHostTbl = -1, int table = -1, int enterType = 0, int reracks = 0, int totalGames = 0);
	void SendAutoInvitePressed(int plrSockIdx, int invitedPlrSockIdx, BOOL bAccepted, char *str = NULL);
	void AlertMessage(char *str, BOOL bDoModal = FALSE);
	void AlertLoginMessage(char *str);
	void SendBootPressed(int plrSockIdx, int bootedPlrSockIdx);
	void SendClearTable(int tblIdx);
	void SendPlayerTextFormat(int plrSockIdx, int textFormat);
	void UpdateSponsor(int plrSockIdx = -1);
	void SendServerTime(int plrSockIdx);
	void SendAbuseLst(int plrSockIdx);
	void ReceiveServerTime(serverTimeSendData *sd);
	CTime GetServerTime(void);
	void SendHighBreak(BOOL bIsHost, int plrSockIdx, int highBreak = -1, int cueError = 0, int high4 = -1);
	void SendAllHighBreaks(int plrSockIdx, int high4 = -1, int cueError = -1);
	void SendAllHighBreaksReset(void);
	void SendTableHostComplete(int tblIdx);
	void SendPlayerTimeUp(BOOL bTimeUp = FALSE, int i = -1);
	void SendPlayerMaxTimeUp(int playerSockIdx);
	void ReceivePlayerTimeUp(int shotTime, BOOL bTimeUp);
	void ReceivePlayerMaxTimeUp(int playerSockIdx);
	void SendPlayerLostConnection(int playerSockIdx, char name[22], int tblIdx = -1);
	void SendPlayerConnection(int playerSockIdx, int gameOver = 0, int gameOverIWon = -1, int playerSockIdx2 = -1, int tblIdx = -1);
	void SendPlayerTargetData(int targetPocket, int targetBall, BOOL bTargetSuccess);
	void SendTMatchTimerLeft(int timeRemaining, int type, char *str, int plrSockIdx = -1, int tblIdx = -1);
	void SendTextMsg(char *str, int type = 0, int plrSockIdx = -1, int tblIdx = -1, int lobbyAndTblIdx = -1);
	void SendTextColorChange(int textcolor);
	void SendServerUpdateVCash();
	void SendClientUpdateVCash(int playerSockIdx, int cash);

	void BootPlayer(int plrSockIdx, int msg = -1);
	void ExitTable2Lobby(void);
	void FreeMouse(void);
	void CloseApp(void);

	void SendRequestPlayerDetails(int plrIdx, BOOL bShowDialog = TRUE);
	void SendRequestIdleAway(BOOL bIdleAway);
	void ReceiveRequest(BYTE *sd, int plrSockIdx);
	void ReceiveRequestPlayerDetails(void *sd_temp, int plrSockIdx);

	void GamePingInit(void);
	void GamePingProcess(void);
	void SendGamePing(int plrSockIdx);
	void GamePingReceive(int plrSockIdx);
	
	void CreateGame(BOOL bIsHost, int playerIdx, BOOL bReJoin = FALSE);
	void CreateGame2(BOOL bIsHost, int playerIdx, BOOL bReJoin = FALSE);
	void CreateGamePost(void);
	void CreateGamePost2(void);

	void CreateGameShell(BOOL bIsHost = TRUE, int playerIdx = -1, BOOL bReJoin = FALSE);
	void CreateGameShell2(BOOL bIsHost = TRUE, int playerIdx = -1, BOOL bReJoin = FALSE);
	void UpdateWindowTitle(int tblIdx);
	void EnableSeats(int plrIdx, BOOL bState);
	void FailedGameShell(int playerIdx);
	void DestroyGameShell(BOOL bClearChatWindow = TRUE, BOOL bCloseApp = FALSE);
	void ReCreateGameShell(void);
	
	void ChangeTitle(CString str, char type = 0);
	//CString CGameAppView::GetTitle(void);
	BOOL OptionNotForGuest(int plrSockIdx, int type = -1);
	void DrawGameMessage(char *str, float realignment = 0.8f);
	void DrawGameMessageXY(char *str, int x = 16, int y = 12);
	void DrawGameMessage(char *str, CRect rect, int textdy = 0);

	void LoadAdvertisements(void);
	void DeleteAdvertisements(void);
	void Process3DAdverts(d3d::Ray ray, float x, float y, float w, float h);
	void Render3DAdverts(void);
	void Render2DAdvertsLinks(void);	

	bool IsLadderMatch(int tableIdx);
	bool InLadderMatch(void);
	int MyTableIdx(void);

	int m_playerStatusExt;
	int PlayerStatus(char *name);
	BOOL LoadPlayer(char *name, Player *plr);
	BOOL SavePlayer(char *name, Player *plr);

	BOOL TopUpPlayer(char *name, int days = 0);
	BOOL BanPlayer(char *name);
	BOOL UnbanPlayer(char *name);
	BOOL WallSysID(char *sysid);
	BOOL UnWallSysID(char *sysid);
	BOOL DenyMacID(char *macid);
	BOOL UnDenyMacID(char *macid);
	BOOL BlockIP(char *ip);
	BOOL UnBlockIP(char *ip);
	

	int LoadPlayerProfile(Player *plr, char *filetype);
	void SavePlayerProfile(Player *plr, char *filetype);
	void DeletePlayerProfile(Player *plr, char *filetype);

	BOOL GetClickBanner(CBitmap *pBanner, CPoint point, CRect rect);
	void PreviousWndEditSetFocus(BOOL bDefault = FALSE);


	int m_rankingstick; // rankings ticker.
	int m_rankingsfreq;	// rankings frequency in frames , 0 = no update.
	void InitRankings(void);
	void SetRankingsFreq(int mins);
	void ProcessRankings(void);
	void InitEventTimer(void);
	void DeInitEventTimer(void);
	void ProcessEventTimer(void);


	void OnPracticeGameType(char m_gametype);

	void HandleReplayCommand(CReplayStrike *strike);

	void HandleServerMessage(char *str, int i);
	void HandleServerMessageInvalid(char *str, int i);
	void HandleServerMessageMSG(char *str, int i);
	void HandleServerMessageTIME(char *str, int i);
	void HandleServerMessageHELP(char *str, int i);
	void HandleServerMessageSTATS(char *str, int i);
	void HandleServerMessageKICK(char *str, int i);
	void HandleServerMessageSTATUS(char *str, int i);
	void HandleServerMessageSETSTATUS(BYTE state, int i);
	void HandleServerMessageHOSTCHECK(int i);
	void HandleServerMessageBAN(char *str, int i);
	void HandleServerMessageUNBAN(char *str, int i);
	void HandleServerMessageHOSTCHECKTIMEOUT(char *str, int i);
	void HandleServerMessageGREETING(char *str, int i);
	void HandleServerMessageCLEARTABLE(char *str, int i);
	void HandleServerMessageBROADCAST(char *str, int i);
	void HandleServerMessageMODS(char *str, int i);
	void HandleServerMessageGATE(char *str, int i);
	void HandleServerMessageSERVER(char *str, int i);
	void HandleServerMessageSTING(char *str, int i);
	void HandleServerMessageCRASH(char *str, int i);
	void HandleServerMessageWallDenyBlock(char *str, int i, char mode);
	void HandleServerMessageUnWallDenyBlock(char *str, int i, char mode);
	void HandleServerMessageDEBUG(char *str, int i);
	void HandleServerMessageADDMOD(char *str, int i);
	void HandleServerMessageDELMOD(char *str, int i);
	void HandleServerMessageMODLIST(char *str, int i);
	void HandleServerMessageRATING(char *str, int i);
	void HandleServerMessagePLAYERS(char *str, int i);
	void HandleServerMessageTEXT(char *str, int i);
	void HandleServerMessageCUE(char *str, int i);
	void HandleServerMessageCHALK(char *str, int i);
	void HandleServerMessageBUY(char *str, int i); 
	void HandleServerMessageSELL(char *str, int i); 
	void HandleServerMessageAUTOINVITE(char *str, int i);
	void HandleServerMessageCLEARMOSTRUNOUTS(char *str, int i);
	void HandleServerMessageCLEARHIGHBREAKS(char *str, int i);
	void HandleServerMessageTOPUP(char *str, int i);
	void HandleServerMessageMOSTRUNOUTS(char *str, int i);
	void HandleServerMessageLEADERBOARD(char *str, int i);
	void HandleServerMessageMUTE(char *str, int i);
	void HandleServerMessageUNMUTE(char *str, int i);
	void HandleServerMessageSYSID(char *str, int i);
	void HandleServerMessageMACID(char *str, int i);
	void HandleServerMessageSPONSOR(char *str, int i);
	void HandleServerMessageSPONSORURL(char *str, int i);
	void HandleServerMessageSTEALTH(char *str, int i);
	void HandleServerMessageUNSTEALTH(char *str, int i);
	void HandleServerMessageSPONSORBACK(char *str, int i);

	void SendPlayerChangeData(int plrIdx, BYTE type, int round = -1, int roundval = -1);
	void ReceivePlayerChangeData(char *buffer, int siz);
	void ReceiveTableChangeData(char *buffer, int siz);

	BOOL InitiateHostCheck(int sockIdx);
	void SendKickRequest(void);
	void ReceiveKickRequest(int plrSockIdx);
	void GetTime(void);

	void PracticeEditRefreshBallSpots(int n);

	// Computer Code (This is for AI opponent play)
	void ComputerPracticeInit(void);
	void ComputerPracticeDeinit(void);
	void ComputerPracticeProcess(void);
	void ComputerPracticeDraw(void);
	void ComputerPracticeOnDraw(CDC* pDC, int x, int y);

	void ReportMatchTourney(int tid, char *name1P, int frames1P, char *name2P, int frames2P);
	
	void GetNextEvent();
	void ProcessEventBroadcasts(int playerSocketIdx = -1);
		
	void UpdateNextEventBroadcastTimes();
	void RemoveExpiredBroadcastTimes();
	void HTTPParseNextEvent(void* _lpBuf, int nBufLen);


	void UpdateTourney(void);
	void HTTPParseTourney(void *_lpBuf, int nBufLen);

	void HTTPStartGateway(int nErrorCode);
	void HTTPParseGateway(void *lpBuf, int nBufLen);
	void HTTPSocketSessionFailed(CHTTPSocketEx *pHTTPSocket);
	void HTTPSocketSessionEnd(void *lpBuf, int nBufLen, CHTTPSocketEx *pHTTPSocket);
	void HTTPSocketSessionStart(int nErrorCode, CHTTPSocketEx *pHTTPSocket);

	void HTTPParseMCode(void *lpBuf, int nBufLen);
	void HTTPRequestMemberShipCode(char *code);
	void AbortMatch(int tblIdx);
	void UpdateLobbyTVClient(int plrSockIdx, char tblIdx);
	void UpdateLobbyTVServer(int plrSockIdx, char tblIdx);
	void UpdateLobbyTableWatch(char tblIdx);
	void RefreshLeaderBoardsClients(void);
	bool LoadReplay(char *fname);

	bool FindRunningProcess(char *process);
	bool CheckCheatPrograms(void);

	void ButtonRefresh();
	void MouseWheel(short zDelta);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameAppView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGameAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Add data members


protected:
	CRect m_rectLast;	//** Added line
	BOOL m_bMouseTracking;	//** Added line



protected:


// Generated message map functions
protected:
	//{{AFX_MSG(CGameAppView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);


	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPracticeLineup();
	afx_msg void OnPracticeStart();
	afx_msg void OnPracticeColours();
	afx_msg void OnPracticeBlack();
	afx_msg void OnPracticeBilliards();
	afx_msg void OnAppAbout();
	afx_msg void OnInternetDetails();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnInternetLobby();
	afx_msg void OnInternetExit();
	afx_msg void OnUpdateInternetDetails(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInternetExit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInternetLobby(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeLineup(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeColours(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeBlack(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeBilliards(CCmdUI* pCmdUI);
	afx_msg void OnPracticeRedsscattered1();
	afx_msg void OnPracticeRedsscattered2();
	afx_msg void OnUpdatePracticeRedsscattered1(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeRedsscattered2(CCmdUI* pCmdUI);
	afx_msg void OnHelpOnline();
	afx_msg void OnOptionsReplaypanel();
	afx_msg void OnUpdateOptionsReplaypanel(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditSelectall();
	afx_msg void OnGamePlayerlist();
	afx_msg void OnUpdateGamePlayerlist(CCmdUI* pCmdUI);
	afx_msg void OnPracticeEdit();
	afx_msg void OnUpdatePracticeEdit(CCmdUI* pCmdUI);
	afx_msg void OnPracticeSave();
	afx_msg void OnUpdatePracticeSave(CCmdUI* pCmdUI);
	afx_msg void OnPracticeLoad();
	afx_msg void OnUpdatePracticeLoad(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeBallinhand(CCmdUI* pCmdUI);
	afx_msg void OnHelpFaq();
	afx_msg void OnHelpVersion();
	afx_msg void OnUpdateHelpVersion(CCmdUI* pCmdUI);
	afx_msg void OnInvitationsDeclineall();
	afx_msg void OnInvitationsDeclineguests();
	afx_msg void OnUpdateInvitationsDeclineguests(CCmdUI* pCmdUI);
	afx_msg void OnRegisterNewaccount();
	afx_msg void OnUpdateRegisterNewaccount(CCmdUI* pCmdUI);
	afx_msg void OnRegisterPurchase();
	afx_msg void OnUpdateRegisterPurchase(CCmdUI* pCmdUI);
	afx_msg void OnRegisterTopupaccount();
	afx_msg void OnUpdateRegisterTopupaccount(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInvitationsDeclineall(CCmdUI* pCmdUI);
	afx_msg void OnDebugActivate();
	afx_msg void OnDebugDeactivate();
	afx_msg void OnRabuseSend();
	afx_msg void OnFileLoad();
	afx_msg void OnUpdateFileLoad(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnGameStart();
	afx_msg void OnUpdateGameStart(CCmdUI* pCmdUI);
	afx_msg void OnGameStandup();
	afx_msg void OnUpdateGameStandup(CCmdUI* pCmdUI);
	afx_msg void OnGameSetup();
	afx_msg void OnUpdateGameSetup(CCmdUI* pCmdUI);
	afx_msg void OnGameRerack();
	afx_msg void OnUpdateGameRerack(CCmdUI* pCmdUI);
	afx_msg void OnGameInvite();
	afx_msg void OnUpdateGameInvite(CCmdUI* pCmdUI);
	afx_msg void OnGameExit();
	afx_msg void OnUpdateGameExit(CCmdUI* pCmdUI);
	afx_msg void OnGameConcede();
	afx_msg void OnUpdateGameConcede(CCmdUI* pCmdUI);
	afx_msg void OnGameBoot();
	afx_msg void OnUpdateGameBoot(CCmdUI* pCmdUI);
	afx_msg void OnGameUndo();
	afx_msg void OnUpdateGameUndo(CCmdUI* pCmdUI);
	afx_msg void OnPLAYERS1PVs2P();
	afx_msg void OnUpdatePLAYERS1PVs2P(CCmdUI* pCmdUI);
	afx_msg void OnHelpGamerules();
	afx_msg void OnUpdateHelpGamerules(CCmdUI* pCmdUI);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSponsorWebsite();
	afx_msg void OnUpdateWideScreenDisabled(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWideScreenEnabled(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	
	//}}AFX_MSG
	afx_msg void OnButtonStart();
	afx_msg void OnButtonReRack();
	afx_msg void OnButtonConcede(); //afx_msg void OnButtonTVMode();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonUndo(); //afx_msg void OnButtonStandUp();
	afx_msg void OnButtonInvite();
	afx_msg void OnButtonBoot();

	afx_msg void OnButtonSetup();
	afx_msg void OnButtonReportAbuse();
	afx_msg void OnButtonStand();
	afx_msg void OnButtonEndPractice();

	afx_msg void OnButtonCueError();
	afx_msg void OnButtonRated();
	afx_msg void OnTableTypeBoxSelect();
	afx_msg void OnButtonMissRule();
		
	afx_msg void OnButtonSay();
	afx_msg void OnButtonSmileySelect();

	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonEnterLobby();
	afx_msg void OnButtonViewTables();
	afx_msg void OnButtonHome();
	afx_msg void OnButtonRegister();
	afx_msg void OnButtonTourneys();
	afx_msg void OnButtonLeagues();
	afx_msg void OnButtonChangeLobby();
	afx_msg void OnButtonMBFullLobby();
	
	
	afx_msg void OnButtonServerConfig();
	afx_msg void OnButtonAccountConfig();
	afx_msg void OnButtonLobbyStat();
	afx_msg void OnButtonHelpLoginError();
	afx_msg void OnHelpMessage();
	
	afx_msg void OnPracticeA();
	afx_msg void OnPracticeB();
	afx_msg void OnPracticeC();
	afx_msg void OnPracticeD();
	afx_msg void OnPracticeE();
	afx_msg void OnPracticeF();
	afx_msg void OnPracticeG();
	afx_msg void OnPracticeH();
	afx_msg void OnPracticeI();
	afx_msg void OnUpdatePracticeA(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeB(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeC(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeD(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeE(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeF(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeG(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeH(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePracticeI(CCmdUI* pCmdUI);

	afx_msg void OnChatEditExLink(NMHDR* in_pNotifyHeader, LRESULT* out_pResult);
	afx_msg void OnSendEditLink(NMHDR* in_pNotifyHeader, LRESULT* out_pResult);

	afx_msg void OnWideScreenDisabled();
	afx_msg void OnWideScreenEnabled();

	afx_msg void OnCompositionChanged(); 
 

	afx_msg void OnDropFile( HDROP hDropInfo );

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnShowFPS();

	afx_msg void OnMousesensitivity1();
	afx_msg void OnMousesensitivity2();
	afx_msg void OnMousesensitivity3();
	afx_msg void OnMousesensitivity4();
	afx_msg void OnMousesensitivity5();
	afx_msg void OnMousesensitivity6();
	afx_msg void OnMousesensitivity7();
	afx_msg void OnMousesensitivity8();
	afx_msg void OnMousesensitivity9();
	
	afx_msg void OnOptionsShowfps();
	afx_msg void OnAchievementsMyachievements();
	afx_msg void OnStatsLadderrankings();
};

#ifndef _DEBUG  // debug version in GameAppView.cpp
inline CGameAppDoc* CGameAppView::GetDocument()
   { return (CGameAppDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

extern FILE *flogptr;

extern CString g_nick;
extern CMenu *g_menu; // pointer to MainFrame's menu
extern BOOL g_bMenuSelect;
extern int g_menuNItemID;
extern int g_menuNItemIDOld;
extern void RectangleShaded(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF color);
extern void RectangleShaded2(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF color);
extern void RectangleShaded3(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF color);
extern BOOL g_bClose;
extern CGameAppView *g_gameAppView;
extern BOOL g_bNewMessage;
extern int g_bpp;
extern int g_guestcount;
extern char *g_ctbl[];

extern int g_pottedSpots[];
extern int g_pottedStripes[];

extern BallsSendData g_ballsSendDataBk;
extern CueBallTipSendDataC g_CueBallTipDataCBk;
extern ballPosSendData g_ballPosDataBk;
extern selectBallSendData g_selectBallDataBk;
extern char g_ResetbufferBk[16];
extern mouseMoveSendData g_mouseMoveDataBk;
extern char g_PlayAgainbufferBk[4];
extern char g_FoulAlertbufferBk[4];
extern char g_MissResetbufferBk[4];
extern MaxTimeUpSendData g_MaxTimeUpDataBk;
extern PlayerTargetSendData g_playerTargetDataBk;
extern char g_Pot8OnBreakRerackbufferBk[4];
extern char g_BadBreakRerackbufferBk[4];


extern int g_strikePowerError;
extern BOOL g_bBallRollOff;
extern BOOL g_bBallJustHitEnergy;
extern BOOL g_bNewParallelCushionLoss;
extern BOOL g_bTextDoubleError;
extern BOOL g_bDetectionO;
extern BOOL g_bTextDoubleErrorAdded;
extern BOOL g_bStrikeDownTable;
extern int g_iPhysicsType;		// 0: Original Phyiscs, 1: 2012 Phyiscs, 2: 2013 Phyiscs


class CConfig {
public:
BOOL m_10Reds;
CString	m_accountSettings;
BOOL m_accountSettingsTick;
BOOL m_boot;
BOOL m_chat;
CString m_filenameRankings;
int	m_guests;
BOOL m_invite;
BOOL m_matchTableAccess;
BOOL m_tableType;
int m_users;
int m_postRankings;
CString m_crcvalid; // e.g. '0xface1234' or empty
int m_day;
int m_month;
int m_year;
int m_freeaccounts;
int m_logTableChat;
BOOL m_mostRunOutsPrize;
};

extern CConfig g_config;
extern char g_ExeName[80];

#define  GAMETYPE_SNOOKER		0
#define  GAMETYPE_POOL			1
#define  GAMETYPE_POOLUK		2

class CBuild {
public:
BOOL main_server;
BOOL test_server;
BOOL addOnSfx;
DWORD gameType;
char version[80]; //char *version; <- this was a potential crash bug!!!
char savepath[80];
};

extern CBuild g_build;


// CRC Check Code ///////////////////////////////////////////////////////////

extern DWORD g_crc;

extern DWORD CRC_Check(char *fname);

extern void DateToStr(char *str, int day, int month, int year);
extern void StrToDate(char *str, int *day, int *month, int *year);
extern int DaysLeft(int c_d,int c_m,int c_y, 
					int x_d,int x_m,int x_y);

extern int g_clkprobe[];
extern int g_clkprobeavg[];
extern char g_basedir[256];
extern CString g_appName;


#define SECTOR_NONE		0
#define SECTOR_TABLE	1
#define SECTOR_MINI		2

extern int g_mouseSector;
extern CWnd *g_wnd;
extern CWnd *g_previousWndEditFocus;
extern int g_previousWndNo;
extern DWORD g_sysID; // unique system ID!
extern WORD g_cpuID; // unique system ID!
extern DWORD g_macID_U; // unique MAC ID Upper Bytes!
extern WORD g_macID_L; // unique MAC ID Lower Bytes!
extern BOOL g_logTableChat;






#endif // __GAMEAPPVIEW_H__
