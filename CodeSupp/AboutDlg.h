// AboutDlg.h : header file
//

#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__

#include "GameApp.h"

/////////////////////////////////////////////////////////////////////////////
// AboutDlg dialog (used for App About)

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	DECLARE_DYNCREATE(CAboutDlg)

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_aboutVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif // __ABOUTDLG_H__
