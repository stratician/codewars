// NotePadDlg.h : header file
//

#ifndef __NOTEPADDLG_H__
#define __NOTEPADDLG_H__

/////////////////////////////////////////////////////////////////////////////
// CNotePadDlg dialog

class CNotePadDlg : public CDialog
{
// Construction
public:
	CNotePadDlg(CWnd* pParent = NULL);   // standard constructor

	CWnd *m_pParent;
	CRichEditCtrl m_richEditNotePad;
	char m_title[80];
	char m_filename[80];
	
	void GetParent(CWnd *pParent);
	void TitleAndFilename(char *title, char *filename);

// Dialog Data
	//{{AFX_DATA(CNotePadDlg)
	enum { IDD = IDD_NOTEPAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotePadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNotePadDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __NOTEPADDLG_H__