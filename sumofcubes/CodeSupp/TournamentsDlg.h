#if !defined(AFX_TOURNAMENTSDLG_H__C878E900_F02E_4199_828C_7AC03352F30D__INCLUDED_)
#define AFX_TOURNAMENTSDLG_H__C878E900_F02E_4199_828C_7AC03352F30D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TournamentsDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTournamentsDlg dialog

class CTournamentsDlg : public CDialog
{
// Construction
public:
	CTournamentsDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);

	CWnd *m_pParent;

// Dialog Data
	//{{AFX_DATA(CTournamentsDlg)
	enum { IDD = IDD_TOURNAMENTS };
	CListBox	m_historyCtrl;
	int		m_rating;
	int		m_winner;
	int		m_runnerUp;
	int		m_semiFinals;
	int		m_quarterFinals;
	int		m_last16;
	int		m_last32;
	int		m_BansUnAuth;
	int		m_BansTotal;
	int		m_BansSpamming;
	int		m_BansMTrials;
	int		m_BansCheating;
	int		m_BansAbuse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTournamentsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTournamentsDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOURNAMENTSDLG_H__C878E900_F02E_4199_828C_7AC03352F30D__INCLUDED_)
