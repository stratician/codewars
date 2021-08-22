// TableSetupDlg2.cpp : implementation file
//

// for iSnooker

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "TableSetupDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableSetupDlg2 dialog


CTableSetupDlg2::CTableSetupDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CTableSetupDlg2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTableSetupDlg2)
	m_shotTime = 0;
	m_bAddTimer = FALSE;
	m_doubles = FALSE;
	//}}AFX_DATA_INIT
	m_pParent = NULL;

	m_oldShotTime = m_shotTime = 20;
	m_bOldAddTimer = m_bAddTimer = FALSE;
	m_level = 1; // 1 => Semi-Pro
	m_gameType = 0;
	m_raceTo = 0;
	m_whoBreaks = 2;
	m_tableColour = 2; // Green table
	m_tableType = 0;
	m_rackType = 0;
	m_ballSize = 0;
	m_oldDoubles = m_doubles = FALSE;
}


void CTableSetupDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableSetupDlg2)
	DDX_Control(pDX, IDC_TABLETYPE2, m_comboTableType);
	DDX_Control(pDX, IDC_RACKTYPE2, m_comboRackType);
	DDX_Control(pDX, IDC_WHOBREAKS, m_comboWhoBreaks);
	DDX_Control(pDX, IDC_TABLECOLOUR, m_comboTableColour);
	DDX_Control(pDX, IDC_RACETO, m_comboRaceTo);
	DDX_Control(pDX, IDC_LEVEL, m_comboLevel);
	DDX_Control(pDX, IDC_GAMETYPE, m_comboGameType);
	DDX_Text(pDX, IDC_SHOTTIME, m_shotTime);
	DDV_MinMaxByte(pDX, m_shotTime, 10, 60);
	DDX_Check(pDX, IDC_ADDTIMER2, m_bAddTimer);
	DDX_Check(pDX, IDC_DOUBLES, m_doubles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTableSetupDlg2, CDialog)
	//{{AFX_MSG_MAP(CTableSetupDlg2)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_EN_UPDATE(IDC_SHOTTIME, OnUpdateShottime)
	ON_CBN_SELCHANGE(IDC_GAMETYPE, OnSelchangeGametype)
	ON_CBN_SELCHANGE(IDC_LEVEL, OnSelchangeLevel)
	ON_CBN_SELCHANGE(IDC_RACETO, OnSelchangeRaceto)
	ON_CBN_SELCHANGE(IDC_TABLECOLOUR, OnSelchangeTablecolour)
	ON_CBN_SELCHANGE(IDC_WHOBREAKS, OnSelchangeWhobreaks)
	ON_BN_CLICKED(IDC_ADDTIMER2, OnAddtimer2)
	ON_CBN_SELCHANGE(IDC_BALLSIZE2, OnSelchangeBallsize2)
	ON_CBN_SELCHANGE(IDC_RACKTYPE2, OnSelchangeRacktype2)
	ON_CBN_SELCHANGE(IDC_TABLETYPE2, OnSelchangeTabletype2)
	ON_BN_CLICKED(IDC_DOUBLES, OnDoubles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableSetupDlg2 message handlers

void CTableSetupDlg2::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CTableSetupDlg2::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();

	// invalid range - so reset shot-time back to previous value
	if(m_shotTime < 10 || m_shotTime > 60)
	{
		m_shotTime = m_oldShotTime;
		UpdateData(FALSE);
	}

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
	!((CGameAppView *)m_pParent)->m_lobby->GameStarted( ((CGameAppView *)m_pParent)->m_playerSockIdx) )
	{

		// go no further! if Ladder Match
		if (((CGameAppView *)m_pParent)->InLadderMatch()) return;

		// check for members or guest?
		int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
		if(plrIdx==-1)
		{
			return;
		}

		// go no further! if in tournament lobby
		if(g_bTournamentLobby)
		{
			return;
		}

		BOOL bIsGuest = m_player[plrIdx].bIsGuest;

		UpdateData(TRUE);
		
		BOOL bShotTime = FALSE;
		if(m_shotTime != m_oldShotTime) bShotTime = TRUE;

		BOOL bAddtimer = FALSE;
		//if(m_bAddTimer != m_bOldAddTimer) bAddtimer = TRUE;
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_SHOTTIME);
		if(pEdit)
		{
			CButton *pButton = (CButton *)GetDlgItem(IDC_ADDTIMER2);
			if(pButton)
			if(m_bOldAddTimer != pButton->GetCheck()) bAddtimer = TRUE;	
		}


		BOOL bDoubles = FALSE;
		CButton *pButton = (CButton *)GetDlgItem(IDC_DOUBLES);
		if(pButton)
		{
			if(m_oldDoubles != pButton->GetCheck())
				bDoubles = TRUE; // change pro table state!
		}
		

								
		BOOL bTableColour = FALSE;
		if(m_tableColour != m_comboTableColour.GetCurSel()) bTableColour = TRUE;

		BOOL bLevel = FALSE;
		if(m_level != m_comboLevel.GetCurSel()) bLevel = TRUE;

		BOOL bGameType = FALSE;
		if(m_gameType != m_comboGameType.GetCurSel()) bGameType = TRUE;

		BOOL bRaceTo = FALSE;
		if(m_raceTo != m_comboRaceTo.GetCurSel()) bRaceTo = TRUE;

		BOOL bWhoBreaks = FALSE;
		if(m_whoBreaks != m_comboWhoBreaks.GetCurSel()) bWhoBreaks = TRUE;

		BOOL bTableType = FALSE;
		if(m_tableType != m_comboTableType.GetCurSel()) bTableType = TRUE;

		BOOL bRackType = FALSE;
		if(m_rackType != m_comboRackType.GetCurSel()) bRackType = TRUE;

		BOOL bBallSize = FALSE;
		//if(m_ballSize != m_comboBallSize.GetCurSel()) bBallSize = TRUE;


		if(!bIsGuest) // Member
		{
			m_tableColour = m_comboTableColour.GetCurSel();
			if(m_tableColour == CB_ERR)
				m_tableColour = 2;

			m_level = m_comboLevel.GetCurSel();
			if(m_level == CB_ERR)
				m_level = 2;

			m_gameType = m_comboGameType.GetCurSel();
			if(m_gameType == CB_ERR)
				m_gameType = 0; // Snooker

			m_raceTo = m_comboRaceTo.GetCurSel();
			if(m_raceTo == CB_ERR)
				m_raceTo = 0;

			m_whoBreaks = m_comboWhoBreaks.GetCurSel();
			if(m_whoBreaks == CB_ERR)
				m_whoBreaks = 2;

			m_tableType = m_comboTableType.GetCurSel();
			if(m_tableType == CB_ERR)
				m_tableType = 0;

			m_rackType = m_comboRackType.GetCurSel();
			if(m_rackType == CB_ERR)
				m_rackType = 0;

			//m_ballSize = m_comboBallSize.GetCurSel();
			//if(m_ballSize == CB_ERR)
				m_ballSize = 0;
		}		
		else // Guest
		{
			m_tableColour = m_comboTableColour.GetCurSel();
			if(m_tableColour == CB_ERR)
				m_tableColour = 2;

			m_level = m_comboLevel.GetCurSel();
			if(m_level == CB_ERR)
				m_level = 2;

			m_gameType = m_comboGameType.GetCurSel();
			if(m_gameType == CB_ERR)
				m_gameType = 1; // 10 Reds

			m_raceTo = m_comboRaceTo.GetCurSel();
			if(m_raceTo == CB_ERR)
				m_raceTo = 0;

			m_whoBreaks = m_comboWhoBreaks.GetCurSel();
			if(m_whoBreaks == CB_ERR)
				m_whoBreaks = 2;

			m_tableType = m_comboTableType.GetCurSel();
			if(m_tableType == CB_ERR)
				m_tableType = 0;

			m_rackType = m_comboRackType.GetCurSel();
			if(m_rackType == CB_ERR)
				m_rackType = 0;

			// m_ballSize = m_comboBallSize.GetCurSel();
			// if(m_ballSize == CB_ERR)
				m_ballSize = 0;
		}
			
		m_oldShotTime = m_shotTime; // update old shotTime
		m_bOldAddTimer = m_bAddTimer; // update old addTimer
		m_oldDoubles = m_doubles; // update old proTable
		

		// send table details
		if(bShotTime || bAddtimer || bTableColour || bLevel || bGameType || bRaceTo || bWhoBreaks ||
		   bTableType || bRackType || bBallSize || bDoubles)
		{
			// defaults - (no changes)
			int _cueError = -1; // level
			BOOL _bRated = 0;
			char _type = -1;
			
			BYTE _shotTime = 0xff;
			BYTE _tableColour = 0xff;
			BYTE _gameType = 0xff;
			BYTE _addTimer = 0xff;
			
			BYTE _raceTo = 0xff;
			BYTE _whoBreaks = 0xff;

			BYTE _tableType = 0xff;
			BYTE _rackType = 0xff;
			BYTE _ballSize = 0xff;

			BYTE _doubles = 0xff;

			// changes
			if(bShotTime) _shotTime = m_shotTime;
			
			if(bAddtimer)
			{
				if(m_bAddTimer == FALSE) _addTimer = 0;
				else _addTimer = 1;
			}


			

			if(bLevel) _cueError = m_level;
			if(bTableColour) _tableColour = m_tableColour;
			if(bGameType) _gameType = m_gameType;
			if(bRaceTo) _raceTo = m_raceTo;
			if(bWhoBreaks) _whoBreaks = m_whoBreaks;
			if(bTableType) _tableType = m_tableType;
			if(bRackType) _rackType = m_rackType;
			if(bBallSize) _ballSize = m_ballSize;

			if(bDoubles)
			{
				if(m_doubles == FALSE) _doubles = 0;
				else _doubles = 1;				
			}


			// If we chose Doubles or any game type other than 15 Reds or Beginner, make the game UnRated //
			if(_doubles==1 || (_gameType>0 && _gameType<4) || _cueError==0) 
				{
				// If we were currently set to Rated, then we need to toggle Rated Setting //
				if(g_gameAppView->m_ratedButton.GetCheck()) _bRated = TRUE;
				//g_gameAppView->m_ratedButton.SetCheck(0);
				}
			else{
				// If we were currently set to UnRated, then we need to toggle Rated Setting //
				// if(!(g_gameAppView->m_ratedButton.GetCheck())) _bRated = TRUE;
				//g_gameAppView->m_ratedButton.SetCheck(1);
				}
						

			((CGameAppView *)m_pParent)->SendTableHostDetails(FALSE, _cueError, _bRated, -1, 0, ((CGameAppView *)m_pParent)->m_playerSockIdx,
														  _shotTime, _tableColour, _gameType, _addTimer, _doubles,
														  _raceTo, _whoBreaks,
														  _ballSize, _rackType, _tableType);
		}
	}
	else
	{
		if(((CGameAppView *)m_pParent)->m_snooker->m_bPracticeMode)
		{
			//UpdateData(TRUE);

			m_gameType = m_comboGameType.GetCurSel();
			if(m_gameType == CB_ERR)
				m_gameType = 0; // 15 Reds Snooker

			((CGameAppView *)m_pParent)->OnPracticeGameType(m_gameType);

			BOOL bLevel = FALSE;
			if(m_level != m_comboLevel.GetCurSel()) bLevel = TRUE;
		
			m_level = m_comboLevel.GetCurSel();
			if(m_level == CB_ERR)
				m_level = 2;
			
			((CGameAppView *)m_pParent)->m_snooker->m_cueError = m_level;
		}
	}
}

