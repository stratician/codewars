// GameSetup.cpp : implementation file
//


//#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "GameSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameSetup dialog

extern CString g_nick;

CGameSetup::CGameSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CGameSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameSetup)
	m_IPAddress = _T("");
	m_UserName = g_nick;//_T("Test");
	//}}AFX_DATA_INIT
	m_playerN = 0; // player doesn't exist currently
}

void CGameSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameSetup)
	DDX_Text(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameSetup, CDialog)
	//{{AFX_MSG_MAP(CGameSetup)
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_JOIN, OnJoin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameSetup message handlers

void CGameSetup::OnCreate() 
{
	// TODO: Add your control notification handler code here
	OnOK();
	
	g_nick = m_UserName;
	//*#ifndef MAIN_SERVER
	if(gameShell) gameShell->SetNick(g_nick);	// *OLD CODE - no purpose for being here now
	//*#endif //!MAIN_SERVER
	m_playerN = 1;
	m_creator = TRUE;
}

void CGameSetup::OnJoin() 
{
	// TODO: Add your control notification handler code here
	OnOK();
	
	//IDC_IPADDRESS
	g_nick = m_UserName;
	//*#ifndef MAIN_SERVER
	if(gameShell) gameShell->SetNick(g_nick);	// *OLD CODE - no purpose for being here now

	//*#endif //!MAIN_SERVER
	m_playerN = 2;
	m_creator = FALSE;
}
