// GameApp.cpp : Defines the class behaviors for the application.
//

//#include "Build.h"
#include "stdafx.h"
#include "GameApp.h" // ?? has an ifdef on main_server dodgy

#include "MainFrm.h"
#include "GameAppDoc.h"
#include "GameAppView.h"
#include "ChatView.h"
#include "../Code/Common.h"

//#include <dwmapi.h>





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_DWMCOMPOSITIONCHANGED	0x031E



/////////////////////////////////////////////////////////////////////////////
// CGameAppApp

BEGIN_MESSAGE_MAP(CGameAppApp, CWinApp)
	//{{AFX_MSG_MAP(CGameAppApp)
	ON_COMMAND(ID_PRACTICE_LOAD, OnPracticeLoad)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameAppApp construction
CShellEx *mainShell; // 'MAIN SERVER' - mainShell
//#ifndef MAIN_SERVER
CShellEx *gameShell; // 'GAME SERVER' - gameShell
//#endif //!MAIN_SERVER
CShellEx *tempShell; // 'to see if a player can host a table, just after login' - tempShell

HWND g_hwnd;
HINSTANCE g_hinst;
int g_active;
CGameAppView *g_gameAppView;
BOOL g_bNewMessage;

CGameAppApp::CGameAppApp()// : CWinApp("Hello")
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance	
	g_hwnd = NULL;
	g_hinst = NULL;
	g_active = 1;
	m_JustIdle = 0;
	g_gameAppView = NULL;
	g_bNewMessage = FALSE;
	mainShell = NULL;
	//#ifndef MAIN_SERVER
	gameShell = NULL;
	//#endif //!MAIN_SERVER

	// Main Shell - Main Server communication (Lobby Room Hosted by the 'ONE' SERVER)
	mainShell = new CShellEx(TRUE);
	mainShell->AttachComm(SHELL_SERVER, new CComm(MAX_LOBBY_PLAYERS, SEND_QUEUE_SIZE)); // also sets max profile capacity. which is used for both clients & servers.
	mainShell->AttachComm(SHELL_CLIENT, new CComm(1, SEND_QUEUE_SIZE));
	g_dataSent = 0;
	g_dataReceived = 0;

	
	
}

CGameAppApp::~CGameAppApp()// : CWinApp("Hello")
{
	// TODO: add destruction code here,
	//if(gameShell) delete gameShell;
	if(mainShell) delete mainShell;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGameAppApp object

CGameAppApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGameAppApp initialization

BOOL CGameAppApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.

	//MessageBox(NULL,"Some message", AfxGetAppName(), MB_OK);
	g_hinst = m_hInstance;
	
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGameAppDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGameAppView));		
	AddDocTemplate(pDocTemplate);

	// create a new (empty) document
	//SetDialogBkColor(RGB(0,0,0), RGB(220,220,220));

	OnFileNew();
	//OnFileOpen();
	
	/*if (m_lpCmdLine[0] == '\0')
    {
		  // Create a new (empty) document.
		  OnFileNew();
    }
	else
	{
		// Open a file passed as the first command line parameter.
		OpenDocumentFile("//m_lpCmdLine);
	}*/

	//pDocTemplate->CreateNewDocument( );



	// Parse command line for standard shell commands, DDE, file open
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

//	// The one and only window has been initialized, so show and update it.
//	m_pMainWnd->ShowWindow(SW_SHOW);
//	m_pMainWnd->UpdateWindow();

//	m_pMainWnd->ShowWindow(SW_HIDE);

 
//	m_pMainWnd->SetWindowText("App's Title");

//	int i = GetThreadPriority();
//	char buffer[80];
//	int t = 0;
//	if(i == THREAD_PRIORITY_NORMAL) t = 1;

	
	//SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	//SetThreadPriority(THREAD_PRIORITY_HIGHEST);//THREAD_PRIORITY_HIGHEST);
	 	
	return TRUE;
}
/*
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About					
class CAboutDlg : public CDialog
	
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CGameAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
*/
/////////////////////////////////////////////////////////////////////////////
// CGameAppApp commands
/*
bool IsCompositionEnabled()
{
    HMODULE library = ::LoadLibrary("dwmapi.dll");
    bool result = false;

    if (0 != library)
    {
        if (0 != ::GetProcAddress(library, 
                                  "DwmIsCompositionEnabled"))
        {
            BOOL enabled = FALSE;
			result = SUCCEEDED(::DwmIsCompositionEnabled(&enabled)) && enabled;
        }

        VERIFY(::FreeLibrary(library));
    }

    return result;
}



HRESULT EnableBlurBehindWindow(HWND window,
                               bool enable = true,
                               HRGN region = 0,
                               bool transitionOnMaximized = false)
{
    DWM_BLURBEHIND blurBehind = { 0 };
    
    blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_TRANSITIONONMAXIMIZED;
    blurBehind.fEnable = enable;
    blurBehind.fTransitionOnMaximized = transitionOnMaximized;

    if (enable && 0 != region)
    {
        blurBehind.dwFlags |= DWM_BB_BLURREGION;
        blurBehind.hRgnBlur = region;
    }

    return ::DwmEnableBlurBehindWindow(window,
                                       &blurBehind);
}	
*/




