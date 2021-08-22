// GuestRegisterDlg.h : header file
//

#ifndef __GUESTREGISTERDLG_H__
#define __GUESTREGISTERDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CGuestRegisterDlg dialog

class CGuestRegisterDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CGuestRegisterDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);

// Dialog Data
	//{{AFX_DATA(CGuestRegisterDlg)
	enum { IDD = IDD_GUESTREGISTERDLG };
	CString	m_registerPrice;
	CString	m_registerURL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuestRegisterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuestRegisterDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedRegisterurl();
};

#endif //__GUESTREGISTERDLG_H__
