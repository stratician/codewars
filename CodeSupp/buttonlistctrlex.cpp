// ButtonListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ButtonListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonListCtrlEx

CButtonListCtrlEx::CButtonListCtrlEx()
{
	m_pParent = NULL;
	m_listCtrlIdx = 0; // Table #0 / Watch
	m_seated = 0;	// Defaultly no one is seated at this Button //
}

CButtonListCtrlEx::~CButtonListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CButtonListCtrlEx, CButtonOwnerEx)//CButton)
	//{{AFX_MSG_MAP(CButtonListCtrlEx)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonListCtrlEx message handlers

void CButtonListCtrlEx::OnClicked() 
{
	return;
	// TODO: Add your control notification handler code here
	CListCtrlEx *listCtrlExParent = (CListCtrlEx *)GetParent();
	if(listCtrlExParent)
	{
		//char buffer[80];
		//sprintf(buffer,"%d",listCtrlExParent->m_totalItems);
		//MessageBox(buffer,"",MB_OK);
	}

	//CGameAppView *gameAppViewParent = (CGameAppView *)listCtrlExParent->GetParent(); // old way - gameappview parent
	CGameAppView *gameAppViewParent = (CGameAppView *)listCtrlExParent->m_pParent; // new way - lobbytabctrl parent
	if(gameAppViewParent)
	{
		//char buffer[80];
		//sprintf(buffer,"%d",gameAppViewParent->m_cyc);
		//MessageBox(buffer,"",MB_OK);
	
		if((m_listCtrlIdx%4) >0)	// Don't do anything if we Clicked 'View' Button
			{
			if(m_enabled)
			if(gameAppViewParent->m_tableNo == -1)
				{
				gameAppViewParent->m_tableNo = m_listCtrlIdx/4;
				gameAppViewParent->m_tableEnter = (m_listCtrlIdx%4)-1; // 0 - View / 1 - Watch / 2 - 1P Join / 3 - 2P Join
				}
			}
		else{
			// 'Lobby TV' Button Click, so we need to update which Table We're watching //
			g_LobbyTVInfo.lobbytablewatch = m_listCtrlIdx/4;
			g_gameAppView->UpdateLobbyTableWatch(m_listCtrlIdx/4);
			// Update the Server Also //
			g_gameAppView->UpdateLobbyTVServer(g_gameAppView->m_playerSockIdx, m_listCtrlIdx/4);
			}
		
	}
}

