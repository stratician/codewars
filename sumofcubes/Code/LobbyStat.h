// LobbyStat.h
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __LOBBYSTAT_H__
#define __LOBBYSTAT_H__

#define LOBBYSTAT_VERSION	0x0001

typedef struct {
DWORD version;
BYTE playersDay[60*24];		// 1.44k (Last 24 Hour period of players online count) e.g. 22
WORD playersDayAvg[60*24];	// 2.88k (24 Hour period average) e.g 22
WORD minuteDay[60*24];		// count of how many times a minute a has been updated useful for calcing averages
BYTE playersYear[366];		// 366 bytes e.g. 300 players Monday, 350 players tuesday etc.
DWORD membersYear[366];		// 1.4k members count.
DWORD gamesYear[366];		// 1.4k # games completed each day.
DWORD sentYear[366];		// 1.4k Kbytes sent each day (max of 4TB)
DWORD receiveYear[366];		// 1.4k Kbytes receive each day
DWORD gamesTotal;			// Total games completed ever.
DWORD loginTotal;			// Total logins ever.
DWORD sentTotal;			// total data in kbytes sent, (max of 4TB)
DWORD receiveTotal;			// -received.
} LStatData;

class CLobbyStatWnd;

class CLobbyStat {
public:
CWnd *m_pParent;
LStatData m_data;
// Transfers
BYTE m_xPlayers;
CLobbyStatWnd *m_lobbyStatWnd;
public:
CLobbyStat(CWnd *pParent);
~CLobbyStat();
void Init(LStatData *data);
void Load(LStatData *data);
void Save(LStatData *data);
void UpdateMinute(void);
void Appear(void);
};

extern CLobbyStat *g_lobbystat;

#endif // __LOBBYSTAT_H__