BOOL CGameAppApp::InitApplication() 
{
	// TODO: Add your specialized code here and/or call the base class

/*
	if(IsCompositionEnabled())
	{
		//HWND hwnd = this->m_pActiveWnd->m_hWnd;
		// If LoadLibrary was successful, it means we are running in Windows Vista	//
		// Disable the Composition Mode as this makes theApp.m_pMainWnd->m_hWndthe game run twice as fast	//
		//HRESULT result = ::DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
		HRESULT result = EnableBlurBehindWindow(NULL, false);
	}

*/


	OSVERSIONINFO osvi;
    BOOL bIsWindowsXPorLater;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    bIsWindowsXPorLater = 
       ( (osvi.dwMajorVersion > 5) ||
       ( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1) ));

    //if(bIsWindowsXPorLater)
    //    printf("The system meets the requirements.\n");
    //else printf("The system does not meet the requirements.\n");


	//char VersionInfo[256];
	//sprintf(VersionInfo, "osvi.dwMajorVersion:%ld", osvi.dwMajorVersion);
	//AfxMessageBox(VersionInfo);

	//g_WindowsVista = false;
	if(osvi.dwMajorVersion >= 6)
	{
		g_WindowsVista = true;
	}

	//g_WindowsVista = false;
/*

#ifndef WIN_VISTA
	g_WindowsVista = false;
#else
	g_WindowsVista = true;
    //if( GetOSDisplayString( szOS ) )
	{
		// AfxMessageBox(szOS);
	}
#endif
*/

	return CWinApp::InitApplication();
}

CDocument* CGameAppApp::OpenDocumentFile(LPCTSTR lpszFileName = "test.txt") 
{
	// TODO: Add your specialized code here and/or call the base class

	
	return CWinApp::OpenDocumentFile(lpszFileName);
}



BOOL CGameAppApp::TranslateAMessage(void)
{
	
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)   // the only way out of the loop
		{
			return FALSE;
		}

		//PreTranslateMessage(&msg);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return TRUE;
	}
	
	return FALSE;
	
}




int CGameAppApp::Run() 
{
	// TODO: Add your specialized code here and/or call the base class

#ifndef MAIN_SERVER

	static float lastTime = (float)timeGetTime();

	MSG msg;
	LONG lCount = 0;
	//static int proceed = 0;
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{


	       	if (msg.message == WM_QUIT) break;   // the only way out of the loop

			if(!PreTranslateMessage(&msg))
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg);
				lCount = 0;
				// OnIdle(lCount);
			}
		}
		else 
		{
			// Sleep(1);
			// Sleep(0);
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime) * 0.001f;

			if(g_gameAppView) g_gameAppView->GameLoop(timeDelta);

			lastTime = currTime;
		}  // if screen updated

	} while (TRUE); // end of loop
		
#endif
	return CWinApp::Run();
}

BOOL CGameAppApp::OnIdle(LONG lCount) 
{
	/*
	// TODO: Add your specialized code here and/or call the base class

	if(!g_build.main_server) //#ifndef MAIN_SERVER
	{
	BOOL bMore = FALSE;
	bMore = CWinApp::OnIdle(lCount);
	
	switch(lCount)
	{
		
		//default:
		case 0:
		{
			if(g_gameAppView)
			{
				g_gameAppView->GameLoop(0);
				bMore = TRUE;
			}
		}
		break;
		


		default:
		{
			
			// Make less processing when shot not in progress //
			//if(g_gameAppView)
			//if( g_gameAppView->m_snooker->m_cueBallSelect != 0 &&
			//	g_gameAppView->m_snooker->m_cueBallSelect != 1 &&
			//	g_gameAppView->m_snooker->m_cueBallSelect != 5 )
			
			{
				g_gameAppView->GameLoop(0);
				bMore = TRUE; // during simulation
				//return FALSE;//TRUE; // during simulation
			}
		}
		break;


	}

	return bMore;
	}
	// else 
	//#else // MAIN_SERVER
*/

/*
	
#ifndef MAIN_SERVER

	static float lastTime = (float)timeGetTime();

	float currTime = (float)timeGetTime();
	float timeDelta = (currTime - lastTime) * 0.001f;

	CWinApp::OnIdle(lCount);
	if(g_gameAppView)
	{
		g_gameAppView->GameLoop(timeDelta);
	}


	lastTime = currTime;

	return true;

#endif
*/

	

	return CWinApp::OnIdle(lCount);





	//#endif //!MAIN_SERVER	
	
}

/*
// Just some code snippet from the net (might be useful?!!)

For getting active view in an MDI app from OnIdle

    CMDIFrameWnd *pFrame = 
             (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

    // Get the active MDI child window.
    CMDIChildWnd *pChild = 
             (CMDIChildWnd *) pFrame->GetActiveFrame();

    // or CMDIChildWnd *pChild = pFrame->MDIGetActive();

    // Get the active view attached to the active MDI child
    // window.
    CTxtOutMDIView* pView = (CTxtOutMDIView*) pChild->GetActiveView();
    
//     pView->YourViewFunction();

Check CFrameWnd::GetActiveView () for more info
*/

