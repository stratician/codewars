// TableSetupDlg.h : header file
//

// for iPool

#ifndef __TABLESETUPDLG_H__
#define __TABLESETUPDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CTableSetupDlg dialog

class CTableSetupDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CTableSetupDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void UpdateShotTime(void);
	void EnableTableCtrls(BOOL bState);
	int m_gameType;
	int m_raceTo;
	int m_whoBreaks;
	int m_tableColour;
	int m_tableType;
	int m_rackType;
	int m_ballSize;
	BYTE m_oldShotTime;
	BOOL m_bOldAddTimer;
	BOOL m_oldProTable;
	BOOL m_oldDoubles;

// Dialog Data
	//{{AFX_DATA(CTableSetupDlg)
	enum { IDD = IDD_TABLESETUPDLG };
	CComboBox	m_comboTableType;
	CComboBox	m_comboRackType;
	CComboBox	m_comboBallSize;
	CComboBox	m_comboWhoBreaks;
	CComboBox	m_comboRaceTo;
	CComboBox	m_comboTableColour;
	CComboBox	m_comboGameType;
	BOOL	m_bAddTimer;
	BYTE	m_shotTime;
	BOOL	m_doubles;
	BOOL	m_proTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTableSetupDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnAddtimer();
	afx_msg void OnUpdateShottime();
	afx_msg void OnSelchangeGametype();
	afx_msg void OnSelchangeRaceto();
	afx_msg void OnSelchangeTablecolour();
	afx_msg void OnSelchangeWhobreaks();
	afx_msg void OnSelchangeBallsize();
	afx_msg void OnSelchangeTabletype();
	afx_msg void OnSelchangeRacktype();
	afx_msg void OnProtable();
	afx_msg void OnDoubles();
	//}}AFX_MSG
	afx_msg void OnButtonShotTime();
	DECLARE_MESSAGE_MAP()
};

#endif //__TABLESETUPDLG_H__
