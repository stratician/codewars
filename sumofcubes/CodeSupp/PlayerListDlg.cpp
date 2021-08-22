// PlayerListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "PlayerListDlg.h"
#include "../Code/Common.h"
#include "../Code/Lobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerListDlg dialog


CPlayerListDlg::CPlayerListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CPlayerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerListDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerListDlg)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerListDlg message handlers

void CPlayerListDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CPlayerListDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

int CPlayerListDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();	
	
	// create child listctrl players window
	CRect rect;
	rect.SetRect(0,0,165+18+20,200+15);
	rect.OffsetRect(0,40);
	m_listPlayers.Create(LVS_OWNERDRAWFIXED | LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER, rect, this, IDC_LISTCTRLPLAYERS+123);
	m_listPlayers.PassParent(m_pParent);
	
	m_listPlayers.AllowColumnSizing(FALSE);
	m_listPlayers.SetGridFlags(CListCtrlEx::GRIDVERT | CListCtrlEx::TEXTCOLOR);

	m_listPlayers.SetColumnFontHeight(1.6f);
	m_listPlayers.TriggerOnMeasureItem(IDC_LISTCTRLPLAYERS+123); // calls this to set the item height, based on font height


	// set the appropriate coloumn titles
	CString strName;
	CString strRating;
	CString strTable;
	
	if(m_listPlayers.m_bToggleName) strName = _T("*Name");
	else strName = _T("Name");

	if(m_listPlayers.m_bToggleRating) strRating = _T("*Rtng");
	else strRating = _T("Rating");

	if(m_listPlayers.m_bToggleTable) strTable = _T("*Tbl");
	else strTable = _T("TableC");
	
	m_listPlayers.CreateColumn(0, strName.GetBuffer(0), 80+20);
	m_listPlayers.CreateColumn(1, strRating.GetBuffer(0), 45);
	m_listPlayers.CreateColumn(2, strTable.GetBuffer(0), 40);
	
	//m_listPlayers.CreateButtons(MAX_TABLEBUTTONS, TRUE);//--
	//m_listPlayers.EnableItemButton(0, TRUE);//--
	//m_listPlayers.EnableItemButton(1, TRUE);//--
	m_listPlayers.SetBkColor(RGB(222,200,135));
	m_listPlayers.SetGridColor(0, RGB(255,255,255));
	m_listPlayers.SetGridColor(1, RGB(200,200,200));
	m_listPlayers.m_crTextColor = RGB(255,255,255);
	m_listPlayers.m_crTextBkColor = RGB(0,0,0);
	//m_listPlayers.ShowScrollBar(SB_VERT,FALSE);
	//m_listPlayers.EnableScrollBarCtrl(SB_VERT,FALSE);
	m_listPlayers.SetItemCount(MAX_LOBBY_PLAYERS);

	RefreshPlayerItem();

	
	return 0;
}

BOOL CPlayerListDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_listPlayers.DestroyWindow();
	
	return CDialog::DestroyWindow();
}

void CPlayerListDlg::SortPlayerRatings(void)
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

void CPlayerListDlg::SortPlayerTables(void)
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

void CPlayerListDlg::SortPlayerNames(void)
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

void CPlayerListDlg::RefreshPlayerItem(void)
{
//BAP_FIX //
//CInviteDlg is not Created in MAIN_SERVER MODE //
//#ifndef MAIN_SERVER
///////////////////////////////////////////////


	if(!m_hWnd) return;
	if( !IsWindowEnabled() ) return;
	
	m_listPlayers.m_totalItems = 0;

	

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



	// Set redraw flag - since we've finished adding items to list
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


// add these lines in ontimer fn. for debugging!
//	char buff[80];
//	sprintf (buff,"GetCount(..) = %d (m_totalItems = %d)", m_playerListDlg.m_listPlayers.GetItemCount(), m_playerListDlg.m_listPlayers.m_totalItems);
//	ErrorMessage(this, buff);

void CPlayerListDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	if(m_listPlayers) 
		{
		m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

		}
}

void CPlayerListDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_listPlayers) m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
