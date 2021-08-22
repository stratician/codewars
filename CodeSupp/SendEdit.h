// SendEdit.h : header file
//

#ifndef __SENDEDIT_H__
#define __SENDEDIT_H__

#define MAX_SENDEDITCHARS		200

/////////////////////////////////////////////////////////////////////////////
// CSendEdit window

class CSendEdit : public CRichEditCtrl//CEdit
{
// Construction
public:
	CSendEdit();
	void CSendEdit::TextAdjust(void);

// Attributes
public:
	CString m_string;
	char m_message[MAX_SENDEDITCHARS];
	CString m_message_bk;
	BOOL m_buttonstate;

// Operations
public:
	virtual BOOL AutoURLDetect(BOOL in_fEnable);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendEdit)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSendEdit();
	void UpdateTextColor(void);
	// Generated message map functions
protected:
	//{{AFX_MSG(CSendEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFontChange();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

inline BOOL CSendEdit::AutoURLDetect( BOOL enable )
{
	ASSERT(::IsWindow(m_hWnd) );
	return ( BOOL )::SendMessage(m_hWnd, EM_AUTOURLDETECT, enable, 0);
}

#endif // __SENDEDIT_H__
