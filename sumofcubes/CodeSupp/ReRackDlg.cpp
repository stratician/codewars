// ReRackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "rerackdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReRackDlg dialog


CReRackDlg::CReRackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReRackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReRackDlg)
	m_bAbortRaceTo = FALSE;
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CReRackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReRackDlg)
	// DDX_Check(pDX, IDC_ABORTRACETO, m_bAbortRaceTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReRackDlg, CDialog)
	//{{AFX_MSG_MAP(CReRackDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReRackDlg message handlers

void CReRackDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CReRackDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	//((CGameAppView *)m_pParent)->SendReRackPressed(((CGameAppView *)m_pParent)->m_playerSockIdx, FALSE, -1, m_bAbortRaceTo); // network game only!
	g_gameAppView->SendReRackPressed(((CGameAppView *)m_pParent)->m_playerSockIdx, FALSE, -1, m_bAbortRaceTo); // network game only!
											  
	CDialog::OnOK();
}

int CReRackDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();

	return 0;
}
