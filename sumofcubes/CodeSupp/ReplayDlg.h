// ReplayDlg.h : header file
//

#ifndef __REPLAYDLG_H__
#define __REPLAYDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CReplayDlg dialog

class CReplayDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CReplayDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	int GetSliderPos(void);
	BOOL m_bToggle;
	CSize m_toggleMax;
	CSize m_toggleMin;
	void UpdateDisplay(CString str);
	void ReplayLoad(void);
	void ReplaySave(void);
	void PlayStep(void);
	void UpdatePlayState(void);
	bool LoadReplay(char *fname);
		
// Dialog Data
	//{{AFX_DATA(CReplayDlg)
	enum { IDD = IDD_REPLAY };
	CSliderCtrl	m_slider;
	CString	m_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg virtual void OnCancel();

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CReplayDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnReplayLl();
	afx_msg void OnReplayR();
	afx_msg void OnReplayLoad();
	afx_msg void OnReplaySave();
	afx_msg void OnClickSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturnSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnReplayL();
	afx_msg void OnReplayRr();
	afx_msg void OnReplayP();
	//}}AFX_MSG
	afx_msg void CancelFn();
	DECLARE_MESSAGE_MAP()
};

#endif //__REPLAYDLG_H__
