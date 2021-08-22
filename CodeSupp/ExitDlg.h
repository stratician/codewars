// ExitDlg.h : header file
//

#ifndef __EXITDLG_H__
#define __EXITDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CExitDlg dialog

class CExitDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;

	CExitDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);

// Dialog Data
	//{{AFX_DATA(CExitDlg)
	enum { IDD = IDD_EXITDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExitDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__EXITDLG_H__