
#if !defined(AFX_TEXTCOLORCHANGERDLG_H__87ECCBF2_BE1E_41B9_BBB1_64E0AB5121E6__INCLUDED_)
#define AFX_TEXTCOLORCHANGERDLG_H__87ECCBF2_BE1E_41B9_BBB1_64E0AB5121E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextColorChangerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextColorChangerDlg dialog

class CTextColorChangerDlg : public CDialog
{
// Construction
public:
	CTextColorChangerDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void Init(BOOL bFirstTime = FALSE);
	void Process(void);
	void ShowLevelText(void);
	void UpdateColor(COLORREF m_textColor, bool bUpdate = true);
	
	bool m_bRefresh;

	int g_grid_x, g_grid_oldx;
	int g_grid_y, g_grid_oldy;

	BOOL FirstTime;// = TRUE;
	float Text_Fade;

	CBitmap stitle;
	CBitmap sfooter;
	CBitmap bitmap[35];

	HBITMAP smileyselectbg;
	HBITMAP stitleEx;
	HBITMAP bitmapEx[32];

	CDC dcMem_buf;
	CDC dcMem;
	CRect rc;
	BITMAP bmp;

	bool m_bOpen;

	COLORREF m_textColor;
	COLORREF m_textColorOld;

	int access_level;

	
	bool m_bMouseOver;

	CWnd *m_pParent;
// Dialog Data
	//{{AFX_DATA(CTextColorChangerDlg)
	enum { IDD = IDD_TEXTCOLORCHANGER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextColorChangerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextColorChangerDlg)
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
	afx_msg LRESULT OnMouseOver(WPARAM wparam, LPARAM lparam);
    afx_msg LRESULT OnMouseOut(WPARAM wparam, LPARAM lparam);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTCOLORCHANGERDLG_H__87ECCBF2_BE1E_41B9_BBB1_64E0AB5121E6__INCLUDED_)
