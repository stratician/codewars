// FoulDlg.h : header file
//

#ifndef __FOULDLG_H__
#define __FOULDLG_H__

#include "GameApp.h"

/////////////////////////////////////////////////////////////////////////////
// CFoulDlg dialog

class CFoulDlg : public CDialog
{
// Construction
public:
	CWnd* m_parent;
	CStatic m_staticText;

	CFoulDlg(CWnd* pParent = NULL);   // standard constructor
	
	void GetParent(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CFoulDlg)
	enum { IDD = IDD_FOULDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFoulDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFoulDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnButtonAlertOK();
	afx_msg void OnButtonAlertCancel();
	afx_msg void OnButtonFoulMissOK();
	afx_msg void OnButtonFoulMissCancel();
	afx_msg void OnButtonFoulMissRB();
	afx_msg void OnButtonBadBreak1();
	afx_msg void OnButtonBadBreak2();
	afx_msg void OnButtonBadBreak3();
	DECLARE_MESSAGE_MAP()
};

#endif // __FOULDLG_H__
