// BootDlg.h : header file
//


#ifndef __BOOTDLG_H__
#define __BOOTDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CBootDlg dialog

class CBootDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CListCtrlEx m_listPlayers;

	CBootDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void RefreshPlayerItem(void);
	
// Dialog Data
	//{{AFX_DATA(CBootDlg)
	enum { IDD = IDD_BOOT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBootDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBootDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__BOOTDLG_H__
