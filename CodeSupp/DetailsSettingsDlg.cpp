// DetailsSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "DetailsSettingsDlg.h"
#include "GameAppView.h" // ????

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDetailsSettingsDlg dialog


CDetailsSettingsDlg::CDetailsSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailsSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsSettingsDlg)
	m_myPort = 2000;
	//}}AFX_DATA_INIT

	// load settings from file
	FILE *fp;

	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, "MySettings.txt");

	if((fp = fopen(fname_full, "r")) == NULL)
	{
		return;
	}
	
	// load my details:
	fscanf(fp, "%d\n", &m_myPort);

	int n, m;
/*	
	char str[80];
	
	for(m=0; m<1; m++)
	{
		for(n=0; n<80; n++)
		{
			fread(&str[n], 1, 1, fp);
			if(str[n] == '\n') {
				str[n] = NULL;
				break;
			}
		}
		
		switch(m)
		{
			case 0:
				m_myPort = CString(str);
				break;
				*/
			/*
			case 1:
				m_password = CString(str);
				break;
			case 2:
				m_email = CString(str);
				break;
			
			case 3:
				m_town = CString(str);
				break;
			case 4:
				m_comments = CString(str);
				break;
			*/
	//	}
	//}

	fclose(fp);
}


void CDetailsSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsSettingsDlg)
	DDX_Text(pDX, IDC_EDITMyPort, m_myPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CDetailsSettingsDlg)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailsSettingsDlg message handlers

void CDetailsSettingsDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();

	// save details to file
	FILE *fp;

	char fname_full[128];
	sprintf(fname_full, "%s%s", g_build.savepath, "MySettings.txt");

	if((fp = fopen(fname_full, "w")) == NULL)
	{
		return;
	}

	// save my details:
	fprintf(fp, "%d\n", m_myPort);
	
	//CString str;

	//str = buffer + "\n";		fwrite(str, str.GetLength(), 1, fp);
	//str = m_password + "\n";	fwrite(str, str.GetLength(), 1, fp);
	//str = m_email + "\n";		fwrite(str, str.GetLength(), 1, fp);
	
	//str = m_town + "\n";		fwrite(str, str.GetLength(), 1, fp);
	//str = m_comments + "\n";	fwrite(str, str.GetLength(), 1, fp);

	

	fclose(fp);

}

int CDetailsSettingsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CenterWindow();
	return 0;
}
