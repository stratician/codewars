// notes: need to move this file & header into code directories [+]

// FoulDlg.cpp : implementation file
//

//#include "../CodeSupp/Build.h"
#include "stdafx.h"
#include "../CodeSupp/GameAppView.h"
#include "FoulDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFoulDlg dialog


CFoulDlg::CFoulDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFoulDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFoulDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFoulDlg::GetParent(CWnd* pParent /*= NULL*/)
{
	m_parent = pParent;
}

void CFoulDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFoulDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFoulDlg, CDialog)
	//{{AFX_MSG_MAP(CFoulDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP	
	ON_BN_CLICKED(IDALERTOK, OnButtonAlertOK)
	ON_BN_CLICKED(IDALERTCANCEL, OnButtonAlertCancel)	
	ON_BN_CLICKED(IDFOULMISSOK, OnButtonFoulMissOK)
	ON_BN_CLICKED(IDFOULMISSCANCEL, OnButtonFoulMissCancel)
	ON_BN_CLICKED(IDFOULMISSRB, OnButtonFoulMissRB)
	ON_BN_CLICKED(IDBADBREAK1, OnButtonBadBreak1)
	ON_BN_CLICKED(IDBADBREAK2, OnButtonBadBreak2)
	ON_BN_CLICKED(IDBADBREAK3, OnButtonBadBreak3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFoulDlg message handlers

void CFoulDlg::OnCancel() 
{
	//-#ifndef MAIN_SERVER

	// TODO: Add extra cleanup here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;	
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_NORMAL); // [?]

	
	CDialog::OnCancel();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnOK() 
{
	//-#ifndef MAIN_SERVER
	
	
	// TODO: Add extra validation here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_NORMAL); //[?]
	
		
	if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
		{
		if((g_build.gameType == GAMETYPE_POOL) && TestFlag( ((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERTUKPOOLFOUL) )
			{
			GAMEAPPVIEW->m_snooker->UpdateGameReceive();
			}
		else{
			GAMEAPPVIEW->m_snooker->PlayAgainReceive();
			}
		}
	else // online
		{
		if((g_build.gameType == GAMETYPE_POOL) && TestFlag( ((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERTUKPOOLFOUL) )
			{
			((CGameAppView *)m_parent)->SendUpdateGame(); // Update this for the Opponent //
			}
		else{
			((CGameAppView *)m_parent)->SendPlayerPlayAgain();
			}
		}

	CDialog::OnOK();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnButtonAlertCancel() 
{
	//-#ifndef MAIN_SERVER

	// TODO: Add extra cleanup here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;	
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERT); // [?]

	
	CDialog::OnCancel();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnButtonAlertOK() 
{
	
	//-#ifndef MAIN_SERVER
	
	// TODO: Add extra validation here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERT); //[?]	

	if(TestFlag( ((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERTPUSHOUT) )
	{
		if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
			GAMEAPPVIEW->m_snooker->FoulAlertReceive(1);
		else // online
			((CGameAppView *)m_parent)->SendPlayerFoulAlert(1);
	}
	else if(TestFlag( ((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERTPUSHOUT2) )
	{
		if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
			GAMEAPPVIEW->m_snooker->PlayAgainReceive();
		else // online
			((CGameAppView *)m_parent)->SendPlayerPlayAgain();
	}	
	else if(TestFlag( ((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERTPOT8ONBREAK) )
	{
		if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
			GAMEAPPVIEW->m_snooker->Pot8OnBreakRerackReceive();
		else
			((CGameAppView *)m_parent)->SendPot8BallOnBreakRerack();
	}	
/*
	else if(TestFlag( ((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERTUKPOOLFOUL) )
		{
		
		//SetFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_BALLINHAND);
		if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
			GAMEAPPVIEW->m_snooker->Pot8OnBreakRerackReceive();
		else
			((CGameAppView *)m_parent)->SendPot8BallOnBreakRerack();
		
		}
*/
	CDialog::OnOK();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnButtonFoulMissCancel() 
{
	//-#ifndef MAIN_SERVER

	// TODO: Add extra cleanup here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;	
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_MISS); // [?]


	CDialog::OnCancel();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnButtonFoulMissOK() 
{

	//-#ifndef MAIN_SERVER

	// TODO: Add extra validation here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_MISS); //[?]
	
	if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
	{
		GAMEAPPVIEW->m_snooker->PlayAgainReceive();
	}
	else // online
		((CGameAppView *)m_parent)->SendPlayerPlayAgain();

	CDialog::OnOK();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnButtonFoulMissRB() 
{
	//-#ifndef MAIN_SERVER

	// TODO: Add extra validation here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_MISS); //[?]
	
	if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
	{
		GAMEAPPVIEW->m_snooker->MissResetReceive();
	}
	else // online
		((CGameAppView *)m_parent)->SendPlayerMissReset();

	CDialog::OnOK();

	//-#endif //!MAIN_SERVER
}


void CFoulDlg::OnButtonBadBreak1() // "Play - On"
{
	//-#ifndef MAIN_SERVER
	
	// TODO: Add extra validation here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERT); //[?]
	
	// carry-on as normal
	
	//((CGameAppView *)m_parent)->SendPot8BallOnBreakRerack();
	
	
	CDialog::OnOK();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnButtonBadBreak2() // "Rerack & You Break"
{
	//-#ifndef MAIN_SERVER
	
	// TODO: Add extra validation here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERT); //[?]
	
	if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
	{
		GAMEAPPVIEW->m_snooker->BadBreakRerackReceive(FALSE);
	}
	else // online
		((CGameAppView *)m_parent)->SendBadBreakRerack(FALSE);
	
	
	CDialog::OnOK();

	//-#endif //!MAIN_SERVER
}

void CFoulDlg::OnButtonBadBreak3() // "Rerack & Opponent Breaks"
{
	//-#ifndef MAIN_SERVER
	
	// TODO: Add extra validation here
	((CGameAppView *)m_parent)->m_snooker->m_bFoulDecideTurn = FALSE;
	((CGameAppView *)m_parent)->m_snooker->m_bFoulShowDialog = FALSE;
	//((CGameAppView *)m_parent)->m_snooker->m_foul = 0;//[?]
	ClearFlag(((CGameAppView *)m_parent)->m_snooker->m_foul, Flg_FOUL_ALERT); //[?]
	
	if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
	{
		GAMEAPPVIEW->m_snooker->BadBreakRerackReceive(TRUE);
	}
	else // online
	((CGameAppView *)m_parent)->SendBadBreakRerack(TRUE);
	
	
	CDialog::OnOK();

	//-#endif //!MAIN_SERVER
}


int CFoulDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect;
	rect.SetRect(0, 0, 250, 32);
	rect.OffsetRect(10, 5);
	
	char str[80];
	sprintf(str, "<empty>");

	m_staticText.Create(str, WS_CHILD | WS_VISIBLE, rect, this); 

	return 0;
}

