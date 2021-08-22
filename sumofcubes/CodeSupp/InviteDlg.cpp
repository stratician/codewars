// InviteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "InviteDlg.h"
#include "../Code/Common.h"
#include "../Code/Lobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInviteDlg dialog


CInviteDlg::CInviteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInviteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInviteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CInviteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInviteDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInviteDlg, CDialog)
	//{{AFX_MSG_MAP(CInviteDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInviteDlg message handlers

void CInviteDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CInviteDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CInviteDlg::OnOK() 
{
	// TODO: Add extra validation here
	int invitedPlrSockIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerSockIdx(m_listPlayers.m_lastSelectedName);
	if( (invitedPlrSockIdx != -1) && (invitedPlrSockIdx != ((CGameAppView *)m_pParent)->m_playerSockIdx) )
	{
		char name[22];
		char invitedName[22];
		int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
		int invitedPlrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(invitedPlrSockIdx);
		if(plrIdx!=-1 && invitedPlrIdx!=-1)
		{
			int plrTblIdx = ((CGameAppView *)m_pParent)->m_lobby->GetTableIdxFromPlayer(((CGameAppView *)m_pParent)->m_playerSockIdx);
			((CGameAppView *)m_pParent)->SendInvitePressed(((CGameAppView *)m_pParent)->m_playerSockIdx, invitedPlrSockIdx,
															m_player[plrIdx].name, m_player[invitedPlrIdx].name, FALSE,
															NULL, plrTblIdx); // network game only!
		}
	}

	CDialog::OnOK();
}

int CInviteDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(g_build.main_server) return -1;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();	
	
	// create child listctrl players window
	CRect rect;
	rect.SetRect(0,0,165+20+18,200+15);
	rect.OffsetRect(0,40);
	m_listPlayers.Create(LVS_OWNERDRAWFIXED | LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER, rect, this, IDC_LISTCTRLPLAYERS+123);
	m_listPlayers.PassParent(m_pParent);

	// set the appropriate coloumn titles
	CString strName;
	CString strRating;
	CString strTable;
	
	if(m_listPlayers.m_bToggleName) strName = _T("*Name");
	else strName = _T("Name");

	if(m_listPlayers.m_bToggleRating) strRating = _T("*Rtng");
	else strRating = _T("Rating");

	if(m_listPlayers.m_bToggleTable) strTable = _T("*Tbl");
	else strTable = _T("TableB");
	
	//m_listPlayers.AllowColumnSizing(FALSE);

	m_listPlayers.SetColumnFontHeight(1.6f);
	m_listPlayers.TriggerOnMeasureItem(IDC_LISTCTRLPLAYERS+123); // calls this to set the item height, based on font height

	m_listPlayers.SetGridFlags(CListCtrlEx::GRIDVERT | CListCtrlEx::TEXTCOLOR);
	m_listPlayers.CreateColumn(0, strName.GetBuffer(0), 80+20);
	m_listPlayers.CreateColumn(1, strRating.GetBuffer(0), 45);
	m_listPlayers.CreateColumn(2, strTable.GetBuffer(0), 40);
	m_listPlayers.SetBkColor(RGB(222,200,135));
	m_listPlayers.SetGridColor(0, RGB(55,55,55));
	m_listPlayers.SetGridColor(1, RGB(20,20,20));
	m_listPlayers.m_crTextColor = RGB(255,255,255);
	m_listPlayers.m_crTextBkColor = RGB(0,95,239);
//	m_listPlayers.ShowScrollBar(SB_VERT,FALSE);
//	m_listPlayers.EnableScrollBarCtrl(SB_VERT,FALSE);
	m_listPlayers.SetItemCount(MAX_LOBBY_PLAYERS);
			
	RefreshPlayerItem();

/*
	// create child scrollbar window for listctrl players window
	rect.SetRect(0,0,165+20,200+15);
	rect.OffsetRect(0,40);
	m_scrollBar.Create(SBS_RIGHTALIGN | SBS_VERT | WS_CHILD | WS_VISIBLE, rect, this, IDC_LISTCTRLPLAYERS+123);
	m_scrollBar.ShowScrollBar(SB_VERT);
	//m_scrollBar.SetScrollRange(SB_VERT,0, INT_MAX);
	m_scrollBar.EnableScrollBar();
	EnableScrollBarCtrl(SB_VERT);
*/		
	return 0;
}

BOOL CInviteDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_listPlayers.DestroyWindow();
	
	return CDialog::DestroyWindow();
}

void CInviteDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
	static BOOL firstTime = TRUE;
	if(firstTime)
	{
		firstTime = FALSE;
	//	m_listPlayers.ShowScrollBar(SB_VERT);
	}
	
