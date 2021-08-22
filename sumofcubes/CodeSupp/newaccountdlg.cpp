// NewAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "NewAccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewAccountDlg dialog


CNewAccountDlg::CNewAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewAccountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAccountDlg)
	m_mCode = _T("");
	m_name = _T("");
	m_password = _T("");
	m_password2 = _T("");
	m_referer = _T("");
	m_email = _T("");
	//}}AFX_DATA_INIT
	m_title = _T("Create new account / Top up account / Revive Account");
	SetProps(TRUE);

	m_FindOutIdx = -1;
	m_FindOutStr = _T("");
}


void CNewAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAccountDlg)
	DDX_Control(pDX, IDC_FINDOUT, m_comboFindOut);
	DDX_Text(pDX, IDC_EDITMCode, m_mCode);
	DDV_MaxChars(pDX, m_mCode, 16);
	DDX_Text(pDX, IDC_EDITName, m_name);
	DDV_MaxChars(pDX, m_name, 19);
	DDX_Text(pDX, IDC_EDITPassword, m_password);
	DDV_MaxChars(pDX, m_password, 18);
	DDX_Text(pDX, IDC_EDITPassword2, m_password2);
	DDV_MaxChars(pDX, m_password2, 18);
	DDX_Text(pDX, IDC_EDITReferer, m_referer);
	DDV_MaxChars(pDX, m_referer, 19);
	DDX_Text(pDX, IDC_EDITEmail, m_email);
	DDV_MaxChars(pDX, m_email, 48);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAccountDlg, CDialog)
	//{{AFX_MSG_MAP(CNewAccountDlg)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_FINDOUT, OnSelchangeFindout)
	ON_CBN_EDITCHANGE(IDC_FINDOUT, OnEditchangeFindout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAccountDlg message handlers

void CNewAccountDlg::EnableField(int ID, BOOL bState)
{
	CEdit *pEdit = (CEdit *)GetDlgItem(ID);
	if(pEdit) pEdit->EnableWindow(bState);
}

void CNewAccountDlg::OnOK() 
{
	// TODO: Add extra validation here
	//m_FindOutIdx = m_comboFindOut.GetCurSel();
	m_comboFindOut.GetWindowText(m_FindOutStr);
		
	CDialog::OnOK();
}

void CNewAccountDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CNewAccountDlg::SetTitle(char *str)
{
	if(str) m_title = str;
}

void CNewAccountDlg::SetProps(BOOL bStateMCODE)
{
	m_bStateMCODE = bStateMCODE;
}

int CNewAccountDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();	
	SetWindowText(m_title);
	return 0;
}

void CNewAccountDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	EnableField(IDC_EDITMCode, m_bStateMCODE);

	// setup 'find out' combo box data
	m_comboFindOut.ResetContent();
	m_comboFindOut.InitStorage(11, 32);
	m_comboFindOut.InsertString(0,"Search Engine");
	m_comboFindOut.InsertString(1,"From a Friend");
	m_comboFindOut.InsertString(2,"Computer Magazine");
	m_comboFindOut.InsertString(3,"Radio Station");
	m_comboFindOut.InsertString(4,"iSnooker / iPool (game)");
	m_comboFindOut.InsertString(5,"Another Online Game");
	m_comboFindOut.InsertString(6,"Free Download Site");
	m_comboFindOut.InsertString(7,"TV");
	m_comboFindOut.InsertString(8,"Internet Forum");
	m_comboFindOut.InsertString(9,"Google Advertisement");
	m_comboFindOut.InsertString(10,"Other (Please specify)");
	if(m_FindOutIdx!=-1)
	{
		m_comboFindOut.SetCurSel(m_FindOutIdx);
		if(m_FindOutIdx == 9)
		{
			m_comboFindOut.SetWindowText(m_FindOutStr);
		}
	}
	else
	{
		CString str = _T("Please Select");
		m_comboFindOut.SetWindowText(str);
	}
}

void CNewAccountDlg::OnSelchangeFindout() 
{
	// TODO: Add your control notification handler code here
	m_FindOutIdx = m_comboFindOut.GetCurSel();
}

void CNewAccountDlg::OnEditchangeFindout() 
{
	// TODO: Add your control notification handler code here
	
}
