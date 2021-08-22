// ForfeitDlg.h : header file
//

#ifndef __FORCEFEITDLG_H__
#define __FORCEFEITDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CForfeitDlg dialog

class CForfeitDlg : public CDialog
{
// Construction
public:
	CForfeitDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);
	
	CWnd *m_pParent;
	int m_forfeitType;

// Dialog Data
	//{{AFX_DATA(CForfeitDlg)
	enum { IDD = IDD_FORFEITDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CForfeitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CForfeitDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAbort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__FORCEFEITDLG_H__