int CTableSetupDlg2::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();

	m_oldShotTime = m_shotTime;  // update old shotTime
	m_bOldAddTimer = m_bAddTimer;  // update old addTimer
	m_oldDoubles = m_doubles;
	
	return 0;
}

void CTableSetupDlg2::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CTableSetupDlg2::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_comboLevel.ResetContent();
	m_comboLevel.InitStorage(3, 32); // 3 items max.
	m_comboLevel.InsertString(0,"Beginner");
	//m_comboLevel.InsertString(1,"Amateur");
	m_comboLevel.InsertString(1,"Semi-Pro");
	m_comboLevel.InsertString(2,"Pro");
	m_comboLevel.SetCurSel(m_level);

	m_comboGameType.ResetContent();
	m_comboGameType.InitStorage(4, 32); // 4 items max. (can change this to more later for more game types)
	m_comboGameType.InsertString(0,"Snooker");
	m_comboGameType.InsertString(1,"Short-Snooker (10 Reds)");
	m_comboGameType.InsertString(2,"Mini-Snooker (6 Reds)");
	m_comboGameType.InsertString(3,"Re-Spot Black (Un-Rated)");
	m_comboGameType.InsertString(4,"Billiards (50 pts)");
	m_comboGameType.InsertString(5,"Billiards (100 pts)");
	m_comboGameType.InsertString(6,"Billiards (200 pts)");
	m_comboGameType.SetCurSel(m_gameType);

	m_comboRaceTo.ResetContent();
	m_comboRaceTo.InitStorage(9, 32); // 9 items max. (can change this to more later)
	m_comboRaceTo.InsertString(0,"1");
	m_comboRaceTo.InsertString(1,"2");
	m_comboRaceTo.InsertString(2,"3");
	m_comboRaceTo.InsertString(3,"4");
	m_comboRaceTo.InsertString(4,"5");
	m_comboRaceTo.InsertString(5,"6");
	m_comboRaceTo.InsertString(6,"7");
	m_comboRaceTo.InsertString(7,"8");
	m_comboRaceTo.InsertString(8,"9");
	m_comboRaceTo.SetCurSel(m_raceTo);

	m_comboWhoBreaks.ResetContent();
	m_comboWhoBreaks.InitStorage(6, 32); // 6 items max. (can change this to more later)
	m_comboWhoBreaks.InsertString(0,"1P Always");
	m_comboWhoBreaks.InsertString(1,"2P Always");
	m_comboWhoBreaks.InsertString(2,"Alternate");
	m_comboWhoBreaks.InsertString(3,"Winner");
	m_comboWhoBreaks.InsertString(4,"Loser");
	m_comboWhoBreaks.InsertString(5,"Player Trailing");
	m_comboWhoBreaks.SetCurSel(m_whoBreaks);
	
	m_comboTableColour.ResetContent();
	m_comboTableColour.InitStorage(8, 32); // 8 items max. (can change this to more later)
	m_comboTableColour.InsertString(0,"Blue");
	m_comboTableColour.InsertString(1,"Red");
	m_comboTableColour.InsertString(2,"Green");
	m_comboTableColour.InsertString(3,"Black");
	m_comboTableColour.InsertString(4,"Yellow");
	m_comboTableColour.InsertString(5,"Purple");
	m_comboTableColour.InsertString(6,"Teal");
	m_comboTableColour.InsertString(7,"Dark Green");
	m_comboTableColour.SetCurSel(m_tableColour);

	m_comboTableType.ResetContent();
	m_comboTableType.InitStorage(1, 32); // 1 items max. (can change this to more later)
	m_comboTableType.InsertString(0,"Normal");
	m_comboTableType.SetCurSel(m_tableType);

	m_comboRackType.ResetContent();
	m_comboRackType.InitStorage(10, 32); // 10 items max. (can change this to more later)
	m_comboRackType.InsertString(0,"Normal");
	m_comboRackType.InsertString(1,"Randomly Spotted Colours");//?
	m_comboRackType.InsertString(2,"Randomly Placed Colours");//?
	m_comboRackType.InsertString(3,"Triangle Mix");//?
	m_comboRackType.InsertString(4,"Reds Behind Blue");
	m_comboRackType.InsertString(5,"Reds Behind Brown");
	m_comboRackType.InsertString(6,"21-Reds");
	m_comboRackType.InsertString(7,"28-Reds");
	m_comboRackType.InsertString(8,"All Blacks");//?
	m_comboRackType.InsertString(9,"Scrap Heap");//?
	m_comboRackType.SetCurSel(m_rackType);

	//m_comboBallSize.ResetContent();
	//m_comboBallSize.InitStorage(2, 32); // 2 items max. (can change this to more later)
	//m_comboBallSize.InsertString(0,"Normal");
	//m_comboBallSize.InsertString(1,"GIANT (Bigger Pockets)");
	//m_comboBallSize.SetCurSel(m_ballSize);

	UpdateShotTime();


	/// Enable / Disable Doubles Button if host or not //
	CButton *pButton = (CButton *)GetDlgItem(IDC_DOUBLES);
	if(pButton)
		{
		if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) ) pButton->EnableWindow(TRUE);
			else pButton->EnableWindow(FALSE);
		}

	/// Enable / Disable AddTime Button if host or not  //
	pButton = (CButton *)GetDlgItem(IDC_ADDTIMER2);
	if(pButton)
		{
		if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) ) pButton->EnableWindow(TRUE);
			else pButton->EnableWindow(FALSE);
		}

}

