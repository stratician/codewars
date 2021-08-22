// rerackdlg.h : header file
//

#ifndef __RERACKDLG_H__
#define __RERACKDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CReRackDlg dialog

class CReRackDlg : public CDialog
{
// Construction
public:
	CReRackDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);

	CWnd *m_pParent;

// Dialog Data
	//{{AFX_DATA(CReRackDlg)
	enum { IDD = IDD_RERACKDLG };
	BOOL	m_bAbortRaceTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReRackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReRackDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__RERACKDLG_H__