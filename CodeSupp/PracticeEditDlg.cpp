// PracticeEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "PracticeEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPracticeEditDlg dialog


CPracticeEditDlg::CPracticeEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPracticeEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPracticeEditDlg)
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CPracticeEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPracticeEditDlg)
	DDX_Control(pDX, IDC_BUTTONPLAYSHOT, m_buttonPlay);
	DDX_Control(pDX, IDC_BUTTONCOMMENT, m_buttonComment);
	DDX_Control(pDX, IDC_BUTTONBLACK, m_buttonBlack);
	DDX_Control(pDX, IDC_BUTTONBLUE, m_buttonBlue);
	DDX_Control(pDX, IDC_BUTTONBROWN, m_buttonBrown);
	DDX_Control(pDX, IDC_BUTTONERASE, m_buttonErase);
	DDX_Control(pDX, IDC_BUTTONGREEN, m_buttonGreen);
	DDX_Control(pDX, IDC_BUTTONPINK, m_buttonPink);
	DDX_Control(pDX, IDC_BUTTONYELLOW, m_buttonYellow);
	DDX_Control(pDX, IDC_BUTTONRED, m_buttonRed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPracticeEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPracticeEditDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTONBLACK, OnButtonblack)
	ON_BN_CLICKED(IDC_BUTTONBLUE, OnButtonblue)
	ON_BN_CLICKED(IDC_BUTTONBROWN, OnButtonbrown)
	ON_BN_CLICKED(IDC_BUTTONERASE, OnButtonerase)
	ON_BN_CLICKED(IDC_BUTTONGREEN, OnButtongreen)
	ON_BN_CLICKED(IDC_BUTTONPINK, OnButtonpink)
	ON_BN_CLICKED(IDC_BUTTONPLAYSHOT, OnButtonplayshot)
	ON_BN_CLICKED(IDC_BUTTONRED, OnButtonred)
	ON_BN_CLICKED(IDC_BUTTONYELLOW, OnButtonyellow)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTONCOMMENT, OnButtoncomment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPracticeEditDlg message handlers

void CPracticeEditDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

int CPracticeEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();

	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = TRUE;
	((CGameAppView *)m_pParent)->m_practiceEditColour = WHITE;

	
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		m_buttonPlay.SetText("play");
		m_buttonRed.SetIcon(IDI_REDBALL, 32, 32);
		m_buttonRed.SetImagePos( CPoint( 6, 7 ) );
		m_buttonYellow.SetIcon(IDI_YELLOWBALL, 32, 32);
		m_buttonYellow.SetImagePos( CPoint( 6, 7 ) );
		m_buttonGreen.SetIcon(IDI_GREENBALL, 32, 32);
		m_buttonGreen.SetImagePos( CPoint( 6, 7 ) );
		m_buttonBrown.SetIcon(IDI_BROWNBALL, 32, 32);
		m_buttonBrown.SetImagePos( CPoint( 6, 7 ) );
		m_buttonBlue.SetIcon(IDI_BLUEBALL, 32, 32);
		m_buttonBlue.SetImagePos( CPoint( 6, 7 ) );
		m_buttonPink.SetIcon(IDI_PINKBALL, 32, 32);
		m_buttonPink.SetImagePos( CPoint( 6, 7 ) );
		m_buttonBlack.SetIcon(IDI_BLACKBALL, 32, 32);
		m_buttonBlack.SetImagePos( CPoint( 6, 7 ) );
		m_buttonErase.SetIcon(IDI_ERASE, 32, 32);
		m_buttonErase.SetImagePos( CPoint( 5, 7 ) );
		m_buttonComment.SetIcon(IDI_COMMENT, 32, 32);
		m_buttonComment.SetImagePos( CPoint( 4, 5 ) );
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		m_buttonPlay.SetText("play");
		m_buttonRed.SetIcon(IDI_SPOTBALL, 32, 32);
		m_buttonRed.SetImagePos( CPoint( 6, 7 ) );
		m_buttonYellow.SetIcon(IDI_STRIPEBALL, 32, 32);
		m_buttonYellow.SetImagePos( CPoint( 6, 7 ) );
		m_buttonGreen.SetIcon(IDI_8BALL, 32, 32);
		m_buttonGreen.SetImagePos( CPoint( 6, 7 ) );
		m_buttonBrown.SetIcon(IDI_9BALL, 32, 32);
		m_buttonBrown.SetImagePos( CPoint( 6, 7 ) );
		//m_buttonBlue.SetIcon(IDI_BLUEBALL, 32, 32);
		//m_buttonBlue.SetImagePos( CPoint( 6, 7 ) );
		//m_buttonPink.SetIcon(IDI_PINKBALL, 32, 32);
		//m_buttonPink.SetImagePos( CPoint( 6, 7 ) );
		//m_buttonBlack.SetIcon(IDI_BLACKBALL, 32, 32);
		//m_buttonBlack.SetImagePos( CPoint( 6, 7 ) );
		m_buttonErase.SetIcon(IDI_ERASE, 32, 32);
		m_buttonErase.SetImagePos( CPoint( 5, 7 ) );
		m_buttonComment.SetIcon(IDI_COMMENT, 32, 32);
		m_buttonComment.SetImagePos( CPoint( 4, 5 ) );
	}
	
	m_commentDlg.DestroyWindow();
	m_commentDlg.Create(IDD_PRACTICECOMMENTS);
	m_commentDlg.CenterWindow();
				
	return 0;
}

