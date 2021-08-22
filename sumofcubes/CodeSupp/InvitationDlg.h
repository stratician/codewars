// InvitationDlg.h : header file
//

#ifndef __INVITATIONDLG_H__
#define __INVITATIONDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CInvitationDlg dialog

class CInvitationDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CStatic m_staticText;
	int m_hostSockIdx;
	int m_initialHostTbl;
	int m_table;
	int m_enterType;
	char m_hostName[80];
	int m_reracks;
	int m_totalGames;

	CInvitationDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void GetInviterHostSockIdx(int hostSockIdx, char *hostName, int initialHostTbl = -1, int table = -1, int enterType = 0, int reracks = 0, int totalGames = 0);

// Dialog Data
	//{{AFX_DATA(CInvitationDlg)
	enum { IDD = IDD_INVITATIONDLG };
	CString m_inviteReplyEdit;
	BOOL m_bIgnorePlayer;
	BOOL m_bDeclineAll;	
	BOOL	m_bDeclineAllGuests;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvitationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInvitationDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnInviteDeclineall();
	afx_msg void OnInviteDeclineallguests();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__INVITATIONDLG_H__