BOOL CGameAppApp::IsIdleMessage( MSG* pMsg )
{
/*
	if (!CWinApp::IsIdleMessage( pMsg ) || 
	    pMsg->message == WM_TIMER) 
		return FALSE;
	else
		return TRUE;
*/

	CWinApp::IsIdleMessage( pMsg );
	return TRUE;
}

BOOL CGameAppApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
		
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CGameAppApp::ProcessMessageFilter(int code, LPMSG lpMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ProcessMessageFilter(code, lpMsg);
}

void CGameAppApp::OnPracticeLoad() 
{
	// TODO: Add your command handler code here
	//OnFileOpen(); // works in initinstance, but don't work here??? maybe it's the type of function.
	//CWinApp::OnFileOpen();
}




/*
#ifdef WIN_VISTA

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#pragma comment(lib, "User32.lib")

#define BUFSIZE 256
#define VER_SUITE_WH_SERVER 0x00008000

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

BOOL CGameAppApp::GetOSDisplayString( LPTSTR pszOS)
{

   // Get Windows Version //
   g_WindowsVista = false;


   OSVERSIONINFOEX osvi;
   SYSTEM_INFO si;
   PGNSI pGNSI;
   PGPI pGPI;
   BOOL bOsVersionInfoEx;
   DWORD dwType;

   ZeroMemory(&si, sizeof(SYSTEM_INFO));
   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
      return 1;

   // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

   pGNSI = (PGNSI) GetProcAddress(
      GetModuleHandle(TEXT("kernel32.dll")), 
      "GetNativeSystemInfo");
   if(NULL != pGNSI)
      pGNSI(&si);
   else GetSystemInfo(&si);

   if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
        osvi.dwMajorVersion > 4 )
   {
      StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));

      // Test for the specific product.

      if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 )
      {
         if( osvi.wProductType == VER_NT_WORKSTATION )
             StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
         else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
      }
      
      if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
      {

		 // Get Windows Version //
		 g_WindowsVista = true;

         if( osvi.wProductType == VER_NT_WORKSTATION )
             StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
         else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));

         pGPI = (PGPI) GetProcAddress(
            GetModuleHandle(TEXT("kernel32.dll")), 
            "GetProductInfo");

         pGPI( 6, 0, 0, 0, &dwType);

         switch( dwType )
         {
            case PRODUCT_ULTIMATE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition" ));
               break;
            case PRODUCT_HOME_PREMIUM:
               StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition" ));
               break;
            case PRODUCT_HOME_BASIC:
               StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition" ));
               break;
            case PRODUCT_ENTERPRISE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
               break;
            case PRODUCT_BUSINESS:
               StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition" ));
               break;
            case PRODUCT_STARTER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition" ));
               break;
            case PRODUCT_CLUSTER_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition" ));
               break;
            case PRODUCT_DATACENTER_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition" ));
               break;
            case PRODUCT_DATACENTER_SERVER_CORE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER_CORE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER_IA64:
               StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
               break;
            case PRODUCT_SMALLBUSINESS_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
               break;
            case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
               StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
               break;
            case PRODUCT_STANDARD_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition" ));
               break;
            case PRODUCT_STANDARD_SERVER_CORE:
               StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
               break;
            case PRODUCT_WEB_SERVER:
               StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition" ));
               break;
         }
         if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
            StringCchCat(pszOS, BUFSIZE, TEXT( ", 64-bit" ));
         else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
            StringCchCat(pszOS, BUFSIZE, TEXT(", 32-bit"));
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
      {

         if( GetSystemMetrics(SM_SERVERR2) )
            StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
         else if ( osvi.wSuiteMask==VER_SUITE_STORAGE_SERVER )
            StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
         else if ( osvi.wSuiteMask==VER_SUITE_WH_SERVER )
            StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Home Server"));
         else if( osvi.wProductType == VER_NT_WORKSTATION &&
                  si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
         {
            StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
         }
         else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003, "));

         // Test for the server type.
         if ( osvi.wProductType != VER_NT_WORKSTATION )
         {
            if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
            {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
            }

            else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
            {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
                else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
            }

            else
            {
                if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
                else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition" ));
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition" ));
                else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
                   StringCchCat(pszOS, BUFSIZE, TEXT( "Web Edition" ));
                else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard Edition" ));
            }
         }
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
      {
         StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
         if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
            StringCchCat(pszOS, BUFSIZE, TEXT( "Home Edition" ));
         else StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
      {
         StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));

         if ( osvi.wProductType == VER_NT_WORKSTATION )
         {
            StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
         }
         else 
         {
            if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
               StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
            else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
               StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
            else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
         }
      }

       // Include service pack (if any) and build number.

      if( _tcslen(osvi.szCSDVersion) > 0 )
      {
          StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
          StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
      }

      TCHAR buf[80];

      StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
      StringCchCat(pszOS, BUFSIZE, buf);

      return TRUE; 
   }

   else
   {  
      printf( "This sample does not support this version of Windows.\n");
      return FALSE;
   }
}

#endif
*/