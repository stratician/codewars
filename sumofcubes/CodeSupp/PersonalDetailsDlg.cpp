// PersonalDetailsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "PersonalDetailsDlg.h"
#include "GameAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersonalDetailsDlg dialog


CPersonalDetailsDlg::CPersonalDetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPersonalDetailsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPersonalDetailsDlg)
	m_comments = _T("");
	m_town = _T("");
	m_age = _T("");
	m_name = _T("");
	m_sex = _T("");
	m_posPlayAv = 0.0f;
	m_pPosPlayAv = 0.0f;
	m_pSafetyPlayAv = 0.0f;
	m_pShotTimeAv = 0.0f;
	m_safetyPlayAv = 0.0f;
	m_shotTimeAv = 0.0f;
	m_breaksAv = 0.0f;
	m_pBreaksAv = 0.0f;
	m_inventory = _T("");
	m_cuetype = _T("");
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CPersonalDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPersonalDetailsDlg)
	DDX_Text(pDX, IDC_EDITComments, m_comments);
	DDX_Text(pDX, IDC_EDITTown, m_town);
	DDX_Text(pDX, IDC_EDITAge, m_age);
	DDX_Text(pDX, IDC_EDITName, m_name);
	DDX_Text(pDX, IDC_EDITSex, m_sex);
	DDX_Text(pDX, IDC_EDITPosPlayAv, m_posPlayAv);
	DDX_Text(pDX, IDC_EDITPPosPlayAv, m_pPosPlayAv);
	DDX_Text(pDX, IDC_EDITPSafetyPlayAv, m_pSafetyPlayAv);
	DDX_Text(pDX, IDC_EDITPShotTimeAv, m_pShotTimeAv);
	DDX_Text(pDX, IDC_EDITSafetyPlayAv, m_safetyPlayAv);
	DDX_Text(pDX, IDC_EDITShotTimeAv, m_shotTimeAv);
	DDX_Text(pDX, IDC_EDITBreaksAv, m_breaksAv);
	DDX_Text(pDX, IDC_EDITPBreaksAv, m_pBreaksAv);
	// DDX_Text(pDX, IDC_EDITInvt, m_inventory);
	DDX_Text(pDX, IDC_EDITCueType, m_cuetype);
	//}}AFX_DATA_MAP
}

void CPersonalDetailsDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

BEGIN_MESSAGE_MAP(CPersonalDetailsDlg, CDialog)
	//{{AFX_MSG_MAP(CPersonalDetailsDlg)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_ENABLE()
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPersonalDetailsDlg message handlers

int CPersonalDetailsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	return 0;
}



void CPersonalDetailsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
}

void CPersonalDetailsDlg::OnEnable(BOOL bEnable) 
{
	CDialog::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
	
}

void CPersonalDetailsDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
			// Show the Text Colors //

	Invalidate(); // causes an ondraw message
}

void CPersonalDetailsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}
