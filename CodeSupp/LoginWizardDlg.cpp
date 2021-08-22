// LoginWizardDlg.cpp : implementation file
//

#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "LoginWizardDlg.h"
#include "../Code/Lobby.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef THE_POOL_CLUB
#define LOGIN_WIZARD_DLG		IDD_LOGINWIZARDDLG3
#else
#define LOGIN_WIZARD_DLG		IDD_LOGINWIZARDDLG2
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginWizardDlg dialog

CLoginWizardDlg::CLoginWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LOGIN_WIZARD_DLG/*CLoginWizardDlg::IDD*/, pParent)
{
	//{{AFX_DATA_INIT(CLoginWizardDlg)
	m_name = _T("");
	m_password = _T("");
	m_bShow = FALSE;
	m_bPass = FALSE;
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	m_bActive = FALSE;
	m_bShow = TRUE;

}

void CLoginWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginWizardDlg)
	DDX_Text(pDX, IDC_EDITName, m_name);
	DDV_MaxChars(pDX, m_name, 19);
	DDX_Text(pDX, IDC_EDITPassword, m_password);
	DDV_MaxChars(pDX, m_password, 18);
	DDX_Check(pDX, IDC_CHECK1, m_bShow);
	DDX_Check(pDX, IDC_PASSCHECK1, m_bPass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginWizardDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_PASSCHECK1, OnPassCheck1)
	ON_BN_CLICKED(ID_PURCHASECODE, OnPurchasecode)
	ON_BN_CLICKED(ID_CREATENEWACCOUNT, OnCreatenewaccount)
	ON_BN_CLICKED(ID_TOPUPACCOUNT, OnTopupaccount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginWizardDlg message handlers


void CLoginWizardDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CLoginWizardDlg::OnOK() 
{
	// TODO: Add extra validation here

	// copy back name & password fields to login details dialog
	UpdateData(TRUE);
	((CGameAppView *)m_pParent)->m_detailsDlg.m_name = m_name;
	((CGameAppView *)m_pParent)->m_detailsDlg.m_password = m_password;
	
	// save details to file
	m_details.m_version = _T("1.01");
	m_details.m_name = ((CGameAppView *)m_pParent)->m_detailsDlg.m_name;
	m_details.m_password = ENCRYPT_STR(((CGameAppView *)m_pParent)->m_detailsDlg.m_password);
	m_details.m_email = ((CGameAppView *)m_pParent)->m_detailsDlg.m_email;
	m_details.m_town = ((CGameAppView *)m_pParent)->m_detailsDlg.m_town;
	m_details.m_comments = ((CGameAppView *)m_pParent)->m_detailsDlg.m_comments;
	if(g_RememberPassword) m_details.m_bpass = _T("SAVE");
		else m_details.m_bpass = _T("");

	SaveDetailsFile("MyDetails.txt", &m_details);
	

	// show internet lobby gateway dialog
	((CGameAppView *)m_pParent)->m_connectStyle = CONNECTSTYLE_NORMAL;
	((CGameAppView *)m_pParent)->ShowTerms(); // This Function Calls up ConnectToLobby if User Agrees with Terms - ConnectToLobby(TRUE);

	// Initialise ignorelst names and fname //
	char buf[80];
	sprintf(buf, "%s_ilst.txt", m_name);
	((CGameAppView *)m_pParent)->m_ignorelst->Load(buf);

	CDialog::OnOK();

}

int CLoginWizardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_bShow = g_gfxDetails.bShowLoginWizard;

	if(g_build.gameType == GAMETYPE_SNOOKER)
		SetWindowText("iSnooker Login Wizard");
	else if(g_build.gameType == GAMETYPE_POOL)
		SetWindowText("iPool Login Wizard");

	CenterWindow();


	


	return 0;
}

void CLoginWizardDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CLoginWizardDlg::CopyPlayerLoginDetails(void)
{
	m_name = ((CGameAppView *)m_pParent)->m_detailsDlg.m_name;
	m_password = ((CGameAppView *)m_pParent)->m_detailsDlg.m_password;
	if(!strcmpi(((CGameAppView *)m_pParent)->m_detailsDlg.m_bpass, "SAVE")) m_bPass = TRUE;
	if(g_RememberPassword) m_bPass = TRUE;
	
}

void CLoginWizardDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	m_bActive = TRUE;
	
	UpdateData(TRUE);
	
	g_gfxDetails.bShowLoginWizard = m_bShow;

	// set menu option show login wizard
	CMenu *menu = AfxGetMainWnd()->GetMenu();
	CMenu *subMenu = NULL;
	if(g_gfxDetails.bShowLoginWizard)
	{
		g_gfxDetails.bShowLoginWizard = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOGIN_WIZARD, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bShowLoginWizard = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOGIN_WIZARD, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
}


void CLoginWizardDlg::OnPassCheck1() 
{
	// TODO: Add your control notification handler code here
	m_bActive = TRUE;
	UpdateData(TRUE);
	
	if(m_bPass==FALSE) g_RememberPassword=FALSE;
		else g_RememberPassword=TRUE;

}

void CLoginWizardDlg::OnCreatenewaccount() 
{
	// copy back name & password fields to login details dialog
	UpdateData(TRUE);
	((CGameAppView *)m_pParent)->m_detailsDlg.m_name = m_name;
	((CGameAppView *)m_pParent)->m_detailsDlg.m_password = m_password;
	
	// save details to file
	m_details.m_version = _T("1.01");
	m_details.m_name = ((CGameAppView *)m_pParent)->m_detailsDlg.m_name;
	m_details.m_password = ENCRYPT_STR(((CGameAppView *)m_pParent)->m_detailsDlg.m_password);
	m_details.m_email = ((CGameAppView *)m_pParent)->m_detailsDlg.m_email;
	m_details.m_town = ((CGameAppView *)m_pParent)->m_detailsDlg.m_town;
	m_details.m_comments = ((CGameAppView *)m_pParent)->m_detailsDlg.m_comments;
	if(g_RememberPassword) m_details.m_bpass = _T("SAVE");
		else m_details.m_bpass = _T("");

	SaveDetailsFile("MyDetails.txt", &m_details);
	
	// TODO: Add your control notification handler code here
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		((CGameAppView *)m_pParent)->m_connectStyle = CONNECTSTYLE_NEWACC;
		((CGameAppView *)m_pParent)->ShowTerms(); // This Function Calls up ConnectToLobby if User Agrees with Terms - ConnectToLobby(TRUE);
		CDialog::OnOK();
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		((CGameAppView *)m_pParent)->m_connectStyle = CONNECTSTYLE_NEWACC;
		((CGameAppView *)m_pParent)->ShowTerms(); // This Function Calls up ConnectToLobby if User Agrees with Terms - ConnectToLobby(TRUE);
		CDialog::OnOK();

		// ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.thepoolclub.com/register.php"),_T(""),NULL,0);	
	}
	
	
	
}

void CLoginWizardDlg::OnTopupaccount() 
{
	// TODO: Add your control notification handler code here
	((CGameAppView *)m_pParent)->m_connectStyle = CONNECTSTYLE_TOPUP;
	((CGameAppView *)m_pParent)->ShowTerms(); // This Function Calls up ConnectToLobby if User Agrees with Terms - ConnectToLobby(TRUE);
	
	CDialog::OnOK();
}

void CLoginWizardDlg::OnPurchasecode() 
{
	// TODO: Add your control notification handler code here
	if(g_build.gameType == GAMETYPE_SNOOKER)
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.thesnookerclub.com/register.php"),_T(""),NULL,0);	
	else if(g_build.gameType == GAMETYPE_POOL)
	ShellExecute(AfxGetMainWnd()->m_hWnd,_T("open"), _T("http://www.thepoolclub.com/register.php"),_T(""),NULL,0);	
	
	CDialog::OnOK();
}
