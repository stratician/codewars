#if !defined(AFX_HTMLVIEWEX_H__2EA29392_3BB7_4AF4_A6E2_0AC2E15798F5__INCLUDED_)
#define AFX_HTMLVIEWEX_H__2EA29392_3BB7_4AF4_A6E2_0AC2E15798F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlViewEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewEx html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CHtmlViewEx : public CHtmlView
{
public:
//protected:
	CHtmlViewEx();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHtmlViewEx)

// html Data
public:
	//{{AFX_DATA(CHtmlViewEx)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	BOOL m_bRefreshPage;
	int m_refreshCounter;
	int m_refreshTimeFrames;
	DWORD m_timeStart;
	DWORD m_timeEnd;

// Operations
public:
	void SetRefreshTime(int timeFrames); 
	BOOL IsRefreshTime(BOOL bSaveRefresh = TRUE); 

// Overrides
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlViewEx)
	public:
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnDownloadComplete();
	virtual void OnTitleChange(LPCTSTR lpszText);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	void OnDestroy();
	void OnNcDestroy();
	void PostNcDestroy();
	void OnDestroyClipboard();

	//}}AFX_VIRTUAL

// Implementation

public:
	virtual ~CHtmlViewEx();
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlViewEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLVIEWEX_H__2EA29392_3BB7_4AF4_A6E2_0AC2E15798F5__INCLUDED_)
