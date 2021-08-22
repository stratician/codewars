// TableTabCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "TableTabCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTableTabCtrlDlg dialog


CTableTabCtrlDlg::CTableTabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTableTabCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTableTabCtrlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CTableTabCtrlDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CTableTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTableTabCtrlDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTableTabCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CTableTabCtrlDlg)
	ON_WM_CREATE()
	ON_WM_MEASUREITEM()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//ON_MESSAGE(WM_SETFONT, OnSetFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTableTabCtrlDlg message handlers

int CTableTabCtrlDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//CenterWindow();

	Mmain_log("[5]");
	// Create child listctrl tables window
	CRect rect;
	int g_FIX_shiftX = 118;
	int offsetX = 788 - ((CGameAppView *)m_pParent)->m_cx; // 800
	int offsetY = 542 - ((CGameAppView *)m_pParent)->m_cy; // 600
	int w = ((CGameAppView *)m_pParent)->m_SizeDesktop.cx;
	int h = (int)(((CGameAppView *)m_pParent)->m_cy - 330+50 + offsetY);
	rect.SetRect(0,0,w/*800*/,h/*400*/);

	
	//BAP_FIX
	// This next line causes a problem is LVS_OWNERDRAWFIXED is set in Debug.. So I've removed this //
	((CGameAppView *)m_pParent)->m_listTables.Create(LVS_NOCOLUMNHEADER | LVS_OWNERDRAWFIXED | LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE | WS_BORDER, rect, this, IDC_LISTCTRLTABLES);
	//((CGameAppView *)m_pParent)->m_listTables.Create(LVS_NOCOLUMNHEADER | LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE | WS_BORDER, rect, this, IDC_LISTCTRLTABLES);
	
	
	((CGameAppView *)m_pParent)->m_listTables.PassParent(m_pParent);

	((CGameAppView *)m_pParent)->m_listTables.SetFont(&((CGameAppView *)m_pParent)->m_listCtrlFont);
	((CGameAppView *)m_pParent)->m_listTables.SetColumnFontHeight(2);
	((CGameAppView *)m_pParent)->m_listTables.TriggerOnMeasureItem(IDC_LISTCTRLTABLES); // calls this to set the item height, based on font height
	((CGameAppView *)m_pParent)->m_listTables.AllowColumnSizing(FALSE);
	((CGameAppView *)m_pParent)->m_listTables.SetGridFlags(CListCtrlEx::TABLE | CListCtrlEx::GRIDHORZ | CListCtrlEx::GRIDSPLIT |
							  CListCtrlEx::HIGHLIGHTROWDISABLED | CListCtrlEx::BUTTONS | CListCtrlEx::GRIDVERT |
							  CListCtrlEx::TEXTCOLOR);

	((CGameAppView *)m_pParent)->m_listTables.SetGridColor(0, RGB(255,0,0));
	((CGameAppView *)m_pParent)->m_listTables.SetGridColor(1, RGB(0,255,0));
	((CGameAppView *)m_pParent)->m_listTables.SetGridSplitOffsetX(70, 0);
		
	((CGameAppView *)m_pParent)->m_listTables.CreateColumn(0, "TableD", 70);
	((CGameAppView *)m_pParent)->m_listTables.CreateColumn(1, "View", 60);
	((CGameAppView *)m_pParent)->m_listTables.CreateColumn(2, "Watch", 48);
	((CGameAppView *)m_pParent)->m_listTables.CreateColumn(3, "Player #1", 80);
	((CGameAppView *)m_pParent)->m_listTables.CreateColumn(4, "Player #2", 80);
	((CGameAppView *)m_pParent)->m_listTables.CreateColumn(5, "Observers", w-380);//800-320);//((CGameAppView *)m_pParent)->m_SizeDesktop.cx);
	

	((CGameAppView *)m_pParent)->m_listTables.CreateButtons(MAX_TABLEBUTTONS, TRUE);

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		((CGameAppView *)m_pParent)->m_listTables.m_TableIcon = AfxGetApp()->LoadIcon(IDI_TABLE);
		((CGameAppView *)m_pParent)->m_listTables.m_TableDimIcon = AfxGetApp()->LoadIcon(IDI_TABLEDIM);
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		((CGameAppView *)m_pParent)->m_listTables.m_TableIcon = 0;//AfxGetApp()->LoadIcon(IDI_POOLTABLE);
		((CGameAppView *)m_pParent)->m_listTables.m_TableDimIcon = 0;//= AfxGetApp()->LoadIcon(IDI_POOLTABLE);//DIM);
	}
	//((CGameAppView *)m_pParent)->m_listTables.SetBkColor(CLR_NONE);//RGB(20,20,20));
	((CGameAppView *)m_pParent)->m_listTables.SetBkColor(RGB(0,220,248));
	((CGameAppView *)m_pParent)->m_listTables.SetGridColor(0, RGB(66,28,19));
	((CGameAppView *)m_pParent)->m_listTables.SetGridColor(1, RGB(46,9,1));				// Middle Line //
	((CGameAppView *)m_pParent)->m_listTables.m_crTextColor = RGB(255,255,255);
	((CGameAppView *)m_pParent)->m_listTables.m_crTextBkColor = RGB(46,9,1);


	Mmain_log("[5.1]");
	
	CString items[6];
	items[1].Format("");
	items[2].Format("");
	items[3].Format(""); // P1
	items[4].Format(""); // P2
	items[5].Format(""); // Watcher
	
	Mmain_log("[5.2]");
	((CGameAppView *)m_pParent)->m_listTables.SetMaxItems(MAX_TABLEITEMS);
	
	for(int i=0;i<MAX_TABLEITEMS;i++)
	{
		items[0].Format("#%d", i+1);
		((CGameAppView *)m_pParent)->m_listTables.CreateTableItem(i, items, "Options:");//optionsStr);
	}
	
	
	items[0].Format("");


	
	#ifndef MAIN_SERVER
	Mmain_log("[5.5]");

	#endif //!MAIN_SERVER


	return 0;
}

void CTableTabCtrlDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDCtl)
	{
		case IDC_LISTCTRLPLAYERS:
		// set to 14 defaulty
		break;
		
		case IDC_LISTCTRLTABLES:
		{
			// sets the control's column height for the items
			lpMeasureItemStruct->itemHeight = ((CGameAppView *)m_pParent)->m_listTables.m_columnHeight;
		}
		break;

		default:
		break;
	}

	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

BOOL CTableTabCtrlDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return 0;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CTableTabCtrlDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
		
	// Do not call CDialog::OnPaint() for painting messages
}



/*
LRESULT CTableTabCtrlDlg::OnSetFont(WPARAM wParam, LPARAM)
{
    LRESULT res = Default();

    CRect rc;
    ((CGameAppView *)m_pParent)->GetWindowRect( &rc );

    WINDOWPOS wp;
    wp.hwnd  = m_hWnd;
    wp.cx    = rc.Width();
    wp.cy    = rc.Height();
    wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    //SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

	((CGameAppView *)m_pParent)->SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
    return res;
}*/