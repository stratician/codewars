// GameSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGameSetup dialog

class CGameSetup : public CDialog
{
// Construction
public:
	CGameSetup(CWnd* pParent = NULL);   // standard constructor
	int m_playerN;						// player #
	BOOL m_creator;						// creator flag
	
// Dialog Data
	//{{AFX_DATA(CGameSetup)
	enum { IDD = IDD_GAMESETUP };
	CString	m_IPAddress;
	CString	m_UserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGameSetup)
	afx_msg void OnCreate();
	afx_msg void OnJoin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
