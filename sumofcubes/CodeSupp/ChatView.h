// ChatView.h : header file
//

#ifndef __CHATVIEW_H__
#define __CHATVIEW_H__

/////////////////////////////////////////////////////////////////////////////
// CChatView view

class CGameAppDoc;

class CChatView : public CEditView
{
//protected:
public:
	CChatView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChatView)

// Attributes
public:
	CGameAppDoc* GetDocument();
	CString m_strText;

// Operations
public:
	void Message(LPCTSTR lpszMessage);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
//protected:
public:
	virtual ~CChatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CChatView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in srvrvw.cpp
inline CGameAppDoc* CChatView::GetDocument()
   { return (CGameAppDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

#endif // __CHATVIEW_H__