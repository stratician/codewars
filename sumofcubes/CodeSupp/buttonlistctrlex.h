// ButtonListCtrlEx.h : header file
//

#ifndef __BUTTONLISTCTRLEX_H__
#define __BUTTONLISTCTRLEX_H__

#include "ButtonOwnerEx.h"

/////////////////////////////////////////////////////////////////////////////
// CButtonListCtrlEx window

class CButtonListCtrlEx : public CButtonOwnerEx//CButton
{
// Construction
public:
	CButtonListCtrlEx();

// Attributes
public:
	CWnd *m_pParent;
	int m_listCtrlIdx;
	BOOL m_seated;
	BOOL m_enabled;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonListCtrlEx)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __BUTTONLISTCTRLEX_H__
