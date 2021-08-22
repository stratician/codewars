// DetailsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "DetailsDlg.h"
#include "../Code/Lobby.h"
#include "build.h"
#include "LoginWizardDlg.h"

#ifdef _DEBUG						   
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern BOOL g_RememberPassword;


/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg dialog


CDetailsDlg::CDetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsDlg)
	m_name = _T("");
	m_password = _T("");
	m_email = _T("");
	m_comments = _T("");
	m_town = _T("");
	m_realName = _T("");
	m_sex = _T("");
	m_age = _T("");
	m_version = _T("");
	m_bpass = _T("");
	//}}AFX_DATA_INIT

	
	
	return;
}

void CDetailsDlg::Init(void)
{
	// load details from file

	LoadDetailsFile("MyDetails.txt", &m_details);
	m_bpass = m_details.m_bpass;
	m_version = m_details.m_version;
		
	if(!strcmpi(m_bpass, "SAVE")) g_RememberPassword = TRUE;
		else g_RememberPassword = FALSE;
	m_name = m_details.m_name;
	if(g_RememberPassword)
	{
		if(!strcmpi(m_version, _T("1.01")))	m_password = DECRYPT_STR(m_details.m_password);
			else m_password = m_details.m_password;
	}
	m_email = _T("");//m_details.m_email; // not used at the mo
	m_town = m_details.m_town;
	m_comments = m_details.m_comments;
	m_realName = m_details.m_realName;
	m_sex = m_details.m_sex;
	m_age = m_details.m_age;
}

void CDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsDlg)
	DDX_Text(pDX, IDC_EDITName, m_name);
	DDV_MaxChars(pDX, m_name, 19);
	DDX_Text(pDX, IDC_EDITPassword, m_password);
	DDV_MaxChars(pDX, m_password, 18);
	DDX_Text(pDX, IDC_EDITEmail, m_email);
	DDX_Text(pDX, IDC_EDITComments, m_comments);
	DDV_MaxChars(pDX, m_comments, 128);
	DDX_Text(pDX, IDC_EDITTown, m_town);
	DDV_MaxChars(pDX, m_town, 28);
	DDX_Text(pDX, IDC_EDITName2, m_realName);
	DDV_MaxChars(pDX, m_realName, 19);
	DDX_Text(pDX, IDC_EDITSex2, m_sex);
	DDV_MaxChars(pDX, m_sex, 1);
	DDX_Text(pDX, IDC_EDITAge2, m_age);
	DDV_MaxChars(pDX, m_age, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsDlg, CDialog)
	//{{AFX_MSG_MAP(CDetailsDlg)
	ON_BN_CLICKED(IDSAVE, OnSave)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg message handlers
void CDetailsDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	OnOK();

	// save details to file
	m_details.m_name = m_name;
	m_details.m_password = ENCRYPT_STR(m_password);
	m_details.m_email = m_email;
	m_details.m_town = m_town;
	m_details.m_comments = m_comments;
	m_details.m_realName = m_realName;
	m_details.m_sex = m_sex;
	m_details.m_age = m_age;
	m_details.m_version = m_version;
	if(g_RememberPassword) m_details.m_bpass = _T("SAVE");
		else m_details.m_bpass = _T("");

		
	SaveDetailsFile("MyDetails.txt", &m_details);
	return;
	
	//g_nick = m_UserName;
	//gameShell->SetNick(g_nick);
	//m_playerN = 1;
	//m_creator = TRUE;	
}

/*
void CDetailsDlg::OnSettings() 
{
	// TODO: Add your control notification handler code here
	//CWnd *wnd = GetParent();
	//((CGameAppView *)wnd)->m_detailsSettingsDlg.Create(IDD_DETAILSSETTINGS);
	//((CGameAppView *)wnd)->m_detailsSettingsDlg.ShowWindow(SW_SHOW);	
}
*/






int CDetailsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();	
	return 0;
}