BOOL CPracticeEditDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
		
	return CDialog::DestroyWindow();
}

void CPracticeEditDlg::OnButtonplayshot() 
{
	// TODO: Add your control notification handler code here
	
	if( !((CGameAppView *)m_pParent)->m_bPracticeEditPlay )
	{
		// play mode
		m_buttonPlay.SetText("play");
		//m_buttonPlay.SetWindowText("play");
		((CGameAppView *)m_pParent)->m_bPracticeEditPlay = TRUE;
		((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Play Mode");	

		// now set rereack type loaded position
		((CGameAppView *)m_pParent)->m_snooker->m_reRackType = RERACK_LOAD;
			
		// invalidate all
		for(int n=0;n<MAX_TOTALBALLS;n++)
		{
			((CGameAppView *)m_pParent)->m_snooker->m_ballPosColour[n].x = 0;
			((CGameAppView *)m_pParent)->m_snooker->m_ballPosColour[n].y = 0;
			((CGameAppView *)m_pParent)->m_snooker->m_ballPosColour[n].colour = -1;
		}
		
		// find the lowest colour ball on table (including reds)
		int lowestColour = BLACK;
		float x,y;
		int colour;

		if(g_build.gameType == GAMETYPE_POOL) lowestColour = 15;

		for(int n=0;n<MAX_TOTALBALLS;n++)
		{
			if( TestFlag(((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_flags, Flg_BALL_ACTIVE) )
			{
			// store ball pos & colour - for re-rack (Load setup)
			x = ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_P.x;
			y = ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_P.y;
			colour = ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_colour;
			((CGameAppView *)m_pParent)->m_snooker->m_ballPosColour[n].x = -x;
			((CGameAppView *)m_pParent)->m_snooker->m_ballPosColour[n].y = -y;
			((CGameAppView *)m_pParent)->m_snooker->m_ballPosColour[n].colour = colour;

			// init a snooker ball
			((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->Init(-x,-y,0, colour, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);

			if(g_build.gameType == GAMETYPE_SNOOKER)
				((CGameAppView *)m_pParent)->PracticeEditRefreshBallSpots(n);
			}
			
			if( TestFlag( ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_flags, Flg_BALL_ACTIVE) )
			if(((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_colour != WHITE)
			if( ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_colour < lowestColour )
				lowestColour = ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_colour;
		}
		
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			if(lowestColour == RED)
			{
				((CGameAppView *)m_pParent)->m_snooker->m_colourseq = 0;
				((CGameAppView *)m_pParent)->m_snooker->m_rednext = TRUE;
			}
			else
			{
				((CGameAppView *)m_pParent)->m_snooker->m_colourseq = lowestColour;
				((CGameAppView *)m_pParent)->m_snooker->m_rednext = FALSE;
			}
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			((CGameAppView *)m_pParent)->m_snooker->m_colourseq = lowestColour;
			((CGameAppView *)m_pParent)->m_snooker->m_rednext = TRUE;
		}
	}
	else 
	{
		// edit mode
		m_buttonPlay.SetText("edit");
		//m_buttonPlay.SetWindowText("edit");
		((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
		((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");
	}
}

void CPracticeEditDlg::OnButtoncomment() 
{
	// TODO: Add your control notification handler code here
	m_commentDlg.ShowWindow(SW_SHOW);
/*
	CEdit *pEdit = (CEdit *)m_commentDlg.GetDlgItem(IDC_PRACTICECOMMENTS_EDIT);
	if(pEdit)
	{
		//int n = pEdit->GetLineCount();
		m_sendEdit.Paste();
		m_sendEdit.SetSel(0,-1);
		CString strText = m_sendEdit.GetSelText();
		strText.TrimRight();
		m_sendEdit.ReplaceSel(strText);		 
	}
	*/
}

void CPracticeEditDlg::OnButtonblack() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");
	((CGameAppView *)m_pParent)->m_practiceEditColour = BLACK;

}

void CPracticeEditDlg::OnButtonblue() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");
	((CGameAppView *)m_pParent)->m_practiceEditColour = BLUE;

}

void CPracticeEditDlg::OnButtonbrown() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");
	((CGameAppView *)m_pParent)->m_practiceEditColour = BROWN;
}

void CPracticeEditDlg::OnButtonerase() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");	
	((CGameAppView *)m_pParent)->m_practiceEditColour = -1;
}

void CPracticeEditDlg::OnButtongreen() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");
	((CGameAppView *)m_pParent)->m_practiceEditColour = GREEN;
}

void CPracticeEditDlg::OnButtonpink() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");
	((CGameAppView *)m_pParent)->m_practiceEditColour = PINK;
}

void CPracticeEditDlg::OnButtonred() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");
	((CGameAppView *)m_pParent)->m_practiceEditColour = RED;
}

void CPracticeEditDlg::OnButtonyellow() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = FALSE;
	m_buttonPlay.SetWindowText("edit");
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Edit Mode");	
	((CGameAppView *)m_pParent)->m_practiceEditColour = YELLOW;
}

void CPracticeEditDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_commentDlg.DestroyWindow();
	
	((CGameAppView *)m_pParent)->m_bPracticeEditPlay = TRUE;
	((CGameAppView *)m_pParent)->ChangeTitle(" Practice - Play Mode");

	// find the lowest colour ball on table (including reds)
	int lowestColour = BLACK;

	for(int n=0;n<MAX_TOTALBALLS;n++)
	{
		((CGameAppView *)m_pParent)->PracticeEditRefreshBallSpots(n);

		if( TestFlag( ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_flags, Flg_BALL_ACTIVE) )
		if(((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_colour != WHITE)
		if( ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_colour < lowestColour )
			lowestColour = ((CGameAppView *)m_pParent)->m_snooker->m_ball[n]->m_colour;
	}
	
	if(lowestColour == RED)
	{
		((CGameAppView *)m_pParent)->m_snooker->m_colourseq = 0;
		((CGameAppView *)m_pParent)->m_snooker->m_rednext = TRUE;
	}
	else
	{
		((CGameAppView *)m_pParent)->m_snooker->m_colourseq = lowestColour;
		((CGameAppView *)m_pParent)->m_snooker->m_rednext = FALSE;
	}
	
	CDialog::OnClose();
}
