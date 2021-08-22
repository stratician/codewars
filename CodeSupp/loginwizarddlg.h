// LoginWizardDlg.h : header file
//

#ifndef __LOGINWIZARDDLG_H__
#define __LOGINWIZARDDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CLoginWizardDlg dialog

class CLoginWizardDlg : public CDialog
{
// Construction
public:
	CLoginWizardDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void CopyPlayerLoginDetails();

	CWnd *m_pParent;
	BOOL m_bActive;

// Dialog Data
	//{{AFX_DATA(CLoginWizardDlg)
	enum { IDD = IDD_LOGINWIZARDDLG };
	CString	m_name;
	CString	m_password;
	BOOL	m_bShow;
	BOOL	m_bPass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginWizardDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCheck1();
	afx_msg void OnPassCheck1();
	afx_msg void OnPurchasecode();
	afx_msg void OnCreatenewaccount();
	afx_msg void OnTopupaccount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__LOGINWIZARDDLG_H__
