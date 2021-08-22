// TournamentsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "TournamentsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTournamentsDlg dialog


CTournamentsDlg::CTournamentsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTournamentsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTournamentsDlg)
	m_winner = 0;
	m_runnerUp = 0;
	m_semiFinals = 0;
	m_quarterFinals = 0;
	m_last16 = 0;
	m_last32 = 0;
	m_BansUnAuth = 0;
	m_BansTotal = 0;
	m_BansSpamming = 0;
	m_BansMTrials = 0;
	m_BansCheating = 0;
	m_BansAbuse = 0;
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CTournamentsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTournamentsDlg)
	DDX_Control(pDX, IDC_LIST1, m_historyCtrl);
	DDX_Text(pDX, IDC_EDITWinner, m_winner);
	DDX_Text(pDX, IDC_EDITRunnerUp, m_runnerUp);
	DDX_Text(pDX, IDC_EDITSemiFinals, m_semiFinals);
	DDX_Text(pDX, IDC_EDITQtrFinals, m_quarterFinals);
	DDX_Text(pDX, IDC_EDITLast16, m_last16);
	DDX_Text(pDX, IDC_EDITLast32, m_last32);
	DDX_Text(pDX, IDC_BAN_UNAUTH, m_BansUnAuth);
	DDX_Text(pDX, IDC_BAN_TOTAL, m_BansTotal);
	DDX_Text(pDX, IDC_BAN_SPAMMING, m_BansSpamming);
	DDX_Text(pDX, IDC_BAN_MTRIALS, m_BansMTrials);
	DDX_Text(pDX, IDC_BAN_CHEATING, m_BansCheating);
	DDX_Text(pDX, IDC_BAN_ABUSE, m_BansAbuse);
	//}}AFX_DATA_MAP
}

void CTournamentsDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

BEGIN_MESSAGE_MAP(CTournamentsDlg, CDialog)
	//{{AFX_MSG_MAP(CTournamentsDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTournamentsDlg message handlers

int CTournamentsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	return 0;
}
