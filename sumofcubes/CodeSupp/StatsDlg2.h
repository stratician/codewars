// StatsDlg2.h : header file
//

#ifndef __STATSDLG2_H__
#define __STATSDLG2_H__

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg2 dialog

class CStatsDlg2 : public CDialog
{
// Construction
public:
	CStatsDlg2(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void GetMSGFocusBack(void);

	CWnd *m_pParent;
	BOOL m_bToggle;
	BOOL m_bDoneToggle;
	BOOL m_bDontCenterWindow;
	BOOL m_bActive;

// Dialog Data
	//{{AFX_DATA(CStatsDlg2)
	enum { IDD = IDD_STATS2 };
	int		m_games;
	int		m_losses;
	CString	m_name;
	int		m_rating;
	int		m_streak;
	int		m_wins;
	CString	m_msg;
	BOOL	m_bIgnore;
	int		m_highestrating;
	CString	m_cash;
	int		m_reracks;
	int		m_runOut8Std;
	int		m_runOut8Pro;
	int		m_runOut8UK;
	int		m_runOut9Std;
	int		m_runOut9Pro;
	int		m_runOut9UK;
	int		m_runOutEngStd;
	int		m_runOutEngPro;
	int		m_runOutEngUK;
	float	m_runOut8Rate;
	float	m_runOut9Rate;
	float	m_runOutEngRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatsDlg2)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnIgnore();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnToggle();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdithighestrating();
	afx_msg void OnEnChangeEditladderpoints();
	afx_msg void OnEnChangeEditladderpoints2();
	CString m_ladderDivision;
	CString m_ladderPoints;
	int m_ladderGames;
	int m_ladderWins;
	int m_ladderLosses;
	int m_ladderHighBreak;
	int m_ladderHighBreakPro;
	CString m_ladderRank;
	CString m_realCash;
};

#endif //__STATSDLG2_H__
