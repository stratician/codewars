#if !defined(AFX_PERSONALDETAILSDLG_H__A6D85A9A_CC83_47AF_87E5_33F220043637__INCLUDED_)
#define AFX_PERSONALDETAILSDLG_H__A6D85A9A_CC83_47AF_87E5_33F220043637__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PersonalDetailsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPersonalDetailsDlg dialog

class CPersonalDetailsDlg : public CDialog
{
// Construction
public:
	CPersonalDetailsDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);

	CWnd *m_pParent;

// Dialog Data
	//{{AFX_DATA(CPersonalDetailsDlg)
	enum { IDD = IDD_PERSONALDETAILS };
	CString	m_comments;
	CString	m_town;
	CString	m_age;
	CString	m_name;
	CString	m_sex;
	float	m_posPlayAv;
	float	m_pPosPlayAv;
	float	m_pSafetyPlayAv;
	float	m_pShotTimeAv;
	float	m_safetyPlayAv;
	float	m_shotTimeAv;
	float	m_breaksAv;
	float	m_pBreaksAv;
	CString	m_inventory;
	CString	m_cuetype;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPersonalDetailsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPersonalDetailsDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSONALDETAILSDLG_H__A6D85A9A_CC83_47AF_87E5_33F220043637__INCLUDED_)
