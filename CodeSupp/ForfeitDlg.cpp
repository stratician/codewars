// ForfeitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ForfeitDlg.h"
#include "../Code/Lobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CForfeitDlg dialog


CForfeitDlg::CForfeitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CForfeitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CForfeitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	m_forfeitType = 0;
}


void CForfeitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CForfeitDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CForfeitDlg, CDialog)
	//{{AFX_MSG_MAP(CForfeitDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDABORT, OnAbort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CForfeitDlg message handlers

void CForfeitDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	// FORFEIT BUTTON:
	if(m_forfeitType == 0)
	{
		((CGameAppView *)m_pParent)->m_lobby->LostConnectionGameOver();
	}
	else if(m_forfeitType == 1)
	{
		((CGameAppView *)m_pParent)->m_lobby->MaxTimeUpGameOver();	
	}

	CDialog::OnOK();
}

void CForfeitDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	// WAIT-MORE BUTTON
	if(m_forfeitType == 0)
	{
		((CGameAppView *)m_pParent)->m_lobby->LostConnectionWaitMore();
	}
	else if(m_forfeitType == 1)
	{
		((CGameAppView *)m_pParent)->m_lobby->MaxTimeUpWaitMore();
	}

	CDialog::OnCancel();
}

int CForfeitDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	return 0;
}

void CForfeitDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CForfeitDlg::OnAbort() 
{
	// TODO: Add your control notification handler code here

	// Call Up Re-Rack Code //
	((CGameAppView *)m_pParent)->SendReRackPressed(g_gameAppView->m_playerSockIdx, TRUE, 8, TRUE); 
	CDialog::OnCancel();	
}
