// GameAppDoc.cpp : implementation of the CGameAppDoc class
//

//#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"

#include "GameAppDoc.h"
#include "ChatView.h"
#include "GameAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameAppDoc //

IMPLEMENT_DYNCREATE(CGameAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameAppDoc, CDocument)
	//{{AFX_MSG_MAP(CGameAppDoc)
	ON_COMMAND(ID_PRACTICE_LOAD, OnPracticeLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameAppDoc construction/destruction

CGameAppDoc::CGameAppDoc()
{
	// TODO: add one-time construction code here
}					   

CGameAppDoc::~CGameAppDoc()
{
	// TODO: add destruction code here
}

BOOL CGameAppDoc::OnNewDocument()
{
	
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	return true;


	CString sTitle;
	CString gameType;
	CString website;

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		gameType = _T("iSnooker");
		website = _T("TheSnookerClub.com");
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		gameType = _T("iPool");
		website = _T("ThePoolClub.com");
	}
		  
	if(g_build.main_server) // #ifdef MAIN_SERVER
	{
		if(g_build.test_server) //	#ifdef TEST_SERVER
		{
		//sTitle = _T("- (TEST) TheSnookerClub.com - iSnooker V");
		sTitle = _T("- (TEST) ");
		sTitle += _T(website);
		sTitle += _T(" - ");
		sTitle += _T(gameType);
		sTitle += _T(" V");
		sTitle += _T(g_build.version);
		sTitle += _T(GAME_VERSION_SUB);
		sTitle += _T(" - Main Server");
		//if(g_gameAppView)
		//sTitle += g_gameAppView->m_lobbySettings->m_name;

		SetTitle(sTitle.GetBuffer(0));
		//SetTitle("- (TEST) TheSnookerClub.com - iSnooker - Main Server"); // set the name of this document
		} else { //#else
		//sTitle = _T("- TheSnookerClub.com - iSnooker V");
		sTitle = _T("- ");
		sTitle += _T(website);
		sTitle += _T(" - ");
		sTitle += _T(gameType);
		sTitle += _T(" V");
		sTitle += _T(g_build.version);
		sTitle += _T(GAME_VERSION_SUB);
		sTitle += _T(" - Main Server");
		//if(g_gameAppView)
		//sTitle += g_gameAppView->m_lobbySettings->m_name;


		SetTitle(sTitle.GetBuffer(0));
		//SetTitle("- TheSnookerClub.com - iSnooker - Main Server"); // set the name of this document
		} // #endif //TEST_SERVER
	} else { //#else
		if(g_build.test_server) // #ifdef TEST_SERVER
		{
		//sTitle = _T("- (TEST) TheSnookerClub.com - iSnooker V");
		sTitle = _T("- (TEST) ");
		sTitle += _T(website);
		sTitle += _T(" - ");
		sTitle += _T(gameType);
		sTitle += _T(" V");
		sTitle += _T(g_build.version);
		sTitle += _T(GAME_VERSION_SUB);
		SetTitle(sTitle.GetBuffer(0));
		//SetTitle("- TheSnookerClub.com - iSnooker"); // set the name of this document
		} else { //#else
		//sTitle = _T("- TheSnookerClub.com - iSnooker V");
		sTitle = _T("- ");
		sTitle += _T(website);
		sTitle += _T(" - ");
		sTitle += _T(gameType);
		sTitle += _T(" V");
		sTitle += _T(g_build.version);
		sTitle += _T(GAME_VERSION_SUB);
		SetTitle(sTitle.GetBuffer(0));
		//SetTitle("- TheSnookerClub.com - iSnooker"); // set the name of this document
		} // #endif //TEST_SERVER
	}//#endif //MAIN_SERVER
		
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGameAppDoc serialization

void CGameAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGameAppDoc diagnostics

#ifdef _DEBUG
void CGameAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGameAppDoc commands

void CGameAppDoc::DeleteContents() 
{
	CDocument::DeleteContents();
}

BOOL CGameAppDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
		
	return TRUE;
}

BOOL CGameAppDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnSaveDocument(lpszPathName);
}

void CGameAppDoc::OnPracticeLoad() 
{
	// TODO: Add your command handler code here
}
