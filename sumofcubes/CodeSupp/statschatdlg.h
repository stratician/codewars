// StatsChatDlg.h : header file
//

#ifndef __STATSCHATDLG_H__
#define __STATSCHATDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CStatsChatDlg dialog

class CStatsChatDlg : public CDialog
{
// Construction
public:
	CStatsChatDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void GetMSGFocusBack(void);

	CWnd *m_pParent;
	CString	m_name;
	BOOL m_bActive;

// Dialog Data
	//{{AFX_DATA(CStatsChatDlg)
	enum { IDD = IDD_STATSCHAT };
	CString	m_msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatsChatDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToggle();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__STATSCHATDLG_H__
