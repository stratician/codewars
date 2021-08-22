// StatsTabCtrlDlg.h : header file
//

#ifndef __STATSTABCTRLDLG_H__
#define __STATSTABCTRLDLG_H__

#include "TabCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CStatsTabCtrlDlg dialog

class CStatsTabCtrlDlg : public CDialog
{
// Construction
public:
	CStatsTabCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	
	CWnd *m_pParent;
	CTabCtrlEx m_tabStats;
	CStatsDlg *m_pStatsDlg;
	BOOL m_bActive;

// Dialog Data
	//{{AFX_DATA(CStatsTabCtrlDlg)
	enum { IDD = IDD_STATSTABCTRL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsTabCtrlDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatsTabCtrlDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __STATSTABCTRLDLG_H__
