#if !defined(AFX_SMILEYSELECTDLG_H__87ECCBF2_BE1E_41B9_BBB1_64E0AB5121E6__INCLUDED_)
#define AFX_SMILEYSELECTDLG_H__87ECCBF2_BE1E_41B9_BBB1_64E0AB5121E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmileySelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmileySelectDlg dialog

class CSmileySelectDlg : public CDialog
{
// Construction
public:
	CSmileySelectDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void Init(BOOL bFirstTime = FALSE);
		

	bool m_bRefresh;
	bool m_bOpen;


	CWnd *m_pParent;
// Dialog Data
	//{{AFX_DATA(CSmileySelectDlg)
	enum { IDD = IDD_SMILEYSELECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmileySelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmileySelectDlg)
	afx_msg void OnPaint();
	afx_msg void OnButton1();
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMILEYSELECTDLG_H__87ECCBF2_BE1E_41B9_BBB1_64E0AB5121E6__INCLUDED_)
