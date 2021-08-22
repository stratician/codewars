// LobbySelect.h : header file
//

#ifndef __LOBBYSELECT_H__
#define __LOBBYSELECT_H__


#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CLobbySelect dialog

class CLobbySelect : public CDialog
{
// Construction
public:
	CListCtrlEx m_listLobbies;
	CScrollBar m_scrollBar;
	CWnd *m_pParent;
	BOOL m_bEraseBkgnd;

	CLobbySelect(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void Delta(int sel);
	void Select(void);
	void SetEraseBkgnd(BOOL bErase = TRUE);
// Dialog Data
	//{{AFX_DATA(CLobbySelect)
	enum { IDD = IDD_LOBBYSELECT };
	CButton	m_enterlobby;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLobbySelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLobbySelect)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelect();
	afx_msg void OnHelp();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __LOBBYSELECT_H__
