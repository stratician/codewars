// TableSetupDlg.cpp : implementation file
//

// for iPool

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "TableSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableSetupDlg dialog


CTableSetupDlg::CTableSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTableSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTableSetupDlg)
	m_bAddTimer = FALSE;
	m_shotTime = 0;
	m_doubles = FALSE;
	m_proTable = TRUE;//FALSE;
	//}}AFX_DATA_INIT
	m_pParent = NULL;

	m_oldShotTime = m_shotTime = 20;
	m_bOldAddTimer = m_bAddTimer = FALSE;
	m_oldProTable = m_proTable = TRUE;//FALSE;
	m_gameType = 0;
	m_raceTo = 0;
	m_whoBreaks = 2;
	m_tableColour = 0;	// Default Blue
	m_tableType = 0;
	m_rackType = 0;
	m_ballSize = 0;
	m_oldDoubles = m_doubles = FALSE;
}


void CTableSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableSetupDlg)
	DDX_Control(pDX, IDC_TABLETYPE, m_comboTableType);
	DDX_Control(pDX, IDC_RACKTYPE, m_comboRackType);
	DDX_Control(pDX, IDC_BALLSIZE, m_comboBallSize);
	DDX_Control(pDX, IDC_WHOBREAKS, m_comboWhoBreaks);
	DDX_Control(pDX, IDC_RACETO, m_comboRaceTo);
	DDX_Control(pDX, IDC_TABLECOLOUR, m_comboTableColour);
	DDX_Control(pDX, IDC_GAMETYPE, m_comboGameType);
	DDX_Check(pDX, IDC_ADDTIMER, m_bAddTimer);
	DDX_Text(pDX, IDC_SHOTTIME, m_shotTime);
	DDV_MinMaxByte(pDX, m_shotTime, 10, 60);
	DDX_Check(pDX, IDC_DOUBLES, m_doubles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTableSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CTableSetupDlg)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_ADDTIMER, OnAddtimer)
	ON_EN_UPDATE(IDC_SHOTTIME, OnUpdateShottime)
	ON_CBN_SELCHANGE(IDC_GAMETYPE, OnSelchangeGametype)
	ON_CBN_SELCHANGE(IDC_RACETO, OnSelchangeRaceto)
	ON_CBN_SELCHANGE(IDC_TABLECOLOUR, OnSelchangeTablecolour)
	ON_CBN_SELCHANGE(IDC_WHOBREAKS, OnSelchangeWhobreaks)
	ON_CBN_SELCHANGE(IDC_BALLSIZE, OnSelchangeBallsize)
	ON_CBN_SELCHANGE(IDC_TABLETYPE, OnSelchangeTabletype)
	ON_CBN_SELCHANGE(IDC_RACKTYPE, OnSelchangeRacktype)
	ON_BN_CLICKED(IDC_PROTABLE, OnProtable)
	ON_BN_CLICKED(IDC_DOUBLES, OnDoubles)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADDTIMER, OnButtonShotTime)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableSetupDlg message handlers

void CTableSetupDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CTableSetupDlg::OnOK() 
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

		// go no further! if you are a guest. ;)
		if(m_player[plrIdx].bIsGuest)
		{
			return;
		}

		// go no further! if in tournament lobby
		if(g_bTournamentLobby)
		{
			return;
		}
		
		BOOL bShotTime = FALSE;
		if(m_shotTime != m_oldShotTime) bShotTime = TRUE;

		BOOL bAddtimer = FALSE;
		
		//if(m_bAddTimer != m_bOldAddTimer) bAddtimer = TRUE;
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_SHOTTIME);
		if(pEdit)
		{
			CButton *pButton = (CButton *)GetDlgItem(IDC_ADDTIMER);
			if(pButton)
			{
				if(m_bOldAddTimer != pButton->GetCheck())
					bAddtimer = TRUE; // change add timer state! (Note: not add timer)
			}
		}

		

		BOOL bProTable = FALSE;
		CButton *pButton = (CButton *)GetDlgItem(IDC_PROTABLE);
		if(pButton)
		{
			if(m_oldProTable != pButton->GetCheck())
				bProTable = TRUE; // change pro table state!
		}

		BOOL bDoubles = FALSE;
		pButton = (CButton *)GetDlgItem(IDC_DOUBLES);
		if(pButton)
		{
			if(m_oldDoubles != pButton->GetCheck())
				bDoubles = TRUE; // change pro table state!
		}

		
		BOOL bTableColour = FALSE;
		if(m_tableColour != m_comboTableColour.GetCurSel()) bTableColour = TRUE;

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
		if(m_ballSize != m_comboBallSize.GetCurSel()) bBallSize = TRUE;

		

		m_tableColour = m_comboTableColour.GetCurSel();
		if(m_tableColour == CB_ERR)
			m_tableColour = 0;	// Default Blue

		m_gameType = m_comboGameType.GetCurSel();
		if(m_gameType == CB_ERR)
			m_gameType = 0;

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

		m_ballSize = m_comboBallSize.GetCurSel();
		if(m_ballSize == CB_ERR)
			m_ballSize = 0;

			
		m_oldShotTime = m_shotTime; // update old shotTime
		m_bOldAddTimer = m_bAddTimer; // update old addTimer
		m_oldProTable = m_proTable; // update old proTable
		m_oldDoubles = m_doubles; // update old proTable
		

		/*	
		// If English Pool or Break, Do Fixed Set Up //
		if(m_comboGameType.GetCurSel() >= 6) 
			{
			bGameType = TRUE;
			bTableType = TRUE;
			bProTable = FALSE;
			m_tableType = 0;
			}
		else{
			if(g_gameAppView)
			if(g_gameAppView->m_snooker->m_M3d_Table_ptr == &g_gameAppView->m_snooker->m_customTable->m_M3d_ukTable)
				{
				// If We previous had UK Table, then Update Everythign since we're changing type // 
				bTableColour = TRUE;
				bGameType = TRUE;
				bRaceTo = TRUE;
				bWhoBreaks = TRUE;
				bTableType = TRUE;
				bRackType = TRUE;
				bBallSize = TRUE;
				bProTable = TRUE;
				}
			}
		*/


		// send table details
		if(bShotTime || bAddtimer || bTableColour || bGameType || bRaceTo || bWhoBreaks ||
		   bTableType || bRackType || bBallSize || bProTable  || bDoubles)
		{
			// defaults - (no changes)
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

			BYTE _proTable = 0xff;
			BYTE _doubles = 0xff;

			// changes
			if(bShotTime) _shotTime = m_shotTime;
			if(bAddtimer)
			{
				if(m_bAddTimer == FALSE) _addTimer = 0;
				else _addTimer = 1;
			}
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

			
			// If we chose Doubles or 6-Ball Game Type or Tables other than Std, Pro, UK make the game UnRated //
			if(_doubles==1 || _gameType == 4 || (_tableType > 2 && _tableType < 10) )
				{
				// If we were currently set to Rated, then we need to toggle Rated Setting //
				if(g_gameAppView->m_ratedButton.GetCheck()) _bRated = TRUE;
				//g_gameAppView->m_ratedButton.SetCheck(0);
				}
			else{
				// If we were currently set to UnRated, then we need to toggle Rated Setting //
				//if(!(g_gameAppView->m_ratedButton.GetCheck())) _bRated = TRUE;
				//g_gameAppView->m_ratedButton.SetCheck(1);
				}
						
		


			((CGameAppView *)m_pParent)->SendTableHostDetails(FALSE, -1,  _bRated, -1, 0, ((CGameAppView *)m_pParent)->m_playerSockIdx,
														  _shotTime, _tableColour, _gameType, _addTimer, _doubles,
														  _raceTo, _whoBreaks,
														  _ballSize, _rackType, _tableType, _proTable);
		}		
	}
}

int CTableSetupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();

	m_oldShotTime = m_shotTime;		// update old shotTime
	m_bOldAddTimer = m_bAddTimer;	// update old addTimer
	m_oldProTable = m_proTable;		// update old proTable
	m_oldDoubles = m_doubles;		// update old doubles
			
	return 0;
}

void CTableSetupDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}


void CTableSetupDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
//	int tblIdx = ((CGameAppView *)m_pParent)->m_lobby->GetTableIdxFromPlayer(((CGameAppView *)m_pParent)->m_playerSockIdx);
//	if(tblIdx == -1)
//	if(m_table[tblIdx].bGameStarted)
//		EnableTableCtrls(FALSE);

	m_comboGameType.ResetContent();
	m_comboGameType.InitStorage(6, 32); // 6 items max. (can change this to more later for more game types)
	m_comboGameType.InsertString(0,"8-Ball");
	m_comboGameType.InsertString(1,"9-Ball");
	m_comboGameType.InsertString(2,"Rotation Ball");
	m_comboGameType.InsertString(3,"Bowls");
	m_comboGameType.InsertString(4,"6-Ball (Un-Rated)");
	m_comboGameType.InsertString(5,"10-Ball");
	m_comboGameType.InsertString(6,"Break");
	m_comboGameType.InsertString(7,"English Pool");
	m_comboGameType.SetCurSel(m_gameType);

		m_comboRaceTo.ResetContent();
	m_comboRaceTo.InitStorage(10, 32); // 10 items max. (can change this to more later)
	m_comboRaceTo.InsertString(0,"1");
	m_comboRaceTo.InsertString(1,"2");
	m_comboRaceTo.InsertString(2,"3");
	m_comboRaceTo.InsertString(3,"4");
	m_comboRaceTo.InsertString(4,"5");
	m_comboRaceTo.InsertString(5,"6");
	m_comboRaceTo.InsertString(6,"7");
	m_comboRaceTo.InsertString(7,"8");
	m_comboRaceTo.InsertString(8,"9");
	m_comboRaceTo.InsertString(9,"10");
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
	m_comboTableColour.InitStorage(12, 32); // 12 items max. (can change this to more later)
	m_comboTableColour.InsertString(0,"Blue");
	m_comboTableColour.InsertString(1,"Red");
	m_comboTableColour.InsertString(2,"Green");
	m_comboTableColour.InsertString(3,"Black");
	m_comboTableColour.InsertString(4,"Yellow");
	m_comboTableColour.InsertString(5,"Purple");
	m_comboTableColour.InsertString(6,"Teal");
	m_comboTableColour.InsertString(7,"Brown");
	m_comboTableColour.InsertString(8,"White");
	m_comboTableColour.InsertString(9,"Blue & Black");
	m_comboTableColour.InsertString(10,"Red & Black");
	m_comboTableColour.InsertString(11,"Purple & Black");
	m_comboTableColour.SetCurSel(m_tableColour);

	m_comboTableType.ResetContent();
	m_comboTableType.InitStorage(9, 32); // 8 items max. (can change this to more later)
	m_comboTableType.InsertString(0,"Normal Table");
	m_comboTableType.InsertString(1,"Pro Table");
	m_comboTableType.InsertString(2,"UK Table");
	m_comboTableType.InsertString(3,"NoPockets: Center Hole");
	m_comboTableType.InsertString(4,"NoPockets: Black-Hole");
	m_comboTableType.InsertString(5,"NoPockets: Random 3-Holes");
	m_comboTableType.InsertString(6,"NoPockets: Random 5-Holes");
	m_comboTableType.InsertString(7,"NoPockets: Random 7-Holes");
	m_comboTableType.InsertString(8,"NoPockets: Random 9-Holes");
	//m_comboTableType.InsertString(?,"Pro");
	//m_comboTableType.InsertString(?,"Diamond");
	//m_comboTableType.InsertString(?,"T-Shape");
	//m_comboTableType.InsertString(?,"Cross");
	//m_comboTableType.InsertString(?,"Oval");
	m_comboTableType.SetCurSel(m_tableType);

	m_comboRackType.ResetContent();
	m_comboRackType.InitStorage(3, 32); // 3 items max. (can change this to more later)
	m_comboRackType.InsertString(0,"Normal");
	m_comboRackType.InsertString(1,"Groups");
	m_comboRackType.InsertString(2,"Random");
	m_comboRackType.SetCurSel(m_rackType);

	m_comboBallSize.ResetContent();
	m_comboBallSize.InitStorage(3, 32); // 3 items max. (can change this to more later)
	m_comboBallSize.InsertString(0,"Normal");
	m_comboBallSize.InsertString(1,"BIG");
	m_comboBallSize.InsertString(2,"small");
	m_comboBallSize.SetCurSel(m_ballSize);

	UpdateShotTime();
}
	
