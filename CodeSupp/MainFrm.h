// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "SplitterWndEx.h"
//#include "SendView.h"
#include "ChatView.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>


#define MAINFRM_MINIMUM_WIDTH				800
#define MAINFRM_MINIMUM_HEIGHT				600
#define MAINFRM_MINIMUM_WIDTH_WS			800 // 960
#define MAINFRM_MINIMUM_HEIGHT_WS			600 // 600


#define MAINFRM_MAXIMUM_WIDTH				1280
#define MAINFRM_MAXIMUM_HEIGHT				1024
#define MAINFRM_MAXIMUM_WIDTH_WS			1125	// 1440
#define MAINFRM_MAXIMUM_HEIGHT_WS			900		// 900



class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	void CMainFrame::SetRefreshRateCheck(int n);

// Attributes
protected:
	//CSplitterWnd m_wndSplitter;
	CSplitterWndEx m_wndSplitter;
//	CSendView m_sendView;
	CChatView m_chatView;
	CWnd m_wnd;
	char *appName;
	bool bFullScreen;

public:

// Operations
public:
	// these are mapped directly to the protected members fn. equivalents below
	void OptionsDrawcoloursl(void);
	//	
	void OptionsDrawerrorsl(void);
	//	
	void OptionsDrawghostcueball(void);
	//	
	void OptionsDrawcueballLe(void);
	//
	void OptionsLowgfx(void);
	void OptionsMediumgfx(void);
	void OptionsHighgfx(void);

	void PracticeBallinhand(void);

	void toggleFullScreen();
	void updateAspect(float ratio = 1.36f);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual void OnGetMinMaxInfo( MINMAXINFO FAR* pMinMaxInfo );
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL
	
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnClose();
	afx_msg void OnOptionsHighPerformance();
	afx_msg void OnOptionsLowgfx();
	afx_msg void OnOptionsMediumgfx();
	afx_msg void OnOptionsHighgfx();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnOptionsReplaypanel();
	afx_msg void OnOptions2d();
	afx_msg void OnOptions2d3d();
	afx_msg void OnOptions2dtvauto();
	afx_msg void OnOptions2d3dfree();
	//afx_msg void OnOptions2DTVA();
	afx_msg void OnSoundBalls();
	afx_msg void OnSoundClapping();
	afx_msg void OnDrawcuetip();
	afx_msg void OnDrawcuetransparent();
	afx_msg void OnDrawcuesolid();
	afx_msg void OnPracticeBallinhand();
	afx_msg void OnOptionsComplexShadows();
	afx_msg void OnOptionsShowminicam();
	afx_msg void OnOptionsSetrefreshrate100hz();
	afx_msg void OnOptionsSetrefreshrate120hz();
	afx_msg void OnOptionsSetrefreshrate140hz();
	afx_msg void OnOptionsSetrefreshrate25hz();
	afx_msg void OnOptionsSetrefreshrate30hz();
	afx_msg void OnOptionsSetrefreshrate50hz();
	afx_msg void OnOptionsSetrefreshrate60hz();
	afx_msg void OnOptionsSetrefreshrate70hz();
	afx_msg void OnOptionsSetrefreshrate72hz();
	afx_msg void OnOptionsSetrefreshrate85hz();
	afx_msg void OnOptionsSetrefreshrateDefault();
	afx_msg void OnOptionsSetrefreshrate150hz();
	afx_msg void OnOptionsSetrefreshrate75hz();
	afx_msg void OnOptionsLoginwizard();
	afx_msg void OnOptionsDefaultScheme();
	afx_msg void OnShowShotInLobbyTV();
	afx_msg void OnOptionsDrawcoloursl();
	afx_msg void OnOptionsDrawerrorsl();
	afx_msg void OnOptionsStaticpercentage();
	afx_msg void OnOptionsDrawghostcueball();
	afx_msg void OnOptionsDrawcueballLe();
	afx_msg void OnFileAutoSave();
	afx_msg void OnSoundVoices();
	afx_msg void OnUpdateSoundVoices(CCmdUI* pCmdUI);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnSizing( UINT nSide, LPRECT lpRect );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles( HDROP hDropInfo );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateOptionsColourblind(CCmdUI *pCmdUI);
	afx_msg void OnOptionsColourblind();
	afx_msg void OnOptionsDottedcue();
	afx_msg void OnOptionsMinicamShowhelpers();
	afx_msg void OnOptionsMinicamLockmouse();
	afx_msg void OnToggleFullscreen();
};

/////////////////////////////////////////////////////////////////////////////
