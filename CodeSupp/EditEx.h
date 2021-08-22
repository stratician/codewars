// EditEx.h : header file
//

#ifndef __EDITEX_H__
#define __EDITEX_H__

/////////////////////////////////////////////////////////////////////////////
// CEditEx window

class CEditEx : public CRichEditCtrl//CEdit//
{
// Construction
public:
	CEditEx();

// Attributes
public:
	bool bHasFocus;
// Operations
public:
	virtual BOOL AutoURLDetect(BOOL in_fEnable);

	void SetColour(COLORREF color);
	void SetUnderlined(void);
	void SetBold(void);
	void SetItalic(void);
	void SetLink(void);
	void SetProtected(void);
	int GetFontSize(void);
	void SetFontSize(int nPointSize);
	CString GetFontName(void);
	void SetFontName(const CString strFontName);
	void SetHasFocus(bool hasFocus);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

inline BOOL CEditEx::AutoURLDetect( BOOL enable )
{
	ASSERT(::IsWindow(m_hWnd) );
	return ( BOOL )::SendMessage(m_hWnd, EM_AUTOURLDETECT, enable, 0);
}


#endif // __EDITEX_H__
