#if !defined(AFX_SHOPDLG_H__B57FC520_40C2_45D7_95B1_14F298CA0CD2__INCLUDED_)
#define AFX_SHOPDLG_H__B57FC520_40C2_45D7_95B1_14F298CA0CD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// shopdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShopDlg dialog

class CShopDlg : public CDialog
{
// Construction
public:
	CShopDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void Draw3D(void);
	void Render3DCue(void);
	void Render3DPen(void);
	void Render3DPurchased(void);
	
	CWnd *m_pParent;
	long m_delay;
	int m_shop_cue_idx, m_shop_cue_nidx;
	int m_shop_text_idx, m_shop_text_nidx;
	unsigned char m_changed;
	float m_vx, m_x;
	BOOL m_bPaintFirstTime;
	CDC m_dblDC;
	CBitmap m_dblBitmap;

	IDirect3DSwapChain9* m_pSwapChain;
	IDirect3DSurface9* m_pRenderSurface;

// Dialog Data
	//{{AFX_DATA(CShopDlg)
	enum { IDD = IDD_SHOP };
	CComboBox	m_combo_ctrl;
	CButton	m_prev;
	CButton	m_next;
	CString	m_combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShopDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonN();
	afx_msg void OnButtonP();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEditchangeComboShop();
	afx_msg void OnSelchangeComboShop();
	afx_msg void OnButtonB1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnButtonUse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOPDLG_H__B57FC520_40C2_45D7_95B1_14F298CA0CD2__INCLUDED_)
