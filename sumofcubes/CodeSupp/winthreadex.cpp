// winthreadex.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "winthreadex.h"

#include "../CodeSupp/GameAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinThreadEx

IMPLEMENT_DYNCREATE(CWinThreadEx, CWinThread)

CWinThreadEx::CWinThreadEx()
{
	m_bKill = FALSE;
	m_bRunning = FALSE;
	bDoingBackgroundProcessing = TRUE;
}

CWinThreadEx::~CWinThreadEx()
{
}

void CWinThreadEx::PassParent(CWnd *pParent)
{
	m_pParent = pParent;
}

BOOL CWinThreadEx::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CWinThreadEx::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWinThreadEx, CWinThread)
	//{{AFX_MSG_MAP(CWinThreadEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinThreadEx message handlers

BOOL CWinThreadEx::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinThread::OnIdle(lCount);
}


int CWinThreadEx::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
  	
	return CWinThread::Run();
}
