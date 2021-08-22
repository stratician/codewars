// NotePadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "NotePadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotePadDlg dialog


CNotePadDlg::CNotePadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNotePadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotePadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = NULL;
	m_title[0] = 0;
	m_filename[0] = 0;
}


void CNotePadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotePadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotePadDlg, CDialog)
	//{{AFX_MSG_MAP(CNotePadDlg)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotePadDlg message handlers

void CNotePadDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

int CNotePadDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
		
	return 0;
}

void CNotePadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	// display title for notepad
	SetWindowText(m_title);
		
	// open file and copy text into notepad edit ctrl window
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_NOTEPADEDIT);
	CString notes;
	int lineCount;
	char letter;

	if(pEdit)
	{
		FILE *fptr = NULL;
		int total = 0;
		int count = 0;
		char letter;

		fptr = fopen(m_filename,"rb");
		if(fptr)
		{
		   while( !feof( fptr ) )
		   {
			  count = fread(&letter, 1, 1, fptr);
			  notes += letter;
		   }
		   fclose(fptr);
		}

		// Text to add to edit ctrl window
		pEdit->SetWindowText(notes.GetBuffer(0));
		//Message(""); // trick!! to make text correct colour
		//m_editExChat.SetWindowText(comments.GetBuffer(0));
		pEdit->SetReadOnly();
	}
}

void CNotePadDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CNotePadDlg::TitleAndFilename(char *title, char *filename)
{
	strcpy(m_title, title);
	strcpy(m_filename, filename);
}
