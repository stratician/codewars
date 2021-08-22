// GameAppDoc.h : interface of the CGameAppDoc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __GAMEAPPDOC_H__
#define __GAMEAPPDOC_H__

class CGameAppDoc : public CDocument
{
protected: // create from serialization only
	CGameAppDoc();
	DECLARE_DYNCREATE(CGameAppDoc)

// Attributes
public:
	
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameAppDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGameAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGameAppDoc)
	afx_msg void OnPracticeLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __GAMEAPPDOC_H__