#if !defined(AFX_TABLETABCTRLDLG_H__22D2FEF2_41E7_4105_B141_B878838E790D__INCLUDED_)
#define AFX_TABLETABCTRLDLG_H__22D2FEF2_41E7_4105_B141_B878838E790D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableTabCtrlDlg.h : header file
//

#include "HtmlViewEx.h"

/////////////////////////////////////////////////////////////////////////////
// CTableTabCtrlDlg dialog

class CTableTabCtrlDlg : public CDialog
{
// Construction
public:
	CTableTabCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	
	CWnd *m_pParent;
	CImageList m_imageListTab;
	CStatic m_static;

// Dialog Data
	//{{AFX_DATA(CTableTabCtrlDlg)
	enum { IDD = IDD_TABLETABCTRL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTableTabCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTableTabCtrlDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	//afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	//afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLETABCTRLDLG_H__22D2FEF2_41E7_4105_B141_B878838E790D__INCLUDED_)
