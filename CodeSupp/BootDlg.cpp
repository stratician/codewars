// BootDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "BootDlg.h"
#include "../Code/Common.h"
#include "../Code/Lobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBootDlg dialog


CBootDlg::CBootDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBootDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBootDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CBootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBootDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBootDlg, CDialog)
	//{{AFX_MSG_MAP(CBootDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBootDlg message handlers

void CBootDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CBootDlg::OnOK() 
{
	// TODO: Add extra validation here

	int bootedPlrSockIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerSockIdx(m_listPlayers.m_lastSelectedName);
	if( (bootedPlrSockIdx != -1) && (bootedPlrSockIdx != ((CGameAppView *)m_pParent)->m_playerSockIdx) )
	{
		((CGameAppView *)m_pParent)->SendBootPressed(((CGameAppView *)m_pParent)->m_playerSockIdx, bootedPlrSockIdx); // network game only!
	}

	CDialog::OnOK();
}

void CBootDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

int CBootDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();	

	// Create child listbox window
	CRect rect;
	rect.SetRect(0,0,165+18-1,200+15);
	rect.OffsetRect(0,40);
	m_listPlayers.Create(LVS_NOCOLUMNHEADER | LVS_OWNERDRAWFIXED | LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER, rect, this, IDC_WINDOWLISTBOX);
	m_listPlayers.PassParent(m_pParent);
	m_listPlayers.SetGridFlags(CListCtrlEx::GRIDVERT | CListCtrlEx::TEXTCOLOR);
	m_listPlayers.CreateColumn(0, "Name", 80 + 30);
	m_listPlayers.CreateColumn(1, "Rating", 80+2+3 - 30);
	m_listPlayers.SetBkColor(RGB(20,20,20));
	m_listPlayers.SetGridColor(0, RGB(155,155,155));
	m_listPlayers.SetGridColor(1, RGB(200,200,200));
	m_listPlayers.m_crTextColor = RGB(255,255,255);
	m_listPlayers.m_crTextBkColor = RGB(55,55,55);

	RefreshPlayerItem();
	
	return 0;
}

void CBootDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_listPlayers.DestroyWindow();
}

void CBootDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CBootDlg::RefreshPlayerItem(void)
{
#ifndef MAIN_SERVER
	if( !IsWindowEnabled() ) return;
	
	
	m_listPlayers.m_totalItems = 0;

	int tblIdx = ((CGameAppView *)m_pParent)->m_lobby->GetTableIdxFromPlayer(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(tblIdx!=-1)
	for(int g=0;g<6;g++)
	for(int n=0;n<MAX_LOBBY_PLAYERS;n++)
	{
		if( !((CGameAppView *)m_pParent)->m_lobby->IsPlayerInGroup(n, g) ) continue;

		if(m_player[n].status && m_player[n].table == tblIdx)
		{
			//AddPlayerItem(m_player[n].name, m_player[n].rating, m_player[n].table+1);
			CString name = m_player[n].name;
			int rating = m_player[n].rating;
			int tableNo = m_player[n].table+1;
			CString items[2];
			items[0] = name;
			items[1].Format("%d", rating);
				
			m_listPlayers.CreateItem(m_listPlayers.m_totalItems, items );
		}
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

#endif
}

void CBootDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	if(m_listPlayers) m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
}

void CBootDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_listPlayers) m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