void CTableSetupDlg2::OnAddtimer2() 
{
	// TODO: Add your control notification handler code here
	if(((CGameAppView *)m_pParent)->m_snooker->m_bPracticeMode)
	{
		UpdateData(FALSE);	
		return;
	}

	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;
		
	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted( ((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby)
		UpdateShotTime();
	else
		UpdateData(FALSE);
}


void CTableSetupDlg2::OnDoubles() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;


	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() && !g_bTournamentLobby);
	else UpdateData(FALSE);	
}

void CTableSetupDlg2::UpdateShotTime(void)
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_SHOTTIME);
	if(pEdit)
	{
		// limit "edit window" text (chars)
		pEdit->SetLimitText(2);
		
		// Set numbers only flag
		pEdit->ModifyStyle(0,ES_NUMBER);

		CButton *pButton = (CButton *)GetDlgItem(IDC_ADDTIMER2);

		if(pButton)
		{
			if(pButton->GetCheck())
				pEdit->EnableWindow(TRUE);
			else
				pEdit->EnableWindow(FALSE);
		}
	}	
}

void CTableSetupDlg2::EnableTableCtrls(BOOL bState = TRUE)
{
	CListCtrl *pList = (CListCtrl *)GetDlgItem(IDC_GAMETYPE);
	if(pList) pList->EnableWindow(bState);

	CListCtrl *pList2 = (CListCtrl *)GetDlgItem(IDC_TABLECOLOUR);
	if(pList2) pList2->EnableWindow(bState);
	
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_SHOTTIME);
	if(pEdit) pEdit->EnableWindow(bState);

	CButton *pButton = (CButton *)GetDlgItem(IDC_ADDTIMER);
	if(pButton) pButton->EnableWindow(bState);
}




