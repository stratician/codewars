// GameApp.h : main header file for the GAMEAPP application
//

#ifndef __GAMEAPP_H__
#define __GAMEAPP_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "../Graphics/resource.h"       // main symbols

#define WIN32_LEAN_AND_MEAN	

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>




#include "ShellEx.h"

//#ifndef	WM_MOUSEWHEEL
//#define WM_MOUSEWHEEL			522 //(dec 522, hex 20A)  ---- 32-bit wParam (highword => wheel distance)
//#endif

/////////////////////////////////////////////////////////////////////////////
// CGameAppApp:
// See GameApp.cpp for the implementation of this class
//

class CGameAppApp : public CWinApp
{
public:
	CGameAppApp();
	~CGameAppApp();

	int m_JustIdle;
	BOOL m_bDoneRun;

	BOOL TranslateAMessage(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameAppApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL InitApplication();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual int Run();
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	//}}AFX_VIRTUAL
	virtual BOOL CGameAppApp::IsIdleMessage( MSG* pMsg );
	BOOL CGameAppApp::GetOSDisplayString( LPTSTR pszOS);

// Implementation

	//{{AFX_MSG(CGameAppApp)
	afx_msg void OnPracticeLoad();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

extern HWND g_hwnd;
extern HINSTANCE g_hinst;
extern int g_active;
extern CShellEx *mainShell; // MAIN SHELL - mainShell
//-#ifndef MAIN_SERVER
extern CShellEx *gameShell; // GAME SHELL - gameShell
//-#endif //!MAIN_SERVER

#endif // __GAMEAPP_H__
