// statsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "statsdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog


CStatsDlg::CStatsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatsDlg::IDD, pParent)
	, m_ladderPoints(_T(""))
	, m_ladderGames(0)
	, m_ladderWins(0)
	, m_ladderLosses(0)
	, m_ladderHighBreak(0)
	, m_ladderHighBreakPro(0)
	, m_ladderDivision(_T(""))
	, m_ladderRank(_T(""))
	, m_realCash(_T(""))
	, m_highStreak(0)
{
	//{{AFX_DATA_INIT(CStatsDlg)

	m_ladderPoints = _T("");
	m_highestrating = 0;
	m_100s = 0;
	m_50s = 0;
	m_60s = 0;
	m_70s = 0;
	m_80s = 0;
	m_90s = 0;
	m_p100s = 0;
	m_p50s = 0;
	m_p60s = 0;
	m_p70s = 0;
	m_p80s = 0;
	m_p90s = 0;
	m_highBreak = 0;
	m_games = 0;
	m_losses = 0;
	m_max147s = 0;
	m_pHighBreak = 0;
	m_pMax147s = 0;
	m_rating = 0;
	m_streak = 0;
	m_wins = 0;
	m_name = _T("");
	m_msg = _T("");
	m_bIgnore = FALSE;
	m_cash = _T("");
	m_rating = 0;
	m_reracks = 0;
	m_highStreak = 0;
	m_realCash = _T("");
	
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	m_bToggle = 0;
	m_bDoneToggle = FALSE;
	m_bDontCenterWindow = FALSE;
	m_bActive = 0;
}


void CStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatsDlg)
	DDX_Text(pDX, IDC_EDIT_100s, m_100s);
	DDX_Text(pDX, IDC_EDIT_50s, m_50s);
	DDX_Text(pDX, IDC_EDIT_60s, m_60s);
	DDX_Text(pDX, IDC_EDIT_70s, m_70s);
	DDX_Text(pDX, IDC_EDIT_80s, m_80s);
	DDX_Text(pDX, IDC_EDIT_90s, m_90s);
	DDX_Text(pDX, IDC_EDIT_p100s, m_p100s);
	DDX_Text(pDX, IDC_EDIT_p50s, m_p50s);
	DDX_Text(pDX, IDC_EDIT_p60s, m_p60s);
	DDX_Text(pDX, IDC_EDIT_p70s, m_p70s);
	DDX_Text(pDX, IDC_EDIT_p80s, m_p80s);
	DDX_Text(pDX, IDC_EDIT_p90s, m_p90s);
	DDX_Text(pDX, IDC_EDITHighBreak, m_highBreak);
	DDX_Text(pDX, IDC_EDITGames, m_games);
	DDX_Text(pDX, IDC_EDITLosses, m_losses);
	DDX_Text(pDX, IDC_EDITMax147s, m_max147s);
	DDX_Text(pDX, IDC_EDITpHighBreak, m_pHighBreak);
	DDX_Text(pDX, IDC_EDITpMax147s, m_pMax147s);
	DDX_Text(pDX, IDC_EDITRating, m_rating);
	DDX_Text(pDX, IDC_EDITStreak, m_streak);
	DDX_Text(pDX, IDC_EDITWins, m_wins);
	DDX_Text(pDX, IDC_EDITName, m_name);
	DDX_Text(pDX, IDC_MSG, m_msg);
	DDV_MaxChars(pDX, m_msg, 85);
	DDX_Check(pDX, IDC_IGNORE, m_bIgnore);
	DDX_Text(pDX, IDC_EDITCash, m_cash);
	DDX_Text(pDX, IDC_EDITReracks, m_reracks);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDITHighestRating, m_highestrating);
	DDX_Text(pDX, IDC_EDITLadderPoints, m_ladderPoints);
	DDX_Text(pDX, IDC_EDITLadderGames, m_ladderGames);
	DDX_Text(pDX, IDC_EDITLadderWins, m_ladderWins);
	DDX_Text(pDX, IDC_EDITLadderLosses, m_ladderLosses);
	DDX_Text(pDX, IDC_EDITLadderHighBreak, m_ladderHighBreak);
	DDX_Text(pDX, IDC_EDITLadderHighBreakPro, m_ladderHighBreakPro);
	DDX_Text(pDX, IDC_EDITLadderDivision, m_ladderDivision);
	DDX_Text(pDX, IDC_EDITLadderRank, m_ladderRank);
	DDX_Text(pDX, IDC_EDITRealCash, m_realCash);
	DDX_Text(pDX, IDC_EDITHighStreak, m_highStreak);
}


BEGIN_MESSAGE_MAP(CStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatsDlg)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_TOGGLE, OnToggle)
	ON_BN_CLICKED(IDC_IGNORE, OnIgnore)
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDITHighestRating, &CStatsDlg::OnEnChangeEdithighestrating)
	ON_EN_CHANGE(IDC_EDITLadderPoints, &CStatsDlg::OnEnChangeEditladderpoints)
	ON_BN_CLICKED(IDACHIEVEMENTS, &CStatsDlg::OnBnClickedAchievements)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg message handlers

int CStatsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	/*if(g_build.gameType == GAMETYPE_POOL)
	{
		CRect tmpRect;
		CSize tmpSize;
		CPoint tmpPoint;
		
		GetWindowRect(&tmpRect);
		tmpSize = tmpRect.Size();
		tmpSize.cx = 200;
		tmpPoint =  tmpRect.TopLeft();
		SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, tmpSize.cx, tmpSize.cy, SWP_NOREDRAW);//SWP_SHOWWINDOW);
	}*/

	//if(!m_bDontCenterWindow)// || !m_bDoneToggle)
	CenterWindow();	

	m_bDoneToggle = FALSE;
	
	return 0;
}

void CStatsDlg::OnOK() // 'Send'
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

void CStatsDlg::OnCancel()  // 'Close'
{
	// TODO: Add extra cleanup here
	m_bDontCenterWindow = FALSE;
	
	CDialog::OnCancel();
}

void CStatsDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CStatsDlg::GetMSGFocusBack(void)
{
	// get focus back on edit ctrl window
	CEdit *pEdit = NULL;
	pEdit = (CEdit *)GetDlgItem(IDC_MSG);	
	if(pEdit)
	{
		pEdit->SetFocus();
	}
}

void CStatsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
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

void CStatsDlg::OnToggle() 
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

void CStatsDlg::OnIgnore() 
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

void CStatsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
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


void CStatsDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
}


void CStatsDlg::OnEnChangeEdithighestrating()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStatsDlg::OnEnChangeEditratingL()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStatsDlg::OnEnChangeEditladderpoints()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStatsDlg::OnEnChangeEditladderpoints2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStatsDlg::OnBnClickedAchievements()
{
	// TODO: Add your control notification handler code here
	// When clicked, Open Achievements for player
	// We must be online to open achievements


	g_gameAppView->CreateAchievements(m_name.GetBuffer());
		



}


void CStatsDlg::OnEnChangeEditcash2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
