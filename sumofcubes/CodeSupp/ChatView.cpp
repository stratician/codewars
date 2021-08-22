// ChatView.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "ChatView.h"
#include "GameAppDoc.h"
#include "GameAppView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatView

IMPLEMENT_DYNCREATE(CChatView, CEditView)

CChatView::CChatView()// : m_strText(_T("The character entered was:"))
{
}

CChatView::~CChatView()
{
}


BEGIN_MESSAGE_MAP(CChatView, CEditView)
	//{{AFX_MSG_MAP(CChatView)
	ON_WM_CHAR()
	ON_WM_CREATE()
	// ON_WM_MOUSEWHEEL()
	//ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatView drawing

void CChatView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CChatView diagnostics

#ifdef _DEBUG
void CChatView::AssertValid() const
{
	CEditView::AssertValid();
}

void CChatView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CGameAppDoc* CChatView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CGameAppDoc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatView message handlers

BOOL CChatView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEditView::PreCreateWindow(cs);
}

void CChatView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	//OnDraw();
	
	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}

int CChatView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//CClientDC clientDC(this);
	
	return 0;
}

void CChatView::Message(LPCTSTR lpszMessage)
{
	CString strTemp = lpszMessage;
	strTemp += _T("\r\n");
	int len = GetWindowTextLength();
	GetEditCtrl().SetSel(len,len);
	GetEditCtrl().ReplaceSel(strTemp);
}




BOOL CChatView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// return true;

	return CView::OnMouseWheel(nFlags, zDelta, pt);

}