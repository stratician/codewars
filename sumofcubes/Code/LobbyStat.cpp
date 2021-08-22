// CLobbyStat

#include <math.h>

#include "../CodeSupp/stdafx.h"
#include "LobbyStat.h"
#include "../CodeSupp/LobbyStatWnd.h"
			
CLobbyStat *g_lobbystat;

CLobbyStat::CLobbyStat(CWnd *pParent)
{
	m_pParent = pParent;
	Init(&m_data);
	Load(&m_data);

	m_lobbyStatWnd = new CLobbyStatWnd;
	m_lobbyStatWnd->m_pParent = this;

	//m_lobbyStatWnd->Appear();
	//m_lobbyStatWnd->Disappear();
}

CLobbyStat::~CLobbyStat()
{
	Save(&m_data);	
	delete m_lobbyStatWnd;
}

void CLobbyStat::Init(LStatData *data)
{
	int n;
	data->version = LOBBYSTAT_VERSION;
	for(n=0;n<60*24;n++)
	{
	data->playersDay[n] = 0;
	data->playersDayAvg[n] = 0;
	data->minuteDay[n] = 0;
	}
	for(n=0;n<366;n++)
	{
	data->playersYear[n] = 0;
	data->membersYear[n] = 0;
	data->gamesYear[n] = 0;
	data->sentYear[n] = 0;
	data->receiveYear[n] = 0;
	data->gamesTotal = 0;
	data->loginTotal = 0;
	data->sentTotal = 0;
	data->receiveTotal = 0;
	}
}

void CLobbyStat::Load(LStatData *data)
{
	FILE *fptr;
	fptr = fopen("lobbystats.dat","rb");
	if(fptr==NULL) return;

	fread(data, sizeof(LStatData), 1, fptr);
	fclose(fptr);
}

void CLobbyStat::Save(LStatData *data)
{
	FILE *fptr;
	fptr = fopen("lobbystats.dat","wb");
	if(fptr==NULL) return;
	fwrite(data, sizeof(LStatData), 1, fptr);
	fclose(fptr);
}

void CLobbyStat::UpdateMinute(void)
{
	int minute_idx;
	CTime timeCurrent = CTime::GetCurrentTime();
	minute_idx = timeCurrent.GetHour() * 60;
	minute_idx+= timeCurrent.GetMinute();
	if(minute_idx<0) minute_idx = 0;
	else if(minute_idx>=60*24) minute_idx = 60*24-1;
	// ------------------ //
	m_data.playersDay[minute_idx] = m_xPlayers;
	m_data.playersDayAvg[minute_idx]+= m_xPlayers;
	m_data.minuteDay[minute_idx]++;

	// Repaint Window //
	if(m_lobbyStatWnd->m_bActive)
	m_lobbyStatWnd->Invalidate(); // causes an ondraw message

	Save(&m_data);
}

void CLobbyStat::Appear(void)
{
	m_lobbyStatWnd->Appear();
	
}
