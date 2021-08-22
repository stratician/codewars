// ReRackAccept2Dlg.h : header file
//

#ifndef __RERACKACCEPT2DLG_H__
#define __RERACKACCEPT2DLG_H__

/////////////////////////////////////////////////////////////////////////////
// CReRackAccept2Dlg dialog

class CReRackAccept2Dlg : public CDialog
{
// Construction
public:
	CReRackAccept2Dlg(CWnd* pParent = NULL);   // standard constructor
	void GetParent(CWnd *pParent);

	CWnd *m_pParent;
	BOOL m_bAbortRaceTo;
// Dialog Data
	//{{AFX_DATA(CReRackAccept2Dlg)
	enum { IDD = IDD_RERACKACCEPT2DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReRackAccept2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReRackAccept2Dlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__RERACKACCEPT2DLG_H__
