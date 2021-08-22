// winthreadex.h : header file
//
#ifndef __WINTHREADEX_H__
#define __WINTHREADEX_H__


#include "../CodeSupp/GameApp.h"

/////////////////////////////////////////////////////////////////////////////
// CWinThreadEx thread

class CWinThreadEx : public CWinThread
{
	DECLARE_DYNCREATE(CWinThreadEx)
//protected:
public:
	CWinThreadEx();           // protected constructor used by dynamic creation

// Attributes
public:
	CWnd *m_pParent;
	// Indicates if we want to kill the thread.
	BOOL	m_bKill;
	BOOL	m_bRunning;
	BOOL bDoingBackgroundProcessing;

// Operations
public:
	void PassParent(CWnd *pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinThreadEx)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
//protected:
	public:
	virtual ~CWinThreadEx();

	// Generated message map functions
	//{{AFX_MSG(CWinThreadEx)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // __WINTHREADEX_H__
