// MainFrm.cpp : implementation of the CMainFrame class
//

#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"
#include "MainFrm.h"
#include "GameAppDoc.h"
#include "GameAppView.h"
//#include "SendView.h"
#include "ChatView.h"
#include "../Code/Lobby.h"
// #include <Dsetup.h>
#include <afxinet.h>


#ifdef _DEBUG										   
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_MENUSELECT()
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPTIONS_HIGHPERFORMANCE, OnOptionsHighPerformance)
	ON_COMMAND(ID_OPTIONS_LOWGFX, OnOptionsLowgfx)
	ON_COMMAND(ID_OPTIONS_MEDIUMGFX, OnOptionsMediumgfx)
	ON_COMMAND(ID_OPTIONS_HIGHGFX, OnOptionsHighgfx)
	ON_WM_MOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_OPTIONS_REPLAYPANEL, OnOptionsReplaypanel)
	ON_COMMAND(ID_OPTIONS_2D, OnOptions2d)
	ON_COMMAND(ID_OPTIONS_2D3D, OnOptions2d3d)
	ON_COMMAND(ID_OPTIONS_2DTVAUTO, OnOptions2dtvauto)
	ON_COMMAND(ID_OPTIONS_2D3DFREE, OnOptions2d3dfree)
	ON_COMMAND(ID_SOUND_BALLS, OnSoundBalls)
	ON_COMMAND(ID_SOUND_CLAPPING, OnSoundClapping)
	ON_COMMAND(ID_DRAWCUETIP, OnDrawcuetip)
	ON_COMMAND(ID_DRAWCUETRANSPARENT, OnDrawcuetransparent)
	ON_COMMAND(ID_DRAWCUESOLID, OnDrawcuesolid)
	ON_COMMAND(ID_PRACTICE_BALLINHAND, OnPracticeBallinhand)
	ON_COMMAND(ID_OPTIONS_COMPLEX_SHADOWS, OnOptionsComplexShadows)
	ON_COMMAND(ID_OPTIONS_SHOWMINICAM, OnOptionsShowminicam)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_100HZ, OnOptionsSetrefreshrate100hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_120HZ, OnOptionsSetrefreshrate120hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_140HZ, OnOptionsSetrefreshrate140hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_25HZ, OnOptionsSetrefreshrate25hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_30HZ, OnOptionsSetrefreshrate30hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_50HZ, OnOptionsSetrefreshrate50hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_60HZ, OnOptionsSetrefreshrate60hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_70HZ, OnOptionsSetrefreshrate70hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_72HZ, OnOptionsSetrefreshrate72hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_85HZ, OnOptionsSetrefreshrate85hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_DEFAULT, OnOptionsSetrefreshrateDefault)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_150HZ, OnOptionsSetrefreshrate150hz)
	ON_COMMAND(ID_OPTIONS_SETREFRESHRATE_75HZ, OnOptionsSetrefreshrate75hz)
	ON_COMMAND(ID_OPTIONS_LOGINWIZARD, OnOptionsLoginwizard)
	ON_COMMAND(ID_OPTIONS_DEFAULTCOLORSCHEME, OnOptionsDefaultScheme)
	ON_COMMAND(ID_OPTIONS_LOBBYTV, OnShowShotInLobbyTV)
	ON_COMMAND(ID_OPTIONS_DRAWCOLOURSL, OnOptionsDrawcoloursl)
	ON_COMMAND(ID_OPTIONS_DRAWERRORSL, OnOptionsDrawerrorsl)
	ON_COMMAND(ID_OPTIONS_STATICPERCENTAGE, OnOptionsStaticpercentage)
	ON_COMMAND(ID_OPTIONS_DRAWGHOSTCUEBALL, OnOptionsDrawghostcueball)
	ON_COMMAND(ID_OPTIONS_DRAWCUEBALL_LE, OnOptionsDrawcueballLe)
	ON_COMMAND(ID_FILE_AUTO_SAVE, OnFileAutoSave)
	ON_COMMAND(ID_SOUND_VOICES, OnSoundVoices)
	ON_UPDATE_COMMAND_UI(ID_SOUND_VOICES, OnUpdateSoundVoices)
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_OPTIONS_COLOURBLIND, &CMainFrame::OnOptionsColourblind)
	ON_COMMAND(ID_OPTIONS_DOTTEDCUEBALL, &CMainFrame::OnOptionsDottedcue)
	ON_COMMAND(ID_OPTIONS_MINICAM_SHOWHELPERS, &CMainFrame::OnOptionsMinicamShowhelpers)
	ON_COMMAND(ID_OPTIONS_MINICAM_LOCKMOUSE, &CMainFrame::OnOptionsMinicamLockmouse)
	ON_COMMAND(ID_TOGGLE_FULLSCREEN, &CMainFrame::OnToggleFullscreen)
END_MESSAGE_MAP()
	

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_APP_ABOUT,
};

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_CONNECTIONS,
	ID_MESSAGES,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction


BOOL widescreen;

CMainFrame::CMainFrame()	 
{
	// TODO: add member initialization code here
	//m_pSendView = new CSendView;	
	//appName = new char[81];
	appName = "Memir Games";
	g_bMenuSelect = 0;
	g_menuNItemID = -1;
	g_menuNItemIDOld = -1;
	g_bWireframeChanged = TRUE;
	g_bAntiAliasChanged = TRUE;
	g_bPerformanceChanged = TRUE;

	bFullScreen = false;

	updateAspect();
	
}

void CMainFrame::updateAspect(float ratio)
{
	CWnd* wndDesktop = GetDesktopWindow();
	CRect drect;
	wndDesktop->GetWindowRect(&drect);

	g_iDesktopWidth = (drect.right - drect.left);
	g_iDesktopHeight = (drect.bottom - drect.top);



	g_iMaxHeightWS = g_iDesktopHeight;
	float fTmp = (float)g_iMaxHeightWS;
	fTmp *= 1.55f;	// Anything over this ratio must be widescreen
	g_iMaxWidthWS = (int)fTmp;
	if (g_iDesktopWidth > g_iMaxWidthWS)
	{
		// If we're in widescreen mode then we need to limit the width to 1.33 x Height.
		fTmp = (float)g_iMaxHeightWS;
		//fTmp *= 1.3333f;
		fTmp *= ratio;
		g_iMaxWidthWS = (int)fTmp;
		widescreen = true;
	}
	else
	{
		widescreen = false;
		g_iMaxWidthWS = g_iDesktopWidth;
	}
}

