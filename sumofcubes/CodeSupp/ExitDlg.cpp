// ExitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ExitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExitDlg dialog


CExitDlg::CExitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExitDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExitDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExitDlg, CDialog)
	//{{AFX_MSG_MAP(CExitDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExitDlg message handlers

void CExitDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CExitDlg::OnOK() 
{
	// TODO: Add extra validation here

	// if clicked 'x' in top right corner
	if(((CGameAppView *)m_pParent)->m_bMainFramePlayingClose)
	{
		((CGameAppView *)m_pParent)->SendConcedePressed(FALSE, ((CGameAppView *)m_pParent)->m_playerSockIdx, TRUE, TRUE); // network game only!		

	}
	else // default
	{
		// concede frame & exit table
		((CGameAppView *)m_pParent)->SendConcedePressed(FALSE, ((CGameAppView *)m_pParent)->m_playerSockIdx, TRUE); // network game only!

		//((CGameAppView *)m_pParent)->ExitTable2Lobby();
	}

	CDialog::OnOK();
}

void CExitDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	if(((CGameAppView *)m_pParent)->m_bMainFramePlayingClose)
	{
		((CGameAppView *)m_pParent)->m_bMainFramePlayingClose = FALSE;
	}

	CDialog::OnCancel();
}

int CExitDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();

	return 0;
}
