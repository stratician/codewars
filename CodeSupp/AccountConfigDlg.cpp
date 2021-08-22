// AccountConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "AccountConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountConfigDlg dialog


CAccountConfigDlg::CAccountConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountConfigDlg)
	m_custom = _T("");
	m_qty = 0;
	//}}AFX_DATA_INIT
	m_pParent = NULL;
}


void CAccountConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountConfigDlg)
	DDX_Control(pDX, IDC_ACCCFG_EDIT, m_acccfgEdit);
	DDX_Text(pDX, IDC_CUSTOM_EDIT, m_custom);
	DDX_Text(pDX, IDC_QTY_EDIT, m_qty);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CAccountConfigDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_1YEAR, On1year)
	ON_BN_CLICKED(IDC_3MONTHS, On3months)
	ON_BN_CLICKED(IDC_6MONTHS, On6months)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountConfigDlg message handlers

void CAccountConfigDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CAccountConfigDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

int CAccountConfigDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	
	return 0;
}

void CAccountConfigDlg::MoveSerials(char *codetype)
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	
	char path[256];
	sprintf(path,"%sSerials.txt", g_basedir);
	char path2[256];
	sprintf(path2,"%sSerialAdd.dat", g_basedir);
	
	CString str = g_serialcode->MoveSerialsDAT(path, path2, codetype, m_qty);
	g_serialcode->SaveSerials(path);
	
	//CString str = g_serialcode->MoveSerialsDAT("Serials.txt", "SerialAdd.dat", codetype, m_qty);

	DWORD ss = 0x0000ffff; // LWORD = 0, HWORD = -1
	m_acccfgEdit.SetSel(ss,FALSE);
	m_acccfgEdit.Clear();
	m_acccfgEdit.ReplaceSel(str);

	FILE *fptr;
	fptr = fopen("SerialsSWREG.txt","wb");
	if(fptr)
	{
		fprintf(fptr,"<<<%sEmpty>>>\r\n",codetype);
		fwrite(str.GetBuffer(0), str.GetLength()+1,1,fptr);
		fclose(fptr);
	}
}

void CAccountConfigDlg::On1year() 
{
	MoveSerials("1Year");
}

void CAccountConfigDlg::On3months() 
{
	// TODO: Add your control notification handler code here
	MoveSerials("3Months");	
}

void CAccountConfigDlg::On6months() 
{
	// TODO: Add your control notification handler code here
	MoveSerials("6Months");	
}

void CAccountConfigDlg::OnCustom() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_custom.IsEmpty())
	{
	AfxMessageBox("Custom Field is Empty");
	return;
	}
	MoveSerials(m_custom.GetBuffer(0));
}
