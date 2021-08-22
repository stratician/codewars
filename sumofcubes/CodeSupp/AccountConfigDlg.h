// AccountConfigDlg.h : header file
//

#ifndef __ACCOUNTCONFIGDLG_H__
#define __ACCOUNTCONFIGDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CAccountConfigDlg dialog

class CAccountConfigDlg : public CDialog
{
// Construction
public:
	CWnd *m_pParent;
	CAccountConfigDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	void MoveSerials(char *codetype);

// Dialog Data
	//{{AFX_DATA(CAccountConfigDlg)
	enum { IDD = IDD_ACCOUNTCONFIG };
	CEdit	m_acccfgEdit;
	CString	m_custom;
	int		m_qty;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccountConfigDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void On1year();
	afx_msg void On3months();
	afx_msg void On6months();
	afx_msg void OnCustom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__ACCOUNTCONFIGDLG_H__
