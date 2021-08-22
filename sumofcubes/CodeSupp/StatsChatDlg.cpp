// StatsChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "StatsChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CStatsChatDlg dialog


CStatsChatDlg::CStatsChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatsChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatsChatDlg)
	m_msg = _T("");
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	m_name = _T("");
	m_bActive = 0;	
}


void CStatsChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsChatDlg)
	DDX_Text(pDX, IDC_MSG, m_msg);
	DDV_MaxChars(pDX, m_msg, 85);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatsChatDlg, CDialog)
	//{{AFX_MSG_MAP(CStatsChatDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_TOGGLE, OnToggle)
	ON_WM_SHOWWINDOW()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsChatDlg message handlers

void CStatsChatDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(((CGameAppView *)m_pParent)->m_inviteDlg.m_hWnd) ((CGameAppView *)m_pParent)->m_inviteDlg.m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
	if(((CGameAppView *)m_pParent)->m_bootDlg.m_hWnd) ((CGameAppView *)m_pParent)->m_bootDlg.m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
	
	CDialog::OnCancel();
}

void CStatsChatDlg::OnOK() 
{
	// TODO: Add extra validation here

	// retrieve send edit message
	UpdateData(TRUE);
	
	//m_msg = CString("");
	int len = m_msg.GetLength();
	if(len>0)
	{
		// build /msg <name> <text>
		char buffer[MAX_CHARS];
		sprintf(buffer, "/msg %s %s", m_name.GetBuffer(0), m_msg.GetBuffer(0));
		
		
		// check if we have any text to send?
		int len = strlen(buffer);
		if(len <=0 || len > MAX_CHARS);
		else
		{
			if(!((CGameAppView *)m_pParent)->OptionNotForGuest(((CGameAppView *)m_pParent)->m_playerSockIdx))
				((CGameAppView *)m_pParent)->MessageSend(buffer);

			// clear send edit message, we've sent it on its way to this player
			m_msg = _T("");
			UpdateData(FALSE);
		}
	}

	// get focus back on edit ctrl window
	CEdit *pEdit = NULL;
	pEdit = (CEdit *)GetDlgItem(IDC_MSG);	
	if(pEdit)
	{
		pEdit->SetFocus();
	}

	
	//CDialog::OnOK();
}

void CStatsChatDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

int CStatsChatDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetWindowText(m_name.GetBuffer(0));
	
	return 0;
}

void CStatsChatDlg::OnToggle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CRect tmpRect;
	CSize tmpSize;
	CPoint tmpPoint;
	GetWindowRect(&tmpRect);
	tmpSize = tmpRect.Size();
	tmpPoint =  tmpRect.TopLeft();

	DestroyWindow();

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
	((CGameAppView *)m_pParent)->m_statsDlg.m_msg = m_msg;

	//((CGameAppView *)m_pParent)->m_statsDlg.Create(IDD_STATS);
	//((CGameAppView *)m_pParent)->m_statsDlg.SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, tmpSize.cx, tmpSize.cy, SWP_SHOWWINDOW|SWP_NOSIZE);
	((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.Create(IDD_STATSTABCTRL);
	((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, tmpSize.cx, tmpSize.cy, SWP_SHOWWINDOW|SWP_NOSIZE);
	}
	else // iPool
	{
	((CGameAppView *)m_pParent)->m_statsDlg2.m_msg = m_msg;

	//((CGameAppView *)m_pParent)->m_statsDlg2.Create(IDD_STATS2);
	//((CGameAppView *)m_pParent)->m_statsDlg2.SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, tmpSize.cx, tmpSize.cy, SWP_SHOWWINDOW|SWP_NOSIZE);
	((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.Create(IDD_STATSTABCTRL);
	((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, tmpSize.cx, tmpSize.cy, SWP_SHOWWINDOW|SWP_NOSIZE);
	}
}

void CStatsChatDlg::GetMSGFocusBack(void)
{
	// get focus back on edit ctrl window
	CEdit *pEdit = NULL;
	pEdit = (CEdit *)GetDlgItem(IDC_MSG);	
	if(pEdit)
	{
		pEdit->SetFocus();
	}
}

void CStatsChatDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	// get focus back on edit ctrl window
	GetMSGFocusBack();
}

void CStatsChatDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

	// save this window as current previous focus
	//g_previousWndEditFocus = (CWnd *)this;
	//ErrorMessage(m_pParent, "g_previousWndEditFocus = m_statsChatsDlg");
}

void CStatsChatDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	if(nState != WA_INACTIVE)
	{
		m_bActive = 1;
		g_previousWndNo = 2;
	}
	else
	{
		m_bActive = 0;
	}
}

void CStatsChatDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	//if(((CGameAppView *)m_pParent)->m_inviteDlg.m_hWnd) ((CGameAppView *)m_pParent)->m_inviteDlg.m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
	//if(((CGameAppView *)m_pParent)->m_bootDlg.m_hWnd) ((CGameAppView *)m_pParent)->m_bootDlg.m_listPlayers.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
}
