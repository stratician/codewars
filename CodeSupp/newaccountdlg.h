// NewAccountDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewAccountDlg dialog

class CNewAccountDlg : public CDialog
{
// Construction
public:
	CNewAccountDlg(CWnd* pParent = NULL);   // standard constructor
	void SetTitle(char *str);
// Dialog Data
	//{{AFX_DATA(CNewAccountDlg)
	enum { IDD = IDD_NEWACCOUNT };
	CComboBox	m_comboFindOut;
	CString	m_mCode;
	CString	m_name;
	CString	m_password;
	CString	m_password2;
	CString	m_referer;
	CString	m_email;
	//}}AFX_DATA
	CString m_title;
	BOOL m_bStateMCODE;
	int m_FindOutIdx;
	CString m_FindOutStr;

	void EnableField(int ID, BOOL bState);
	void SetProps(BOOL bStateMCODE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewAccountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewAccountDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeFindout();
	afx_msg void OnEditchangeFindout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
