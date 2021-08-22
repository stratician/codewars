// concededlg.h : header file
//

#ifndef __CONCEDEDLG_H__
#define __CONCEDEDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CConcedeDlg dialog

class CConcedeDlg : public CDialog
{
// Construction
public:
	CConcedeDlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);

	CWnd *m_pParent;

// Dialog Data
	//{{AFX_DATA(CConcedeDlg)
	enum { IDD = IDD_CONCEDEDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcedeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConcedeDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__CONCEDEDLG_H__
