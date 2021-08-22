#ifndef __DETAILSSETTINGSDLG_H__
#define __DETAILSSETTINGSDLG_H__


// DetailsSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetailsSettingsDlg dialog




class CDetailsSettingsDlg : public CDialog
{
// Construction
public:
	CDetailsSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailsSettingsDlg)
	enum { IDD = IDD_DETAILSSETTINGS };
	int		m_myPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailsSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailsSettingsDlg)
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // __DETAILSSETTINGSDLG_H__