CMainFrame::~CMainFrame()
{
	//if(m_pSendView) delete m_pSendView;
	//m_pSendView = 0;
	//delete appName;
}

void CMainFrame::toggleFullScreen()
{
	if (!bFullScreen) {
		updateAspect(1.5);
		ShowWindow(SW_NORMAL);

		// Fullscreen test
		ModifyStyle(WS_MAXIMIZEBOX, 0);
		ModifyStyle(WS_MINIMIZEBOX, 0);
		ModifyStyle(WS_CAPTION, 0);

		ShowWindow(SW_SHOWMAXIMIZED);
		
		bFullScreen = true;
	}
	else {
		updateAspect(1.36);
		ModifyStyle(0, WS_MAXIMIZEBOX);
		ModifyStyle(0, WS_MINIMIZEBOX);
		ModifyStyle(0, WS_CAPTION);

		ShowWindow(SW_NORMAL);

		g_gameAppView->ResizeScreenToSaved();

		bFullScreen = false;
	}
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// Mouse Wheel is now controlled from the MainFrame.... 

	// g_gameAppView->MouseWheel(zDelta);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CMainFrame::OnGetMinMaxInfo( MINMAXINFO FAR* pMinMaxInfo )
{

	/*
	if(widescreen)
	{

		pMinMaxInfo->ptMaxPosition.x = (g_iDesktopWidth - g_iMaxWidthWS) / 2;
		pMinMaxInfo->ptMaxPosition.y = 0;



		//pMinMaxInfo->ptMaxPosition.x = (g_iDesktopWidth - g_iMaxWidthWS) / 2;

		// Preferred Maximum X & Y.
		const int MAX_SIZE_X = g_iMaxWidthWS;
		const int MAX_SIZE_Y = g_iMaxHeightWS - 20;

		// Preferred Minimum X & Y.
		const int MIN_SIZE_X = 800;
		const int MIN_SIZE_Y = 600;

		// Set the maximum size. Used while maximizing.
		pMinMaxInfo->ptMaxSize.x = MAX_SIZE_X;
		pMinMaxInfo->ptMaxSize.y = MAX_SIZE_Y;

		// Set the Minimum Track Size. Used while resizing.
		pMinMaxInfo->ptMinTrackSize.x = MIN_SIZE_X;
		pMinMaxInfo->ptMinTrackSize.y = MIN_SIZE_Y;

		// Set the Maximum Track Size. Used while resizing.
		pMinMaxInfo->ptMaxTrackSize.x = MAX_SIZE_X;
		pMinMaxInfo->ptMaxTrackSize.y = MAX_SIZE_Y;

		RecalcLayout(); 
	}
	*/
}



BOOL GetAppVersion( char *LibName, WORD *MajorVersion, WORD *MinorVersion, WORD *BuildNumber, WORD *RevisionNumber )
{ 
	DWORD dwHandle, 
	dwLen;
	UINT BufLen;
	LPTSTR lpData;
	VS_FIXEDFILEINFO
	*pFileInfo; 
	dwLen = GetFileVersionInfoSize( LibName, &dwHandle );
	if (!dwLen) return FALSE; 
	lpData = (LPTSTR) malloc (dwLen);
	if (!lpData) return FALSE; 

	if( !GetFileVersionInfo( LibName, dwHandle, dwLen, lpData ) ) 
	{  
		free (lpData); 
		return FALSE; 
	} 
	
	if( VerQueryValue( lpData, "\\", (LPVOID *) &pFileInfo, (PUINT)&BufLen ) )  
	{  
		*MajorVersion = HIWORD(pFileInfo->dwFileVersionMS);  
		*MinorVersion = LOWORD(pFileInfo->dwFileVersionMS);  
		*BuildNumber = HIWORD(pFileInfo->dwFileVersionLS);  
		*RevisionNumber = LOWORD(pFileInfo->dwFileVersionLS);  
		free (lpData);  
		return TRUE; 
	} 
	
	free (lpData); 
	
	return FALSE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.lpszClass = AfxRegisterWndClass(CS_BYTEALIGNWINDOW | CS_BYTEALIGNCLIENT);

	// Check if we need to install DirectX March 2008 Version //


	//::LoadLibrary("redist//dsetup.dll");



	/*
	char buf[256];
	sprintf(buf, "DirectX Lib Version is %d.%d.%d.%d\n",
		   MajorVersion, MinorVersion,
		   BuildNumber, RevisionNumber);
	AfxMessageBox(buf);
		
	DWORD dwVersion;
	DWORD dwRevision;
	if (DirectXSetupGetVersion(&dwVersion, &dwRevision))
	{
		char buf[256];
		sprintf(buf, "DirectX version is %d.%d.%d.%d\n",
			   HIWORD(dwVersion), LOWORD(dwVersion),
			   HIWORD(dwRevision), LOWORD(dwRevision));
		AfxMessageBox(buf);
	}
	*/




	//if(widescreen) cs.style ^= WS_MAXIMIZEBOX;

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{

	// Enable Drag and Drop Files
	DragAcceptFiles();

	// We must be in 1024 x 768 Mode or Greather //
	// set the window size and center on screen
	SetWindowPos(&wndTop, 0, 0, 800, 600, 0);
	CenterWindow();
	
	ShowWindow(SW_HIDE);


	
	g_menu = this->GetMenu();

	//g_menu->Detach();
	// 'Options'
	CMenu *subMenu = g_menu->GetSubMenu(OPTIONS_MENU); // options sub-menu
	subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_CHECKED | MF_BYPOSITION); // medium gfx detail
	subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_CHECKED | MF_BYPOSITION); // 2D/3D mode


	

	// POOL (Practice Menu)
	#ifdef THE_POOL_CLUB
	// modify practice menu
	CMenu *practiceMenu = g_menu->GetSubMenu(PRACTICE_MENU); // options sub-menu
	
	// Game Types - One of these is always selected - Defaultly 8-Ball Setup //
	practiceMenu->ModifyMenu(0, MF_BYPOSITION, ID_PRACTICE_START, "8-Ball Setup\tF8");
	practiceMenu->ModifyMenu(1, MF_BYPOSITION, ID_PRACTICE_LINEUP, "9-Ball Setup\tF8");
	practiceMenu->ModifyMenu(2, MF_BYPOSITION, ID_PRACTICE_COLOURS, "8-Ball Scattered\tF8");
	practiceMenu->ModifyMenu(3, MF_BYPOSITION, ID_PRACTICE_BLACK, "9-Ball Scattered\tF8");
	practiceMenu->ModifyMenu(4, MF_BYPOSITION, ID_PRACTICE_REDSSCATTERED1, "Rotation Ball Setup\tF8");
	practiceMenu->ModifyMenu(5, MF_BYPOSITION, ID_PRACTICE_REDSSCATTERED2, "Bowls Setup\tF8");
	practiceMenu->ModifyMenu(6, MF_BYPOSITION, ID_PRACTICE_A, "6-Ball Setup\tF8");
	practiceMenu->InsertMenu(7, MF_BYPOSITION, ID_PRACTICE_B, "6-Ball Scattered\tF8");
	practiceMenu->InsertMenu(8, MF_BYPOSITION, ID_PRACTICE_C, "10-Ball Setup\tF8");
	practiceMenu->InsertMenu(9, MF_BYPOSITION, ID_PRACTICE_D, "10-Ball Scattered\tF8");
	practiceMenu->InsertMenu(10, MF_BYPOSITION, ID_PRACTICE_E, "Break Setup\tF8");
	practiceMenu->InsertMenu(11, MF_BYPOSITION, ID_PRACTICE_F, "English Pool Setup\tF8");
	// Table Types - One of these is always selected - Defaultly the Normal Table //
	practiceMenu->InsertMenu(12, MF_BYPOSITION, ID_PRACTICE_G, "Normal Table");
	practiceMenu->InsertMenu(13, MF_BYPOSITION, ID_PRACTICE_H, "Pro Table");
	practiceMenu->InsertMenu(14, MF_BYPOSITION, ID_PRACTICE_I, "English Pool Table");
	//practiceMenu->InsertMenu(12, MF_BYPOSITION, ID_PRACTICE_I, "English Pool Table");
	
			
	//practiceMenu->RemoveMenu(4, MF_BYPOSITION);
	//practiceMenu->RemoveMenu(4, MF_BYPOSITION);

	// modify file menu
	CMenu *fileMenu = g_menu->GetSubMenu(FILE_MENU); // options sub-menu
	fileMenu->ModifyMenu(FILE_LOADFRAMES, MF_BYPOSITION, ID_FILE_LOAD, "Load Rack... (Replay)");
	fileMenu->ModifyMenu(FILE_SAVEFRAMES, MF_BYPOSITION, ID_FILE_SAVE, "Save Rack... (Replay)");
	fileMenu->ModifyMenu(FILE_AUTOSAVEFRAMES, MF_BYPOSITION, ID_FILE_AUTO_SAVE, "Auto Save Racks (Online)");

	#endif
	

	

BOOL result;
	result =  CFrameWnd::OnCreateClient(lpcs, pContext);

	return result;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

#define ISNK_VER	"2.0.05"

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

/*
	//--- begin calculating remote file size -----
	char		szSizeBuffer[32];
	DWORD		dwLengthSizeBuffer = 32;
	HINTERNET	hFileInfo;
	hFileInfo = InternetOpenUrl( session, "http://www.thesnookerclub.com/autoupdate/isnk/update.txt", NULL, 0, 0, 0);
	BOOL bQuery = ::HttpQueryInfo(hFileInfo,HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer, &dwLengthSizeBuffer, NULL) ;


	DWORD m_dwTotalSize;
	if( bQuery )
	{    
		  m_dwTotalSize = atol(szSizeBuffer);
		  ::InternetCloseHandle(hFileInfo);
	}
	else
	{
		  m_strError = "Error: Unable to retrieve file size ! ";
		  return FALSE;
	}
	//--- end calculating remote file size -----

	// Check that we are using the Latest Version of iSnooker / iPool //
	CInternetSession InternetSession; 
	CInternetFile* pInternetFile=NULL;
	CStdioFile* MyFile = NULL;
	InternetSession.
	MyFile = InternetSession.OpenURL("http://www.thesnookerclub.com/autoupdate/isnk/update.txt", 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD);
	if(MyFile != NULL)
	{
		CString str;
		MyFile->ReadString(str);
		InternetSession.Close();

		//AfxMessageBox(str);
		//AfxMessageBox(ISNK_VER);
		if(strcmpi(str, ISNK_VER) !=0)	// We don't have the correct version //
		{
			AfxMessageBox("A New Beta Version is available. You will now be directed to our website where you can install the latest Beta version");
			char dbuf[128];
			sprintf(dbuf, "http://www.thesnookerclub.com/update.php?ver=%s", ISNK_VER);

			SHELLEXECUTEINFO ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = dbuf;		
			ShExecInfo.lpParameters = "";	
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;	
			ShellExecuteEx(&ShExecInfo);
			exit(0);
			//WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
		}
	}		


	*/


#ifndef MAIN_SERVER

	// d3dx9_43.dll
	
	WORD MajorVersion;
	WORD MinorVersion;
	WORD BuildNumber;
	WORD RevisionNumber;

	//if(!GetAppVersion("d3dx9_37.dll", &MajorVersion, &MinorVersion, &BuildNumber, &RevisionNumber))
	if(!GetAppVersion("d3dx9_43.dll", &MajorVersion, &MinorVersion, &BuildNumber, &RevisionNumber))
	{
		char buf[256];
		#ifdef THE_POOL_CLUB
			sprintf(buf, "Microsoft DirectX must be installed to run iPool. Click 'YES' to proceed with DirectX installation");
		#else
			sprintf(buf, "Microsoft DirectX must be installed to run iSnooker. Click 'YES' to proceed with DirectX installation");
		#endif

		if (AfxMessageBox(buf, MB_YESNO) == IDNO) 
		{
			// Close Application
			exit(0);
		}
		else
		{
			//DirectXSetup(cs.hwndParent, "redist", DSETUP_DIRECTX); 
			SHELLEXECUTEINFO ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = "redist\\DXSETUP.exe";
			ShExecInfo.lpParameters = "/silent";	
			ShExecInfo.lpDirectory = NULL;
			ShExecInfo.nShow = SW_SHOW;
			ShExecInfo.hInstApp = NULL;	
			ShellExecuteEx(&ShExecInfo);
			WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
			//system("redist\\DXSETUP.exe");
		}
	}
	
#endif

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
/*	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}*/
	
	

	return 0;
}



BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{

	// TODO: Add your specialized code here and/or call the base class
	//dwStyle|=CS_BYTEALIGNWINDOW|CS_BYTEALIGNCLIENT;
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);


}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	
	int width = 0;
	int height = 0;

	CFrameWnd::OnSize(nType, cx, cy);

	return;

	CRect rect;
	GetClientRect(&rect);

	if(g_WideScreenEnabled)
	{
		if( cx > g_iMaxWidthWS || cy > g_iMaxHeightWS)
		{
			// Make the Width 1.3 * the Height //
			if(cx > g_iMaxWidthWS) width = g_iMaxWidthWS;
			if(cy > g_iMaxHeightWS) height = g_iMaxHeightWS;

			if(width == 0) width = cx;
			if(height == 0) height = cy;

			
			if(nType == 2) // Maximized Button was Clicked //
			{
				//ShowWindow(SW_SHOWDEFAULT);
			}
			SetWindowPos(&wndTop, 0, 0, width, height, 0);
			CenterWindow();
		}
		
	}

}


void CMainFrame::OnSizing(UINT nSide, LPRECT lpRect) 
{
	CFrameWnd::OnSizing(nSide, lpRect);
	return;
	// TODO: Add your message handler code here
	
	int w, h;
	w = lpRect->right - lpRect->left;
	h = lpRect->bottom - lpRect->top;

	// check for size limit

	if(g_WideScreenEnabled)
	{
		int width_ws = g_iDesktopHeight * 1.25f;

		if(w<MAINFRM_MINIMUM_WIDTH_WS) lpRect->right = lpRect->left+MAINFRM_MINIMUM_WIDTH_WS;
		if(h<MAINFRM_MINIMUM_HEIGHT_WS) lpRect->bottom = lpRect->top+MAINFRM_MINIMUM_HEIGHT_WS;
		if(w>g_iMaxWidthWS) lpRect->right = lpRect->left+g_iMaxWidthWS;
		if(h>g_iMaxHeightWS) lpRect->bottom = lpRect->top+g_iMaxHeightWS;
	}
	else
	{
		if(w<MAINFRM_MINIMUM_WIDTH) lpRect->right = lpRect->left+MAINFRM_MINIMUM_WIDTH;
		if(h<MAINFRM_MINIMUM_HEIGHT) lpRect->bottom = lpRect->top+MAINFRM_MINIMUM_HEIGHT;
		//if(w>MAINFRM_MAXIMUM_WIDTH) lpRect->right = lpRect->left+MAINFRM_MAXIMUM_WIDTH;
		//if(h>MAINFRM_MAXIMUM_HEIGHT) lpRect->bottom = lpRect->top+MAINFRM_MAXIMUM_HEIGHT;
	}



}



