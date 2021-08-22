// GuestRegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "ChatView.h"
#include "GuestRegisterDlg.h"
#include "GameAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuestRegisterDlg dialog


CGuestRegisterDlg::CGuestRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGuestRegisterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuestRegisterDlg)
	m_registerPrice = _T("");
	m_registerURL = _T("");
	//}}AFX_DATA_INIT

	char buffer[128];
	char buffer2[128];
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		sprintf(buffer,"For just $24  (6 months) or $37  (1 year)");
		sprintf(buffer2,"URL: http://www.thesnookerclub.com/register.html");
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		sprintf(buffer,"For just $17  (6 months) or $27  (1 year)");
		sprintf(buffer2,"URL: http://www.thepoolclub.com/register.html");		
	}
	m_registerPrice = buffer;
	m_registerURL = buffer2;
}


void CGuestRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuestRegisterDlg)
	DDX_Text(pDX, IDC_REGISTERPRICE, m_registerPrice);
	DDX_Text(pDX, IDC_REGISTERURL, m_registerURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuestRegisterDlg, CDialog)
	//{{AFX_MSG_MAP(CGuestRegisterDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_REGISTERURL, &CGuestRegisterDlg::OnStnClickedRegisterurl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuestRegisterDlg message handlers

void CGuestRegisterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CGuestRegisterDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(g_build.gameType == GAMETYPE_SNOOKER)
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.thesnookerclub.com/index2.php?mainframe=register.html"),_T(""),NULL,0);
	else if(g_build.gameType == GAMETYPE_POOL)
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.thepoolclub.com/index2.php?mainframe=register.html"),_T(""),NULL,0);
	
	CDialog::OnOK();
}

int CGuestRegisterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	char buffer[128];
	char buffer2[128];
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		sprintf(buffer,"For just $7 CAD (1 Month), $16 (3 Months), $25 CAD (6 Months) or $38 CAD (1 Year)");
		sprintf(buffer2,"URL: http://www.thesnookerclub.com/register.php");
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		sprintf(buffer,"For just $4 CAD (3 Month) or $6 CAD  (6 Months) or $10 CAD  (1 Year)");
		sprintf(buffer2,"URL: http://www.thepoolclub.com/register.php");		
	}
	m_registerPrice = buffer;
	m_registerURL = buffer2;
	
	return 0;
}


void CGuestRegisterDlg::OnStnClickedRegisterurl()
{
	// TODO: Add your control notification handler code here
}
