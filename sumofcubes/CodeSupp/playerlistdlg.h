// PlayerListDlg.h : header file
//

#ifndef __PLAYERLISTDLG_H__
#define __PLAYERLISTDLG_H__


/////////////////////////////////////////////////////////////////////////////
// CPlayerListDlg dialog

class CPlayerListDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CListCtrlEx m_listPlayers;
	CScrollBar m_scrollBar;

	CPlayerListDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void SortPlayerRatings(void);
	void SortPlayerTables(void);
	void SortPlayerNames(void);
	void RefreshPlayerItem(void);

// Dialog Data
	//{{AFX_DATA(CPlayerListDlg)
	enum { IDD = IDD_PLAYERLIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerListDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayerListDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__PLAYERLISTDLG_H__
