// LobbySelect.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "LobbySelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLobbySelect dialog


CLobbySelect::CLobbySelect(CWnd* pParent /*=NULL*/)
	: CDialog(CLobbySelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLobbySelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
	m_bEraseBkgnd = TRUE;
}


void CLobbySelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLobbySelect)
	DDX_Control(pDX, IDSELECT, m_enterlobby);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLobbySelect, CDialog)
	//{{AFX_MSG_MAP(CLobbySelect)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDSELECT, OnSelect)
	ON_BN_CLICKED(IDHELP, OnHelp)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLobbySelect message handlers

void CLobbySelect::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

int CLobbySelect::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();	

	CRect rectClient;
	GetClientRect(&rectClient);

		// create child listctrl players window
	CRect rect;

	//rect.SetRect(0,0,165+20+18,200+15);
	rect.SetRect(20,40,rectClient.right-20,rectClient.bottom-40);
	//rect.OffsetRect(0,40);
	m_listLobbies.Create(LVS_OWNERDRAWFIXED | LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER, rect, this, IDC_LISTCTRLPLAYERS+123);
	m_listLobbies.PassParent(m_pParent);
	
	m_listLobbies.AllowColumnSizing(FALSE);
	m_listLobbies.SetGridFlags(CListCtrlEx::GRIDVERT | CListCtrlEx::TEXTCOLOR | CListCtrlEx::LOBBYSELECT);
	m_listLobbies.CreateColumn(0, "Lobby", 80);
	m_listLobbies.CreateColumn(1, "Description", 220);
	m_listLobbies.CreateColumn(2, "Players", 80);
	m_listLobbies.CreateColumn(3, "Ver", 90);
	

	
	m_listLobbies.SetBkColor(RGB(100,100,100));
	m_listLobbies.SetGridColor(0, RGB(155,155,155));
	m_listLobbies.SetGridColor(1, RGB(200,200,200));
	m_listLobbies.m_crTextColor = RGB(255,255,255);
	m_listLobbies.m_crTextBkColor = RGB(100,100,100);
//	m_listLobbies.ShowScrollBar(SB_VERT,FALSE);
//	m_listLobbies.EnableScrollBarCtrl(SB_VERT,FALSE);
	m_listLobbies.SetItemCount(64);


	//m_enterlobby.Create(
	//m_enterlobby.EnableWindow(FALSE);
	
	/*CString str[2];
	str[0] = "Lobby1";
	str[1] = "Desc1";
	m_listLobbies.CreateItem(0, str);
	
	str[0] = "Lobby2";
	str[1] = "Desc2";
	m_listLobbies.CreateItem(1, str);
	*/

	return 0;
}

void CLobbySelect::OnSelect() 
{
	// TODO: Add your control notification handler code here
	Select();
}

void CLobbySelect::Select(void)
{
	if(((CGameAppView *)m_pParent)->m_lobbySelectSuccess)
	{
	BOOL bSelectDone;
	bSelectDone = ((CGameAppView *)m_pParent)->OnLobbySelect(m_listLobbies.m_lastSelectedNItem);
	if(bSelectDone)
		{
		//EndDialog(0);
		DestroyWindow();
		}
	}
}

void CLobbySelect::Delta(int sel)
{

	if(!((CGameAppView *)m_pParent)->m_lobbySelectSuccess)
	{
	//AfxMessageBox("!((CGameAppView *)m_pParent)->m_lobbySelectSuccess");
	m_enterlobby.EnableWindow(FALSE);
	((CGameAppView *)m_pParent)->m_EnterLobbyButton.EnableWindow(FALSE);
	}
	else {
			//AfxMessageBox("TRUE");
			if(sel==-1)
			{
				m_enterlobby.EnableWindow(FALSE);
				((CGameAppView *)m_pParent)->m_EnterLobbyButton.EnableWindow(FALSE);
			}
			else 
			{
				m_enterlobby.EnableWindow(TRUE);
				((CGameAppView *)m_pParent)->m_EnterLobbyButton.EnableWindow(TRUE);
			}
		}
}

void CLobbySelect::OnHelp() 
{
	// TODO: Add your control notification handler code here
	if(g_build.gameType == GAMETYPE_SNOOKER)
		ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.thesnookerclub.com/faq/lobbyselect.html"),_T(""),NULL,0);
	else if(g_build.gameType == GAMETYPE_POOL)
		ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.thepoolclub.com/faq/lobbyselect.html"),_T(""),NULL,0);	
}

BOOL CLobbySelect::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_bEraseBkgnd)
		return CDialog::OnEraseBkgnd(pDC);
	else
		return 0;
}

void CLobbySelect::SetEraseBkgnd(BOOL bErase)
{
	m_bEraseBkgnd = bErase;
}