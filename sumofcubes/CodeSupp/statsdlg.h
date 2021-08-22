// statsdlg.h : header file
//

#ifndef __STATSDLG_H__
#define __STATSDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CStatsDlg dialog

class CStatsDlg : public CDialog
{
// Construction
public:
	CStatsDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void GetMSGFocusBack(void);

	CWnd *m_pParent;
	BOOL m_bToggle;
	BOOL m_bDoneToggle;
	BOOL m_bDontCenterWindow;
	BOOL m_bActive;

// Dialog Data
	//{{AFX_DATA(CStatsDlg)
	enum { IDD = IDD_STATS };
	int		m_100s;
	int		m_50s;
	int		m_60s;
	int		m_70s;
	int		m_80s;
	int		m_90s;
	int		m_p100s;
	int		m_p50s;
	int		m_p60s;
	int		m_p70s;
	int		m_p80s;
	int		m_p90s;
	int		m_highBreak;
	int		m_games;
	int		m_losses;
	int		m_max147s;
	int		m_pHighBreak;
	int		m_pMax147s;
	int		m_rating;
	int		m_streak;
	int		m_wins;
	CString	m_name;
	int		m_pWildHighBreak;
	int		m_wildHighBreak;
	CString	m_msg;
	BOOL	m_bIgnore;
	int		m_highestrating;
	CString	m_cash;
	int		m_reracks;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatsDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnToggle();
	afx_msg void OnIgnore();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_highestlobbyrating;
	int m_highesttourneyrating;
	afx_msg void OnEnChangeEdithighestrating();
	afx_msg void OnEnChangeEditratingL();
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
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedAchievements();
	afx_msg void OnEnChangeEditcash2();
	CString m_realCash;
	int m_highStreak;
};

#endif //__STATSDLG_H__
