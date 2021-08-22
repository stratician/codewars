// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "ChatView.h"
#include "AboutDlg.h"
#include "GameAppView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg

IMPLEMENT_DYNCREATE(CAboutDlg, CDialog)

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_aboutVersion = _T("");
	//}}AFX_DATA_INIT
	char buffer[80];
	if(g_build.gameType == GAMETYPE_SNOOKER)
		sprintf(buffer,"iSnooker Version %s", g_build.version);
	else if(g_build.gameType == GAMETYPE_POOL)
		sprintf(buffer,"iPool Version %s", g_build.version);
	m_aboutVersion = buffer;	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_ABOUTVERSION, m_aboutVersion);
	//}}AFX_DATA_MAP
}

void CAboutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	char buffer[80];
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		SetWindowText("About TheSnookerClub.com - iSnooker");
		sprintf(buffer,"iSnooker Version %s", g_build.version);
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		SetWindowText("About ThePoolClub.com - iPool");
		sprintf(buffer,"iPool Version %s", g_build.version);
	}
	m_aboutVersion = buffer;	

	return 0;
}
