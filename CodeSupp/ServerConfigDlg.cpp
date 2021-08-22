// ServerConfigDlg.cpp : implementation file
//

//#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"
#include "ServerConfigDlg.h"
#include "GameAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerConfigDlg dialog


CServerConfigDlg::CServerConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerConfigDlg)
	m_10Reds = FALSE;
	m_accountSettings = _T("");
	m_accountSettingsTick = FALSE;
	m_boot = FALSE;
	m_chat = FALSE;
	m_filenameRankings = _T("");
	m_guests = 0;
	m_invite = FALSE;
	m_matchTableAccess = FALSE;
	m_tableType = FALSE;
	m_users = 0;
	m_postRankings = 0;
	m_crcvalid = _T("");
	m_day = 0;
	m_month = 0;
	m_year = 0;
	m_freeaccounts = FALSE;
	m_logTableChat = FALSE;
	m_logSocketData = FALSE;
	m_logGameResult = FALSE;
	m_daysFreeTrial = 0;
	m_mostRunOutsPrize = FALSE;
	//}}AFX_DATA_INIT

		m_day = 1;
		m_month = 12;
		m_year = 3; // 1st Dec 2003 defaults to.
		m_freeaccounts = FALSE;
		m_logSocketData = FALSE;
		m_logGameResult = FALSE;

	//-#ifdef MAIN_SERVER
	// open 'serverConfig.txt' - this contains the server's configuration settings
	if(Load()); 
	else
	{
		// since file doesn't exist, setup default settings and write to our file
		
		// Guest Settings
		m_tableType = FALSE;
		m_invite = FALSE;
		m_boot = FALSE;
		m_chat = FALSE;
		m_matchTableAccess = FALSE;
		m_10Reds = FALSE;
		
		// General Settings
		m_postRankings = 30; // 30 mins
		m_filenameRankings = _T("rankings.html");
		m_users = 32; // max total users allowed at any one time
		m_guests = 16; // max guests allowed at any one time

		// Account Settings
		m_accountSettings = _T("1234");
		m_accountSettingsTick = TRUE;
		m_crcvalid = _T("");

		m_day = 1;
		m_month = 12;
		m_year = 3; // 1st Dec 2003 defaults to.
		m_freeaccounts = FALSE;
		g_socketlog = m_logSocketData;
		g_gamelog = m_logGameResult;
		g_daysFreeTrial = m_daysFreeTrial;
		Save();
	}	
	m_pParent = NULL;
	//-#endif
}


void CServerConfigDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CServerConfigDlg::Save(void)
{
	FILE *fptr = NULL;
	UpdateConfig();

	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, "serverConfig.txt");

	fptr = fopen(fname_full,"w");
	if(fptr)
	{
		int v32;
		fprintf(fptr,"%d\n",m_tableType);
		fprintf(fptr,"%d\n",m_invite);
		fprintf(fptr,"%d\n",m_boot);
		fprintf(fptr,"%d\n",m_chat);
		fprintf(fptr,"%d\n",m_matchTableAccess);
		fprintf(fptr,"%d\n",m_10Reds);

		fprintf(fptr,"%d\n",m_postRankings);
		fprintf(fptr,"%s\n",m_filenameRankings.GetBuffer(0));
		fprintf(fptr,"%d\n",m_users);
		fprintf(fptr,"%d\n",m_guests);
				
		fprintf(fptr,"%s\n",m_accountSettings.GetBuffer(0));
		fprintf(fptr,"%d\n",m_accountSettingsTick);

		fprintf(fptr,"%d\n",m_day);
		fprintf(fptr,"%d\n",m_month);
		fprintf(fptr,"%d\n",m_year);
		v32 = m_freeaccounts;
		fprintf(fptr,"%d\n",v32);
		v32 = m_logTableChat;
		fprintf(fptr,"%d\n",v32);		
		v32 = m_logSocketData;
		fprintf(fptr,"%d\n",v32);
		v32 = m_logGameResult;
		fprintf(fptr,"%d\n",v32);
		v32 = m_daysFreeTrial;
		fprintf(fptr,"%d\n",v32);		
		v32 = m_mostRunOutsPrize;
		fprintf(fptr,"%d\n",v32);

		fclose(fptr);
	}
}

BOOL CServerConfigDlg::Load(void)
{
	FILE *fptr = NULL;

	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, "serverConfig.txt");

	fptr = fopen(fname_full,"r");
	if(fptr)
	{
		char str[81];
		int v32;
		fscanf(fptr,"%d",&m_tableType);
		fscanf(fptr,"%d",&m_invite);
		fscanf(fptr,"%d",&m_boot);
		fscanf(fptr,"%d",&m_chat);
		fscanf(fptr,"%d",&m_matchTableAccess);
		fscanf(fptr,"%d",&m_10Reds);
		
		fscanf(fptr,"%d",&m_postRankings);
		fscanf(fptr,"%s",str);
		m_filenameRankings = CString(str);
		fscanf(fptr,"%d",&m_users);
		fscanf(fptr,"%d",&m_guests);
				
		fscanf(fptr,"%s",str);
		m_accountSettings = CString(str);
		fscanf(fptr,"%d",&m_accountSettingsTick);

		fscanf(fptr,"%d",&m_day);
		fscanf(fptr,"%d",&m_month);
		fscanf(fptr,"%d",&m_year);
		v32=-1;
		fscanf(fptr,"%d",&v32);
		if(v32!=-1)	m_freeaccounts = v32; // incase it aint in the file

		v32=-1;
		fscanf(fptr,"%d",&v32);
		if(v32!=-1)	m_logTableChat = v32; // incase it aint in the file
		
		v32=-1;
		fscanf(fptr,"%d",&v32);
		if(v32!=-1)	m_logSocketData = v32; // incase it aint in the file	

		v32=-1;
		fscanf(fptr,"%d",&v32);
		if(v32!=-1)	m_logGameResult = v32; // incase it aint in the file	

		v32=-1;
		fscanf(fptr,"%d",&v32);
		if(v32!=-1)	m_daysFreeTrial = v32; // incase it aint in the file	

		v32=-1;
		fscanf(fptr,"%d",&v32);
		if(v32!=-1)	m_mostRunOutsPrize = v32; // incase it aint in the file	

		fclose(fptr);
		UpdateConfig();
		return TRUE;
	}
	
	return FALSE;
}

