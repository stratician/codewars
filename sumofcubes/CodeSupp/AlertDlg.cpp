// AlertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "AlertDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg dialog


CAlertDlg::CAlertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	strcpy(m_str, "Empty");
//	m_bAddExtraButton = FALSE;
//	m_extraButtonID = 0xffff;
//	strcpy(m_extraStr, "Empty");
}


void CAlertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertDlg, CDialog)
	//{{AFX_MSG_MAP(CAlertDlg)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg message handlers

void CAlertDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CAlertDlg::SetStr(char *str)
{
	if(!str) return;
	if(!str[0]) return;
	strcpy(m_str, str);
}

/*
void CAlertDlg::SetExtraStr(char *str)
{
	if(!str) return;
	if(!str[0]) return;
	strcpy(m_extraStr, str);
}
*/

void CAlertDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

int CAlertDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();

	CRect rect;

	rect.SetRect(0, 0, 250, 96);
	rect.OffsetRect(10,10);
	
	m_staticText.Create(m_str, WS_CHILD | WS_VISIBLE, rect, this); 

	return 0;
}

void CAlertDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
}
