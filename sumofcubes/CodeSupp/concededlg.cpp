// concededlg.cpp : implementation file
//

#include "../CodeSupp/stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "concededlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConcedeDlg dialog


CConcedeDlg::CConcedeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConcedeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcedeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CConcedeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcedeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConcedeDlg, CDialog)
	//{{AFX_MSG_MAP(CConcedeDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcedeDlg message handlers

void CConcedeDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CConcedeDlg::OnOK() 
{
	// TODO: Add extra validation here
	// Get my table Idx
	g_gameAppView->SendConcedePressed(FALSE, ((CGameAppView *)m_pParent)->m_playerSockIdx); // network game only!
	

	CDialog::OnOK();
}

int CConcedeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	return 0;
}