void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{

    CString    sFile;
    DWORD      nBuffer    = 0;
    // Get the number of files dropped
    UINT nFilesDropped    = DragQueryFile (hDropInfo, 0xFFFFFFFF, NULL, 0);
    // If more than one, only use the first
    if (nFilesDropped > 0)
    {
        // Get the buffer size for the first filename
        nBuffer = DragQueryFile (hDropInfo, 0, NULL, 0);
        // Get path and name of the first file
        DragQueryFile (hDropInfo, 0, sFile.GetBuffer (nBuffer + 1), nBuffer + 1);
        sFile.ReleaseBuffer ();
        // Do something with the path
		
		// If this is a Replay File, then Open it
		g_gameAppView->LoadReplay(sFile.GetBuffer(0));

    }
    // Free the memory block containing the dropped-file information
    DragFinish(hDropInfo);

	
	
}


void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
	CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
		
	// TODO: Add your message handler code here
	if(nFlags == 0xFFFF && hSysMenu == 0) 
	{
		g_bMenuSelect = 0;
		g_menuNItemID = -1;
		g_menuNItemIDOld = -1;

		if(g_gameAppView->m_lobby->m_bInLobby)
			{
			g_Menu_Refresh = TRUE;
			g_gameAppView->Draw2DLobby();
			
			}
	}
	else
	{
		g_menuNItemIDOld = g_menuNItemID;
		if(nItemID <= (MAX_MENU_ITEMS-1) && nItemID >=0) g_menuNItemID = nItemID;

		
		g_bMenuSelect = 1;
		if(g_gameAppView) 
		if(g_gameAppView->m_lobby->m_bInLobby)
			{
			g_Menu_Refresh = TRUE;
			g_gameAppView->Draw2DLobby();
			}
		
	}
	
}



void CMainFrame::OnClose() 
{
	if(g_build.main_server)
	{
		if (AfxMessageBox("Quit Server App?",MB_YESNO) == IDNO) return;
	}
	else
	{
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			if (AfxMessageBox("Are you sure you want to close iSnooker",MB_YESNO) == IDNO) return;
		}
		else
		{
			if (AfxMessageBox("Are you sure you want to close iPool",MB_YESNO) == IDNO) return;
		}
	}

	///////
	#ifndef MAIN_SERVER
	if(g_gameAppView)
	{				
		if(!g_gameAppView->m_lobby->m_bInLobby)
		{
			if(!g_gameAppView->m_snooker->m_bPracticeMode)
			{
				// if player is still involved in a game?
				//if(g_gameAppView->m_lobby->GameStarted(g_gameAppView->m_playerSockIdx) &&
				//   m_myself.status && (g_gameAppView->m_snooker->m_gameOver == 0))
				if(g_gameAppView->m_lobby->GameStarted(g_gameAppView->m_playerSockIdx) &&
				   m_myself.status && (g_gameAppView->m_snooker->m_gameOver == 0 || g_gameAppView->m_snooker->m_gameOverIWon == 1) )
				{
					g_gameAppView->m_bMainFramePlayingClose = TRUE;
					g_gameAppView->m_exitDlg.DestroyWindow();
					g_gameAppView->m_exitDlg.Create(IDD_EXITDLG);
					g_gameAppView->m_exitDlg.ShowWindow(SW_SHOW);

					return;
				}
			}
		}
		/*
		else
		{
			OnInternetExit();
			return;
		}
		*/
	}
	#endif //!MAIN_SERVER
	///////
	

	// TODO: Add your message handler code here and/or call default
	if(gameShell) 
	{
		if(gameShell->Client)
			gameShell->Client->Close();

		if(gameShell->Server)
			gameShell->Server->Close();

		delete gameShell;
		gameShell = NULL;
	}

	if(mainShell) 
	{
		if(mainShell->Client)
			mainShell->Client->Close();

		if(mainShell->Server)
			mainShell->Server->Close();

		delete mainShell;
		mainShell = NULL;
	}

	g_bClose = 1;
	

	CFrameWnd::OnClose();

	//PostQuitMessage(123); // Close & stop any respawns!!
}