void CServerConfigDlg::UpdateConfig(void)
{
	g_config.m_10Reds = m_10Reds;
	g_config.m_accountSettings = m_accountSettings;
	g_config.m_accountSettingsTick = m_accountSettingsTick;
	g_config.m_boot = m_boot;
	g_config.m_chat = m_chat;
	g_config.m_filenameRankings = m_filenameRankings;
	g_config.m_guests = m_guests;
	g_config.m_invite = m_invite;
	g_config.m_matchTableAccess = m_matchTableAccess;
	g_config.m_tableType = m_tableType;
	g_config.m_users = m_users;
	g_config.m_postRankings = m_postRankings;
	g_config.m_crcvalid = m_crcvalid;
	// only sets rankings frequency if it changes

	g_config.m_day = m_day;
	g_config.m_month = m_month;
	g_config.m_year = m_year;
	g_config.m_freeaccounts = m_freeaccounts;

	g_config.m_logTableChat = m_logTableChat;

	g_config.m_mostRunOutsPrize = m_mostRunOutsPrize;

	g_socketlog = m_logSocketData;

	g_gamelog = m_logGameResult;
	g_daysFreeTrial = m_daysFreeTrial;
	//-#ifdef MAIN_SERVER
	
	if(m_pParent!=NULL)	{
					//	((CGameAppView *)m_pParent)->SetRankingsFreq(g_config.m_postRankings);
						//if(g_config.m_postRankings)	((CGameAppView *)m_pParent)->m_ranking->m_bEnable = TRUE;
						//else ((CGameAppView *)m_pParent)->m_ranking->m_bEnable = FALSE;						
						}

	//-#endif //MAIN_SERVER
	
}

void CServerConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerConfigDlg)
	DDX_Check(pDX, IDC_SCFG_10REDS, m_10Reds);
	DDX_Text(pDX, IDC_SCFG_ACCOUNTSETTINGS, m_accountSettings);
	DDV_MaxChars(pDX, m_accountSettings, 80);
	DDX_Check(pDX, IDC_SCFG_ACCOUNTSETTINGSTICK, m_accountSettingsTick);
	DDX_Check(pDX, IDC_SCFG_BOOT, m_boot);
	DDX_Check(pDX, IDC_SCFG_CHAT, m_chat);
	DDX_Text(pDX, IDC_SCFG_FILENAMERANKINGS, m_filenameRankings);
	DDV_MaxChars(pDX, m_filenameRankings, 80);
	DDX_Text(pDX, IDC_SCFG_GUESTS, m_guests);
	DDX_Check(pDX, IDC_SCFG_INVITE, m_invite);
	DDX_Check(pDX, IDC_SCFG_MATCHTABLEACCESS, m_matchTableAccess);
	DDX_Check(pDX, IDC_SCFG_TABLETYPE, m_tableType);
	DDX_Text(pDX, IDC_SCFG_USERS, m_users);
	DDX_Text(pDX, IDC_SCFG_POSTRANKINGS, m_postRankings);
	DDX_Text(pDX, IDC_SCFG_CRCVALID, m_crcvalid);
	DDV_MaxChars(pDX, m_crcvalid, 80);
	DDX_Text(pDX, IDC_SCFG_DAY, m_day);
	DDV_MinMaxInt(pDX, m_day, 1, 31);
	DDX_Text(pDX, IDC_SCFG_MONTH, m_month);
	DDV_MinMaxInt(pDX, m_month, 1, 12);
	DDX_Text(pDX, IDC_SCFG_YEAR, m_year);
	DDV_MinMaxInt(pDX, m_year, 0, 99);
	DDX_Check(pDX, IDC_SCFG_FREEACCOUNTS, m_freeaccounts);
	DDX_Check(pDX, IDC_SCFG_LOGTABLECHAT, m_logTableChat);
	DDX_Check(pDX, IDC_SCFG_LOGSOCKETDATA, m_logSocketData);
	DDX_Check(pDX, IDC_SCFG_LOGGAMERESULT, m_logGameResult);
	DDX_Text(pDX, IDC_SCFG_DAYSFREETRIAL, m_daysFreeTrial);
	DDV_MinMaxInt(pDX, m_daysFreeTrial, 0, 365);
	DDX_Check(pDX, IDC_SCFG_MOSTRUNOUTSPRIZE, m_mostRunOutsPrize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CServerConfigDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerConfigDlg message handlers

int CServerConfigDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();	

	return 0;
}

void CServerConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	Save();
	
	CDialog::OnOK();
}

void CServerConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
