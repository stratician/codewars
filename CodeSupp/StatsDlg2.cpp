// StatsDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "StatsDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg2 dialog


CStatsDlg2::CStatsDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CStatsDlg2::IDD, pParent)
	, m_ladderPoints(_T(""))
	, m_ladderGames(0)
	, m_ladderWins(0)
	, m_ladderLosses(0)
	, m_ladderHighBreak(0)
	, m_ladderHighBreakPro(0)
	, m_ladderDivision(_T(""))
	, m_ladderRank(_T(""))
	, m_realCash(_T(""))
{
	//{{AFX_DATA_INIT(CStatsDlg2)
	m_games = 0;
	m_losses = 0;
	m_name = _T("");
	m_rating = 0;
	m_streak = 0;
	m_wins = 0;
	m_msg = _T("");
	m_bIgnore = FALSE;
	m_cash = _T("");
	m_reracks = 0;
	m_runOut8Std = 0;
	m_runOut8Pro = 0;
	m_runOut8UK = 0;
	m_runOut9Std = 0;
	m_runOut9Pro = 0;
	m_runOut9UK = 0;
	m_runOutEngStd = 0;
	m_runOutEngPro = 0;
	m_runOutEngUK = 0;
	m_runOut8Rate = 0.0f;
	m_runOut9Rate = 0.0f;
	m_runOutEngRate = 0.0f;
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	m_bToggle = 0;
	m_bDoneToggle = FALSE;
	m_bDontCenterWindow = FALSE;
	m_bActive = 0;
}


void CStatsDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsDlg2)
	DDX_Text(pDX, IDC_EDITGames, m_games);
	DDX_Text(pDX, IDC_EDITLosses, m_losses);
	DDX_Text(pDX, IDC_EDITName, m_name);
	DDX_Text(pDX, IDC_EDITRating, m_rating);
	DDX_Text(pDX, IDC_EDITStreak, m_streak);
	DDX_Text(pDX, IDC_EDITWins, m_wins);
	DDX_Text(pDX, IDC_MSG, m_msg);
	DDV_MaxChars(pDX, m_msg, 85);
	DDX_Check(pDX, IDC_IGNORE, m_bIgnore);
	DDX_Text(pDX, IDC_EDITHighestRating, m_highestrating);
	DDX_Text(pDX, IDC_EDITCash, m_cash);
	DDX_Text(pDX, IDC_EDITReracks, m_reracks);
	DDX_Text(pDX, IDC_EDIT_RUNOUT8, m_runOut8Std);
	DDX_Text(pDX, IDC_EDIT_RUNOUT8_PRO, m_runOut8Pro);
	DDX_Text(pDX, IDC_EDIT_RUNOUT8_UK, m_runOut8UK);
	DDX_Text(pDX, IDC_EDIT_RUNOUT9, m_runOut9Std);
	DDX_Text(pDX, IDC_EDIT_RUNOUT9_PRO, m_runOut9Pro);
	DDX_Text(pDX, IDC_EDIT_RUNOUT9_UK, m_runOut9UK);
	DDX_Text(pDX, IDC_EDIT_RUNOUTENG, m_runOutEngStd);
	DDX_Text(pDX, IDC_EDIT_RUNOUTENG_PRO, m_runOutEngPro);
	DDX_Text(pDX, IDC_EDIT_RUNOUTENG_UK, m_runOutEngUK);
	DDX_Text(pDX, IDC_EDIT_RUNOUT8_RATE, m_runOut8Rate);
	DDX_Text(pDX, IDC_EDIT_RUNOUT9_RATE, m_runOut9Rate);
	DDX_Text(pDX, IDC_EDIT_RUNOUTENG_RATE, m_runOutEngRate);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDITLadderPoints, m_ladderPoints);
	DDX_Text(pDX, IDC_EDITLadderGames, m_ladderGames);
	DDX_Text(pDX, IDC_EDITLadderWins, m_ladderWins);
	DDX_Text(pDX, IDC_EDITLadderLosses, m_ladderLosses);
	DDX_Text(pDX, IDC_EDITLadderHighBreak, m_ladderHighBreak);
	DDX_Text(pDX, IDC_EDITLadderHighBreakPro, m_ladderHighBreakPro);
	DDX_Text(pDX, IDC_EDITLadderDivision, m_ladderDivision);
	DDX_Text(pDX, IDC_EDITLadderRank, m_ladderRank);
	DDX_Text(pDX, IDC_EDITRealCash, m_realCash);
}


BEGIN_MESSAGE_MAP(CStatsDlg2, CDialog)
	//{{AFX_MSG_MAP(CStatsDlg2)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_IGNORE, OnIgnore)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_TOGGLE, OnToggle)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDITHighestRating, &CStatsDlg2::OnEnChangeEdithighestrating)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg2 message handlers

int CStatsDlg2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	m_bDoneToggle = FALSE;

	return 0;
}

void CStatsDlg2::OnOK() // 'Send'
{
	// TODO: Add extra validation here

	// retrieve send edit message
	UpdateData(TRUE);
	
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

void CStatsDlg2::OnCancel() // 'Close'
{
	// TODO: Add extra cleanup here
	m_bDontCenterWindow = FALSE;
	
	CDialog::OnCancel();
}

void CStatsDlg2::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CStatsDlg2::GetMSGFocusBack(void)
{
	// get focus back on edit ctrl window
	CEdit *pEdit = NULL;
	pEdit = (CEdit *)GetDlgItem(IDC_MSG);	
	if(pEdit)
	{
		pEdit->SetFocus();
	}
}

void CStatsDlg2::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	// find whether this player has been ignored
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(m_name);
	if(plrIdx!=-1)
	{
		// this player has been ignored (i.e. ignore /msg & invites)
		m_bIgnore = m_player[plrIdx].bIgnore;
		UpdateData(FALSE);
	}
	
	// get focus back on edit ctrl window
	GetMSGFocusBack();	
}

void CStatsDlg2::OnToggle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_bToggle^=1;
	//m_bDoneToggle = TRUE;
	m_bDontCenterWindow = TRUE;
	CRect tmpRect;
	CSize tmpSize;
	CPoint tmpPoint;
	//GetWindowRect(&tmpRect);
	((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.GetWindowRect(&tmpRect);
	tmpSize = tmpRect.Size();
	tmpPoint =  tmpRect.TopLeft();

	//DestroyWindow();
	((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.DestroyWindow();

	((CGameAppView *)m_pParent)->m_statsChatDlg.m_name = m_name;
	((CGameAppView *)m_pParent)->m_statsChatDlg.m_msg = m_msg;
	
	((CGameAppView *)m_pParent)->m_statsChatDlg.Create(IDD_STATSCHAT);
	((CGameAppView *)m_pParent)->m_statsChatDlg.SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, tmpSize.cx, tmpSize.cy, SWP_SHOWWINDOW|SWP_NOSIZE);

	//ShowWindow(SW_SHOW);	
}

void CStatsDlg2::OnIgnore() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(m_name);
	if(plrIdx!=-1)
	{
		// this player has been ignored (i.e. ignore /msg & invites)
		m_player[plrIdx].bIgnore = m_bIgnore;
		if(m_bIgnore == FALSE) ((CGameAppView *)m_pParent)->m_ignorelst->DeleteName(m_name.GetBuffer(0));
			else ((CGameAppView *)m_pParent)->m_ignorelst->AddName(m_name.GetBuffer(0));

		((CGameAppView *)m_pParent)->m_ignorelst->Save();

	}
}

void CStatsDlg2::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
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

void CStatsDlg2::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here

}

void CStatsDlg2::OnEnChangeEdithighestrating()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