void CMainFrame::OnOptionsLowgfx() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);

	g_gfxDetails.bOptionsLowgfx = 1;
	g_gfxDetails.bOptionsMediumgfx = 0;
	g_gfxDetails.bOptionsHighgfx = 0;
	subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	
	/*
	// Store orginal Render Target //
	g_pDevice->GetRenderTarget(0, &pRenderSurface);
	D3DSURFACE_DESC desc;
	pRenderSurface->GetDesc( &desc );
	HRESULT hr;
	*/

	/*
	pRenderSurfaceMiniCam->Release();
	hr = g_pDevice->CreateRenderTarget( 192, 192, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, true, &pRenderSurfaceMiniCam, NULL);
	g_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurfaceMiniCam);
	*/

	SaveGfxDetails();
}

void CMainFrame::OnOptionsMediumgfx() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);

	g_gfxDetails.bOptionsLowgfx = 0;
	g_gfxDetails.bOptionsMediumgfx = 1;
	g_gfxDetails.bOptionsHighgfx = 0;
	subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);

	/*
	// Store orginal Render Target //
	g_pDevice->GetRenderTarget(0, &pRenderSurface);
	D3DSURFACE_DESC desc;
	pRenderSurface->GetDesc( &desc );
	HRESULT hr;
	*/

	/*
	pRenderSurfaceMiniCam->Release();
	hr = g_pDevice->CreateRenderTarget( 304, 304, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, true, &pRenderSurfaceMiniCam, NULL);
	g_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurfaceMiniCam);
	*/

	SaveGfxDetails();
}

void CMainFrame::OnOptionsHighgfx() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);

	g_gfxDetails.bOptionsLowgfx = 0;
	g_gfxDetails.bOptionsMediumgfx = 0;
	g_gfxDetails.bOptionsHighgfx = 1;
	subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_CHECKED | MF_BYPOSITION);

	SaveGfxDetails();
}

void CMainFrame::OnOptions2d() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	
	g_gfxDetails.bOptions2DMode = 1;
	g_gfxDetails.bOptions2D3DMode = 0;
	g_gfxDetails.bOptions2DTVAutoMode = 0;
	g_gfxDetails.bOptions2D3DFreeMode = 0;
	g_gameAppView->m_bUpdateTable = TRUE;
	subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_CHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_UNCHECKED | MF_BYPOSITION);


	SaveGfxDetails();
}

void CMainFrame::OnOptions2d3d() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	
	g_gfxDetails.bOptions2DMode = 0;
	g_gfxDetails.bOptions2D3DMode = 1;
	g_gfxDetails.bOptions2DTVAutoMode = 0;
	g_gfxDetails.bOptions2D3DFreeMode = 0;
	g_gameAppView->m_bUpdateTable = TRUE;
	subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_CHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_UNCHECKED | MF_BYPOSITION);
	
	SaveGfxDetails();
}

void CMainFrame::OnOptions2dtvauto() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	
	g_gfxDetails.bOptions2DMode = 0;
	g_gfxDetails.bOptions2D3DMode = 0;
	g_gfxDetails.bOptions2DTVAutoMode = 1;
	g_gfxDetails.bOptions2D3DFreeMode = 0;
	g_gameAppView->m_bUpdateTable = TRUE;
	subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_CHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_UNCHECKED | MF_BYPOSITION);


	SaveGfxDetails();
}



void CMainFrame::OnOptions2d3dfree() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	
	g_gfxDetails.bOptions2DMode = 0;
	g_gfxDetails.bOptions2D3DMode = 0;
	g_gfxDetails.bOptions2DTVAutoMode = 0;
	g_gfxDetails.bOptions2D3DFreeMode = 1;
	g_gameAppView->m_bUpdateTable = TRUE;
	subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_CHECKED | MF_BYPOSITION);

	SaveGfxDetails();
}





void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
	
	// TODO: Add your message handler code here
}

void CMainFrame::OnOptionsReplaypanel() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnSoundBalls() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_SOUND);
		if(subSubMenu)
		{
			if(subSubMenu->GetMenuState(OPTIONS_SOUND_BALL_SFX, MF_BYPOSITION) == MF_UNCHECKED)
			{
				subSubMenu->CheckMenuItem(OPTIONS_SOUND_BALL_SFX, MF_CHECKED | MF_BYPOSITION);
				g_gfxDetails.bSoundBalls = 1;
			}
			else
			{
				subSubMenu->CheckMenuItem(OPTIONS_SOUND_BALL_SFX, MF_UNCHECKED | MF_BYPOSITION);
				g_gfxDetails.bSoundBalls = 0;
			}

			SaveGfxDetails();
		}
	}
}

void CMainFrame::OnSoundClapping() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_SOUND);
		if(subSubMenu)
		{
			if(subSubMenu->GetMenuState(OPTIONS_SOUND_CLAPPING_SFX, MF_BYPOSITION) == MF_UNCHECKED)
			{
				subSubMenu->CheckMenuItem(OPTIONS_SOUND_CLAPPING_SFX, MF_CHECKED | MF_BYPOSITION);
				g_gfxDetails.bSoundClapping = 1;
			}
			else
			{
				subSubMenu->CheckMenuItem(OPTIONS_SOUND_CLAPPING_SFX, MF_UNCHECKED | MF_BYPOSITION);
				g_gfxDetails.bSoundClapping = 0;
			}

			SaveGfxDetails();
		}
	}
}