//	m_listPlayers.ShowScrollBar(SB_VERT);
//	m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CInviteDlg::SortPlayerRatings(void)
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
			//AddPlayerItem(m_player[m].name, m_player[m].rating, m_player[m].table+1);
			CString name = m_player[m].name;
			int rating = m_player[m].rating;
			int tableNo = m_player[m].table+1;
			CString items[3];
			items[0] = name;
			items[1].Format("%d", rating);
			if(!tableNo) items[2].Format("");
			else items[2].Format("%d", tableNo);
		
			m_listPlayers.CreateItem(m_listPlayers.m_totalItems, items );
		}

		bestRatingLimit = bestRating;
		bestRating = 0;
		bestIdx = -1;
		}
		else break;
	}
}

void CInviteDlg::SortPlayerTables(void)
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
			//AddPlayerItem(m_player[m].name, m_player[m].rating, m_player[m].table+1);
			CString name = m_player[m].name;
			int rating = m_player[m].rating;
			int tableNo = m_player[m].table+1;
			CString items[3];
			items[0] = name;
			items[1].Format("%d", rating);
			if(!tableNo) items[2].Format("");
			else items[2].Format("%d", tableNo);
		
			m_listPlayers.CreateItem(m_listPlayers.m_totalItems, items );
		}

		bestTableLimit = bestTable;
		bestTable = 50;
		bestIdx = -1;
		}
		else break;
	}
}

void CInviteDlg::SortPlayerNames(void)
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
			//AddPlayerItem(m_player[m].name, m_player[m].rating, m_player[m].table+1);
			CString name = m_player[m].name;
			int rating = m_player[m].rating;
			int tableNo = m_player[m].table+1;
			CString items[3];
			items[0] = name;
			items[1].Format("%d", rating);
			if(!tableNo) items[2].Format("");
			else items[2].Format("%d", tableNo);
		
			m_listPlayers.CreateItem(m_listPlayers.m_totalItems, items );
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

void CInviteDlg::RefreshPlayerItem(void)
{
//BAP_FIX //
//CInviteDlg is not Created in MAIN_SERVER MODE //
//#ifndef MAIN_SERVER
/////////////////////////////////////////

	
	if(!m_hWnd) return;
	if( !IsWindowEnabled() ) return;
	


	m_listPlayers.m_totalItems = 0;


	//char buffer[256];
	//sprintf(buffer, "N = %d, R = %d, T = %d", m_listPlayers.m_bToggleName, m_listPlayers.m_bToggleRating, m_listPlayers.m_bToggleTable);
	//ErrorMessage(m_pParent, buffer);	
	if(!m_listPlayers.m_bToggleName && 
	   !m_listPlayers.m_bToggleRating &&
	   !m_listPlayers.m_bToggleTable) // default
	{
		for(int g=0;g<6;g++) 
		for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
		{
			if( !((CGameAppView *)m_pParent)->m_lobby->IsPlayerInGroup(n, g) ) continue;

			if(m_player[n].status)
			{
				//AddPlayerItem(m_player[n].name, m_player[n].rating, m_player[n].table+1);
				CString name = m_player[n].name;
				int rating = m_player[n].rating;
				int tableNo = m_player[n].table+1;
				CString items[3];
				items[0] = name;
				items[1].Format("%d", rating);
				if(!tableNo) items[2].Format("");
				else items[2].Format("%d", tableNo);
			
				m_listPlayers.CreateItem(m_listPlayers.m_totalItems, items );
			}
		}
	}
	else if(m_listPlayers.m_bToggleName) // Name
	{
		SortPlayerNames();
	}
	else if(m_listPlayers.m_bToggleRating) // Rating
	{
		SortPlayerRatings();
	}
	else if(m_listPlayers.m_bToggleTable) // Table
	{
		SortPlayerTables();
	}


	//m_listPlayers.RedrawItems(0, m_listPlayers.m_totalItems);


	// clear redraw flag - while we add items to list
	m_listPlayers.SetRedraw(FALSE);

	// clear dead players from end of list
	for(int n=m_listPlayers.m_totalItems;n<MAX_LOBBY_PLAYERS;n++)
	{
		m_listPlayers.DeleteItem(n);
	}

	// Set redraw flag - since we've finished adding items to list
	m_listPlayers.SetRedraw(TRUE);
	


	// search list for previous selection name (not the player list!)
	for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
	{
		CString str;
			
		str = m_listPlayers.GetItemText(n, 0);
		if( strcmpi(str, m_listPlayers.m_lastSelectedName) == 0 )
		{
			m_listPlayers.SetItemState(n, LVIS_SELECTED, LVIS_SELECTED);
			break;
		}		
	}

	// we must force the scrollbar to be on, since SetRedraw screws with this //
	m_listPlayers.ShowScrollBar(SB_VERT);


// CInviteDlg isn't Created in MAIN_SERVER MODE //
//#endif
// END of BAP_FIX

}

void CInviteDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	RefreshPlayerItem();
	
	CDialog::OnTimer(nIDEvent);
}

void CInviteDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	if(m_listPlayers) m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
}

void CInviteDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_listPlayers) m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
