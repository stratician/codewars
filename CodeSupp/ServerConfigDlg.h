// ServerConfigDlg.h : header file
//

#ifndef __SERVERCONFIGDLG_H__
#define __SERVERCONFIGDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CServerConfigDlg dialog

class CServerConfigDlg : public CDialog
{
// Construction
public:
	CServerConfigDlg(CWnd* pParent = NULL);   // standard constructor
	CWnd *m_pParent;

	void GetParent(CWnd *pParent);
	void Save(void);
	BOOL Load(void);
	void UpdateConfig(void);

// Dialog Data
	//{{AFX_DATA(CServerConfigDlg)
	enum { IDD = IDD_SERVERCONFIG };
	BOOL	m_10Reds;
	CString	m_accountSettings;
	BOOL	m_accountSettingsTick;
	BOOL	m_boot;
	BOOL	m_chat;
	CString	m_filenameRankings;
	int		m_guests;
	BOOL	m_invite;
	BOOL	m_matchTableAccess;
	BOOL	m_tableType;
	int		m_users;
	int		m_postRankings;
	CString	m_crcvalid;
	int		m_day;
	int		m_month;
	int		m_year;
	BOOL	m_freeaccounts;
	BOOL	m_logTableChat;
	BOOL	m_logSocketData;
	BOOL	m_logGameResult;
	int		m_daysFreeTrial;
	BOOL	m_mostRunOutsPrize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerConfigDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __SERVERCONFIGDLG_H__