void CMainFrame::OnSoundVoices() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_SOUND);
		if(subSubMenu)
		{
			if(subSubMenu->GetMenuState(OPTIONS_SOUND_VOICES, MF_BYPOSITION) == MF_UNCHECKED)
			{
				subSubMenu->CheckMenuItem(OPTIONS_SOUND_VOICES, MF_CHECKED | MF_BYPOSITION);
				g_gfxDetails.bSoundVoices = 1;
			}
			else
			{
				subSubMenu->CheckMenuItem(OPTIONS_SOUND_VOICES, MF_UNCHECKED | MF_BYPOSITION);
				g_gfxDetails.bSoundVoices = 0;
			}

			SaveGfxDetails();
		}
	}
}

void CMainFrame::OnDrawcuetip() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_DRAW_EYE_CAM_CUE);
		if(subSubMenu)
		{
			g_gfxDetails.bEyeCamDrawCueTip = 1;
			g_gfxDetails.bEyeCamDrawCueTransparent = 0;
			g_gfxDetails.bEyeCamDrawCueSolid = 0;

			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TIP, MF_CHECKED | MF_BYPOSITION);
			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TRANSPARENT, MF_UNCHECKED | MF_BYPOSITION);
			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_SOLID, MF_UNCHECKED | MF_BYPOSITION);
			
			SaveGfxDetails();
		}
	}
}

void CMainFrame::OnDrawcuetransparent() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_DRAW_EYE_CAM_CUE);
		if(subSubMenu)
		{
			g_gfxDetails.bEyeCamDrawCueTip = 0;
			g_gfxDetails.bEyeCamDrawCueTransparent = 1;
			g_gfxDetails.bEyeCamDrawCueSolid = 0;

			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TIP, MF_UNCHECKED | MF_BYPOSITION);
			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TRANSPARENT, MF_CHECKED | MF_BYPOSITION);
			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_SOLID, MF_UNCHECKED | MF_BYPOSITION);
			
			SaveGfxDetails();
		}
	}
}

void CMainFrame::OnDrawcuesolid() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_DRAW_EYE_CAM_CUE);
		if(subSubMenu)
		{
			g_gfxDetails.bEyeCamDrawCueTip = 0;
			g_gfxDetails.bEyeCamDrawCueTransparent = 0;
			g_gfxDetails.bEyeCamDrawCueSolid = 1;

			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TIP, MF_UNCHECKED | MF_BYPOSITION);
			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TRANSPARENT, MF_UNCHECKED | MF_BYPOSITION);
			subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_SOLID, MF_CHECKED | MF_BYPOSITION);
			
			SaveGfxDetails();
		}
	}
}

void CMainFrame::OnPracticeBallinhand() 
{
	// TODO: Add your command handler code here
	// 'Practice'
	#ifdef THE_POOL_CLUB
	
	CMenu *subMenu = GetMenu()->GetSubMenu(PRACTICE_MENU);
	if(subMenu->GetMenuState(POOL_PRACTICE_BALLINHANDALWAYS, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_practiceDetails.bOptionsBallInHandAlways = TRUE;
		subMenu->CheckMenuItem(POOL_PRACTICE_BALLINHANDALWAYS, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_practiceDetails.bOptionsBallInHandAlways = FALSE;
		subMenu->CheckMenuItem(POOL_PRACTICE_BALLINHANDALWAYS, MF_UNCHECKED | MF_BYPOSITION);
	}

	#else // SNOOKER CLUB
	
	CMenu *subMenu = GetMenu()->GetSubMenu(PRACTICE_MENU);
	if(subMenu->GetMenuState(PRACTICE_BALLINHANDALWAYS, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_practiceDetails.bOptionsBallInHandAlways = TRUE;
		subMenu->CheckMenuItem(PRACTICE_BALLINHANDALWAYS, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_practiceDetails.bOptionsBallInHandAlways = FALSE;
		subMenu->CheckMenuItem(PRACTICE_BALLINHANDALWAYS, MF_UNCHECKED | MF_BYPOSITION);
	}

	#endif
}



void CMainFrame::OnOptionsHighPerformance() 
{
	// TODO: Add your command handler code here
	// 'Options'
	g_bPerformanceChanged = TRUE;

	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_HIGH_PERFORMANCE,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bHighPerformance = TRUE;
		subMenu->CheckMenuItem(OPTIONS_HIGH_PERFORMANCE, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bHighPerformance = FALSE;
		subMenu->CheckMenuItem(OPTIONS_HIGH_PERFORMANCE, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();
}

void CMainFrame::OnOptionsComplexShadows() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_COMPLEX_SHADOWS,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsComplexShadows = TRUE;
		subMenu->CheckMenuItem(OPTIONS_COMPLEX_SHADOWS, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsComplexShadows = FALSE;
		subMenu->CheckMenuItem(OPTIONS_COMPLEX_SHADOWS, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();
}

void CMainFrame::OnOptionsShowminicam() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if (subMenu->GetMenuState(OPTIONS_SHOW_MINICAM, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsShowMiniCam = TRUE;
		subMenu->CheckMenuItem(OPTIONS_SHOW_MINICAM, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsShowMiniCam = FALSE;
		subMenu->CheckMenuItem(OPTIONS_SHOW_MINICAM, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();
}



void CMainFrame::OnOptionsColourblind()
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if (subMenu->GetMenuState(OPTIONS_COLOUR_BLIND, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsColourBlind = TRUE;
		subMenu->CheckMenuItem(OPTIONS_COLOUR_BLIND, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsColourBlind = FALSE;
		subMenu->CheckMenuItem(OPTIONS_COLOUR_BLIND, MF_UNCHECKED | MF_BYPOSITION);
	}


	SaveGfxDetails();
}
/*
#define OPTIONS_SET_REFRESH_RATE		1

// Menu 'Options->Set Refresh Rate'
#define OPTIONS_SET_REFRESH_RATE_DEFAULT	0
#define OPTIONS_SET_REFRESH_RATE_25			1
#define OPTIONS_SET_REFRESH_RATE_30			2
#define OPTIONS_SET_REFRESH_RATE_50			3
#define OPTIONS_SET_REFRESH_RATE_60			4
#define OPTIONS_SET_REFRESH_RATE_70			5
#define OPTIONS_SET_REFRESH_RATE_72			6
#define OPTIONS_SET_REFRESH_RATE_75			7
#define OPTIONS_SET_REFRESH_RATE_85			8
#define OPTIONS_SET_REFRESH_RATE_100		9
#define OPTIONS_SET_REFRESH_RATE_120		10
#define OPTIONS_SET_REFRESH_RATE_140		11
#define OPTIONS_SET_REFRESH_RATE_150		12

int optionsSetRefreshRate;
*/

void CMainFrame::SetRefreshRateCheck(int n)
{
	int m;
	CMenu *subSubMenu;
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu)
	{
		subSubMenu = subMenu->GetSubMenu(OPTIONS_SET_REFRESH_RATE);
		if(subSubMenu)
		{	
			for(m=0;m<13;m++)
			{
				subSubMenu->CheckMenuItem(m, MF_UNCHECKED | MF_BYPOSITION);
			}

			subSubMenu->CheckMenuItem(n, MF_CHECKED | MF_BYPOSITION);
		}
	}
}

void CMainFrame::OnOptionsSetrefreshrateDefault() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_DEFAULT;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();
}

void CMainFrame::OnOptionsSetrefreshrate25hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_25;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();
}

void CMainFrame::OnOptionsSetrefreshrate30hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_30;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();
}

void CMainFrame::OnOptionsSetrefreshrate50hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_50;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();
}

void CMainFrame::OnOptionsSetrefreshrate60hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_60;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();
}

void CMainFrame::OnOptionsSetrefreshrate70hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_70;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();
}

void CMainFrame::OnOptionsSetrefreshrate72hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_72;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();	
}

void CMainFrame::OnOptionsSetrefreshrate75hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_75;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();	
}

void CMainFrame::OnOptionsSetrefreshrate85hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_85;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();	
}

void CMainFrame::OnOptionsSetrefreshrate100hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_100;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();	
}

