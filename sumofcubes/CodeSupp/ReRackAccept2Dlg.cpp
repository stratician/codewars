// ReRackAccept2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ReRackAccept2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReRackAccept2Dlg dialog


CReRackAccept2Dlg::CReRackAccept2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReRackAccept2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReRackAccept2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
	m_bAbortRaceTo = FALSE;
}


void CReRackAccept2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReRackAccept2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReRackAccept2Dlg, CDialog)
	//{{AFX_MSG_MAP(CReRackAccept2Dlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReRackAccept2Dlg message handlers

void CReRackAccept2Dlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}


void CReRackAccept2Dlg::OnOK()
{
	// TODO: Add extra validation here
	((CGameAppView *)m_pParent)->SendReRackPressed(((CGameAppView *)m_pParent)->m_playerSockIdx, TRUE, 0, m_bAbortRaceTo); // network game only!

	CDialog::OnOK();
}

int CReRackAccept2Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	return 0;
}

void CReRackAccept2Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	((CGameAppView *)m_pParent)->SendReRackPressed(((CGameAppView *)m_pParent)->m_playerSockIdx, TRUE, 1, m_bAbortRaceTo); // network game only!

	CDialog::OnCancel();
}
