// StatsTabCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "StatsTabCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatsTabCtrlDlg dialog


CStatsTabCtrlDlg::CStatsTabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatsTabCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatsTabCtrlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
	m_pStatsDlg = NULL;
	m_bActive = 0;
}

void CStatsTabCtrlDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CStatsTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsTabCtrlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatsTabCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CStatsTabCtrlDlg)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsTabCtrlDlg message handlers

int CStatsTabCtrlDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	int state = PLRSTATE_AVAILABLE;
	int plrIdx = -1;
	CenterWindow();

	// create child tabctrl player-stats window
	CRect tmpRect;
	CSize tmpSize;
	CPoint tmpPoint;
	GetWindowRect(&tmpRect);
	tmpSize = tmpRect.Size();
	tmpPoint = tmpRect.TopLeft();

	CRect rect;
	rect.SetRect(0,0,tmpSize.cx, tmpSize.cy-20);
	m_tabStats.Create(TCS_OWNERDRAWFIXED | TCS_SINGLELINE | WS_CHILD | WS_VISIBLE, rect, this, IDD_STATSTABCTRL);
	m_tabStats.PassParent(m_pParent);

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		((CGameAppView *)m_pParent)->m_statsDlg.Create(CStatsDlg::IDD, &m_tabStats);
		((CGameAppView *)m_pParent)->m_personalDetailsDlg.Create(CPersonalDetailsDlg::IDD, &m_tabStats);
		((CGameAppView *)m_pParent)->m_tournamentsDlg.Create(CTournamentsDlg::IDD, &m_tabStats);
		m_tabStats.CreateItem(0, "Stats.", (CWnd *) &((CGameAppView *)m_pParent)->m_statsDlg);
		m_tabStats.CreateItem(1, "Bio.", (CWnd *) &((CGameAppView *)m_pParent)->m_personalDetailsDlg);
		m_tabStats.CreateItem(2, "History.", (CWnd *) &((CGameAppView *)m_pParent)->m_tournamentsDlg);
		
		/*
		// Only Create if we are Selecting our own BIO //
		if(!strcmpi(m_player[((CGameAppView *)m_pParent)->m_playerIdx].name, ((CGameAppView *)m_pParent)->m_statsDlg.m_name))
			{
			((CGameAppView *)m_pParent)->m_shopDlg.Create(CShopDlg::IDD, &m_tabStats);
			m_tabStats.CreateItem(3, "Buy - Change Cue / Text", (CWnd *) &((CGameAppView *)m_pParent)->m_shopDlg);
			}
		*/
		
		m_tabStats.SetNormalColor(RGB(75, 75 ,75));
		m_tabStats.SetSelectedColor(RGB(0, 0, 0));

		// get plrIdx from stats dlg's player name field
		plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(((CGameAppView *)m_pParent)->m_statsDlg.m_name);
	}
	else // iPool
	{
		((CGameAppView *)m_pParent)->m_statsDlg2.Create(CStatsDlg2::IDD, &m_tabStats);
		((CGameAppView *)m_pParent)->m_personalDetailsDlg.Create(IDD_PERSONALDETAILS2, &m_tabStats);
		((CGameAppView *)m_pParent)->m_tournamentsDlg.Create(CTournamentsDlg::IDD, &m_tabStats);
		m_tabStats.CreateItem(0, "Stats.", (CWnd *) &((CGameAppView *)m_pParent)->m_statsDlg2);
		m_tabStats.CreateItem(1, "Bio.", (CWnd *) &((CGameAppView *)m_pParent)->m_personalDetailsDlg);
		m_tabStats.CreateItem(2, "History.", (CWnd *) &((CGameAppView *)m_pParent)->m_tournamentsDlg);
		m_tabStats.m_created = TRUE;

		/*
		// Only Create if we are Selecting our own BIO //
		if(!strcmpi(m_player[((CGameAppView *)m_pParent)->m_playerIdx].name, ((CGameAppView *)m_pParent)->m_statsDlg2.m_name))
			{
			((CGameAppView *)m_pParent)->m_shopDlg.Create(CShopDlg::IDD, &m_tabStats);
			m_tabStats.CreateItem(3, "Buy - Change Cue / Text", (CWnd *) &((CGameAppView *)m_pParent)->m_shopDlg);
			}
		*/
		m_tabStats.SetNormalColor(RGB(75, 75, 75));
		m_tabStats.SetSelectedColor(RGB(0, 0, 0));

		// get plrIdx from stats dlg's player name field
		plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(((CGameAppView *)m_pParent)->m_statsDlg2.m_name);
	}

	// check player state and display appropriate dialogue heading
	if(plrIdx!=-1)
	{			
		state = m_player[plrIdx].state&PLRSTATE_STATUSMASK;
		switch(state)
		{
		case PLRSTATE_AVAILABLE:
		SetWindowText("Player Profile - (Status: Available)");
		break;
		case PLRSTATE_BUSY:
		SetWindowText("Player Profile - (Status: Busy)");
		break;
		case PLRSTATE_BRB:
		SetWindowText("Player Profile - (Status: Be Right Back)");
		break;
		case PLRSTATE_AWAY:
		case PLRSTATE_AWAY2:
		SetWindowText("Player Profile - (Status: Away)");
		break;
		case PLRSTATE_PHONE:
		SetWindowText("Player Profile - (Status: Phone)");
		break;
		case PLRSTATE_LUNCH:
		SetWindowText("Player Profile - (Status: Lunch)");
		break;
		case PLRSTATE_DND:
		SetWindowText("Player Profile - (Status: Do Not Disturb)");
		break;
		}
	}
	
	return 0;
}

BOOL CStatsTabCtrlDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_tabStats.DestroyWindow();

	return CDialog::DestroyWindow();
}

void CStatsTabCtrlDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(g_build.gameType == GAMETYPE_SNOOKER) ((CGameAppView *)m_pParent)->m_statsDlg.ShowWindow(SW_SHOW);
		else ((CGameAppView *)m_pParent)->m_statsDlg2.ShowWindow(SW_SHOW);
}

void CStatsTabCtrlDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pStatsDlg)
		delete m_pStatsDlg;
}

void CStatsTabCtrlDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CStatsTabCtrlDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	if(nState != WA_INACTIVE)
	{
		m_bActive = 1;
		g_previousWndNo = 1;
	}
	else
	{
		m_bActive = 0;
	}
}

void CStatsTabCtrlDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	// TODO: Add your message handler code here

}
