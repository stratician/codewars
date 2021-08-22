// PracticeEditDlg.h : header file
//
#ifndef __PRACTICEEDITDLG_H__
#define __PRACTICEEDITDLG_H__

#include "ButtonOwnerEx.h"

/////////////////////////////////////////////////////////////////////////////
// CPracticeEditDlg dialog

class CPracticeEditDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CDialog m_commentDlg;

	CPracticeEditDlg(CWnd* pParent = NULL);   // standard constructor
	
	void GetParent(CWnd *pParent);

// Dialog Data
	//{{AFX_DATA(CPracticeEditDlg)
	enum { IDD = IDD_PRACTICEEDIT };
	CButtonOwnerEx	m_buttonPlay;
	CButtonOwnerEx	m_buttonComment;
	CButtonOwnerEx	m_buttonBlack;
	CButtonOwnerEx	m_buttonBlue;
	CButtonOwnerEx	m_buttonBrown;
	CButtonOwnerEx	m_buttonErase;
	CButtonOwnerEx	m_buttonGreen;
	CButtonOwnerEx	m_buttonPink;
	CButtonOwnerEx	m_buttonYellow;
	CButtonOwnerEx	m_buttonRed;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPracticeEditDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPracticeEditDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonblack();
	afx_msg void OnButtonblue();
	afx_msg void OnButtonbrown();
	afx_msg void OnButtonerase();
	afx_msg void OnButtongreen();
	afx_msg void OnButtonpink();
	afx_msg void OnButtonplayshot();
	afx_msg void OnButtonred();
	afx_msg void OnButtonyellow();
	afx_msg void OnClose();
	afx_msg void OnButtoncomment();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __PRACTICEEDITDLG_H__
