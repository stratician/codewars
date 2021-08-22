#if !defined(AFX_SHOP_H__B7FD113C_786A_4A02_948C_F4D9BB002321__INCLUDED_)
#define AFX_SHOP_H__B7FD113C_786A_4A02_948C_F4D9BB002321__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// shop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShop dialog

class CShop : public CDialog
{
// Construction
public:
	CShop(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShop)
	enum { IDD = IDD_SHOP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShop)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShop)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOP_H__B7FD113C_786A_4A02_948C_F4D9BB002321__INCLUDED_)