void CTableSetupDlg2::OnUpdateShottime() 
{
	// TODO: Add your control notification handler code here
	// Practice mode
	if(((CGameAppView *)m_pParent)->m_snooker->m_bPracticeMode)
	{	
		UpdateData(FALSE);
		return;
	}

	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else UpdateData(FALSE);
}

void CTableSetupDlg2::OnSelchangeGametype() 
{
	// TODO: Add your control notification handler code here
	// Practice mode
	if(((CGameAppView *)m_pParent)->m_snooker->m_bPracticeMode)
	{
		//m_comboGameType.SetCurSel(m_gameType);
		return;
	}

	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboGameType.SetCurSel(m_gameType);	
}

void CTableSetupDlg2::OnSelchangeLevel() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx)/* &&
		!m_player[plrIdx].bIsGuest*/ && !((CGameAppView *)m_pParent)->InLadderMatch() && 
		!g_bTournamentLobby);
	else m_comboLevel.SetCurSel(m_level);	
}

void CTableSetupDlg2::OnSelchangeRaceto() 
{
	// TODO: Add your control notification handler code here
	// Practice mode
	if(((CGameAppView *)m_pParent)->m_snooker->m_bPracticeMode)
	{
		m_comboRaceTo.SetCurSel(m_raceTo);
		return;
	}

	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboRaceTo.SetCurSel(m_raceTo);	
}

void CTableSetupDlg2::OnSelchangeTablecolour() 
{
	// TODO: Add your control notification handler code here
	// Practice mode
	if(((CGameAppView *)m_pParent)->m_snooker->m_bPracticeMode)
	{
		m_comboTableColour.SetCurSel(m_tableColour);
		return;
	}

	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboTableColour.SetCurSel(m_tableColour);	
}

void CTableSetupDlg2::OnSelchangeWhobreaks() 
{
	// TODO: Add your control notification handler code here
	// Practice mode
	if(((CGameAppView *)m_pParent)->m_snooker->m_bPracticeMode)
	{
		m_comboWhoBreaks.SetCurSel(m_whoBreaks);
		return;
	}

	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboWhoBreaks.SetCurSel(m_whoBreaks);	
}

void CTableSetupDlg2::OnSelchangeBallsize2() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboBallSize.SetCurSel(m_ballSize);
}

void CTableSetupDlg2::OnSelchangeTabletype2() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboTableType.SetCurSel(m_tableType);	
}

void CTableSetupDlg2::OnSelchangeRacktype2() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboRackType.SetCurSel(m_rackType);
}
