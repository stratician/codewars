// InviteDlg.h : header file
//

#ifndef __INVITEDLG_H__
#define __INVITEDLG_H__

#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CInviteDlg dialog

class CInviteDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CListCtrlEx m_listPlayers;
	CScrollBar m_scrollBar;
	
	CInviteDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void SortPlayerRatings(void);
	void SortPlayerTables(void);
	void SortPlayerNames(void);
	void RefreshPlayerItem(void);
	

// Dialog Data
	//{{AFX_DATA(CInviteDlg)
	enum { IDD = IDD_INVITE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInviteDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInviteDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__INVITEDLG_H__
