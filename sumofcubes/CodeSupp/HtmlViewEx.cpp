// HtmlViewEx.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "HtmlViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewEx

IMPLEMENT_DYNCREATE(CHtmlViewEx, CHtmlView)

CHtmlViewEx::CHtmlViewEx()
{
	//{{AFX_DATA_INIT(CHtmlViewEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bRefreshPage = FALSE;
	m_refreshCounter = 0;
	m_refreshTimeFrames = -1; // not used defaulty
	m_timeEnd = m_timeStart = 0;
}

CHtmlViewEx::~CHtmlViewEx()
{
}

void CHtmlViewEx::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlViewEx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlViewEx, CHtmlView)
	//{{AFX_MSG_MAP(CHtmlViewEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	ON_WM_DESTROYCLIPBOARD()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewEx diagnostics

#ifdef _DEBUG
void CHtmlViewEx::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlViewEx::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmlViewEx message handlers

int CHtmlViewEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_bRefreshPage = FALSE;
	m_refreshCounter = 0;
	m_refreshTimeFrames = -1; // not used!
	
	return 0;
}

void CHtmlViewEx::SetRefreshTime(int time) 
{
	m_refreshTimeFrames = time;
	m_refreshCounter = 0;
	m_timeEnd = m_timeStart = 0;
}

BOOL CHtmlViewEx::IsRefreshTime(BOOL bSaveRefresh) 
{
	return FALSE;
	if(!m_bRefreshPage && m_refreshTimeFrames != -1)
	{
		if(!m_refreshCounter)
		{
			m_timeStart = GetTickCount();
			m_timeEnd = m_timeStart;
		}
		else
			m_timeEnd = GetTickCount();

		m_refreshCounter++; // just a counter

		DWORD duration = m_timeEnd - m_timeStart;
		if(duration >= m_refreshTimeFrames)
		{
			m_refreshCounter = 0;
			m_timeEnd = m_timeStart = 0;
			
			if(bSaveRefresh)
				m_bRefreshPage = TRUE;
			return TRUE;
		}

		
		//char buffer[80];
		//sprintf(buffer,"duration = %d, m_timeEnd = %d", duration, m_timeEnd);
		//ErrorMessage(g_wnd, buffer);
	}

	if(m_bRefreshPage)
		return TRUE;

	return FALSE;
}

void CHtmlViewEx::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CHtmlView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}



void CHtmlViewEx::OnDestroyClipboard()
{
	// This is required to stop crashing on XP when closing the CHtmlViewEx //
}

void CHtmlViewEx::PostNcDestroy()
{
	// This is required to stop crashing on XP when closing the CHtmlViewEx //
}

void CHtmlViewEx::OnNcDestroy()
{
	// This is required to stop crashing on XP when closing the CHtmlViewEx //
}

void CHtmlViewEx::OnDestroy()
{
	CWnd::OnDestroy();
}

BOOL CHtmlViewEx::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return 0;
	//return CHtmlView::OnEraseBkgnd(pDC);
}

void CHtmlViewEx::OnNavigateComplete2(LPCTSTR strURL)
{
	CHtmlView::OnNavigateComplete2(strURL);
	return;

	// TODO: Add your specialized code here and/or call the base class
 
	// NOTE: the following code below is application-specific, you may wish to remove if not required

	// if failed to load page used auxilliary page instead, else hard luck ;)
	CString string1, string2;
	string1 = GetLocationName();
	string2 = GetLocationURL();
	//CString string3;
	//string3 = string1 +",  " + string2;
	//ErrorMessage(g_wnd, string3.GetBuffer(0));
	/*
	if( string1.CompareNoCase(string2) == 0 ||
	    string1.CompareNoCase(_T("Cannot find server")) == 0 ) // couldn't find link
	{
		if(string2.CompareNoCase(g_URLBanner) == 0)
		{
			Navigate2(g_URLBanner2,NULL,NULL);
		}
		//else if(string2.CompareNoCase(g_URLMessageBoard) == 0)
		//{			
		//	Navigate2(g_URLMessageBoard2,NULL,NULL);
		//}
		else if(string2.CompareNoCase(g_lobbyMessageBoard) == 0)
		{			
			Navigate2(g_lobbyMessageBoard2,NULL,NULL);
		}		
	}

  */
	CHtmlView::OnNavigateComplete2(strURL);
}

void CHtmlViewEx::OnDownloadComplete() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnDownloadComplete();
/*
	CString string1, string2;
	string1 = GetLocationName();
	string2 = GetLocationURL();
	string1 = string1 +",  " + string2;
	ErrorMessage(g_wnd, string1.GetBuffer(0));
*/
}

void CHtmlViewEx::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	CHtmlView::OnDraw(pDC);
}

void CHtmlViewEx::OnTitleChange(LPCTSTR lpszText) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CHtmlView::OnTitleChange(lpszText);
}

//void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel)
//{
//	char buffer[256];
//	sprintf(buffer, "lpszURL = %s, dwError = %d", lpszURL, dwError);
//	ErrorMessage(g_wnd, buffer);
//}