void CMainFrame::OnOptionsSetrefreshrate120hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_120;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();	
}

void CMainFrame::OnOptionsSetrefreshrate140hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_140;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();	
}

void CMainFrame::OnOptionsSetrefreshrate150hz() 
{
	// TODO: Add your command handler code here
	g_gfxDetails.optionsSetRefreshRate = OPTIONS_SET_REFRESH_RATE_150;

	SetRefreshRateCheck(g_gfxDetails.optionsSetRefreshRate);

	SaveGfxDetails();	
}

void CMainFrame::OnOptionsLoginwizard() 
{
	// TODO: Add your command handler code here
	if(g_gameAppView)
		g_gameAppView->m_loginWizardDlg.m_bActive = TRUE; // assume already active

	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_SHOW_LOGIN_WIZARD,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bShowLoginWizard = TRUE;
		subMenu->CheckMenuItem(OPTIONS_SHOW_LOGIN_WIZARD, MF_CHECKED | MF_BYPOSITION);
	}
	else 
	{
		g_gfxDetails.bShowLoginWizard = FALSE;
		subMenu->CheckMenuItem(OPTIONS_SHOW_LOGIN_WIZARD, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();
}



void CMainFrame::OnOptionsDefaultScheme() 
{
	// TODO: Add your command handler code here


	BOOL type = 0;
	BOOL old_bDefaultColorScheme = g_gfxDetails.bDefaultColorScheme;
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DEFAULT_SCHEME,MF_BYPOSITION) == MF_UNCHECKED)
	{
#ifndef THE_POOL_CLUB
		g_gameAppView->Message("Dark Scheme - Restart iSnooker for changes to take effect", 10);
#else 
		g_gameAppView->Message("Dark Scheme - Restart iPool for changes to take effect", 10);
#endif
		g_gfxDetails.bDefaultColorScheme = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DEFAULT_SCHEME, MF_CHECKED | MF_BYPOSITION);
		type = 0;
	}
	else 
	{
#ifndef THE_POOL_CLUB
		g_gameAppView->Message("Light Scheme - Restart iSnooker for changes to take effect", 10);
#else 
		g_gameAppView->Message("Light Scheme - Restart iPool for changes to take effect", 10);
#endif
		g_gfxDetails.bDefaultColorScheme = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DEFAULT_SCHEME, MF_UNCHECKED | MF_BYPOSITION);
		type = 1;
	}
	
	
	SaveGfxDetails();
	
}



void CMainFrame::OnShowShotInLobbyTV() 
{
	// TODO: Add your command handler code here


	BOOL type = 0;
	
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_SHOW_LOBBYTV, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bShowShotInLobbyTV = TRUE;
		subMenu->CheckMenuItem(OPTIONS_SHOW_LOBBYTV, MF_CHECKED | MF_BYPOSITION);
		type = 0;
	}
	else 
	{
		g_gfxDetails.bShowShotInLobbyTV = FALSE;
		subMenu->CheckMenuItem(OPTIONS_SHOW_LOBBYTV, MF_UNCHECKED | MF_BYPOSITION);
		type = 1;
	}
	
	
	SaveGfxDetails();
	

	if(type==0) g_gameAppView->Message("Shots will be shown in Lobby TV - Please disable if this causes lag", 10);
		else g_gameAppView->Message("Only ball positions will be shown in Lobby TV - Fastest Performance", 10);


}


void CMainFrame::OnOptionsDrawcoloursl() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_COLOUR_SL,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawColourSL = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_COLOUR_SL, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawColourSL = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_COLOUR_SL, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();
}

void CMainFrame::OnOptionsDrawerrorsl() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_ERROR_SL,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawErrorSL = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_ERROR_SL, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawErrorSL = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_ERROR_SL, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();
}

void CMainFrame::OnOptionsDrawghostcueball() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_GHOST_CUE_BALL,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawGhostCueBall = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_GHOST_CUE_BALL, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawGhostCueBall = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_GHOST_CUE_BALL, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();	
}

void CMainFrame::OnOptionsDrawcueballLe() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_CUEBALL_LE,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawCueBallLE = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_CUEBALL_LE, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawCueBallLE = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_CUEBALL_LE, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();	
}

void CMainFrame::OnOptionsStaticpercentage() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_STATIC_POWER,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawStaticPower = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_STATIC_POWER, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawStaticPower = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_STATIC_POWER, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();
}

