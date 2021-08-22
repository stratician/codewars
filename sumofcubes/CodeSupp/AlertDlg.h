// AlertDlg.h : header file
//

#ifndef __ALERTDLG_H__
#define __ALERTDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CAlertDlg dialog

class CAlertDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CStatic m_staticText;
	char m_str[MAX_CHARS];
//	BOOL m_bAddExtraButton;
//	CButton m_extraButton;
//	UINT m_extraButtonID;
//	char m_extraStr[81]; // for button label

	CAlertDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void SetStr(char *str);
//	void SetExtraStr(char *str);

// Dialog Data
	//{{AFX_DATA(CAlertDlg)
	enum { IDD = IDD_ALERTDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__ALERTDLG_H__