void CTableSetupDlg::OnButtonShotTime(void)
{
	UpdateShotTime();
}

void CTableSetupDlg::UpdateShotTime(void)
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_SHOTTIME);
	if(pEdit)
	{
		// limit "edit window" text (chars)
		pEdit->SetLimitText(2);
		
		// Set numbers only flag
		pEdit->ModifyStyle(0,ES_NUMBER);
		
		CButton *pButton = (CButton *)GetDlgItem(IDC_ADDTIMER);

		if(pButton)
		{
			if(pButton->GetCheck())
				pEdit->EnableWindow(TRUE);
			else
				pEdit->EnableWindow(FALSE);
		}
	}	
}

void CTableSetupDlg::EnableTableCtrls(BOOL bState = TRUE)
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

void CTableSetupDlg::OnAddtimer() 
{	  
	// TODO: Add your control notification handler code here
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

void CTableSetupDlg::OnProtable() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;


	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby && m_comboGameType.GetCurSel()<6);
	else UpdateData(FALSE);	
}

void CTableSetupDlg::OnUpdateShottime() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else UpdateData(FALSE);
}

void CTableSetupDlg::OnSelchangeGametype() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboGameType.SetCurSel(m_gameType);	
}

void CTableSetupDlg::OnSelchangeRaceto() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboRaceTo.SetCurSel(m_raceTo);
}

void CTableSetupDlg::OnSelchangeTablecolour() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby && m_comboTableType.GetCurSel()!=2);
	else m_comboTableColour.SetCurSel(m_tableColour);
}

void CTableSetupDlg::OnSelchangeWhobreaks() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby);
	else m_comboWhoBreaks.SetCurSel(m_whoBreaks);
}

void CTableSetupDlg::OnSelchangeBallsize() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby && m_comboGameType.GetCurSel()<6);
	else m_comboBallSize.SetCurSel(m_ballSize);
}

void CTableSetupDlg::OnSelchangeTabletype() 
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

void CTableSetupDlg::OnSelchangeRacktype() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;

	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() &&
		!g_bTournamentLobby && m_comboGameType.GetCurSel()<6);
	else m_comboRackType.SetCurSel(m_rackType);
}

void CTableSetupDlg::OnDoubles() 
{
	// TODO: Add your control notification handler code here
	int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);
	if(plrIdx==-1) return;


	if( ((CGameAppView *)m_pParent)->m_lobby->HostOfTable(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!((CGameAppView *)m_pParent)->m_lobby->GameStarted(((CGameAppView *)m_pParent)->m_playerSockIdx) &&
		!m_player[plrIdx].bIsGuest && !((CGameAppView *)m_pParent)->InLadderMatch() && !g_bTournamentLobby);
	else UpdateData(FALSE);	
}