void CMainFrame::OnFileAutoSave() 
{
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(FILE_MENU);
	if(subMenu->GetMenuState(FILE_AUTOSAVEFRAMES,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bAutoSaveFrames = TRUE;
		subMenu->CheckMenuItem(FILE_AUTOSAVEFRAMES, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bAutoSaveFrames = FALSE;
		subMenu->CheckMenuItem(FILE_AUTOSAVEFRAMES, MF_UNCHECKED | MF_BYPOSITION);
	}
	
	SaveGfxDetails();	
}

void CMainFrame::OptionsDrawcoloursl(void) // F2
{
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_COLOUR_SL,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawColourSL = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_COLOUR_SL, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawColourSL = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_COLOUR_SL, MF_UNCHECKED | MF_BYPOSITION);
	}
}

void CMainFrame::OptionsDrawerrorsl(void) // F3
{
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_ERROR_SL,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawErrorSL = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_ERROR_SL, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawErrorSL = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_ERROR_SL, MF_UNCHECKED | MF_BYPOSITION);
	}
}

void CMainFrame::OptionsDrawghostcueball(void) // F4
{
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_GHOST_CUE_BALL,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawGhostCueBall = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_GHOST_CUE_BALL, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawGhostCueBall = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_GHOST_CUE_BALL, MF_UNCHECKED | MF_BYPOSITION);
	}
}

void CMainFrame::OptionsDrawcueballLe(void) // F5
{
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if(subMenu->GetMenuState(OPTIONS_DRAW_CUEBALL_LE,MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDrawCueBallLE = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_CUEBALL_LE, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDrawCueBallLE = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DRAW_CUEBALL_LE, MF_UNCHECKED | MF_BYPOSITION);
	}
}

void CMainFrame::OptionsLowgfx(void) // F6
{
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);

	g_gfxDetails.bOptionsLowgfx = 1;
	g_gfxDetails.bOptionsMediumgfx = 0;
	g_gfxDetails.bOptionsHighgfx = 0;
	subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
}
void CMainFrame::OptionsMediumgfx(void)
{
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);

	g_gfxDetails.bOptionsLowgfx = 0;
	g_gfxDetails.bOptionsMediumgfx = 1;
	g_gfxDetails.bOptionsHighgfx = 0;
	subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
}
void CMainFrame::OptionsHighgfx(void)
{
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);

	g_gfxDetails.bOptionsLowgfx = 0;
	g_gfxDetails.bOptionsMediumgfx = 0;
	g_gfxDetails.bOptionsHighgfx = 1;
	subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
	subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
}

void CMainFrame::PracticeBallinhand(void) 
{
	OnPracticeBallinhand();
}

void CMainFrame::OnUpdateSoundVoices(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	#ifdef THE_POOL_CLUB // iPool
	pCmdUI->Enable(FALSE);
	#endif //MAIN_SERVER
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	// Destroy Smiley Window //
	
	if(g_UserMouseClick) 
	{
		if(nState != WA_INACTIVE)
		if(g_gameAppView) 
		{
			if(g_gameAppView->m_SmileySelectDlg)
			if(g_gameAppView->m_SmileySelectDlg.IsWindowVisible())
			{
				g_gameAppView->m_SmileySelectDlg.DestroyWindow();
			}

			if(g_gameAppView->m_TextColorChangerDlg)
			if(g_gameAppView->m_TextColorChangerDlg.IsWindowVisible())
			{
				g_gameAppView->m_TextColorChangerDlg.DestroyWindow();
			}
		}
	}


	if(g_gameAppView)
	{
		// Make sure all our 3D Updats again //
		// g_gameAppView->m_bOnSize = TRUE;
		// g_gameAppView->m_bOnSizeDrawTmpSurf = TRUE;
		// g_gameAppView->m_bUpdateTable = TRUE;

		// Make sure any Mouse States are off //
		g_gameAppView->m_bRButtonDown = 0;
		g_gameAppView->m_bLButtonDown = 0;
		g_gameAppView->m_bMButtonDown = 0;
		g_gameAppView->m_bRButtonJustUp = 0;
		g_gameAppView->m_bLButtonJustUp = 0;
		g_gameAppView->m_bMButtonJustUp = 0;
	}


	g_UserMouseClick = TRUE;
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnMouseMove(nFlags, point);
}





void CMainFrame::OnOptionsDottedcue()
{
	
	// TODO: Add your command handler code here
	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if (subMenu->GetMenuState(OPTIONS_DOTTEDCUEBALL, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsDottedCueBall = TRUE;
		subMenu->CheckMenuItem(OPTIONS_DOTTEDCUEBALL, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsDottedCueBall = FALSE;
		subMenu->CheckMenuItem(OPTIONS_DOTTEDCUEBALL, MF_UNCHECKED | MF_BYPOSITION);
	}


	SaveGfxDetails();
}


void CMainFrame::OnOptionsMinicamShowhelpers()
{
	// TODO: Add your command handler code here

	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if (subMenu->GetMenuState(OPTIONS_MINICAM_SHOWHELPERS, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsMiniCamShowHelpers = TRUE;
		subMenu->CheckMenuItem(OPTIONS_MINICAM_SHOWHELPERS, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsMiniCamShowHelpers = FALSE;
		subMenu->CheckMenuItem(OPTIONS_MINICAM_SHOWHELPERS, MF_UNCHECKED | MF_BYPOSITION);
	}


	SaveGfxDetails();

}


void CMainFrame::OnOptionsMinicamLockmouse()
{
	// TODO: Add your command handler code here


	CMenu *subMenu = GetMenu()->GetSubMenu(OPTIONS_MENU);
	if (subMenu->GetMenuState(OPTIONS_MINICAM_LOCKMOUSE, MF_BYPOSITION) == MF_UNCHECKED)
	{
		g_gfxDetails.bOptionsMiniCamLockMouse = TRUE;
		subMenu->CheckMenuItem(OPTIONS_MINICAM_LOCKMOUSE, MF_CHECKED | MF_BYPOSITION);
	}
	else
	{
		g_gfxDetails.bOptionsMiniCamLockMouse = FALSE;
		subMenu->CheckMenuItem(OPTIONS_MINICAM_LOCKMOUSE, MF_UNCHECKED | MF_BYPOSITION);
	}


	SaveGfxDetails();
}


void CMainFrame::OnToggleFullscreen()
{
	// TODO: Add your command handler code here
	toggleFullScreen();
}
