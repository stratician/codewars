// TabCtrlEx.h : header file
//

#ifndef __TABCTRLEX_H__
#define __TABCTRLEX_H__

#include "../Code/Common.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx window

class CTabCtrlEx : public CTabCtrl
{
// Construction
public:
	CTabCtrlEx();

// Attributes
public:
	CWnd *m_pParent;
	int m_totalColumns;
	int m_totalItems;
	int m_maxItems;
	int m_columnHeight;
	CPoint m_cornerPtTabs;
	int m_SelectedTabN;
	BOOL m_created;

	COLORREF m_crSelected;
	COLORREF m_crDisabled;
	COLORREF m_crNormal;
	COLORREF m_crMouseOver;
	COLORREF m_crTab;
	COLORREF m_crTabSelected;
	
	BOOL m_bMouseOver;
	BOOL m_bColorMouseOver;
	BOOL m_bColorNormal;
	BOOL m_bColorDisabled;
	BOOL m_bColorSelected;
	BOOL m_bColorTab;
	BOOL m_bColorTabSelected;
	
	int m_iIndexMouseOver;

	BOOL m_bSetPaddingSize;
	CSize m_padSize;
	CRect m_tabTotalRect;
	BOOL m_bSelChanged;
	
// Operations
public:
	void PassParent(CWnd *pParent);
	void SetPaddingSize(CSize padSize = CSize(5, 5));
	CRect GetTabTotalRect(void);

	void SetCornerPointTabs(CPoint pt);
	void SetDisabledColor(COLORREF cr);
	void SetSelectedColor(COLORREF cr);
	void SetNormalColor(COLORREF cr);
	void SetMouseOverColor(COLORREF cr);
	void SetTabColor(COLORREF cr);
	void SetTabSelectedColor(COLORREF cr);
	void CreateItem(int n, LPTSTR lpszCaption, CWnd* pWnd);
	
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabCtrlEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __TABCTRLEX_H__
