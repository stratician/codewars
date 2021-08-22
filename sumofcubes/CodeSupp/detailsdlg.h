// DetailsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg dialog

class CDetailsDlg : public CDialog
{
// Construction
public:
	CDetailsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailsDlg)
	enum { IDD = IDD_DETAILS };
	CString	m_name;
	CString	m_password;
	CString	m_email;
	CString	m_comments;
	CString	m_town;
	CString	m_realName;
	CString	m_sex;
	CString	m_age;
	CString	m_version;
	CString	m_bpass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void Init(void);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailsDlg)
	afx_msg void OnSave();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
