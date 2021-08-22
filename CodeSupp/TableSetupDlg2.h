// TableSetupDlg2.h : header file
//

// for iSnooker

#ifndef __TABLESETUPDLG2_H__
#define __TABLESETUPDLG2_H__

/////////////////////////////////////////////////////////////////////////////
// CTableSetupDlg2 dialog

class CTableSetupDlg2 : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CTableSetupDlg2(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void UpdateShotTime(void);
	void EnableTableCtrls(BOOL bState);
	int m_level;
	int m_gameType;
	int m_raceTo;
	int m_whoBreaks;
	int m_tableColour;
	int m_tableType;
	int m_rackType;
	int m_ballSize;
	BYTE m_oldShotTime;
	BOOL m_bOldAddTimer;
	BOOL m_oldDoubles;

// Dialog Data
	//{{AFX_DATA(CTableSetupDlg2)
	enum { IDD = IDD_TABLESETUPDLG2 };
	CComboBox	m_comboTableType;
	CComboBox	m_comboRackType;
	CComboBox	m_comboBallSize;
	CComboBox	m_comboWhoBreaks;
	CComboBox	m_comboTableColour;
	CComboBox	m_comboRaceTo;
	CComboBox	m_comboLevel;
	CComboBox	m_comboGameType;
	BYTE	m_shotTime;
	BOOL	m_bAddTimer;
	BOOL	m_doubles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableSetupDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTableSetupDlg2)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnUpdateShottime();
	afx_msg void OnSelchangeGametype();
	afx_msg void OnSelchangeLevel();
	afx_msg void OnSelchangeRaceto();
	afx_msg void OnSelchangeTablecolour();
	afx_msg void OnSelchangeWhobreaks();
	afx_msg void OnAddtimer2();
	afx_msg void OnSelchangeBallsize2();
	afx_msg void OnSelchangeRacktype2();
	afx_msg void OnSelchangeTabletype2();
	afx_msg void OnDoubles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__TABLESETUPDLG2_H__
