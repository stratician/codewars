// ButtonEx.h : header file
//

#ifndef __BUTTONEX_H__
#define __BUTTONEX_H__

#include "CoreEx.h"

/////////////////////////////////////////////////////////////////////////////

// font data
typedef struct fntData_typ {
// required	CFont::CreateFont params
CFont *font;
// required CDC::SetBkMode param
int nBkMode;
// required CDC::SetTextColor param
COLORREF crColor;
// required CDC::DrawText params
int nCount;
LPRECT lpRect;
UINT nFormat;
} fntData, *fntData_ptr;


// CButtonEx window

class CButtonEx : /*public CCoreEx,*/ public CButton {
// Construction
public:
	CButtonEx();

// Attributes
public:
	CCoreEx *m_pParent; // pointer to Parent - for Retrieving Event Messages.
	CWnd *m_parent;
	int m_id;
	char m_name[22];
	

// Operations
public:
void SetId(int id);
void PassParent(CCoreEx *pParent);
void GetParent(CWnd *pParent);
void Clicked();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonEx)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnClicked();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __BUTTONEX_H__
